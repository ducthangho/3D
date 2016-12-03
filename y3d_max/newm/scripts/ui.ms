global cmdOutputToFileOn = True

fn makeLowPoly ratio = 
(	
	if $selection.count == 0 then return ""
	ResetPivot $
	local filePath = tempPath + "\\" +exportHightPolyFileName
	exportObjectToFBX filePath
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
	button btnUnwrap "Unwrap" pos:[53,189] width:90 height:34
	button btnAttach "Attach các object" pos:[48,17] width:99 height:35
	
	button btnMakeLowPoly "MakeLowPoly" pos:[12,262] width:78 height:32
	
	edittext edtRatio "ratio" pos:[102,259] width:76 height:35
	edittext edtTileSize "TileSize (square)" pos:[19,115] width:159 height:23
	edittext edtDensity "Density (pixel/m2)" pos:[16,80] width:162 height:23
	
	edittext edtPadding "Padding (pixel)   " pos:[19,151] width:159 height:23
	on DraftUIForTest open do
	(
		edtRatio.text = "0.1"
		edtDensity.text = "128"
		edtTileSize.text = "1024"
		edtPadding.text = "5"
	)
	on btnUnwrap pressed do
	(
		local density = edtDensity.text as integer
		local tileSize = edtTileSize.text as integer
		local padding = edtPadding.text as float
		unwrap density tileSize padding
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

fn unwrap density tileSize padding= 
(	
	padding = padding/tileSize
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

	local start = timeStamp()
	local binFile = fopen (exportFolder +"\\"+ binarySelectedFace)  "wb"
	for i = 1 to sf.count do
	(
		WriteLong binFile sf[i]
	)
	fclose binFile
	exportObjectToFBX  (exportFolder + "\\" + exportFileName)
	local cmd = blenderPath + " -b -P "+ pythonScript + " -- unwrap " + "\"" + exportFolder + "\" "+(padding as string)
	if cmdOutputToFileOn do
		cmd += " > " + DoscommandOutput 
	print cmd
	val = HiddenDOSCommand(cmd)
	importObjectFromFBXFile  (tempPath + "\\"+ importFileName)
	local end = timeStamp()
	format "Import Export FBX took % seconds\n" ((end - start) / 1000.0)
	convertToPoly $	

	$.EditablePoly.SetSelection #Face (sf as bitarray)
	modPanel.addModToSelection (Unwrap_UVW ())
	subobjectLevel = 3

	local u = $.modifiers[1]
	local x = 1
	local y = 1
	local width =1
	local height =1
	local area_uvw = 1
	local areaGeom = 1
	local f_area =1
	local totalAreaTile_Pixel = tileSize*tileSize
	local totalAreaTile_M2 = (totalAreaTile_Pixel as float) / density

	u.unwrap2.setTVElementMode true
	local userSelected = sf as bitarray

	local listRect = #()
	local start = timeStamp()

	local temp1
	local temp2
	local needScale = 1
	local needScaleEdge = 1
	local testWidth 
	local testHeight

	while userSelected.numberSet != 0 do
		for i in userSelected do
		(
			u.selectFaces #{i}
			u.selectElement()
			local selected = u.getSelectedFaces()
			u.unwrap4.getArea selected &x &y &width &height &area_uvw &areaGeom

			local currentAreaOfIsland_M2 = area_uvw * totalAreaTile_M2

			needScale = 1
			needScaleEdge = 1
			
			if currentAreaOfIsland_M2 > 0 do
			(
				needScale = areaGeom/currentAreaOfIsland_M2
				needScaleEdge = sqrt needScale
			)
	
			testWidth = width*needScaleEdge + padding
			testHeight = height*needScaleEdge + padding

			if needScale > 10000000 do
				needScaleEdge = 1
			
			if testWidth > 1 then
			(	
				if testWidth > testHeight then
				(
					needScaleEdge = (1.0f - padding)/width
				)
				else 
				(
					needScaleEdge = (1.0f - padding)/height 
				)
			) else if testHeight > 1 do
			(
				if testHeight > testWidth then
				(	
					needScaleEdge = (1.0f - padding)/height
				)					
				else
				(	
					needScaleEdge = (1.0f - padding)/width
				) 
			)

			u.unwrap2.scaleSelectedXY needScaleEdge needScaleEdge [x,y,0]
			u.unwrap4.getArea selected &x &y &width &height &area_uvw &areaGeom
			
			temp1 = width + padding
			temp2 = height + padding
			temp1 = if (temp1 > 1) then 1 else temp1
			temp2 = if (temp2 > 1) then 1 else temp2

			local rect = SRect x:-1 y:-1 width:temp1 height:temp2 index:selected x_old:x y_old:y

			append listRect rect
			userSelected = userSelected - selected
			exit
		)

	local end = timeStamp()
	format "Calculate island took % seconds\n" ((end - start) / 1000.0)
	
	local start = timeStamp()
	qsort listRect CompareHeight	
	local end = timeStamp()
	format "quick sort took % seconds\n" ((end - start) / 1000.0)

	print ("Num island : " + listRect.count as string)

	local enclosingRectArg = #(1,1)
	local listRectArg = #()
	for rect in listRect do
	(
		join listRectArg #(rect.x,rect.y,rect.width,rect.height) 
	)

	local unFitRect = #(1)
	local listSRect = #()

	if sf.count == (getNumFaces $) then local tileNumber = 0 else local tileNumber = 1
	while unFitRect.count != 0 do 
	(
		listSRect = #()
		unFitRect = #()
		local j = 1

		local start = timeStamp()
		fpbasics.pack &listRectArg &enclosingRectArg
		local end = timeStamp()
		format "call fb pack took % seconds\n" ((end - start) / 1000.0)

		for i in 1 to listRectArg.count by 4 do
		(	
			udim_u = mod tileNumber 10
			udim_v = tileNumber / 10
			if (listRectArg[i] == -1) then
			(
				join unFitRect #(listRectArg[i], listRectArg[i+1], listRectArg[i+2], listRectArg[i+3])
				append listSRect listRect[j]
			)else
			(
				local rect = listRect[j]
				u.unwrap2.selectFaces rect.index
				u.unwrap2.moveSelected [-rect.x_old, -rect.y_old, 0]
				u.unwrap2.moveSelected [udim_u+listRectArg[i+1],udim_v+listRectArg[i],0]
			)
			j+=1
		)

		listRectArg = unFitRect
		listRect = listSRect

		tileNumber+=1

		if TileNumber > 200 do
		( 
			for i in 1 to listRectArg.count by 4 do
				format "x = %, y = %, width = %, height = %\n" listRectArg[i] listRectArg[i+1] listRectArg[i+2] listRectArg[i+3]  
			return 1;
		)
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