import bpy
import struct
import bmesh


ExportFolder = "F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\export\\blender\\"
importFileName  = "3dsNodes.fbx"
exportFileName = "blenderNodes.fbx"
binarySelectedFace = "3dsSelectedFace.bin"
binaryFacesObjects = "3dsFaces.bin"

def importFBX(filePath):
	 bpy.ops.import_scene.fbx(filepath=filePath, axis_forward='-Z', axis_up='Y', directory="", filter_glob="*.fbx", ui_tab='MAIN', use_manual_orientation=False,
			global_scale=1.0, bake_space_transform=False, use_custom_normals=True,use_image_search=True, use_alpha_decals=False, decal_offset=0.0,
			use_anim=True, anim_offset=1.0, use_custom_props=True, use_custom_props_enum_as_string=True, ignore_leaf_bones=False, force_connect_children=False, 
			automatic_bone_orientation=False, primary_bone_axis='Y', secondary_bone_axis='X', use_prepost_rot=True  				
		)

def readSelectefFace(binFile):
	faceIds = []
	f = open(binFile,"rb")
	while True:
		n = f.read(4);
		if not n:
			break;
		fid = struct.unpack('i',n)[0]
		faceIds.append(fid)
		print (fid)
	f.close()
	return faceIds
# importFBX(ExportFolder+fileName)

def readcstr(f):
	buf = bytearray()
	while True:
		b = f.read(1)
		if not b or b == b'\x00':
			return str(buf,'utf8')
		else:
			buf = buf + b

def deleteAllObject():
	if bpy.ops.object.mode_set.poll():
		bpy.ops.object.mode_set(mode='OBJECT')
		bpy.ops.object.select_all(action='SELECT')
		bpy.ops.object.delete(use_global=True)

def unwrapFromBlender():
	deleteAllObject()
	importFBX(ExportFolder+importFileName)
	
	selectedFace = readSelectefFace(ExportFolder+binarySelectedFace)
	obj = bpy.data.objects[0]
	obj.select = True
	scene = bpy.context.scene
	scene.objects.active = obj
	bpy.ops.object.mode_set(mode='EDIT')

	# bpy.context.object.data.uv_textures.active_index = 1
	bpy.ops.mesh.select_all(action='DESELECT')

	me = obj.data
	bm = bmesh.from_edit_mesh(me)
	bm.faces.ensure_lookup_table()

	bpy.context.tool_settings.mesh_select_mode = (False, False, True)

	for id in selectedFace:
		bm.faces[id-1].select = True
	bm.free()
	bmesh.update_edit_mesh(obj.data, True)

	bpy.ops.uv.smart_project()

	me = obj.data
	bm = bmesh.from_edit_mesh(me)
	bm.faces.ensure_lookup_table()
	bpy.ops.mesh.select_all(action='SELECT')
	for id in selectedFace:
		bm.faces[id-1].select = False
	bm.free()
	bpy.ops.uv.smart_project()

def test(binFile):
	f = open(binFile,"rb")

	n = readcstr(f)
	print (n)
	print (struct.unpack('i',f.read(4))[0])

	n = readcstr(f)
	print (n)
	print (struct.unpack('i',f.read(4))[0])

	n = readcstr(f)
	print (n)
	print (struct.unpack('i',f.read(4))[0])

	n = readcstr(f)
	print (n)
	a = f.read(4)
	print (len(n))

def separateObject():
	bpy.ops.object.mode_set(mode='OBJECT')
	bpy.ops.object.duplicate(linked=False, mode='TRANSLATION')

	nodes = []
	f = open(ExportFolder+binaryFacesObjects,"rb")
	while(True):
		name = readcstr(f)
		if len(name) == 0 :
			break
		numFace = struct.unpack('i',f.read(4))[0]
		nodes.append([name,numFace])
		print ([name,numFace])
	f.close()
	
	for node in nodes:
		numFace = node[1]
		name = node[0]
		print (name)

def test2(binFile):
	unwrapFromBlender()

def test3():
	unwrapFromBlender()
	separateObject()
	# test(ExportFolder+binaryFacesObjects)

# test2(ExportFolder+binarySelectedFace)
# test(ExportFolder+binaryFacesObjects)
test3()