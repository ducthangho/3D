global cmdOutputToFileOn = True
global ResultbakeFolder = (getDir #image)+"\\myBake"
-- function
global attachObject
global detachObject
global makeLowPoly
global initProjectionMod
global testStealNormal
global unwrap
global stealNormal

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

rollout BakeUI "Steal Normal" width:235 height:168
(
	button 'btnInitProjection' "Init projection" pos:[3,50] width:73 height:29 align:#left
	button 'btnSetAsHightPolys' "Set As High Polys" pos:[46,12] width:102 height:26 align:#left

	local hightPolyObjs
	local lowPoly

	button 'btnTestStealNormal' "Test Steal Normal" pos:[48,89] width:93 height:29 align:#left
	button 'btnPushCage' "+" pos:[77,51] width:30 height:29 align:#left
	button 'btnFastPushCage' "++" pos:[108,51] width:30 height:29 align:#left
	button 'btnShrinkCage' "-" pos:[140,51] width:30 height:29 align:#left
	button 'btnFastShrinkCage' "--" pos:[170,51] width:30 height:29 align:#left
	button 'btnBakeNormal' "Bake" pos:[24,129] width:73 height:29 align:#left
	button 'btnResetCage' "reset cage" pos:[202,51] width:30 height:29 align:#left
	button 'btnBakeAndSeeResult' "Bake And See Result" pos:[107,130] width:115 height:29 align:#left
	on btnInitProjection pressed do
	(
		lowPoly = selection[1]
		local size = 2048
		print  ResultbakeFolder
	
		makeDir ResultbakeFolder all:true
	
		print "ssss"
	
		initProjectionMod hightPolyObjs lowPoly 
	)
	on btnSetAsHightPolys pressed do
	(
		hightPolyObjs = for obj in $ collect obj
	)
	on btnTestStealNormal pressed do
	(
		hightPolyObjs = selection[1]
		testStealNormal hightPolyObjs
		lowPoly = selection[1]
	)
	on btnPushCage pressed do
	(
		local proMod = FindPMod lowPoly
		proMod.pushCage  0.5
	)
	on btnFastPushCage pressed do
	(
		local proMod = FindPMod lowPoly
		proMod.pushCage  1.1
	)
	on btnShrinkCage pressed do
	(
		local proMod = FindPMod lowPoly
		proMod.pushCage  -0.5
	)
	on btnFastShrinkCage pressed do
	(
		local proMod = FindPMod lowPoly
		proMod.pushCage  -1.1
	)
	on btnBakeNormal pressed do
	(
		stealNormal lowPoly
	)
	on btnResetCage pressed do
	(
		local proMod = FindPMod lowPoly
		proMod.resetCage()
	)
	on btnBakeAndSeeResult pressed do
	(
		stealNormal lowPoly
		local lowPolyName = lowPoly.name
		local lowpolynamenormal = lowPolyName+"_normal"
		local lowPolyNormal = getNodeByName lowpolynamenormal exact:true all:false ignoreCase:false

		local gap = 3
		local maxBBx = lowPoly.max.x
		local minBBx = lowPoly.min.x
		local perimeter = (maxBBx - minBBx)

		hightPolyObjs.pos.x += (perimeter + gap)
		lowPolyNormal.pos.x -= (perimeter + gap)


		hightPolyObjs.wirecolor = color 156 156 156
		lowPolyNormal.wirecolor = color 156 156 156

		actionMan.executeAction -844228238 "12"
		max create mode
	)	
)

rollout DraftUIForTest "MainUI" width:194 height:407
(
	button 'btnUnwrap' "Unwrap" pos:[53,189] width:90 height:34 align:#left
	button 'btnAttach' "Attach các object" pos:[5,29] width:91 height:30 align:#left
	
	button 'btnMakeLowPoly' "MakeLowPoly" pos:[12,262] width:78 height:32 align:#left
	
	edittext 'edtRatio' "ratio" pos:[103,261] width:76 height:35 align:#left
	edittext 'edtTileSize' "TileSize (square)" pos:[19,115] width:159 height:23 align:#left
	edittext 'edtDensity' "Density (pixel/m2)" pos:[16,80] width:162 height:23 align:#left
	
	edittext 'edtPadding' "Padding (pixel)   " pos:[19,151] width:159 height:23 align:#left
	GroupBox 'grpStealDetail' "Steal Detail" pos:[9,338] width:180 height:61 align:#left
	button 'btnStealNormal' "Steal Normal" pos:[46,359] width:110 height:27 align:#left

	button 'btnDetach' "Detach Object" pos:[99,30] width:91 height:30 align:#left
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
		
		unwrap selection[1] density:density tileSize:tileSize padding:padding
	)
	on btnAttach pressed do
	(
		attachObject selection 
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
	on btnDetach pressed do
	(
		local binaryFacesObjectsFile = exportFolder +"\\"+ binaryFacesObjects
		detachObject $ binaryFacesObjectsFile
	)
)

fn unwrap obj density:128 tileSize:1024 padding:5 = 
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

	if (ClassOf obj) != Editable_mesh do 
	(
		convertToPoly obj
		convertToMesh obj
	)

	local sf = for i in obj.selectedFaces collect i.index

	local start = timeStamp()
	local binFile = fopen (exportFolder +"\\"+ binarySelectedFace)  "wb"
	for i = 1 to sf.count do
	(
		WriteLong binFile sf[i]
	)
	fclose binFile
	select obj
	exportObjectToFBX  (exportFolder + "\\" + exportFileName)
	local cmd = blenderPath + " -b -P "+ pythonScript + " -- unwrap " + "\"" + exportFolder + "\" "+(padding as string)
	if cmdOutputToFileOn do
		cmd += " > " + DoscommandOutput 
	print cmd
	val = HiddenDOSCommand(cmd)
	importObjectFromFBXFile  (tempPath + "\\"+ importFileName)
	local end = timeStamp()
	format "Import Export FBX took % seconds\n" ((end - start) / 1000.0)
	obj = selection[1]
	convertToPoly obj	

	obj.EditablePoly.SetSelection #Face (sf as bitarray)
	addmodifier obj (Unwrap_UVW ())
	subobjectLevel = 3

	print obj.name

	local u = obj.modifiers["Unwrap UVW"]
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

	if sf.count == (getNumFaces obj) then local tileNumber = 0 else local tileNumber = 1
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

fn initProjectionMod hightPolyObjs lowPoly = 
(	
	max modify mode
	addModifier lowpoly (ProjectionMod()) 
	proMod = FindPMod lowPoly
	for obj in hightPolyObjs do 
	(
		obj.wirecolor = color 156 6 6
		proMod.addObjectNode obj
	)
	proMod.resetCage()
	proMod.displayCage = true
	proMod.displayCageShaded = true
	proMod.displayCageOffset = true
	proMod.pushCage 1.1
)

fn stealNormal lowPoly size:512 ResultbakeFolder:ResultbakeFolder = 
(
	local proMod = FindPMod lowPoly
	lowPoly.INodeBakeProjProperties.enabled = True
	lowPoly.INodeBakeProjProperties.BakeObjectLevel = True
	lowPoly.INodeBakeProjProperties.BakeSubObjLevels = False
	lowPoly.INodeBakeProjProperties.projectionMod = proMod

	print "------------Inspect--------------"
	print lowPoly.INodeBakeProjProperties.useCage
	print lowPoly.INodeBakeProjProperties.projectionMod

	local currentRenderer = renderers.current as string
	local bakeNormal 

	if ((matchPattern currentRenderer pattern:"*mental_ray*") or\
	 (matchPattern currentRenderer pattern:"*Scanline*")) then
	(
		bakeNormal = NormalsMap()
		bakeNormal.useNormalBump = true
		bakeNormal.useHeightAsAlpha = true
		bakeNormal.fileType = (ResultbakeFolder+"\\"+lowPoly.name+"_normal_mentalray.png")
	)else if (matchPattern currentRenderer pattern:"*V_Ray_Adv*") then
	(
		bakeNormal = VrayNormalsMap()
		bakeNormal.fileType = (ResultbakeFolder+"\\"+lowPoly.name+"_normal_vray.png")
	)else if (matchPattern currentRenderer pattern:"*Krakatoa*") then
	(

	)
	
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

struct paramStealNormal (hightPolyObjs,lowPoly=undefined, size=512, bakeFolder=ResultbakeFolder)

fn testStealNormal hightPolyObjs lowPoly:undefined size:512 bakeFolder:ResultbakeFolder = 
(	
	local name = hightPolyObjs.name
	if lowPoly == undefined then
	(	
		select hightPolyObjs
		IsolateSelection.EnterIsolateSelectionMode() -- IsolateSelection.ExitIsolateSelectionMode()
		local lowpolyname = name+"_lowPoly"
		local lowpolynamenormal = name+"_lowPoly_normal"
		-- make lowpoly
		copy hightPolyObjs name:lowpolyname
		local lowPoly = getNodeByName lowpolyname exact:true all:false ignoreCase:false
		select lowPoly
		max modify  mode
		addModifier lowPoly (Edit_Normals())
		addModifier lowpoly (ProOptimizer()) 
		optMod = lowpoly.modifiers["ProOptimizer"]
		optMod.VertexPercent = 20
		optMod.OptimizationMode = 2
		optMod.KeepNormals = true
		optMod.NormalMode = 2
		optMod.NormalThreshold = 10
		optMod.MergePoints = true
		optMod.MergePointsThreshold  = 0
		optMod.MergeFaces = true
		optMod.MergeFacesAngle = 0
		optMod.CompactFaces = true
		optMod.PreventFlip = true
		optMod.Calculate = true

		unwrap lowPoly
		delete lowPoly
		lowPoly = selection[1]
		lowPoly.name = lowpolyname

		copy lowPoly name:lowpolynamenormal	
		local lowPolyNormal = getNodeByName lowpolynamenormal exact:true all:false ignoreCase:false
		initProjectionMod hightPolyObjs lowpoly
	)else 
	(
		
	)
)

fn attachObject objectArray = 
(
	convertToPoly objectArray
	convertToMesh objectArray
	local binFile = fopen (exportFolder +"\\"+ binaryFacesObjects)  "wb"
	if (objectArray.count > 1) do 
	(
		local finalObj = $[1]
		WriteString binFile (finalObj.name as string)
		WriteLong binFile (finalObj.numFaces as integer)
		WriteFloat binFile finalObj.pivot[1]
		WriteFloat binFile finalObj.pivot[2]
		WriteFloat binFile finalObj.pivot[3]
		local so = objectArray as array
		for i = 2 to so.count do
		(
			obj = so[i]
			local name = obj.name
			WriteString binFile (name as string)
			local numFaces = obj.numFaces 
			WriteLong binFile (numFaces as integer)
			WriteFloat binFile obj.pivot[1]
			WriteFloat binFile obj.pivot[2]
			WriteFloat binFile obj.pivot[3]
			attach finalObj obj
		)
	)
	finalObj.name = finalObj.name + "_attached"
	fclose binFile
	max modify mode
	subobjectLevel = 4
)

fn detachObject obj binaryFacesObjectsFile = 
(
	local binFile = fopen binaryFacesObjectsFile  "rb"
	fseek binFile 0 #seek_end
	local end = ftell binFile
	if end == 0 do return 0
	print ("End: "+end as string)
	fseek binFile 0 #seek_set	
	ResetPivot obj --w
	local pos = obj.pos

	while ((ftell binFile) < end) do
	(
		local objName = ReadString binFile
		print ("objName: "+objName)
		local objNumFaces = ReadLong binFile
		print ("objNumFaces: "+objNumFaces as string)
		local pivot1 = ReadFloat binFile
		local pivot2 = ReadFloat binFile
		local pivot3 = ReadFloat binFile
		local pivot = [pivot1,pivot2,pivot3]
		local faces = #{1..objNumFaces}
		local newMesh = meshop.detachFaces obj faces delete:true asMesh:true
		update obj
		emesh = Editable_mesh pos:pos name:objName pivot:pivot
		emesh.mesh = newMesh --assign the detached faces to the new mesh
		update emesh
	)
	fclose binFile
	delete obj
)

DraftUIForTestPos = [10,10]
createdialog DraftUIForTest pos:DraftUIForTestPos

DraftUIForTestPos = GetDialogPos DraftUIForTest
DraftUIForTestSize = GetDialogSize DraftUIForTest
BakeUIPos = [DraftUIForTestPos[1]+DraftUIForTestSize[1],DraftUIForTestPos[2]]
createdialog BakeUI pos:BakeUIPos