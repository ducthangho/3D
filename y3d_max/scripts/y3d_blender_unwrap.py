import bpy
import bmesh
import os
import struct

def move_uv(move_u,move_v,ob,faceIds):
	scene = bpy.context.scene
	if ob.type == 'MESH':
		scene.objects.active = ob
		bpy.ops.object.mode_set(mode='EDIT', toggle=False)
		bpy.ops.mesh.select_all(action='SELECT')
		bpy.ops.uv.smart_project()
		bpy.ops.mesh.select_all(action='DESELECT')
		bpy.ops.object.mode_set(mode='OBJECT', toggle=False)
		for i in faceIds:
			print(i)
			ob.data.polygons[i-1].select = True
		bpy.ops.object.mode_set(mode='EDIT', toggle=False)
		bpy.ops.mesh.select_all(action='SELECT')
		# bm = bmesh.from_edit_mesh(ob.data)
		# for i in faceIds:
		# 	bm.faces[i].select = True
		# bmesh.update_edit_mesh(ob.data, True)
		bpy.ops.uv.smart_project()
		bpy.context.area.type = 'IMAGE_EDITOR'
		#bpy.context.screen.areas[4].type = 'IMAGE_EDITOR'
		# print(bpy.context.screen.areas[0].type)

		# for area in bpy.context.screen.areas :
		#     if area.type == 'IMAGE_EDITOR' :
		#         if area.spaces.active.image.type == 'RENDER_RESULT' :
		#             area.spaces.active.image = None


		bpy.ops.transform.translate(value=(1, 0, 0), constraint_axis=(True, False, False))
		bpy.ops.uv.select_all(action='SELECT')
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
	# objPath = os.path.join('D:\\','y3d.obj')
	faceIds = readY3DInfo('D:\\y3d.bin')
	print(faceIds[1])
	objPath = 'D:\\y3d.obj'
	# bpy.ops.import_scene.obj(objPath)
	bpy.ops.import_scene.obj(filepath=objPath)
	print("chzao")
	ob = bpy.data.objects[0]
	print (ob.name)
	if ob.type == 'MESH':
		# faceIds = readY3DInfo(os.path.join('D:\\','y3d.bin'))
		move_uv(1,0,ob,faceIds)
		bpy.ops.object.select_all(action='DESELECT')
		scene = bpy.context.scene
		scene.objects.active = ob
		ob.select = True
		bpy.ops.export_scene.obj(filepath=objPath, use_selection=True, axis_forward='-Z', axis_up='Y')


getY3dObj()
