(
global exportObj
global importObj
global getUVFromBlenderFBX
local infoFilePath = "D:\\\y3d.bin"
local OBJFilePath = "D:\\y3d.obj"
local blenderPath = "D:\\app\blender\blender.exe"
-- local pythonFile = "$userScripts/scripts/y3d_blender_unwrap.py"
local pythonFile = getDir #userscripts+"\scripts\y3d_blender_unwrap.py"
struct yface (index, f_area, area_uvw, fw, fh, udim, x, y)

fn uVTRIDTransfer o1 o2 uVTRFromChannel uVTRToChannel = 
	(
	-- variables
	uvpos = #()
	
	-- prep objects
	obj1 = copy o1
	convertToPoly obj1
	obj2 = o2

	-- add unwrap modifiers
	addmodifier obj1 (Unwrap_UVW())
	obj1.modifiers[1].setMapChannel uVTRFromChannel
	addmodifier obj2 (Unwrap_UVW())
	obj2.modifiers[1].name = "y3d_unwrap"
	obj2.modifiers[1].setMapChannel uVTRToChannel

	-- number of faces
	numfaces1 = polyop.getNumFaces obj1
	numfaces2 = polyop.getNumFaces obj2

	print "numfaces1"
	print numfaces1
	print "numfaces2"
	print numfaces2

	if numfaces1 == 0 or numfaces2 == 0 then (MessageBox "One of your input objects contains 0 faces. Please select new objects." title:"uVTransfer")
	else
		(
		if numfaces1 != numfaces2 then (MessageBox "at least one object has a different number of faces from the source, and so can't be processed." title:"uVTransfer")
		else
			(
			-- Select first object and open uv editing
			select obj1
			max modify mode
			obj1.modifiers[1].unwrap.edit ()
			obj1.modifiers[1].unwrap.move ()
			
			-- store the uvs positions of all vertexes
			for i = 1 to numfaces1 do
				(
				sourceverts = polyop.getMapFace obj1 uVTRFromChannel i
				allUVPositions = #()
				for v1 in sourceverts do
					(
					append allUVPositions (obj1.modifiers[1].unwrap.getVertexPosition currenttime v1)
					)
				append uvpos #(i,allUVPositions)
				)
								
			-- Select second object and open uv editing...
			select obj2
			max modify mode
			obj2.modifiers[1].unwrap.edit ()
			obj2.modifiers[1].unwrap.move ()
			
			-- break all texture verts
			allverts = for i = 1 to (obj2.modifiers[1].numberVertices()) collect i
			obj2.modifiers[1].selectVertices (allverts as bitarray)
			obj2.modifiers[1].breakSelected()			
	
			-- apply source uv positions to target
			for myvert in uvpos do
				(
				targetverts = polyop.getMapFace obj2 uVTRToChannel myvert[1]
				for v2 = 1 to targetverts.count do
					(
					obj2.modifiers[1].setVertexPosition 1 targetverts[v2] myvert[2][v2]
					)
				)
			)
		)

	-- Cleanup
	allverts = for i = 1 to (obj2.modifiers[1].numberVertices()) collect i
	obj2.modifiers[1].selectVertices (allverts as bitarray)
	obj2.modifiers[1].weldSelected()
	obj2.modifiers[1].selectVertices #{}
	delete obj1
	subobjectLevel = 3
	)


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

		theINI =objexp.getIniName()
		setIniSetting theINI"Optimize" "optVertex" "0"
		setIniSetting theINI"Optimize" "optNormals" "0"
		setIniSetting theINI"Optimize" "optTextureCoords" "0"
		setIniSetting theINI"Output" "Precision" "4"
		setIniSetting theINI"Geometry" "SmoothingGroups" "0"
		setIniSetting theINI"Geometry" "FaceType" "2"
		setIniSetting theINI"Geometry" "FlipZyAxis" "1"
		setIniSetting theINI"Geometry" "Normals" "0"
		setIniSetting theINI"Geometry" "Shapes" "0"
		setIniSetting theINI"Geometry" "TextureCoords" "1"


		exportFile OBJFilePath #noPrompt selectedOnly:true
		obj.name = oldName
		cmd = (blenderPath+ " -noaudio --factory-startup -b --python "+ pythonFile)
		print "Please wait..."
		print (cmd)
		val = DOSCommand (cmd)
		print val

		print "importing....."

		theINI = objimp.getIniName()
		setIniSetting theINI"General" "ResetScene" "0"
		setIniSetting theINI"Objects" "AsEditablePoly" "1"
		setIniSetting theINI"Objects" "SingleMesh" "1"
		setIniSetting theINI"Objects" "Retriangulate" "0"
		setIniSetting theINI"Geometry" "SmoothingGroups" "0"
		setIniSetting theINI"Geometry" "TextureCoords" "1"


		importFile OBJFilePath #noPrompt

		tempObj = selection[selection.count]

		deleteModifier obj 1
		oldClass = classof obj
		if (classof tempObj!=oldClass) then
			(
				if oldClass==Editable_Poly then ConvertTo tempObj Editable_Poly
				else if oldClass==Editable_Mesh then ConvertTo tempObj Editable_Mesh
			)


		uVTRIDTransfer tempObj obj 1 mapChannel
		-- delete tempObj
		-- if IsValidNode $Y3D_temp then
		-- 	(
		-- 		print "Apply UV's"
		-- 		addModifier $Y3D_temp (Unwrap_UVW())
		-- 		uvData = (copy $Y3D_temp.modifiers[1])
		-- 		addModifier obj uvData
		-- 		collapseStack obj
		-- 		delete $Y3D_temp
		-- 	)
	)
	
	
		--------------------------begin:Ham Minh Them------------------
	
	blenderExe = "\"C:\\Program Files\\Blender Foundation\\Blender\\blender.exe\""
	
	exportFolder = "F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\export\\blender\\"
	exportFileName = "3DSexport.fbx"
	binaryFile = "ImportantFace.bin"
	importFileBlender = "export.fbx"
	blenderFile = "unwraper.blend"
	
	scriptFolder  = getDir #userscripts+"\\m1\scripts\\"
	pythonScript = scriptFolder + "y3d_blender_unwrap_fbx.py"
	pythonScriptTest = scriptFolder + "test.py"
	
	fn WriteImportantFacesToFile obj faceArray = 
	(
		local binFile = fopen (exportFolder+binaryFile)  "wb"
		WriteLong binFile faceArray.count #unsigned
		local vv
		for i = 1 to faceArray.count do
			(
				vv = faceArray[i].index as integer
				WriteLong binFile vv
			)
		fclose binFile
	)

	fn exportObjectToFBXFile obj exportFileName= 
	(
		print "heloooooooo"
-- 		FBXExporterSetParam "NormalsPerPoly" false
-- 		FBXImporterSetParam "Animation" false
-- 		FBXImporterSetParam "ASCII" false
-- 		FBXImporterSetParam "AxisConversionMethod" "Fbx_Root"
-- 		FBXImporterSetParam "BakeAnimation" false
-- 		FBXImporterSetParam "Cameras" false
-- 		FBXImporterSetParam "FileVersion" "FBX201400"
-- 		FBXImporterSetParam "SmoothingGroups" false
-- 		FBXImporterSetParam "SmoothMeshExport" false
-- 		FBXImporterSetParam "TangentSpaceExport" false
-- 		FBXImporterSetParam "Triangulate" false

		local filePath = exportFolder + exportFileName
		exportFile filePath #noPrompt selectedOnly:true
	)

	fn ImportObjectFromFBXFile importFileName = 
	(
		FBXImporterSetParam "Mode" #create
		local filePath = exportFolder + importFileName
		importFile filePath #noPrompt
	)

	fn getUVFromBlenderFBX obj faceArray = 
	(
		WriteImportantFacesToFile obj faceArray
		exportObjectToFBXFile obj exportFileName 

		local cmd = blenderExe + " -b -P "+pythonScript
		local val = DOSCommand(cmd)

		ImportObjectFromFBXFile importFileBlender

		$.name = $.name+"_import"
	)
	
	
	fn test = 
	(
		print importFileBlender
-- 		cmd = ("C:\Program\ Files\Blender\ Foundation\Blender\blender.exe ")
-- 		val = DOSCommand (blenderExe +" -b -P F:\\WorkSpace\\3D\\y3d_max\\scripts\\test.py")
-- 		val = DOSCommand ("python F:\\WorkSpace\\3D\\y3d_max\\scripts\\test.py")
		val = DOSCommand ("\"c:\Program Files\Blender Foundation\Blender\blender.exe\" -b -P F:\\WorkSpace\\3D\\y3d_max\\scripts\\test.py")
		print val
		val = DOSCommand ("\"c:\Program Files\Blender Foundation\Blender\blender\" -b -P F:\\WorkSpace\\3D\\y3d_max\\scripts\\y3d_blender_unwrap_fbx.py")
		blender = exportFolder + blenderFile
		cmd = blenderExe + " -b -P "+pythonScript
		val = DOSCommand(cmd)
	)
	------------------------end: Ham MinhThem -------------------
)	
