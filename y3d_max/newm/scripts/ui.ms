fn makeLowPoly ratio = 
(	
	if $selection.count == 0 then return ""
	ResetPivot $
	local filePath = tempPath + "\\" +exportHightPolyFileName
	exportFile filePath #noPrompt selectedOnly:true 
-- 	augument order: 	1 : What to do , 2 : ExportFolder, 3 : other need argument
	local cmd = blenderPath + " --factory-startup -b -P \"" + pythonScript + "\""
	cmd += " -- makeLowPoly \""+tempPath + "\""
	if (ratio!=-1) then
		cmd += " " + ratio as string
	cmd += " > " + DoscommandOutput 
	print cmd
	val = HiddenDOSCommand cmd
	delete $
	importObjectFromFBXFile  (tempPath + "\\"+ importLowPolyFileName)
)

rollout DraftUIForTest "Untitled" width:190 height:303
(
	button btnUnwrap "ExportFBX" pos:[46,96] width:90 height:34
	button btnAttach "Attach các object" pos:[41,24] width:99 height:35
	
	button btnMakeLowPoly "MakeLowPoly" pos:[16,164] width:78 height:32
	edittext edtRatio "ratio" pos:[106,164] width:76 height:35

	on DraftUIForTest open do
	(
		edtRatio.text = "0.1"
	)

	on btnUnwrap pressed do
	(
		unwrap()
	)
	on btnAttach pressed do
	(
		convertToPoly $
		convertToMesh $
		local binFile = fopen (exportFolder +"\\"+ binaryFacesObjects)  "wb"
		if (selection.count > 1) do 
		(
			local finalObj = $[1]
			WriteString binFile (finalObj.name as string)
			WriteLong binFile (finalObj.numFaces as integer)
			local so = selection as array
			for i = 2 to so.count do
			(
				obj = so[i]
				local name = obj.name
				WriteString binFile (name as string)
				local numFaces = obj.numFaces 
				WriteLong binFile (numFaces as integer)
				attach finalObj obj
			)
		)
		fclose binFile
		max modify mode
		subobjectLevel = 4
	)

	on btnMakeLowPoly pressed  do
	(	
		local ratio = edtRatio.text as float
		if (edtRatio.text=="") then
			ratio = -1;
		makeLowPoly ratio
	)
)

fn unwrap = 
(	
	local sf = for i in $.selectedFaces collect i.index
	local binFile = fopen (exportFolder +"\\"+ binarySelectedFace)  "wb"
	for i = 1 to sf.count do
	(
		WriteLong binFile sf[i]
	)
	fclose binFile
	exportObjectToFBX  (exportFolder + "\\" + exportFileName)
	local cmd = blenderPath + " -b -P "+ pythonScript + " -- unwrap " + "\"" + tempPath + "\""
	cmd += " > " + DoscommandOutput 
	print cmd
	val = HiddenDOSCommand(cmd)
)

-- fn makeLowPoly ratio = 
-- (	
-- 	exportObjectToFBX exportFolder + "\\" + exportHightPolyFileName
-- 	local pythonScript = scriptFolder + "blender.py"
-- 	local cmd = blenderPath + " -b -P "+ pythonScript + " -- makeLowPoly "+ ratio as string
-- 	print cmd
-- 	val = HiddenDOSCommand(cmd)
-- )

fn test = 
(
	local scriptFolder = ypath  + "\\newm\\scripts\\"
	local pythonScript = scriptFolder  + "test.py"
	local DoscommandOutput = scriptFolder + "hiddencmdout.tmp"
	local cmd = blenderPath + " -b -P "+ pythonScript + " > "+ DoscommandOutput
	print cmd
	Doscommand (cmd)
	--HiddenDoscommand "dir > %temp%\\hiddencmdout.tmp" startpath:"c:\\"
	
)


fn readBinaryFile binaryFile = 
(	
	local binFile = fopen binaryFile  "rb"
	fseek binFile 0 #seek_end
	local end = ftell binFile
	fseek binFile 0 #seek_set	
	while ((ftell binFile) < end) do
	(
		local a = ReadString binFile
		print a
		local b = ReadLong binFile
		print b
	)
	fclose binFile
)

createdialog DraftUIForTest

--test()
