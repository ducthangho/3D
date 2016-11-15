# Copyright (c) 2013-2014 Sandstorm Development Group, Inc. and contributors
# Licensed under the MIT License:
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

@0xcbac686543505bdb;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("format3d");

struct Point3 {  
  x @0 : Float32;
  y @1 : Float32;
  z @2 : Float32;
}

struct Box3 {
  pmin @0 : Point3;
  pmax @1 : Point3;
}


struct TriFace {
  v0 @0 : Int32;
  v1 @1 : Int32;
  v2 @2 : Int32;
  smGroup @3 : UInt8; #This contains the smoothing groups assigned to this face. 
  material @4 : UInt8; #This is the material ID assigned to this face. 
}

struct MNFaceInfo {
  deg @0 : Int32;  
  smGroup @1 : UInt8; #This contains the smoothing groups assigned to this face. 
  material @2 : UInt8; #This is the material ID assigned to this face. 
}


struct MNMap {  
  f @0 : List(Int32); #The array of map faces. List(MNMapFace)
  v @1 : List(Point3); #The array of UVVerts.
  channel @2 : Int8; 
}


struct Mesh{
  v @0 : List(Point3);
  faces : union{
	f3 @1 : List(TriFace); 
	mnfaces : group {
		info @2 : List(MNFaceInfo);
		f @3 : List(Int32);
	}
	
  }
  
  selLevel @4 : Int32;
  dispFlags @5 : Int32; 
  bdgBox @6 : Box3;
  m @7 : List(MNMap);  
  vSel @8 : List(Bool);
  eSel @9 : List(Bool);
  fSel @10 : List(Bool);
  eVis @11 : List(Bool);
  mpNormal @12 : List(Point3);  #Normal
  mpFace @13 : List(Int32);  
}

struct Object{
  name @0:Text;  
  type @1 : Int8;
  mesh @2 : Mesh;  
  tm @3: List(Float32);
  iFlags @4: UInt32;
  parent @5:Text;
  groupType @6: UInt8;
  group : union {
	index @7 : Int32;
	grName @8 : Text;
  }
  handle @9 : UInt32;  
  pivot @10: List(Float32);
}

struct Objects{
	objs @0: List(Object);
}

struct Anything{
	items : union {
		empty @0 : Bool;
		obj @1 : Object;
		objs @2 : List(Object);
		mesh @3 : Mesh;
		normals : group {
			mpNormal @4 : List(Point3);  #Normal
			mpFace @5 : List(Int32);
		}
		selection : group {
			vSel @6 : List(Bool);
			eSel @7 : List(Bool);
			fSel @8 : List(Bool);
			eVis @9 : List(Bool);
		}
		uvmaps @10 : List(MNMap); 
	}
	masterScale @11 : Float32;#Master Scale
	groupInfo @12 : List(Text);
}