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
            return YWorker.Parser.ParseFrom(x);
        }

        public static bool removeWorkerById(int wid)
        {
            return rc.Sut.HashDelete("yworkers", wid.ToString());
        }

        public static bool updateWorker(YWorker yw)
        {
            Console.WriteLine(String.Format("Update {0} to database", yw.Wname));
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
