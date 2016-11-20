import bpy
import struct
import bmesh
import sys

# ExportFolder = "F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\export\\blender\\"
# ExportFolder = "D:\\"

importFileName  = "3dsNodes.fbx"
exportFileName = "blenderNodes.fbx"
importFileNameLowPoly = "3dsNodesHightPoly.fbx"
exportFileNameLowPoly = "blenderNodesLowPoly.fbx"
lowPolyFileName = "blenderLowPoly.fbx"
binarySelectedFace = "3dsSelectedFace.bin"
binaryFacesObjects = "3dsFaces.bin"
# savedFile = ExportFolder+"3dsMax.blend"
# savedFileAfterMakeLowPoly = ExportFolder + "saveFileAfterMakeLowPoly.blend"
# argMakeLowPoly = 'makeLowPoly'
# argUnwrap = 'unwrap'

def importFBX(filePath):
	print(filePath)
	bpy.ops.import_scene.fbx(filepath=filePath, axis_forward='-Z', axis_up='Y', directory="", filter_glob="*.fbx", ui_tab='MAIN', use_manual_orientation=False,
			global_scale=1.0, bake_space_transform=False, use_custom_normals=True,use_image_search=True, use_alpha_decals=False, decal_offset=0.0,
			use_anim=True, anim_offset=1.0, use_custom_props=True, use_custom_props_enum_as_string=True, ignore_leaf_bones=False, force_connect_children=False, 
			automatic_bone_orientation=False, primary_bone_axis='Y', secondary_bone_axis='X', use_prepost_rot=True  				
		)

def exportFBX(filepath):
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
	f.close()
	return faceIds

def readcstr(f):
	buf = bytearray()
	while True:
		b = f.read(1)
		if not b or b == b'\x00':
			return str(buf,'utf8')
		else:
			buf = buf + b

def deleteAllObject():
	if(len(bpy.data.objects)!=0):
		if bpy.ops.object.mode_set.poll():
			bpy.ops.object.mode_set(mode='OBJECT')
			bpy.ops.object.select_all(action='SELECT')
			bpy.ops.object.delete(use_global=True)
		else:
			bpy.data.objects[0].select = True
			bpy.ops.object.select_all(action='SELECT')
			bpy.ops.object.delete(use_global=True)

def unwrapFromBlender(ExportFolder = "D:\\"):
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

	exportFBX(ExportFolder+exportFileName)
	bpy.ops.wm.save_as_mainfile(filepath=(ExportFolder+"3dsMax.blend"))

def makeLowPoly(ExportFolder = "D:\\", ratio = None):
	deleteAllObject()
	importFBX(ExportFolder+importFileNameLowPoly)
	if ratio == None:
		for obj in bpy.data.objects:
				numFace = len(obj.data.polygons)
				if numFace < 576:
					continue
				bpy.context.scene.objects.active = obj
				bpy.ops.object.modifier_add(type='DECIMATE')
				if numFace > 991:
					bpy.context.object.modifiers["Decimate"].ratio = 0.09
				elif numFace > 575:
					bpy.context.object.modifiers["Decimate"].ratio = 0.45
				bpy.ops.object.modifier_apply(apply_as='DATA',modifier='Decimate')
	else:
		for obj in bpy.data.objects:
				bpy.context.scene.objects.active = obj
				bpy.ops.object.modifier_add(type='DECIMATE')
				bpy.context.object.modifiers["Decimate"].ratio = ratio
				bpy.ops.object.modifier_apply(apply_as='DATA',modifier='Decimate')
	exportFBX(ExportFolder+exportFileNameLowPoly)	
	bpy.ops.wm.save_as_mainfile(filepath=(ExportFolder + "saveFileAfterMakeLowPoly.blend"))

# def test():
# 	argv = sys.argv
# 	argv = argv[argv.index("--") + 1:]  # get all args after "--"

# 	if(argv[0] == argMakeLowPoly):
# 		if(len(argv) == 1):
# 			makeLowPoly()
# 		else:
# 			ratio = float(argv[1])
# 			makeLowPoly(ratio)

# 	if(argv[0] == argUnwrap):
# 		unwrapFromBlender()
	
# test()

def yrun():
	argv = sys.argv
	argv = argv[argv.index("--") + 1:]  # get all args after "--"
	ExportFolder = argv[0] + "\\" # truyen tham so la duong dan temp cua 3dmax
	print(ExportFolder)
	if(argv[1] == "makeLowPoly"):
		if(len(argv) == 3):
			ratio = float(argv[2])
			makeLowPoly(ExportFolder,ratio)			
		else:
			makeLowPoly(ExportFolder)		
	elif (argv[1] == "unwrap"):
			unwrapFromBlender()		
yrun()