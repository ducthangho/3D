--Translate tile to original coordinate

(
	global bakeTile
	global resetTilePosition
	global bakeFolder = getDir #image
	global prefixFName = ""
	global mapBakeType
	global bakeVrayCompleteTile
	global filetype_bake = "jpg"

	fn bake  obj sizeX sizeY Mapchannel fname bakeMapType bakeMapTypeName =
	(
		obj.iNodeBakeProperties.removeAllBakeElements()
		be1 = bakeMapType --instance of the bake element class
		be1.outputSzX = sizeX
		be1.outputSzY = sizeY
		-- be1.fileType = (bakeFolder+"\\"+obj.name+bakeMapTypeName+tileName+".jpg")
		be1.fileType = (bakeFolder+"\\"+prefixFName+fname+"."+filetype_bake)
		be1.fileName = filenameFromPath be1.fileType
		be1.filterOn = true --enable filtering
		--be1.shadowsOn = false --disable shadows
		--be1.lightingOn = false --disable lighting
		be1.enabled = true --enable baking


		obj.INodeBakeProperties.addBakeElement be1 --add first element
		obj.INodeBakeProperties.bakeEnabled = true --enabling baking
		obj.INodeBakeProperties.bakeChannel = MapChannel --channel to bake
		obj.INodeBakeProperties.nDilations = 1 --expand the texturea bit
		select obj --we are baking the selection, so we select the object
	--Call the renderer to bake both elements:
		render rendertype:#bakeSelected vfb:off progressBar:true outputSize:[sizeX,sizeY]
	)

	fn BakeVrayCompletemap obj sizeX sizeY Mapchannel fname =
	(
		bm = Vraycompletemap()
		bm.color_mapping = true
		bm.targetMapSlotName = "Self-Illumination"
		--showProperties bakeMaptype
		bake obj sizeX sizeY Mapchannel fname bm "VrayCompletemap"
	)

	fn bakeTile obj sizeX sizeY MapChannel u v mapType=
	(
		if mapType == 1 then
			(
				if prefixFName=="" then prefixFName = obj.name + "_" + "VrayCompletemap_"
				fname = 1000+(u+1)+(v*10)
				bakeVrayCompleteTile obj sizeX sizeY MapChannel u v (fname as string)
			)

	)

	fn moveTileToOriginal object u v =
	(

		modPanel.setCurrentObject object.modifiers[#y3d_unwrap]
		subobjectLevel = 3
		actionMan.executeAction 0 "40021"  -- Selection: Select All
		object.modifiers[#y3d_unwrap].unwrap2.MoveSelected [-u,-v,0]
	)

	fn bakeVrayCompleteTile obj sizeX sizeY MapChannel u v fname=
	(
		moveTileToOriginal obj u v
		-- udim = 1000+(u+1)+(v*10)
		-- tileName = udim as string
		BakeVrayCompletemap obj sizeX sizeY MapChannel fname
		moveTileToOriginal obj -u -v
	)


)
