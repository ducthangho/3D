(
	global exportObj
	global importObj
	local infoFilePath = "D:\\\y3d.bin"
	local OBJFilePath = "D:\\y3d.obj"
	local blenderPath = "D:\\app\blender\blender.exe"
	-- local pythonFile = "$userScripts/scripts/y3d_blender_unwrap.py"
	local pythonFile = getDir #userscripts+"\scripts\y3d_blender_unwrap.py"
	struct yface (index, f_area, area_uvw, fw, fh, udim, x, y)

	fn exportObj obj faceArray =
		(
		binFile = fopen infoFilePath "wb"
		WriteLong binFile faceArray.count #unsigned
		for i = 1 to faceArray.count do
			(
				vv = faceArray[i].index as integer
				WriteLong binFile vv
			)
		fclose binFile

		exportFile OBJFilePath #noPrompt selectedOnly:true
		-- oldName = exportNode.name
		-- exportNode.name = obj.name + "_temp"
		--
		-- exportNode.name = oldName

		cmd = (blenderPath+ " -noaudio --factory-startup -b --python "+ pythonFile)
		print "Please wait..."
		print (cmd)
		val = DOSCommand (cmd)
		print val
		)

	fn importObj =
		(
		print "importing....."
 		importFile OBJFilePath #noPrompt

		if IsValidNode $RoadKillOutput then
			(
			print "Apply UV's"
			addModifier $RoadKillOutput (Unwrap_UVW())
			uvData = (copy $RoadKillOutput.modifiers[1])
			addModifier exportNode uvData
			collapseStack exportNode
			delete $RoadKillOutput
			)
		)
)
