import bpy
import bmesh
import os
import struct

def move_uv(move_u,move_v,ob,faceIds,selMode = True):
	scene = bpy.context.scene

	if ob.type == 'MESH':
		scene.objects.active = ob
		# obj = bpy.context.active_object
		bpy.ops.object.mode_set(mode='EDIT', toggle=False)
		# bpy.ops.mesh.select_all(action='SELECT')
		# bpy.ops.uv.smart_project()
		if selMode:
			bpy.ops.mesh.select_all(action='DESELECT')
		else:
			bpy.ops.mesh.select_all(action='SELECT')

		bpy.ops.object.mode_set(mode='OBJECT', toggle=False)
		for i in faceIds:
			ob.data.polygons[i-1].select = selMode

		bpy.ops.object.mode_set(mode='EDIT', toggle=False)

		bpy.ops.uv.smart_project()

		original_area = bpy.context.area.type
		bpy.ops.wm.context_set_enum(data_path="area.type",value="IMAGE_EDITOR")
		bpy.ops.uv.select_all(action='SELECT')


		# original_area = bpy.context.space_data.type
		# bpy.context.area.type = 'IMAGE_EDITOR'

		bpy.ops.transform.translate(value=(move_u, move_v, 0), constraint_axis=(True, False, False))
		bpy.ops.wm.context_set_enum(data_path="area.type",value=original_area)
		# bpy.ops.wm.context_set_enum(data_path="area.type",value="VIEW_3D")
		# bpy.ops.mesh.select_all(action='SELECT')
		# bm = bmesh.from_edit_mesh(ob.data)
		# for i in faceIds:
		# 	bm.faces[i].select = True
		# bmesh.update_edit_mesh(ob.data, True)
		# bpy.ops.uv.smart_project()
		#bpy.context.area.type = 'IMAGE_EDITOR'
		#bpy.context.screen.areas[4].type = 'IMAGE_EDITOR'
		# print(bpy.context.screen.areas[0].type)
		# bpy.ops.transform.translate(value=(1, 0, 0), constraint_axis=(True, False, False))
		# bpy.ops.uv.select_all(action='SELECT')
		# bpy.ops.object.mode_set(mode='OBJECT', toggle=False)

def smart_unwrap(ob,faceIds,selMode = True):
	scene = bpy.context.scene

	if ob.type == 'MESH':
		scene.objects.active = ob
		# obj = bpy.context.active_object
		bpy.ops.object.mode_set(mode='EDIT', toggle=False)
		if selMode:
			bpy.ops.mesh.select_all(action='DESELECT')
		else:
			bpy.ops.mesh.select_all(action='SELECT')


		bm = bmesh.from_edit_mesh(ob.data)
		bm.faces.ensure_lookup_table()
		for i in faceIds:
			bm.faces[i-1].select = selMode
		bmesh.update_edit_mesh(ob.data, True)
		bpy.ops.uv.smart_project()
		bpy.ops.object.mode_set(mode='OBJECT', toggle=False)


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

def getY3dObj():
	fw = "Y"
	up = "-Z"
	# objPath = os.path.join('D:\\','y3d.obj')
	faceIds = readY3DInfo('D:\\y3d.bin')
	objPath = 'D:\\y3d.obj'
	# bpy.ops.import_scene.obj(filepath='D:\\y3d2.obj')
	bpy.ops.import_scene.obj(filepath=objPath,use_split_objects=False, use_split_groups=False,use_smooth_groups=False, use_groups_as_vgroups=True, split_mode='OFF',axis_forward=fw, axis_up=up)
	# ob = bpy.data.objects['Y3D_temp']
	ob = bpy.data.objects[len(bpy.data.objects)-1]
	ob.select = True
	# ob.name = "Y3D_temp"
	if ob.type == 'MESH':
		smart_unwrap(ob,faceIds)
		smart_unwrap(ob,faceIds,False)
		bpy.ops.object.select_all(action='DESELECT')
		scene = bpy.context.scene
		scene.objects.active = ob
		ob.select = True
		# bpy.ops.export_scene.obj(filepath=objPath, use_selection=True, use_materials=False, axis_forward='-Z', axis_up='Y')
		bpy.ops.export_scene.obj(filepath='D:\\y3d.obj', use_selection=True,global_scale =1, use_materials=False, use_vertex_groups=True, axis_forward=fw, axis_up=up)


getY3dObj()
