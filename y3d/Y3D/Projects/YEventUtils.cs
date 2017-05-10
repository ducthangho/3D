using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using y3d.e;
using y3d.s;
using Grpc.Core;
using Google.Protobuf;

using System.Reactive;
using System.Reactive.Linq;
using System.Reactive.Disposables;
using System.Windows.Forms;
using System.Collections.ObjectModel;
using System.Collections.Specialized;

namespace Y3D.Projects
{
    class YEventUtils
    {
        static bool EditMode = false;
        static public bool editInCopy = true;
        //static List<YEvent> events = new List<YEvent>();
        static ObservableCollection<YEvent> events = new ObservableCollection<YEvent>();
        static IObservable<YEvent> last_e = Observable.Interval(TimeSpan.FromMilliseconds(20)).Select(l =>
        {
            if (events.Count < 1) return null;
            var x = events.Last();
            events.Clear();
            return x;
            //var e = events.ToObservable().Latest();
            //if (e.Count() > 0) return e.Last();
            //return null;
        }
        );
        static AsyncClientStreamingCall<YEvent, ResponseEvent> streamCall = null;
        static IObservable<EventPattern<NotifyCollectionChangedEventArgs>> eventChanges = null;
        static IDisposable subcribe_e = null;
        static public string tname = "";
        static public string surfix_clone_from = "_high";
        static public string surfix_save_to = "_low";
        static public string surfix_tmp = "_low_tmp";
        //static public async Task doStreamEvent()
        //{
        //    streamCall = Utils.MaxClient.DoStreamClient();
        //}

        //static public void init()
        //{
        //    var ee = events.ToObservable();
        //    var last_e = Observable.Interval(TimeSpan.FromMilliseconds(200)).Select(l => events.Last());
        //    last_e.Subscribe(async e =>
        //    {
        //        if (e!=null)
        //            await streamCall.RequestStream.WriteAsync(e);
        //    });
        //    //var last_e = events.Select(l => events.Last());
        //}

        static public void reload(bool mode, YEventList initEvents, bool makeCopy=true)
        {
            tname = Utils.CurrentTest.Oname + "_" + Utils.CurrentTest.Id;
            editInCopy = makeCopy;
            if (mode)
                startEditMode(initEvents);
            else
                endEditMode(initEvents);
        }

        static public async Task startEditMode(YEventList el)
        {
            
            //eventChanges = Observable.FromEventPattern((EventHandler<NotifyCollectionChangedEventArgs> ev) => new NotifyCollectionChangedEventHandler(ev),
            //    ev => events.CollectionChanged += ev,
            //    ev => events.CollectionChanged -= ev
            //);
            EditMode = true;
            if (streamCall != null) streamCall.Dispose();
            streamCall = Utils.MaxClient.DoStreamClient();

            if (subcribe_e != null) subcribe_e.Dispose();
            subcribe_e = last_e.Subscribe(async e => {
                if (e!=null)
                    await streamCall.RequestStream.WriteAsync(e);
            });

            if (editInCopy)
            {
                YEvent clone_e = new YEvent();
                clone_e.Yclone = new EClone();
                clone_e.Yclone.Oname = tname + surfix_clone_from;
                clone_e.Yclone.Cname = tname + surfix_tmp;
                //clone_e.Yclone.ConvertType = ConvertType.EditableMesh;
                await streamCall.RequestStream.WriteAsync(clone_e);
            }
            foreach (var e in el.Events)
            {
                await streamCall.RequestStream.WriteAsync(e);
            }
        }

        static public async Task endEditMode(YEventList el)
        {
            EditMode = false;
            if (subcribe_e != null) subcribe_e.Dispose();
            DialogResult dialogResult = MessageBox.Show("Do you want to save the changes?", "Apply changes", MessageBoxButtons.YesNo);
            if (dialogResult == DialogResult.Yes)
            {
                YEvent remove_e = new YEvent();
                remove_e.Del = new EDelete();
                remove_e.Del.Names.Add(tname + surfix_save_to);

                YEvent rename_e = new YEvent();
                rename_e.Rename = new ERename();
                rename_e.Rename.Oname = tname + surfix_tmp;
                rename_e.Rename.Nname = tname + surfix_save_to;

                YEvent clone_e = new YEvent();
                clone_e.Yclone = new EClone();
                clone_e.Yclone.Oname = tname + surfix_tmp;
                clone_e.Yclone.Cname = tname + surfix_save_to;
                clone_e.Yclone.CloneType = EClone.Types.CloneType.NodeInstance;
                YEventList yel = new YEventList();
                yel.Events.Add(remove_e);
                yel.Events.Add(clone_e);

                Utils.doManyEvent(yel);
                //await streamCall.RequestStream.WriteAsync(remove_e);
                //await streamCall.RequestStream.WriteAsync(rename_e);
                // remove save_to object and rename clone object to orginial object
            } else
            {
                //YEvent remove_e = new YEvent();
                //remove_e.Del = new EDelete();
                //remove_e.Del.Names.Add(tname + surfix_tmp);
                //await streamCall.RequestStream.WriteAsync(remove_e);
            }

            if (streamCall != null)
            {
                if (el!=null)
                {
                    foreach (var e in el.Events)
                    {
                        await streamCall.RequestStream.WriteAsync(e);
                    }
                }
                await streamCall.RequestStream.CompleteAsync();
                streamCall.Dispose();
            }
            
        }


        static public async Task addEvent(YEvent ye)
        {
            await streamCall.RequestStream.WriteAsync(ye);
        }

        static public void addObservableEvent(YEvent ye)
        {
            events.Add(ye);
        }
    } 
}
