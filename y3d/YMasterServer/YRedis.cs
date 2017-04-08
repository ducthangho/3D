using System;
using System.IO;
using System.Threading.Tasks;
using Google.Protobuf;
using Grpc.Core;
using System.Diagnostics;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.Collections.ObjectModel;

using StackExchange.Redis;
using StackExchange.Redis.Extensions.Core;
using y3d.e;

namespace YMasterServer
{
    public class ProtobufSerializer : ISerializer
    {
        public byte[] Serialize(object item)
        {
            var x = (Google.Protobuf.IMessage)item;
            return x.ToByteArray();
        }

        public Task<byte[]> SerializeAsync(object item)
        {
            return Task.Factory.StartNew(() => Serialize(item));
        }

        public object Deserialize(byte[] serializedObject)
        {
            return Deserialize<object>(serializedObject);
        }

        public Task<object> DeserializeAsync(byte[] serializedObject)
        {
            return Task.Factory.StartNew(() => Deserialize(serializedObject));
        }

        public T Deserialize<T>(byte[] serializedObject)
        {
            //if (typeof(T).GetInterface("IMessage")!=null)
            //{
            //    //Convert.ChangeType()
            //    var obj = default(T) as IMessage;

            //    var x = zun1.ToObject<T>(serializedObject);
            //    return x;
            //}
            //var x = zun1.ToObject<T>(serializedObject);
            return default(T);

            //using (var ms = new MemoryStream(serializedObject))
            //{
            //    return Serializer.Deserialize<T>(ms);
            //}
        }
        public Task<T> DeserializeAsync<T>(byte[] serializedObject)
        {
            return Task.Factory.StartNew(() => Deserialize<T>(serializedObject));
        }
    }


    public class YRedis
    {
        public static CacheClientBase rc;
        public static void InitRedis()
        {
            YRedis.rc = new CacheClientBase(new ProtobufSerializer());
        }

        public static int incrWorkerId()
        {
            rc.Db.StringIncrement("max_worker_id");
            return (int)rc.Db.StringGet("max_worker_id");
        }

        public static int getMaxWorkerId()
        {
            return (int)rc.Db.StringGet("max_worker_id");
        }

        public static bool addWorker(YWorker yw)
        {
            return rc.Sut.HashSet("yworkers", yw.Wid.ToString(), yw);
        }

        public static YWorker getWorkerById(int wid)
        {
            var x = rc.Db.HashGet("yworkers", wid.ToString());
            return (x.IsNullOrEmpty)?null:YWorker.Parser.ParseFrom(x);
        }

        public static bool removeWorkerById(int wid)
        {
            //Console.WriteLine(String.Format("Remove Worker {0} from database", wid));
            return rc.Sut.HashDelete("yworkers", wid.ToString());
        }

        public static bool updateWorker(YWorker yw)
        {
            Console.WriteLine(String.Format("Update {0} to database (netstate:{1}, type:{2})", yw.Wname,yw.NetState,yw.Wtype.ToString()));
            return rc.Sut.HashSet("yworkers", yw.Wid.ToString(), yw);
        }

        public static List<YWorker> getAllWorker()
        {
            var x = rc.Db.HashValues("yworkers");
            List<YWorker> ret = new List<YWorker>();
            for (int i = 0; i < x.Length; i++)
            {
                ret.Add(YWorker.Parser.ParseFrom(x[i]));
            }
            return ret;
        }

        public static List<YWorker> getWorkersByMachine(YMachine req, bool include=true)
        {
            var ww = rc.Db.HashValues("yworkers");
            List<YWorker> ret = new List<YWorker>();
            foreach (var w in ww)
            {
                var yw = YWorker.Parser.ParseFrom(w);

                if (yw.MachineIp == req.IpAddress)
                {
                    if (include) ret.Add(yw);
                } else
                {
                    if (!include) ret.Add(yw);
                }
            }
            return ret;
        }

        public static List<YWorker> getWorkersByMachine(String ip, bool include = true)
        {
            var ww = rc.Db.HashValues("yworkers");
            List<YWorker> ret = new List<YWorker>();
            foreach (var w in ww)
            {
                var yw = YWorker.Parser.ParseFrom(w);
                if (ip=="127.0.0.1"||ip=="localhost")
                {
                    if (include) ret.Add(yw);
                } else
                {
                    if (yw.MachineIp == ip)
                    {
                        if (include) ret.Add(yw);
                    }
                    else
                    {
                        if (!include) ret.Add(yw);
                    }
                }
            }
            return ret;
        }

        public static void saveWorkers2Temp()
        {
            var x = rc.Db.HashGetAll("yworkers");
            rc.Db.HashSet("yworker_temp", x);
        }

        public static void loadWorkerFromTemp()
        {
            var x = rc.Db.HashGetAll("yworker_temp");
            rc.Db.HashSet("yworkers", x);
            var xx = 1;
            //rc.Db.H
            //rc.Db.KeyDelete("yworker_temp");
        }

        public static UserSetting getSettingByUser(string uname)
        {
            var x = rc.Db.HashGet("usettings", uname);
            if (x.IsNullOrEmpty)
            {
                UserSetting ret = new UserSetting();
                ret.Apps.Add(YMServer.YSys.Apps);
                ret.Workspace = YMServer.YSys.WorkingFolder + "\\" + uname + "\\";
                rc.Sut.HashSet("usettings", uname, ret);
                return ret;
            }
            return UserSetting.Parser.ParseFrom(x);
        }

        public static bool updateUserSetting(string uname, UserSetting usetting)
        {
            return rc.Sut.HashSet("usettings", uname, usetting);
        }

        public static UserResponse SignUp(string u, string p)
        {
            UserResponse ur = new UserResponse();
            ur.Rep = new ResultReply();
            ur.Rep.Error = false;
            //var user = rc.Db.HashGet("yusers",u);
            //if (!user.IsNullOrEmpty)
            if (rc.Db.HashExists("yusers", u))
            {
                ur.Rep.Error = true;
                ur.Rep.Message = String.Format("User ({0}) already exists, please choose another!", u);
                return ur;
            }
            YUser yu = new YUser();
            yu.Username = u;
            yu.Password = p;
            rc.Sut.HashSet("yusers", u, yu);
            ur.User = yu;
            ur.User.Password = "";
            ur.Usetting = getSettingByUser(yu.Username);
            return ur;
        }

        public static UserResponse SignIn(string u, string p)
        {
            UserResponse ur = new UserResponse();
            ur.Rep = new ResultReply();
            ur.Rep.Error = true;
            var user = rc.Db.HashGet("yusers", u);
            if (!user.IsNullOrEmpty)
            {
                var yu = YUser.Parser.ParseFrom(user);
                if (yu.Password==p)
                {
                    ur.Rep.Error = false;
                    ur.User = yu;
                    ur.User.Password = "";
                    ur.Usetting = getSettingByUser(yu.Username);
                    return ur;
                }
            }
            ur.Rep.Message = "The user does not exist on this server or the password that was entered does not match the user!";
            return ur;
        }

        //public static UserSetting MixUserSettings(string uname)
        //{
        //    UserSetting ret = new UserSetting();
        //    var s = rc.Db.HashGet("usettings", uname);
        //    if (s.IsNullOrEmpty)
        //    {
        //        ret.Apps.Add(YMServer.YSys.Apps);
        //        ret.Workspace = YMServer.YSys.WorkingFolder + "\\" + uname + "\\";
        //    } else
        //    {
        //        var settings = UserSetting.Parser.ParseFrom(s);
        //        foreach (var app in YMServer.YSys.Apps)
        //        {
        //            if (!settings.Apps.ContainsKey(app.Key))
        //            {
        //                ret.Apps.Add(app.Key, app.Value);
        //            }
        //        }
        //        if (ret.Workspace.Length<1)
        //            ret.Workspace = YMServer.YSys.WorkingFolder + "\\" + uname + "\\";
        //    }
        //    return ret;
        //}


    }

    public class CacheClientBase : IDisposable
    {
        public readonly IDatabase Db;
        public readonly ICacheClient Sut;
        public ISerializer Serializer;
        public CacheClientBase(ISerializer serializer)
        {
            Serializer = serializer;
            var mux = ConnectionMultiplexer.Connect(new ConfigurationOptions
            {
                DefaultVersion = new Version(3, 0, 500),
                EndPoints = { { "localhost", 6379 } },
                AllowAdmin = true
            });

            Sut = new StackExchangeRedisCacheClient(mux, Serializer);
            Db = Sut.Database;
        }
        public void Dispose()
        {
            //Db.FlushDatabase();
            Db.Multiplexer.GetSubscriber().UnsubscribeAll();
            Db.Multiplexer.Dispose();
            Sut.Dispose();
        }
    }

}
