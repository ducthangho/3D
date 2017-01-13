﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;
using Google.Protobuf;
using Autodesk.Max;
using ManagedServices;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Threading;
using System.IO;

namespace YMaxServer.Utils
{
    public class YOList
    {
        public static YGroup activeGroup = null;
        public static YArea activeArea = null;

        public static event Action<YGroup> OnUpdateByGroup;
        public static event Action<YArea> OnUpdateByArea;

        public static Dictionary<string, YGroup> mapGroup = new Dictionary<string, YGroup>();



        public static void updateByArea(YArea y)
        {
            OnUpdateByArea(y);
        }

        public static void updateByGroup(YGroup g)
        {
            OnUpdateByGroup(g);
        }

        //public static List<YObject> yoo = new List<YObject>();
        public static List<YArea> GetYAreas()
        {
            if (YOList.AllYArea.Count == 0)
            {
                var area = new YArea();
                area.Name = "Default Area";

                YOList.AllYArea.Add(area);
                YOList.activeArea = area;
            }
            return YOList.AllYArea;
        }
        static private List<YArea> AllYArea = new List<YArea>();

        public static YArea findAreaByOName(string oName)
        {
            foreach (var ya in AllYArea)
            {
                foreach (var o in ya.Objs)
                {
                    Loader.Global.TheListener.EditStream.Printf(o.Name + "\n", null);
                    if (o.Name == oName)
                    {
                        return ya;
                    }
                }
            }
            return null;

        }
        static void getMesh(IIGameScene gs, INodeTab node_to_xref)
        {
            //Loader.Core.LoadFromFile(ff, true);

            //Loader.Global.TheListener.EditStream.Printf("MESH:" + "\n", null);
            List<string> oname_high = new List<string>();
            List<string> oname_low = new List<string>();

            var low_nametab = Loader.Global.NameTab.Create();
            var high_nametab = Loader.Global.NameTab.Create();

            var node_low = Loader.Global.NodeTab.Create();

            var meshes = gs.GetIGameNodeByType(Autodesk.Max.IGameObject.ObjectTypes.Mesh);

                for (int ix = 0; ix < meshes.Count; ++ix)
                {
                try
                {
#if MAX2017
                var indexer = ix;
#else
                    var indexer = new IntPtr(ix);
#endif
                    //var meshNode = meshes[indexer];
                    var meshNode = meshes[ix];
                    //Loader.Global.TheListener.EditStream.Printf(meshNode.Name + "\n", null);
                    IIGameMesh node = meshNode.IGameObject.AsGameMesh();
                    //var sb = meshNode.NodeID;
                    //Loader.Global.TheListener.EditStream.Printf(sb.ToString() + "\n", null);
                    //if (meshNode.NodeParent != null)
                    //    Loader.Global.TheListener.EditStream.Printf(meshNode.NodeParent.Name + ":" + meshNode.Name + "\n", null);
                    var isInit = node.InitializeData;
                    ////Loader.Global.TheListener.EditStream.Printf(node.MaxObject.ClassName + "\n", null);
                    //if ((node.MaxObject.ClassName == "Editable Mesh") || (node.MaxObject.ClassName == "Editable Poly"))
                    var cname = node.MaxObject.ClassName;
                    YMesh.Types.MeshType mtype = YMesh.Types.MeshType.Unknown; // chuyen thanh None o protobuf scheme sau
                    if (cname == "Editable Mesh")
                        mtype = YMesh.Types.MeshType.EditableMesh;
                    else if (cname == "Editable Poly")
                        mtype = YMesh.Types.MeshType.EditablePoly;
                    else if (cname == "Box")
                        mtype = YMesh.Types.MeshType.Box;
                    else if (cname == "Cone")
                        mtype = YMesh.Types.MeshType.Cone;
                    else if (cname == "Sphere")
                        mtype = YMesh.Types.MeshType.Sphere;
                    else if (cname == "GeoSphere")
                        mtype = YMesh.Types.MeshType.GeoSphere;
                    else if (cname == "Cylinder")
                        mtype = YMesh.Types.MeshType.Cylinder;
                    else if (cname == "Tube")
                        mtype = YMesh.Types.MeshType.Tube;
                    else if (cname == "Pyramid")
                        mtype = YMesh.Types.MeshType.Pyramid;
                    else if (cname == "Teapot")
                        mtype = YMesh.Types.MeshType.Teapot;
                    else if (cname == "Plane")
                        mtype = YMesh.Types.MeshType.Plane;
                    //Loader.Global.TheListener.EditStream.Printf(cname + "\n", null);
                    //if ((cname == "Editable Mesh") || (cname == "Editable Poly") || (cname == "Camera") )
                    //if ((cname != "Target")&&(cname !="Dummy"))
                    if (mtype != YMesh.Types.MeshType.Unknown)
                    {
                        var yo = new YObject();
                        yo.Mesh = new YMesh();
                        yo.Name = meshNode.Name;
                        yo.Mesh.NumFaces = node.NumberOfFaces;
                        var rid = -1;
                        //rid = YProject.face_range.getColorId(node.NumberOfFaces);
                        //if (rid >= 0)
                        //{
                        //    var f = YProject.face_range.fr[rid];
                        //    meshNode.MaxNode.WireColor = Color.FromArgb(f.color.R, f.color.G, f.color.B);

                        //    //if ((rid== Utilities.YProject.face_range.fr.Count) || (rid== Utilities.YProject.face_range.fr.Count-1))
                        //    if (yo.Mesh.NumFaces>1000)
                        //    {
                        //        high_nametab.AddName(yo.Name);
                        //        node_to_xref.AppendNode(Loader.Core.GetINodeByName(yo.Name), true,1000);
                        //    }else
                        //    {
                        //        //oname_low.Add(yo.Name);
                        //        low_nametab.AddName(yo.Name);
                        //        node_low.AppendNode(Loader.Core.GetINodeByName(yo.Name), true, 1000);
                        //    }
                        //}
                        //else
                        //{
                        //    meshNode.MaxNode.WireColor = Color.Gray;
                        //}
                        //yo.Mesh.Mtype = mtype;

                        //if (cname == "Editable Mesh")
                        //    yo.Mesh.Mtype = YMesh.Types.MeshType.EditableMesh;
                        //else if (cname == "Editable Poly")
                        //    yo.Mesh.Mtype = YMesh.Types.MeshType.EditablePoly;
                        //else if (cname == "Box")
                        //    yo.Mesh.Mtype = YMesh.Types.MeshType.Box;
                        //else if (cname == "Cone")
                        //    yo.Mesh.Mtype = YMesh.Types.MeshType.Cone;
                        //else if (cname == "Sphere")
                        //    yo.Mesh.Mtype = YMesh.Types.MeshType.Sphere;
                        //else if (cname == "GeoSphere")
                        //    yo.Mesh.Mtype = YMesh.Types.MeshType.GeoSphere;
                        //else if (cname == "Cylinder")
                        //    yo.Mesh.Mtype = YMesh.Types.MeshType.Cylinder;
                        //else if (cname == "Tube")
                        //    yo.Mesh.Mtype = YMesh.Types.MeshType.Tube;
                        //else if (cname == "Pyramid")
                        //    yo.Mesh.Mtype = YMesh.Types.MeshType.Pyramid;
                        //else if (cname == "Teapot")
                        //    yo.Mesh.Mtype = YMesh.Types.MeshType.Teapot;
                        //else if (cname == "Plane")
                        //    yo.Mesh.Mtype = YMesh.Types.MeshType.Plane;
                        //if ((cname == "Editable Mesh") || (cname == "Editable Poly"))
                        //{
                        //    yo.Mesh.NumFaces = node.NumberOfPolygons;
                        //} else if (cname != "Target")
                        //{
                        //    yo.Mesh.NumFaces = node.NumberOfFaces;
                        //}
                        YPoint3 min = new YPoint3();
                        YPoint3 max = new YPoint3();
                        yo.Mesh.Bbox = new YBox3();
                        var bb = Loader.Global.Box3.Create();
                        node.GetBoundingBox(bb);
                        min.X = bb.Min.X;
                        min.Y = bb.Min.Y;
                        min.Z = bb.Min.Z;
                        max.X = bb.Max.X;
                        max.Y = bb.Max.Y;
                        max.Z = bb.Max.Z;
                        yo.Mesh.Bbox.Pmin = min;
                        yo.Mesh.Bbox.Pmax = max;

                        var tm = meshNode.GetObjectTM(0);
                        var t = tm.Translation;
                        yo.Pos = new YPoint3();
                        yo.Pos.X = t.X;
                        yo.Pos.Y = t.Y;
                        yo.Pos.Z = t.Z;

                        yo.Otype = ObjectType.Geometry;

                        if (meshNode.NodeParent!=null)
                        {
                            if (meshNode.NodeParent.IsGroupOwner)
                            {
                                //mapGroup[meshNode.NodeParent.Name].Objs.Add(yo);
                                if (mapGroup.ContainsKey(meshNode.NodeParent.Name))
                                {
                                    mapGroup[meshNode.NodeParent.Name].Objs.Add(yo);
                                }
                            }
                        } else
                        {
                            if (activeArea != null)
                                activeArea.Objs.Add(yo);
                        }

                        //if (activeArea != null)
                        //    activeArea.Objs.Add(yo);
                        //yoo.Add(yo);
                    }
                }
            catch (Exception e)
            {
                Loader.Global.TheListener.EditStream.Printf("\n Error:" + e.Message + "\n");
            }

        }
            Loader.Core.FileSaveNodes(node_to_xref, Path.Combine(YProject.oFileDir + Path.DirectorySeparatorChar + "y3d_data", YProject.oFileName + "_low0.max"));
            Loader.Core.MergeFromFile(Path.Combine(YProject.oFileDir, YProject.oFileName + ".max"), true, true, false, 3, high_nametab, 1, 1);
            Loader.Core.GetSelNodeTab(node_to_xref);
            Loader.Core.FileSaveNodes(node_to_xref, Path.Combine(YProject.oFileDir + Path.DirectorySeparatorChar + "y3d_data", YProject.oFileName + "_high.max"));
            ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yms.xref_low \"" + YProject.oFileDir + "\" \"" + YProject.oFileName + "\"");
            Loader.Core.MergeFromFile(Path.Combine(YProject.oFileDir, YProject.oFileName + ".max"), true, true, false, 3, low_nametab, 1, 1);
            var ss = Loader.Core.FileSave;
        }

        static void getCam(IIGameScene gs)
        {
            var cams = gs.GetIGameNodeByType(Autodesk.Max.IGameObject.ObjectTypes.Camera);
            if (cams.Count>0)
            {
                // switch to camera 1, Maximize Viewport Toggle, Views: Perspective User View, Viewport Visual Style Wireframe
                ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("actionMan.executeAction 0 \"160\"");
            }
            ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("max vpt persp user;actionMan.executeAction 0 \"551\"");
            Loader.Core.SetViewportMax(true);

            for (int i = 0; i < cams.Count; i++)
            {
#if MAX2017
                var indexer = i;
#else
                var indexer = new IntPtr(i);
#endif
                var cameraNode = cams[indexer];

#if !MAX2017
                Marshal.FreeHGlobal(indexer);
#endif
                var yo = new YObject();
                yo.Name = cameraNode.Name;
                yo.Otype = ObjectType.Camera;
                var tm = cameraNode.GetObjectTM(0);
                var t = tm.Translation;
                yo.Pos = new YPoint3();
                yo.Pos.X = t.X;
                yo.Pos.Y = t.Y;
                yo.Pos.Z = t.Z;
                if (activeArea != null)
                    activeArea.Objs.Add(yo);
            }
        }

        static void getLight(IIGameScene gs)
        {
            var lights = gs.GetIGameNodeByType(Autodesk.Max.IGameObject.ObjectTypes.Light);
            for (int i = 0; i < lights.Count; i++)
            {
#if MAX2017
                var indexer = i;
#else
                var indexer = new IntPtr(i);
#endif
                var lightNode = lights[indexer];

#if !MAX2017
                Marshal.FreeHGlobal(indexer);
#endif
                var yo = new YObject();
                yo.Name = lightNode.Name;
                yo.Otype = ObjectType.Light;
                var tm = lightNode.GetObjectTM(0);
                var t = tm.Translation;
                yo.Pos = new YPoint3();
                yo.Pos.X = t.X;
                yo.Pos.Y = t.Y;
                yo.Pos.Z = t.Z;
                if (activeArea != null)
                    activeArea.Objs.Add(yo);
            }
        }

        public static void InitGroup()
        {
            var root = Loader.Core.RootNode;
            //Loader.Core.RootNode.SetGroupHeadOpen(true);
            //Loader.Core.RootNode.SetGroupMemberOpen(true);
            if (root.NumberOfChildren>0)
            {
                for (int i = 0; i < root.NumberOfChildren; i++)
                {
                    var n = root.GetChildNode(i);
                    //Loader.Global.TheListener.EditStream.Printf(n.Name+ " G  \n", null);
                    if (n.IsGroupHead)
                    {
                        n.SetGroupHeadOpen(true);
                        //n.SetGroupMemberOpen(true);
                        var yg = new YGroup();
                        yg.Name = n.Name;
                        mapGroup.Add(n.Name, yg);
                        buildGroup(n, yg);
                        activeArea.Groups.Add(yg);
                    }
                }
            }
        }

        public static void buildGroup(IINode node, YGroup yg)
        {
            if (node.NumberOfChildren>0)
            {
                for (int i = 0; i < node.NumberOfChildren; i++)
                {
                    var child = node.GetChildNode(i);
                    if (child.IsGroupHead) {
                        YGroup nyg = new YGroup();
                        nyg.Name = child.Name;
                        yg.Children.Add(nyg);
                        mapGroup.Add(child.Name, nyg);
                        buildGroup(child, nyg);
                    }
                }
            }
        }

        public static void ObjectFromMax()
        {
            Loader.Global.BroadcastNotification(SystemNotificationCode.Custom1); // rename object from C++
            var gameScene = Loader.Global.IGameInterface;
            gameScene.InitialiseIGame(false);
            var node_to_xref = Loader.Global.NodeTab.Create();
            InitGroup();
            getCam(gameScene);
            getLight(gameScene);
            getMesh(gameScene, node_to_xref);
        }

        public void MeshData2Resource()
        {
            // save to flatbuffer
        }

        public static void resetData()
        {
            activeArea.Objs.Clear();
            activeGroup.Objs.Clear();
            mapGroup.Clear();
            AllYArea.Clear();
        }

        public static void make4test(string oname)
        {
            var n = Loader.Core.GetINodeByName(oname);

            var nt1 = Loader.Global.NodeTab.Create();
            var nt2 = Loader.Global.NodeTab.Create();
            var nt3 = Loader.Global.NodeTab.Create();
            var nt4 = Loader.Global.NodeTab.Create();

            Loader.Core.GetSelNodeTab(nt1);
            Loader.Core.CloneNodes(nt1, n.ObjOffsetPos, true, CloneType.Copy, nt2, nt2);
            Loader.Core.CloneNodes(nt1, n.ObjOffsetPos, true, CloneType.Copy, nt3, nt3);
            Loader.Core.CloneNodes(nt1, n.ObjOffsetPos, true, CloneType.Copy, nt4, nt4);

            nt2[0].ResetTransform(1, false);
            nt2[0].ResetPivot(1);

            nt2[0].Name = n.Name + "_low";
            nt3[0].Name = n.Name + "_high";
            nt4[0].Name = n.Name + "_cage";
            //var o = n.ObjectRef;
            ////Loader.Core.CreateObjectNode(o, "botay");
            //MessageBox.Show(n.XRefFileCount.ToString());
            //var nn2 = Loader.Global.IGameInterface.GetIGameNode(nt2[0]);
            //if (nn2.IGameObject.IsObjectXRef)
            //{
            //    MessageBox.Show("co xref");
            //}
            //else
            //{
            //    MessageBox.Show("thuong");
            //}
            //Loader.Core.Ma
            //if (n.ob)
            //{
            //    MessageBox.Show("co xref");
            //    //Loader.Global.IObjXRefManager18.Instance_.MergeXRefItemsIntoScene(nt2[0].ref)
            //} else
            //{
            //    MessageBox.Show("thuong");
            //}
        }

        public static void testExport(Object x)
        {
            if (x is YObject)
            {
                var y = (YObject)x;
                //string cmd = "select $" + y.Name + ";";
                ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yms.test_export \"" + YProject.oFileDir + "\" \"" + y.Name + "\"");
                //Loader.Core.ex
            }
        }

        public static void selectByNode(IINode n)
        {

        }

        public static void testz()
        {
            var a = "1";
            var b = "2";
            ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yms.pre_optimize \"" + a+ "\" \"" + b + "\"");
        }

    }
}