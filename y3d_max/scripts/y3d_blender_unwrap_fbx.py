import bpy
import struct
import bmesh

ExportFolder = "F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\export\\blender\\"
binFile = "ImportantFace.bin"
exportFile = "export.fbx"
importFile = "3DSexport.fbx"
def importFBX(fileName):
	bpy.ops.import_scene.fbx(filepath=fileName, axis_forward='-Z', axis_up='Y', directory="", filter_glob="*.fbx", ui_tab='MAIN', use_manual_orientation=False,
			global_scale=1.0, bake_space_transform=False, use_custom_normals=True,use_image_search=True, use_alpha_decals=False, decal_offset=0.0,
			use_anim=True, anim_offset=1.0, use_custom_props=True, use_custom_props_enum_as_string=True, ignore_leaf_bones=False, force_connect_children=False, 
			automatic_bone_orientation=False, primary_bone_axis='Y', secondary_bone_axis='X', use_prepost_rot=True  				
		)


def deleteAllObject():
	if bpy.ops.object.mode_set.poll():
	   bpy.ops.object.mode_set(mode='OBJECT')
	   bpy.ops.object.select_all(action='SELECT')
	   bpy.ops.object.delete(use_global=True)
			  
	
def readY3DInfo(filePath):
	faceIds = []
	f = open(filePath, "rb")
	n = struct.unpack('i',f.read(4))[0]
	print(n)
	while n>0:
		n = n-1
		fid = struct.unpack('i',f.read(4))[0]
		faceIds.append(fid)
		print(fid)
	f.close()
	return faceIds

def readY3DInfo(fileName):
	faceIds = []
	f = open(fileName, "rb")
	n = struct.unpack('i',f.read(4))[0]
	print(n)
	while n>0:
		n = n-1
		fid = struct.unpack('i',f.read(4))[0]
		faceIds.append(fid)
		#print(fid)
	f.close()
	return faceIds

def exportFBX(exportFile):
	filepath = ExportFolder+exportFile
	bpy.ops.export_scene.fbx(filepath=filepath, check_existing=True, axis_forward='-Z', axis_up='Y',
		filter_glob="*.fbx", version='BIN7400',ui_tab='MAIN',use_selection=True,
		   global_scale=1.0, apply_unit_scale=True, bake_space_transform=False, object_types={'MESH'},
			use_mesh_modifiers=True, mesh_smooth_type='OFF',	use_mesh_edges=False, use_tspace=False, use_custom_props=False, add_leaf_bones=True, primary_bone_axis='Y', secondary_bone_axis='X', use_armature_deform_only=False, bake_anim=True, bake_anim_use_all_bones=True, bake_anim_use_nla_strips=True, bake_anim_use_all_actions=True, bake_anim_force_startend_keying=True, bake_anim_step=1.0, bake_anim_simplify_factor=1.0, use_anim=True, use_anim_action_all=True, use_default_take=True, use_anim_optimize=True, anim_optimize_precision=6.0, path_mode='AUTO', embed_textures=False, batch_mode='OFF', use_batch_own_dir=True, use_metadata=True)


def smart_unwrap(obj,importantFace):
	bpy.context.object.data.uv_textures.active_index = 1
	bpy.ops.mesh.select_all(action='DESELECT')

	me = obj.data
	bm = bmesh.from_edit_mesh(me)
	bm.faces.ensure_lookup_table()
	for id in importantFace:
		print (id-1)
		bm.faces[id-1].select = True
	bmesh.update_edit_mesh(obj.data, True)
	bpy.ops.uv.smart_project()

	bpy.ops.mesh.select_all(action='INVERT')
	bpy.ops.uv.smart_project()
	
def unWrapObjectFromBlender(fileName):
	deleteAllObject()
	importFBX(ExportFolder+fileName)
	importantFace = readY3DInfo(ExportFolder+binFile)

	obj = bpy.data.objects[0]
	obj.select = True
	scene = bpy.context.scene
	scene.objects.active = obj
	bpy.ops.object.mode_set(mode='EDIT')
	
	smart_unwrap(obj,importantFace)
	exportFBX(exportFile)
	

unWrapObjectFromBlender(importFile)

# original_area = bpy.context.area.type
# bpy.context.area.type = 'IMAGE_EDITOR'
# bpy.ops.mesh.select_all(action='SELECT')
# bpy.ops.uv.select_all(action='SELECT')
# bpy.ops.transform.translate(value=(0.125, 0, 0), constraint_axis=(True, False, False))
# bpy.context.area.type = original_area
