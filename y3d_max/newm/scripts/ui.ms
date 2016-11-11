rollout DraftUIForTest "Untitled" width:190 height:303
(
	button btnUnwrap "ExportFBX" pos:[44,111] width:90 height:34
	button btnAttach "Attach các object" pos:[41,24] width:99 height:35
	
	on btnUnwrap pressed do
	(
		local sf = for i in $.selectedFaces collect i.index
		local binFile = fopen (exportFolder + binarySelectedFace)  "wb"
		for i = 1 to sf.count do
		(
			WriteLong binFile sf[i]
		)
		fclose binFile

		exportObjectToFBX $ exportFileName
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
readBinaryFile (exportFolder + binaryFacesObjects)