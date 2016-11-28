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
	button btnUnwrap "Unwrap" pos:[55,169] width:90 height:34
	button btnAttach "Attach các object" pos:[48,17] width:99 height:35
	
	button btnMakeLowPoly "MakeLowPoly" pos:[12,235] width:78 height:32
	edittext edtRatio "ratio" pos:[102,236] width:76 height:35



	edittext edtTileSize "TileSize (square)" pos:[19,123] width:159 height:35
	edittext edtDensity "Density (pixel/m2)" pos:[16,80] width:162 height:35
	
	on DraftUIForTest open do
	(
		edtRatio.text = "0.1"
		edtDensity.text = "128"
		edtTileSize.text = "1028"
		print edtDensity
	)
	on btnUnwrap pressed do
	(
		local density = edtDensity.text as integer
		local tileSize = edtTileSize.text as integer
		unwrap density tileSize
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
	on btnMakeLowPoly pressed do
	(	
		local ratio = edtRatio.text as float
		if (edtRatio.text=="") then
			ratio = -1;
		makeLowPoly ratio
	)
)

fn unwrap density tileSize= 
(	
	fn compareHeight rect1 rect2 = 
	(
		case of
		(
			(rect1.height <  rect2.height) : 1
			(rect1.height > rect2.height) : -1
			default : 0
		)
	)

	struct SRect (x,y,width,height,index,x_old,y_old)
	local sf = for i in $.selectedFaces collect i.index
	local binFile = fopen (exportFolder +"\\"+ binarySelectedFace)  "wb"
	for i = 1 to sf.count do
	(
		WriteLong binFile sf[i]
	)
	fclose binFile
	exportObjectToFBX  (exportFolder + "\\" + exportFileName)
	local cmd = blenderPath + " -b -P "+ pythonScript + " -- unwrap " + "\"" + exportFolder + "\""
	cmd += " > " + DoscommandOutput 
	print cmd
	val = HiddenDOSCommand(cmd)
	importObjectFromFBXFile  (tempPath + "\\"+ importFileName)
	convertToPoly $	

	$.EditablePoly.SetSelection #Face (sf as bitarray)
	modPanel.addModToSelection (Unwrap_UVW ())
	subobjectLevel = 3

	local u = $.modifiers[1]

	local x =1
	local y = 1
	local width =1
	local height =1
	local area_uvw = 1
	local areaGeom
	local f_area =1
	local totalAreaTile_Pixel = tileSize*tileSize
	local totalAreaTile_M2 = (totalAreaTile_Pixel as float) / density

	u.unwrap2.setTVElementMode true
	local userSelected = sf as bitarray


	local listRect = #()

	local start = timeStamp()
	while userSelected.numberSet != 0 do
		for i in userSelected do
		(
			u.selectFaces #{i}
			u.selectElement()
			local selected = u.getSelectedFaces()
			u.unwrap4.getArea selected &x &y &width &height &area_uvw &areaGeom
			
			--print selected
			--format "before: x = %, y = %, width = %, height = % ,area_uvw = %, areaGeom = %;\n" x y width height area_uvw areaGeom

			local currentAreaOfIsland_M2 = area_uvw * totalAreaTile_M2
			local needScale = areaGeom/currentAreaOfIsland_M2
			local needScaleEdge = sqrt needScale

			local testWidth = width*needScaleEdge
			local testHeight = height*needScaleEdge
			if testWidth > 1 then
			(
				if testWidth > testHeight then
					needScaleEdge = 1.0f/width
				else 
					needScaleEdge = 1.0f/height 
			) else if testHeight > 1 do
			(
				if testHeight > testWidth then
					needScaleEdge = 1.0f/height
				else 
					needScaleEdge = 1.0f/width
			)
			
			--format "needScale = %, needScaleEdge = %\n"  needScale needScaleEdge
			local a = width*needScaleEdge
			local b = height*needScaleEdge
			
			u.unwrap2.scaleSelectedXY needScaleEdge needScaleEdge [x,y,0]
			u.unwrap4.getArea selected &x &y &width &height &area_uvw &areaGeom
			
			local rect = SRect x:-1 y:-1 width:width height:height index:selected x_old:x y_old:y

			--format "after: x = %, y = %, width = %, height = % ,area_uvw = %, areaGeom = %;\n" x y width height area_uvw areaGeom
			--format "calculated: width = %, heith = %, area_uvw = % \n\n" (a) (b) (area_uvw * totalAreaTile_M2)
			
			append listRect rect
			userSelected = userSelected - selected
			exit
		)

		local end = timeStamp()
		format "Processing took % seconds\n" ((end - start) / 1000.0)
		
		local start = timeStamp()
		qsort listRect CompareHeight	
		local end = timeStamp()
		format "quick sort took % seconds\n" ((end - start) / 1000.0)

		print ("============Num IsLand=================\n")
		print listRect.count

		local enclosingRectArg = #(1,1)
		local listRectArg = #()
		for rect in listRect do
		(
			join listRectArg #(rect.x,rect.y,rect.width,rect.height) 
		)

		
		
		
		local unFitRect = #(1)
		local listSRect = #()
		local tileNumber = 1
		while unFitRect.count != 0 do 
		(
			listSRect = #()
			unFitRect = #()
			local j = 1
			fpbasics.pack &listRectArg &enclosingRectArg

			print ("==========================================")
			print listRectArg
			print listRectArg.count
			print ("==========================================")

			for i in 1 to listRectArg.count by 4 do
			(	
				format "x = %, y = %, width = %, height = %\n" listRectArg[i] listRectArg[i+1] listRectArg[i+2] listRectArg[i+3] 
				udim_u = mod tileNumber 10
				udim_v = tileNumber / 10
				if (listRectArg[i] == -1) then
				(
					join unFitRect #(listRectArg[i], listRectArg[i+1], listRectArg[i+2], listRectArg[i+3])
					append listSRect listRect[j]
				)else
				(
					local rect = listRect[j]
					--print listRectArg[i]
					--format "rect.x = %, rect.y = %, rect.x_old = %, rect.y_old = %, index = %\n" rect.x rect.y rect.x_old rect.y_old index
					
					u.unwrap2.selectFaces rect.index
					u.unwrap2.moveSelected [-rect.x_old, -rect.y_old, 0]
					u.unwrap2.moveSelected [udim_u+listRectArg[i+1],udim_v+listRectArg[i],0]
				)
				j+=1
			)

			print "UnFitRect count:"
			print unFitRect.count
			print listSRect.count

			listRectArg = unFitRect
			listRect = listSRect

			tileNumber+=1
		)
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