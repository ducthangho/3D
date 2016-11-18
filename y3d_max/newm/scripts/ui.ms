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
		if (selection.count > 1) do 
		(
			local binFile = fopen (exportFolder + binaryFacesObjects)  "wb"
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
			fclose binFile
		)
		max modify mode
		subobjectLevel = 4
	)

	on btnMakeLowPoly pressed  do
	(	
		local ratio = edtRatio.text as float
		makeLowPoly ratio
	)
)

fn unwrap = 
(	
	local sf = for i in $.selectedFaces collect i.index
		local binFile = fopen (exportFolder + binarySelectedFace)  "wb"
		for i = 1 to sf.count do
		(
			WriteLong binFile sf[i]
		)
		fclose binFile
	
		exportObjectToFBX $ exportFileName
	
		local pythonScript = scriptFolder + "blender.py"
		local cmd = blenderPath + " -b -P "+ pythonScript + " -- unwrap"
		val = DOSCommand(cmd)
)

fn makeLowPoly ratio = 
(	
	exportObjectToFBX $ exportHightPolyFileName
	local pythonScript = scriptFolder + "blender.py"
	local cmd = blenderPath + " -b -P "+ pythonScript + " -- makeLowPoly "+ ratio as string
	--local cmd = blenderPath + " -b -P "+ pythonScript + " -- makeLowPoly "
	val = DOSCommand(cmd)
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