
-- global variables and functions
if minh_init != true then
(
	struct minhClass
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
		scriptFolder  = ypath  + "\\newm\\scripts",
		pythonScript =  ypath + "\\newm\\scripts\\blender.py",
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

		fn makeLowPoly ratio = 
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

		fn apply_normal tex_path oname  =
		(
			local currentRenderer = renderers.current as string
			lowpoly =  execute ("$'"+oName + "'")
			select lowpoly			
			
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

			local shellMat = Shell_Material originalMaterial:lowPoly.material\
				bakedMaterial:mat viewportMtlIndex:1 renderMtlIndex:1
			lowPoly.material = shellMat	
			showTextureMap lowPoly.material lowPoly.material.bakedMaterial true		
		),

		fn test1 oname = 
		(
			MessageBox oname
		),

		fn bake_3dmax high_obj low_obj bitmap_out tile_size =
		(
			MessageBox high_obj
		)

	)

	global minh = minhClass()

	print "minh has been loaded"
	minh_init = true
)
