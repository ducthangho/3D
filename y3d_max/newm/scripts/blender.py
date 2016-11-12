import bpy
import struct
import bmesh

ExportFolder = "F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\export\\blender\\"
importFileName  = "3dsNodes.fbx"
exportFileName = "blenderNodes.fbx"
lowPolyFileName = "blenderLowPoly.fbx"
binarySelectedFace = "3dsSelectedFace.bin"
binaryFacesObjects = "3dsFaces.bin"
saveFile = "F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\export\\blender\\3dsMax.blend"


def importFBX(filePath):
	 bpy.ops.import_scene.fbx(filepath=filePath, axis_forward='-Z', axis_up='Y', directory="", filter_glob="*.fbx", ui_tab='MAIN', use_manual_orientation=False,
			global_scale=1.0, bake_space_transform=False, use_custom_normals=True,use_image_search=True, use_alpha_decals=False, decal_offset=0.0,
			use_anim=True, anim_offset=1.0, use_custom_props=True, use_custom_props_enum_as_string=True, ignore_leaf_bones=False, force_connect_children=False, 
			automatic_bone_orientation=False, primary_bone_axis='Y', secondary_bone_axis='X', use_prepost_rot=True  				
		)

def exportFBX(exportFile):
	filepath = ExportFolder+exportFile
	bpy.ops.export_scene.fbx(filepath=filepath, check_existing=True, axis_forward='-Z', axis_up='Y',
		filter_glob="*.fbx", version='BIN7400',ui_tab='MAIN',use_selection=True,
		   global_scale=1.0, apply_unit_scale=True, bake_space_transform=False, object_types={'MESH'},
			use_mesh_modifiers=True, mesh_smooth_type='OFF',	use_mesh_edges=False, use_tspace=False, use_custom_props=False, add_leaf_bones=True, primary_bone_axis='Y', secondary_bone_axis='X', use_armature_deform_only=False, bake_anim=True, bake_anim_use_all_bones=True, bake_anim_use_nla_strips=True, bake_anim_use_all_actions=True, bake_anim_force_startend_keying=True, bake_anim_step=1.0, bake_anim_simplify_factor=1.0, use_anim=True, use_anim_action_all=True, use_default_take=True, use_anim_optimize=True, anim_optimize_precision=6.0, path_mode='AUTO', embed_textures=False, batch_mode='OFF', use_batch_own_dir=True, use_metadata=True)


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
	obj.name += "_blend"
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
	bpy.context.active_object.name = "temp"

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

	bpy.context.tool_settings.mesh_select_mode = (False, False, True)

	count = 0
	a = len(nodes)

	for node in nodes:
		numFace = node[1]
		name = node[0]

		count += 1
		if count > a - 1 :
			bpy.context.active_object.name = name
			bpy.context.active_object["LowPoly"] = 1
			bpy.context.active_object["NumFace"] = numFace
			break
		obj = bpy.context.active_object

		if bpy.ops.object.mode_set.poll():
			bpy.ops.object.mode_set(mode='OBJECT')
			bpy.ops.object.select_all(action='DESELECT')

		bpy.ops.object.mode_set(mode='EDIT')
		bpy.ops.mesh.select_all(action='DESELECT')

		me = obj.data
		bm = bmesh.from_edit_mesh(me)
		bm.faces.ensure_lookup_table()
		
		for x in range(0,numFace):
			bm.faces[x].select = True	

		bpy.ops.mesh.separate(type='SELECTED')
		bpy.context.selected_objects[0].name = name
		bpy.context.selected_objects[0]["LowPoly"] = 1
		bpy.context.selected_objects[0]["NumFace"] = numFace
		bm.free()
		bmesh.update_edit_mesh(obj.data, True)

def makeLowPoly():
	bpy.ops.object.mode_set(mode='OBJECT')
	for obj in bpy.data.objects:
		if "LowPoly" in obj:
			bpy.context.scene.objects.active = obj
			numFace = obj["NumFace"]
			if numFace < 576:
				continue
			bpy.ops.object.modifier_add(type='DECIMATE')
			print ("Numface: "+str(numFace))
			if numFace > 991:
				bpy.context.object.modifiers["Decimate"].ratio = 0.09
			elif numFace > 575:
				bpy.context.object.modifiers["Decimate"].ratio = 0.45
			bpy.ops.object.modifier_apply(apply_as='DATA',modifier='Decimate')

def test2(binFile):
	unwrapFromBlender()

def test3():
	unwrapFromBlender()
	exportFBX(exportFileName)
	separateObject()
	makeLowPoly()
	bpy.ops.wm.save_as_mainfile(filepath=saveFile)
	
	if bpy.ops.object.mode_set.poll():
		bpy.ops.object.mode_set(mode='OBJECT')
		bpy.ops.object.select_all(action='DESELECT')
	for obj in bpy.data.objects:
		if "LowPoly" in obj:
			obj.select = True
	exportFBX(lowPolyFileName)
	# test(ExportFolder+binaryFacesObjects)

# test2(ExportFolder+binarySelectedFace)
# test(ExportFolder+binaryFacesObjects)
test3()