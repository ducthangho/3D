(
global exportObj
global importObj
local infoFilePath = "D:\\\y3d.bin"
local OBJFilePath = "D:\\y3d.obj"
local blenderPath = "D:\\app\blender\blender.exe"
-- local pythonFile = "$userScripts/scripts/y3d_blender_unwrap.py"
local pythonFile = getDir #userscripts+"\scripts\y3d_blender_unwrap.py"
struct yface (index, f_area, area_uvw, fw, fh, udim, x, y)

fn getUVFromBlender obj faceArray =
	(
		oldName = obj.name
		obj.name = "Y3D_temp"
		binFile = fopen infoFilePath "wb"
		WriteLong binFile faceArray.count #unsigned
		for i = 1 to faceArray.count do
			(
				vv = faceArray[i].index as integer
				WriteLong binFile vv
			)
		fclose binFile
		exportFile OBJFilePath #noPrompt selectedOnly:true
		obj.name = oldName
		cmd = (blenderPath+ " -noaudio --factory-startup -b --python "+ pythonFile)
		print "Please wait..."
		print (cmd)
		val = DOSCommand (cmd)
		print val

		print "importing....."
		importFile OBJFilePath #noPrompt
		if IsValidNode $Y3D_temp then
			(
				print "Apply UV's"
				addModifier $Y3D_temp (Unwrap_UVW())
				uvData = (copy $Y3D_temp.modifiers[1])
				addModifier obj uvData
				collapseStack obj
				delete $Y3D_temp
			)
	)
)
