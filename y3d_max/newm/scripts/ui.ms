global cmdOutputToFileOn = True
global bakeFolder = (getDir #image)+"\\myBake"

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

rollout BakeUI "Steal Normal" width:158 height:102
(
	button btnStealNormal "Steal Normal" pos:[27,56] width:101 height:36
	button btnSetAsHightPolys "Set As High Polys" pos:[28,12] width:102 height:26

	local hightPolyObjs

	on btnSetAsHightPolys pressed do
	(
		hightPolyObjs = for obj in $ collect obj
	)

	on btnStealNormal pressed do
	(
		local lowPoly = $
		local size = 2048
		print  bakeFolder

		makeDir bakeFolder all:true

		stealNormal hightPolyObjs lowPoly size:size
	)
)


rollout DraftUIForTest "MainUI" width:194 height:407 
(
	button btnUnwrap "Unwrap" pos:[53,189] width:90 height:34
	button btnAttach "Attach các object" pos:[48,17] width:99 height:35
	
	button btnMakeLowPoly "MakeLowPoly" pos:[12,262] width:78 height:32
	
	edittext edtRatio "ratio" pos:[103,261] width:76 height:35
	edittext edtTileSize "TileSize (square)" pos:[19,115] width:159 height:23
	edittext edtDensity "Density (pixel/m2)" pos:[16,80] width:162 height:23
	
	edittext edtPadding "Padding (pixel)   " pos:[19,151] width:159 height:23
	GroupBox grpStealDetail "Steal Detail" pos:[9,338] width:180 height:61
	button btnStealNormal "Steal Normal" pos:[46,359] width:110 height:27
	
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
	on btnStealNormal pressed do
	(
		DraftUIForTestPos = GetDialogPos DraftUIForTest
		DraftUIForTestSize = GetDialogSize DraftUIForTest
		BakeUIPos = [DraftUIForTestPos[1]+DraftUIForTestSize[1],DraftUIForTestPos[2]]
		createdialog BakeUI pos:BakeUIPos
	)
)

fn unwrap density tileSize padding = 
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

	if (ClassOf $) != Editable_mesh do 
	(
		convertToPoly $
		convertToMesh $
	)

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

fn stealNormal hightPolyObjs lowPoly size:512 bakeFolder:bakeFolder = 
(	
	local currentRenderer = renderers.current as string
	local bakeNormal 

	if ((matchPattern currentRenderer pattern:"*mental_ray*") or\
	 (matchPattern currentRenderer pattern:"*Scanline*")) then
	(
		bakeNormal = NormalsMap()
		bakeNormal.useNormalBump = true
		bakeNormal.useHeightAsAlpha = true
		bakeNormal.fileType = (bakeFolder+"\\"+lowPoly.name+"_normal_mentalray.png")
	)else if (matchPattern currentRenderer pattern:"*V_Ray_Adv*") then
	(
		bakeNormal = VrayNormalsMap()
		bakeNormal.fileType = (bakeFolder+"\\"+lowPoly.name+"_normal_vray.png")
	)else if (matchPattern currentRenderer pattern:"*Krakatoa*") then
	(

	)

	addModifier lowpoly (ProjectionMod()) 
	proMod = FindPMod lowPoly
	for obj in hightPolyObjs do (proMod.addObjectNode obj)
	proMod.resetCage()
	proMod.pushCage 1.1
	
	lowPoly.INodeBakeProjProperties.enabled = True
	lowPoly.INodeBakeProjProperties.BakeObjectLevel = True
	lowPoly.INodeBakeProjProperties.BakeSubObjLevels = False
	lowPoly.INodeBakeProjProperties.projectionMod = proMod

	print "------------Inspect--------------"
	print lowPoly.INodeBakeProjProperties.useCage
	print lowPoly.INodeBakeProjProperties.projectionMod

	
	bakeNormal.outputSzX = size
	bakeNormal.outputSzY = size
	bakeNormal.fileName = filenameFromPath bakeNormal.fileType
	bakeNormal.enabled = true

	lowpoly.iNodeBakeProperties.removeAllBakeElements() 
	lowpoly.INodeBakeProperties.bakeEnabled = true 
	lowpoly.INodeBakeProperties.bakeChannel = 1 
	lowpoly.INodeBakeProperties.nDilations = 4
	lowpoly.INodeBakeProperties.addBakeElement bakeNormal

	render rendertype:#bakeSelected vfb:off progressBar:true outputSize:[size,size]

	local bitmap =bitmaptexture filename:bakeNormal.fileType

	if ((matchPattern currentRenderer pattern:"*mental_ray*") or\
		 (matchPattern currentRenderer pattern:"*Scanline*")) then
	(
		local normalbumpMap = Normal_Bump()
		normalbumpMap.normal_map = bitmap
		local mat = standard()
		mat.bumpMap = normalbumpMap
		mat.bumpMapAmount = 90

	)else if (matchPattern currentRenderer pattern:"*V_Ray_Adv*") then
	(	
 		local vnormalMap = VRayNormalMap()
 		vnormalMap.normal_map = bitmap
 		local mat = VrayMtl()
 		mat.texmap_bump = vnormalMap
 		mat.texmap_bump_multiplier = 90
	)else if (matchPattern currentRenderer pattern:"*Krakatoa*") then
	(

	)

	local shellMat = Shell_Material originalMaterial:lowPoly.material\
		bakedMaterial:mat viewportMtlIndex:1 renderMtlIndex:1
	lowPoly.material = shellMat	
	showTextureMap lowPoly.material lowPoly.material.bakedMaterial true
)


DraftUIForTestPos = [10,10]
createdialog DraftUIForTest pos:DraftUIForTestPos

--createdialog BakeUI pos:B