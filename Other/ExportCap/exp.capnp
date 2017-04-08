# 3d.capnp
@0xcbac686543505bdb;
$import "/capnp/c++.capnp".namespace("format3d");
struct Point3 @0xc26571d6c4078489 {  # 16 bytes, 0 ptrs
  x @0 :Float32;  # bits[0, 32)
  y @1 :Float32;  # bits[32, 64)
  z @2 :Float32;  # bits[64, 96)
}
struct Box3 @0xccfdf72800028cb8 {  # 0 bytes, 2 ptrs
  pmin @0 :Point3;  # ptr[0]
  pmax @1 :Point3;  # ptr[1]
}
struct MNVert @0xd548f2fa0058bb48 {  # 24 bytes, 0 ptrs
  flags @0 :UInt32;  # bits[0, 32)
  x @1 :Float32;  # bits[32, 64)
  y @2 :Float32;  # bits[64, 96)
  z @3 :Float32;  # bits[96, 128)
  orig @4 :Int32;  # bits[128, 160)
}
struct MNEdge @0x9db77979bf59ad4c {  # 24 bytes, 0 ptrs
  flags @0 :UInt32;  # bits[0, 32)
  v1 @1 :Int32;  # bits[32, 64)
  v2 @2 :Int32;  # bits[64, 96)
  f1 @3 :Int32;  # bits[96, 128)
  f2 @4 :Int32;  # bits[128, 160)
  track @5 :Int32;  # bits[160, 192)
}
struct MNFace @0xbab571806e4c494f {  # 16 bytes, 6 ptrs
  flags @0 :UInt32;  # bits[0, 32)
  deg @1 :UInt8;  # bits[32, 40)
  vtx @2 :List(Int32);  # ptr[0]
  edg @3 :List(Int32);  # ptr[1]
  diag @4 :List(Int32);  # ptr[2]
  smGroup @5 :UInt8;  # bits[40, 48)
  material @6 :UInt8;  # bits[48, 56)
  track @7 :Int32;  # bits[64, 96)
  visedg @8 :List(Bool);  # ptr[3]
  edgsel @9 :List(Bool);  # ptr[4]
  bndedg @10 :List(Bool);  # ptr[5]
}
struct MNFaceStripped @0xda63927eb0a7d72f {  # 8 bytes, 1 ptrs
  deg @0 :Int8;  # bits[0, 8)
  vtx @1 :List(Int32);  # ptr[0]
  smGroup @2 :UInt8;  # bits[8, 16)
  material @3 :UInt8;  # bits[16, 24)
}
struct TriFace @0xea04a9b6249da4a1 {  # 16 bytes, 0 ptrs
  v0 @0 :Int32;  # bits[0, 32)
  v1 @1 :Int32;  # bits[32, 64)
  v2 @2 :Int32;  # bits[64, 96)
  smGroup @3 :UInt8;  # bits[96, 104)
  material @4 :UInt8;  # bits[104, 112)
}
struct QuadFace @0xb7cf5512b90c3a5f {  # 24 bytes, 0 ptrs
  v0 @0 :Int32;  # bits[0, 32)
  v1 @1 :Int32;  # bits[32, 64)
  v2 @2 :Int32;  # bits[64, 96)
  v3 @3 :Int32;  # bits[96, 128)
  smGroup @4 :UInt8;  # bits[128, 136)
  material @5 :UInt8;  # bits[136, 144)
}
struct PerData @0x9b7dc22fdfa402e6 {  # 16 bytes, 1 ptrs
  dnum @0 :Int32;  # bits[0, 32)
  type @1 :Int32;  # bits[32, 64)
  alloc @2 :Int32;  # bits[64, 96)
  data @3 :Data;  # ptr[0]
}
struct MNMapFace @0xcbd71226cf43e22c {  # 0 bytes, 1 ptrs
  tv @0 :List(Int32);  # ptr[0]
}
struct MNMap @0xa142aaa1dbd4ee7c {  # 8 bytes, 2 ptrs
  f @0 :List(Int32);  # ptr[0]
  v @1 :List(Point3);  # ptr[1]
  channel @2 :Int8;  # bits[0, 8)
}
struct MNNormalSpec @0xa4c09a2a48491986 {  # 0 bytes, 2 ptrs
  mpNormal @0 :List(Point3);  # ptr[0]
  mpFace @1 :List(List(Int32));  # ptr[1]
}
struct MNMesh @0x85cd2621efd33144 {  # 8 bytes, 16 ptrs
  v @0 :List(MNVert);  # ptr[0]
  e @1 :List(MNEdge);  # ptr[1]
  f @2 :List(MNFace);  # ptr[2]
  m @3 :List(MNMap);  # ptr[3]
  vd @4 :List(PerData);  # ptr[4]
  vdSupport @5 :List(Bool);  # ptr[5]
  ed @6 :List(PerData);  # ptr[6]
  edSupport @7 :List(Bool);  # ptr[7]
  selLevel @8 :Int32;  # bits[0, 32)
  dispFlags @9 :Int32;  # bits[32, 64)
  vedg @10 :List(Int32);  # ptr[8]
  vfac @11 :List(Int32);  # ptr[9]
  bdgBox @12 :Box3;  # ptr[10]
  hmap @13 :List(MNMap);  # ptr[11]
  normals @14 :MNNormalSpec;  # ptr[12]
  vSel @15 :List(Bool);  # ptr[13]
  eSel @16 :List(Bool);  # ptr[14]
  fSel @17 :List(Bool);  # ptr[15]
}
struct MNMeshStripped @0xd97ae6df6ee01f73 {  # 8 bytes, 10 ptrs
  v @0 :List(Point3);  # ptr[0]
  f @1 :List(MNFaceStripped);  # ptr[1]
  selLevel @2 :Int32;  # bits[0, 32)
  dispFlags @3 :Int32;  # bits[32, 64)
  bdgBox @4 :Box3;  # ptr[2]
  m @5 :List(MNMap);  # ptr[3]
  hmap @6 :List(MNMap);  # ptr[4]
  vSel @7 :List(Bool);  # ptr[5]
  eSel @8 :List(Bool);  # ptr[6]
  fSel @9 :List(Bool);  # ptr[7]
  mpNormal @10 :List(Point3);  # ptr[8]
  mpFace @11 :List(List(Int32));  # ptr[9]
}
struct PolyObj @0x979fe08790a6d18e {  # 8 bytes, 2 ptrs
  name @0 :Text;  # ptr[0]
  type @1 :Int8;  # bits[0, 8)
  mmesh @2 :MNMesh;  # ptr[1]
}
struct PolyObjStripped @0xf7d06c305524f669 {  # 8 bytes, 2 ptrs
  name @0 :Text;  # ptr[0]
  type @1 :Int8;  # bits[0, 8)
  mmesh @2 :MNMeshStripped;  # ptr[1]
}
struct ListObjects @0xd9c7fcb57f5143bb {  # 0 bytes, 2 ptrs
  polyObjs @0 :List(PolyObjStripped);  # ptr[0]
  triObjs @1 :List(PolyObj);  # ptr[1]
}
struct Mesh @0xde34b57feb18bf7c {  # 16 bytes, 10 ptrs
  v @0 :List(Point3);  # ptr[0]
  faces :group {
    union {  # tag bits [0, 16)
      f3 @1 :List(TriFace);  # ptr[1], union tag = 0
      f4 @2 :List(QuadFace);  # ptr[1], union tag = 1
    }
  }
  selLevel @3 :Int32;  # bits[32, 64)
  dispFlags @4 :Int32;  # bits[64, 96)
  bdgBox @5 :Box3;  # ptr[2]
  m @6 :List(MNMap);  # ptr[3]
  vSel @7 :List(Bool);  # ptr[4]
  eSel @8 :List(Bool);  # ptr[5]
  fSel @9 :List(Bool);  # ptr[6]
  eVis @10 :List(Bool);  # ptr[7]
  mpNormal @11 :List(Point3);  # ptr[8]
  mpFace @12 :List(Int32);  # ptr[9]
}
struct Object @0x91eff6b5ec2e89a9 {  # 8 bytes, 2 ptrs
  name @0 :Text;  # ptr[0]
  type @1 :Int8;  # bits[0, 8)
  mesh @2 :Mesh;  # ptr[1]
}
struct Objects @0xa3063ea15a0ad310 {  # 0 bytes, 1 ptrs
  objs @0 :List(Object);  # ptr[0]
}
struct Anything @0x98cf9f3feec0aa21 {  # 8 bytes, 4 ptrs
  items :group {
    union {  # tag bits [16, 32)
      empty @0 :Bool;  # bits[0, 1), union tag = 0
      obj @1 :Object;  # ptr[0], union tag = 1
      objs @2 :List(Object);  # ptr[0], union tag = 2
      mesh @3 :Mesh;  # ptr[0], union tag = 3
      normals :group {  # union tag = 4
        mpNormal @4 :List(Point3);  # ptr[0]
        mpFace @5 :List(Int32);  # ptr[1]
      }
      selection :group {  # union tag = 5
        vSel @6 :List(Bool);  # ptr[0]
        eSel @7 :List(Bool);  # ptr[1]
        fSel @8 :List(Bool);  # ptr[2]
        eVis @9 :List(Bool);  # ptr[3]
      }
      uvmaps @10 :List(MNMap);  # ptr[0], union tag = 6
    }
  }
}
