--Translate tile to original coordinate
global bakeVrayCompleteTile

fn bakeVrayCompleteTile obj sizeX sizeY MapChannel u v = 
(
	moveTileToOriginal obj u v
	udim = 1000+(u+1)+(v*10)
	tileName = udim as string
	BakeVrayCompletemap obj sizeX sizeY MapChannel tileName
)

fn moveTileToOriginal object u v =
(
	
	modPanel.setCurrentObject object.modifiers[#Unwrap_UVW]
	subobjectLevel = 3
	actionMan.executeAction 0 "40021"  -- Selection: Select All
	object.modifiers[#Unwrap_UVW].unwrap2.MoveSelected [-u,-v,0]
)

--reset entire tiles position
fn resetTilePosition object =
(
	moveTileToOriginal object 0 0
)

fn bake  obj sizeX sizeY Mapchannel tileName bakeMapType bakeMapTypeName = 
(
	obj.iNodeBakeProperties.removeAllBakeElements() 
	be1 = bakeMapType --instance of the bake element class
	be1.outputSzX = sizeX
	be1.outputSzY = sizeY
	be1.fileType = (getDir #image+"\\"+obj.name+bakeMapTypeName+tileName+".jpg")
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


fn BakeVrayCompletemap obj sizeX sizeY Mapchannel tileName = 
(
	bakeMaptype = Vraycompletemap()
	--showProperties bakeMaptype
	bake obj sizeX sizeY Mapchannel tileName bakeMaptype "VrayCompletemap"
)

-- bakeDiffuseTile $ 1024 1024 2 0 0
