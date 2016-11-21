using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
//using Y3D.Models;
using Y3D.Entities;
using Google.Protobuf;
using Autodesk.Max;
using ManagedServices;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Drawing;

namespace YMax.Utilities
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

        public static List<YObject> yoo = new List<YObject>();
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

        static void getMesh(IIGameScene gs, INodeTab node_to_xref)
        {
            List<string> oname_high = new List<string>();
           
            var meshes = gs.GetIGameNodeByType(Autodesk.Max.IGameObject.ObjectTypes.Mesh);
            try
            {
                for (int ix = 0; ix < meshes.Count; ++ix)
                {
#if MAX2017
                var indexer = ix;
#else
                    var indexer = new IntPtr(ix);
#endif
                    var meshNode = meshes[indexer];
                    IIGameMesh node = meshNode.IGameObject.AsGameMesh();
                    //var sb = meshNode.NodeID;
                    //Loader.Global.TheListener.EditStream.Printf(sb.ToString() + "\n", null);
                    //if (meshNode.NodeParent != null)
                    //    Loader.Global.TheListener.EditStream.Printf(meshNode.NodeParent.Name + ":" + meshNode.Name + "\n", null);
                    var isInit = node.InitializeData;
                    //Loader.Global.TheListener.EditStream.Printf(node.MaxObject.ClassName + "\n", null);
                    //if ((node.MaxObject.ClassName == "Editable Mesh") || (node.MaxObject.ClassName == "Editable Poly"))
                    if (node.MaxObject.ClassName != "Target")
                    {
                        var yo = new YObject();
                        yo.Mesh = new YMesh();
                        yo.Name = meshNode.Name;
                        yo.Mesh.NumFaces = node.NumberOfFaces;
                        var rid = -1;
                        rid = Utilities.YProject.face_range.getColorId(node.NumberOfFaces);
                        if (rid >= 0)
                        {
                            var f = Utilities.YProject.face_range.fr[rid];
                            meshNode.MaxNode.WireColor = Color.FromArgb(f.color.R, f.color.G, f.color.B);

                            //if ((rid== Utilities.YProject.face_range.fr.Count) || (rid== Utilities.YProject.face_range.fr.Count-1))
                            if (yo.Mesh.NumFaces>10000)
                            {
                                oname_high.Add(yo.Name);
                                node_to_xref.AppendNode(Loader.Core.GetINodeByName(yo.Name), true,1000);
                            }
                        }
                        else
                        {
                            meshNode.MaxNode.WireColor = Color.Gray;
                        }
                        if (node.MaxObject.ClassName == "Editable Mesh")
                            yo.Mesh.Mtype = YMesh.Types.MeshType.EditableMesh;
                        else if (node.MaxObject.ClassName == "Editable Poly")
                            yo.Mesh.Mtype = YMesh.Types.MeshType.EditablePoly;
                        else if (node.MaxObject.ClassName == "Box")
                            yo.Mesh.Mtype = YMesh.Types.MeshType.Box;
                        else if (node.MaxObject.ClassName == "Cone")
                            yo.Mesh.Mtype = YMesh.Types.MeshType.Cone;
                        else if (node.MaxObject.ClassName == "Sphere")
                            yo.Mesh.Mtype = YMesh.Types.MeshType.Sphere;
                        else if (node.MaxObject.ClassName == "GeoSphere")
                            yo.Mesh.Mtype = YMesh.Types.MeshType.GeoSphere;
                        else if (node.MaxObject.ClassName == "Cylinder")
                            yo.Mesh.Mtype = YMesh.Types.MeshType.Cylinder;
                        else if (node.MaxObject.ClassName == "Tube")
                            yo.Mesh.Mtype = YMesh.Types.MeshType.Tube;
                        else if (node.MaxObject.ClassName == "Pyramid")
                            yo.Mesh.Mtype = YMesh.Types.MeshType.Pyramid;
                        else if (node.MaxObject.ClassName == "Teapot")
                            yo.Mesh.Mtype = YMesh.Types.MeshType.Teapot;
                        else if (node.MaxObject.ClassName == "Plane")
                            yo.Mesh.Mtype = YMesh.Types.MeshType.Plane;
                        //if ((node.MaxObject.ClassName == "Editable Mesh") || (node.MaxObject.ClassName == "Editable Poly"))
                        //{
                        //    yo.Mesh.NumFaces = node.NumberOfPolygons;
                        //} else if (node.MaxObject.ClassName != "Target")
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
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                Loader.Global.TheListener.EditStream.Printf("\n" + e.Message);
                throw;
            }
            
            //Loader.Core.ConvertFlagedNodesToXRefs()
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

        public static void TestGroup()
        {
            var root = Loader.Core.RootNode;
            //Loader.Core.RootNode.SetGroupHeadOpen(true);
            //Loader.Core.RootNode.SetGroupMemberOpen(true);
            if (root.NumberOfChildren>0)
            {
                for (int i = 0; i < root.NumberOfChildren; i++)
                {
                    var n = root.GetChildNode(i);
                    Loader.Global.TheListener.EditStream.Printf(n.Name+ " G  \n", null);
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
            //IIGameNode az;
            //az.MaxNode.GetF
            //Loader.Core.FileOpen();
            //MaxscriptSDK.ExecuteMaxscriptCommand("max mtledit");
            //var root = Loader.Core.RootNode;
            Loader.Global.BroadcastNotification(SystemNotificationCode.Custom1); // rename object from C++
            var gameScene = Loader.Global.IGameInterface;
            gameScene.InitialiseIGame(false);
            var node_to_xref = Loader.Global.NodeTab.Create();
            TestGroup();
            getMesh(gameScene, node_to_xref);
            getCam(gameScene);
            getLight(gameScene);
            Loader.Core.OpenGroup(node_to_xref, false);
            Loader.Core.SelectNodeTab(node_to_xref,true,true);
            ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yxref_selected(); actionMan.executeAction 0 \"40876\"");
            //Loader.Global.ObjXRefManager.
            //Loader.Core.SaveNodesAsVersion("D:\\00.max", node_to_xref,2015);
            //var XrefManager = Loader.Global.IObjXRefManager8.Instance;



            //Loader.Global.IObjXRefRecord
            //XrefManager.SetRecordSrcFile(Loader.Global.IObjXRefRecord., "D:\\00.max");

            //var x = Loader.Global.IObjXRefRecord;

            //var meshes = gameScene.GetIGameNodeByType(Autodesk.Max.IGameObject.ObjectTypes.Mesh);
            //var m = Loader.Global.NewDefaultStdMat;
            //m.SetDiffuse(Loader.Global.Color.Create(255, 0, 255),0);
            //MessageBox.Show(meshes.Count.ToString());


        }

        public void MeshData2Resource()
        {
            // save to flatbuffer
        }

        public static void ztest()
        {

        }

    }
}
