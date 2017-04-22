
-- global variables and functions
if yms_init != true then
(
	struct ymsClass
	(
		tempPath = pathConfig.GetDir #temp,
		exportFolder = tempPath, 
		exportFileName = "3dsNodes.fbx",
		importFileName = "blenderNodes.fbx",
		exportHightPolyFileName = "3dsNodesHightPoly.fbx",
		importLowPolyFileName = "blenderNodesLowPoly.fbx",
		binaryFacesObjects = "3dsFaces.bin",
		binarySelectedFace = "3dsSelectedFace.bin",
		blenderPath = "\"C:\\Program Files\\Blender Foundation\\Blender\\blender.exe\"",
		scriptFolder  = ypath  + "\\yms\\scripts",
		pythonScript =  ypath + "\\yms\\python\\blender.py",
		blenderFile = exportFolder+"\\3dsMax.blend",
		DoscommandOutput = ypath + "\\hiddencmdout.tmp",

		fn objectToFBX path = 
		(
			FBXExporterSetParam  "FileVersion" "FBX201400" 
			exportFile path #noPrompt selectedOnly:true 
		),

		fn objectFromFBXFile filePath = 
		(
			FBXImporterSetParam "Mode" #create
			importFile filePath #noPrompt
		),

		fn unwrap3dmax angle spacing = 
		(
			max modify mode
			s1 = selection[1]
			-- if (classof s1.baseobject) != Editable_Poly then convertto s1 editable_poly
			subobjectLevel = 0	
			if (s1.modifiers["y3d_unwrap"]==undefined) then (
				u1 = unwrap_UVW()
				u1.name = "y3d_unwrap"
				addModifier s1 u1
			)
			u1 = s1.modifiers["y3d_unwrap"]
			modPanel.setCurrentObject s1.modifiers[#y3d_unwrap]
			subobjectLevel = 3
			max select all
			u1.flattenMap angle #() spacing true 0 true true	
			subobjectLevel = 0
		),

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
			local sf = for i in $.selectedFaces collect i.index

			local start = timeStamp()
			local binFile = fopen (exportFolder +"\\"+ binarySelectedFace)  "wb"
			for i = 1 to sf.count do
			(
				WriteLong binFile sf[i]
			)
			fclose binFile
			objectToFBX  (exportFolder + "\\" + exportFileName)
			local cmd = blenderPath + " -b -P "+ pythonScript + " -- unwrap " + "\"" + exportFolder + "\" "+(padding as string)
			-- if cmdOutputToFileOn do
			-- 	cmd += " > " + DoscommandOutput 
			print cmd
			val = HiddenDOSCommand(cmd)
			objectFromFBXFile  (tempPath + "\\"+ importFileName)
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
		),


		fn test_xref oname = 
		(
			MessageBox oname
		),

		fn yxref_selected = 
		(
			fname = substring maxFileName 1 (maxFileName.count-4)
			path = maxFilePath + fname + "_test_"
			high_path = path + "high.max"
			low_path = path + "low.max"
			ResetPivot selection
			-- ResetTransform selection
			-- for nn in selection do 
			-- 	(
			-- 		ResetPivot nn
			-- 		ResetTransform nn
			-- 	)
			saveNodes selection high_path
			makeLowPoly -1
			max delete
			importFile (pathConfig.GetDir #temp + "\\blenderNodesLowPoly.fbx") #noPrompt
			saveNodes selection low_path
			max delete
			xref_b = objXRefMgr.AddXRefItemsFromFile low_path
			items = #()
			xref_b.GetItems #XRefObjectType &items
			for item in items do 
				(
					objXRefMgr.SetProxyItemSrcFile item high_path
					item.proxyItemName = item.objectName
				)
		),

		fn xref_low oFileDir oFileName= 
		(
			print (oFileDir +"\\"+ oFileName);
			oNames = #()
			for ss in selection do 
				(
					append oNames ss.name
				)
			objXRefMgr.dupObjNameAction  = #deleteOld
			xref_b = objXRefMgr.AddXRefItemsFromFile (oFileDir +"\\"+ oFileName + "_low0.max") objNames:oNames
			items = #()
			xref_b.GetItems #XRefObjectType &items
			for item in items do 
				(
					objXRefMgr.SetProxyItemSrcFile item (oFileDir +"\\"+ oFileName + "_high.max")
					item.proxyItemName = item.objectName
				)		
			-- delete not HL
			-- loadMaxFile strStudioFile useFileUnits:true --load the latest lighting file
			-- mergeMaxFile strFile #select #noRedraw #mergeDups #useMergedMtlDups #neverReparent --merge in everything from the hold-file
		),


		fn test_export oFileDir oName = 
		(
			max group open
			oo =  execute ("$'"+oName + "'")
			select oo
			max modify mode
			-- eDir = oFileDir +"\\y3d_data\\";
			eDir = oFileDir +"\\y3d_data\\"+ oName + "\\";
			makeDir eDir all:false

			theINI =objexp.getIniName()
			setIniSetting theINI"Optimize" "optVertex" "0"
			setIniSetting theINI"Optimize" "optNormals" "0"
			setIniSetting theINI"Optimize" "optTextureCoords" "0"
			setIniSetting theINI"Output" "Precision" "4"
			setIniSetting theINI"Geometry" "SmoothingGroups" "0"
			setIniSetting theINI"Geometry" "FaceType" "0"
			setIniSetting theINI"Geometry" "FlipZyAxis" "1"
			setIniSetting theINI"Geometry" "Normals" "0"
			setIniSetting theINI"Geometry" "Shapes" "0"
			setIniSetting theINI"Geometry" "TextureCoords" "1"
			setIniSetting theINI"Material" "CreateMatLibrary" "0"
			setIniSetting theINI"Material" "UseMaterial" "0"

			if (classof oo == XRefObject) then
				(
					oo.displayProxy = true;
					exportFile (eDir + oName + "_high.obj") #noPrompt selectedOnly:true 
					oo.displayProxy = false;
					unwrap3dmax 66 0.001
					exportFile (eDir + oName + "_low.obj") #noPrompt selectedOnly:true 
				)
			else 
				(
					unwrap3dmax 66 0.001
					exportFile (eDir + oName + ".obj") #noPrompt selectedOnly:true 
				)
			
		),

		fn set_display_proxy oname is_display =
		(
			oo =  execute ("$'"+oname + "'")
			if (classof oo == XRefObject) then
				(
					if (is_display) then
					(
						oo.displayProxy = true
					)
					else
					(
						oo.displayProxy = false
					)
				)
		),
		fn isolate_layer lname =
		(
			for i = 0 to layerManager.count-1 do
			(
				ilayer = layerManager.getLayer i
				if (ilayer.name==lname) then 
					ilayer.ishidden = false
				else
					ilayer.ishidden = true
			)				
		),

		fn delete_layer lname = 
		(
			LayerManager.deleteLayerByName lname
		),
		fn create_layer lname add_select isolate= 
		(
			ll = LayerManager.newLayerFromName lname
			if (add_select) then
			(
				for s in selection do 
				(
					ll.addnode s
				)
			)
			if (isolate) then
			(
				isolate_layer lname
			)
		),
		fn pre_optimize oFileDir oFileName projectPath = 
		(
			-- fname = substring maxFileName 1 (maxFileName.count-4)
			-- path = maxFilePath + fname + "_"
			
			-- MessageBox curFile;
			--oFileDir = "D:\\3D\\3dmax\\1";
			--oFileName = "phongkhach 1";
			BatchProOptimizer.SourceFileMode = 0;
			BatchProOptimizer.SourceFileFiles = oFileDir +"\\"+ oFileName + ".max";

			BatchProOptimizer.OptimizationLevel1 = 90;
			BatchProOptimizer.OptimizationLevel2 = 0;
			BatchProOptimizer.OptimizationLevel3 = 0;
			BatchProOptimizer.OptimizationLevel4 = 0;
			BatchProOptimizer.OptimizationLevel5 = 0;

			BatchProOptimizer.OptimizationLevelMode = 0;
			BatchProOptimizer.OptimizeBordersMode = 1; --Protect Borders 
			BatchProOptimizer.AutoRename = false;
			BatchProOptimizer.DestFileNameMode = 1;
			BatchProOptimizer.DestFolderMode = 2;
			--BatchProOptimizer.DestFolderName = ".\\"+oFileName+"_y3d";
			BatchProOptimizer.DestFolderName = projectPath;
			BatchProOptimizer.batch();
			loadMaxFile (projectPath+"\\" + oFileName + "90.max") useFileUnits:true quiet:true
			-- MessageBox "load xong"
		),

		fn new_project_from_file fname = 
		(
			MessageBox fname
			resetMaxFile #noPrompt
			mergeMaxFile fname #select #noRedraw #mergeDups #useMergedMtlDups #neverReparent quiet:true
			local arrObjects = selection as array --
			MessageBox (arrObjects.count as string)
			for obj in arrObjects where obj.parent == undefined do
				(
					print obj.name
				)
		),

		fn exportObjectToFBX filename exportFolder = 
		(
			if (exportFolder =="") then
				exportFolder = maxFilePath
			local filePath = exportFolder + filename
			exportFile filePath #noPrompt selectedOnly:true 
		),

		fn lowpoly_blender ratio = 
		(	
			tempPath = pathConfig.GetDir #temp
			blenderPath = "\"C:\\Program Files\\Blender Foundation\\Blender\\blender.exe\""
			exportFile (tempPath + "\\3dsNodesHightPoly.fbx") #noPrompt selectedOnly:true 
			local pythonScript = ypath  + "\\r2\\python\\blender.py"
			local cmd = blenderPath + " --factory-startup -b -P \"" + pythonScript + "\""
			cmd += " -- \"" + tempPath + "\""
			cmd += " makeLowPoly"
			if (ratio!=-1) then
				cmd += " " + ratio as string
			print cmd
			val = HiddenDOSCommand(cmd)
			-- val = DOSCommand(cmd)
		),

		fn object_test_mode oname intest=
		(
			if (intest) then
			(
				lowpoly =  execute ("$'"+oName + "'")
			)else(

			)
		),

		fn apply_normal tex_path oname  =
		(
			MessageBox tex_path
			local currentRenderer = renderers.current as string
			lowpoly =  execute ("$'"+oName + "'")
			select lowpoly		

			IsolateSelection.EnterIsolateSelectionMode()
			
			local bitmap =bitmaptexture filename:tex_path

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

			local lowPolyOriginalName = lowPoly.name+"_lowPoly_NoNormal"	
			local lowPolyNoMatName = lowPoly.name+"_lowPoly_noMat"
			local hightPolyNoMatName = lowPoly.name+"_hightPoly_noMat"
			local hightPolyName = lowPoly.name+"_hightPoly"

			copy lowPoly name:lowPolyOriginalName
			local lowPolyOriginal = getNodeByName lowPolyOriginalName exact:true all:false ignoreCase:false
			if (classof lowPolyOriginal == XRefObject) do lowPolyOriginal.displayProxy = false
			
			if (lowPolyOriginal.actualBaseObject as string)== "Editable Poly" then
				convertToPoly lowPolyOriginal
			else if (lowPolyOriginal.actualBaseObject as string) == "Editable Mesh" do
				convertToMesh lowPolyOriginal

			copy lowPoly name:hightPolyName
			local hightPoly = getNodeByName hightPolyName exact:true all:false ignoreCase:false
			if (classof hightPoly == XRefObject) do hightPoly.displayProxy = true

			print (hightPoly.actualBaseObject as string)

			if (hightPoly.actualBaseObject as string)== "Editable Poly" then
				convertToPoly hightPoly
			else if (hightPoly.actualBaseObject as string) == "Editable Mesh" do
				convertToMesh hightPoly

			local maxBBx = lowPoly.max.x
			local minBBx = lowPoly.min.x
			local maxBBy = lowPoly.max.y
			local minBBy = lowPoly.min.y
			local gap = 10
			local perimeterX = maxBBx - minBBx
			local perimeterY = maxBBy - minBBy
			local distanceMoveY = gap + perimeterY
			local distanceMoveX = gap + perimeterX

			move lowPolyOriginal [distanceMovex,-distanceMoveY,0]
			move hightPoly [-distanceMovex,-distanceMoveY,0]

			copy lowPolyOriginal name: lowPolyNoMatName
			local lowPolyNoMat = getNodeByName lowPolyNoMatName exact:true all:false ignoreCase:false
			lowPolyNoMat.material = undefined

			copy hightPoly name: hightPolyNoMatName
			local hightPolyNoMat = getNodeByName hightPolyNoMatName exact:true all:false ignoreCase:false
			hightPolyNoMat.material = undefined

			move lowPolyNoMat [distanceMovex,0,0]
			move hightPolyNoMat [-distanceMovex,0,0]

			local shellMat = Shell_Material originalMaterial:lowPoly.material\
				bakedMaterial:mat viewportMtlIndex:1 renderMtlIndex:1
			lowPoly.material = shellMat	
			showTextureMap lowPoly.material lowPoly.material.bakedMaterial true	

			local color = lowPoly.material.bakedMaterial.diffuse
			print (color as string)

			lowPolyNoMat.wirecolor = color
			hightPolyNoMat.wirecolor = color	
			-- lowPoly.material = undefined
		)

	)

	global yms = ymsClass()

	print "yms has been loaded"
	yms_init = true
)
