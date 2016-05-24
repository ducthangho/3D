struct SIsland (indexes,f_area,area_uvw,fw,fh,udim,x,y,data)
struct Sface (index, f_area, area_uvw, fw, fh, udim, x, y, flag)
struct tileStruct (u,v,sizeX,sizeY)

fn recalculate &faces u =
(
	local x = 0
	local y = 0
	local width = 0
	local heigt = 0
	local areaUVW = 0
	local areaGeom =0
 	local index = 0

	for i = 1 to faces.count do
	(
		local index = faces[i].index
		u.unwrap4.getArea (#(index) as bitarray) &x &y &width &height &areaUVW &areaGeom

		faces[i].x = x
		faces[i].y = y
		faces[i].fw = width
		faces[i].fh = height
		faces[i].area_uvw = areaUVW
		faces[i].f_area = areaGeom
	)
)

fn getListRectsForIsland islands =
(
	local r = #()
	for i = 1 to islands.count do
	(
		local reg = MRectangle width:islands[i].fw height:islands[i].fh id:islands[i].indexes old_x: islands[i].x old_y:islands[i].y
		append r reg
	)
	return r
)

fn getListRects faces =
(
	local r = #()
	for i = 1 to faces.count do
	(
		local reg = MRectangle width:faces[i].fw height:faces[i].fh id:faces[i].index
		append r reg
	)
	return r
)

fn Multiply10powerX listRect n invert=
(
	if n == 1 do return listRect
	if invert == false then
	(
		for i = 1 to listRect.count do
		(
			local r = listRect[i]
			w = n*r.width
			h = n*r.width
			if w > 10 then
				listRect[i].width = floor (w + 0.5)
			else listRect[i].width = w
			if h > 10 then
				listRect[i].height = floor (n * r.height+0.5)
			else listRect[i].height = h
		)
	)else
	(
		for i = 1 to listRect.count do
		(
			local r = listRect[i]
			listRect[i].width = (r.width as float) / n
			listRect[i].height = (r.height as float) / n
		)
	)

	return listRect
)

fn repos &faces r s u=
(

	print "aaaaa"
	print faces[1].x

	for i =1 to r.count do
	(
		local l = r[i]
		for j =1 to l.count do
			for k = 1 to faces.count do
			(
				if( l[j].id == faces[k].index) do
				(
					local x = l[j].x / s
					local y = l[j].y  / s

					format " x y = % % \n" x y

					local uidm_u = mod i 10
					local udim_v = i / 10
					local udim = 1001 + uidm_u + udim_v * 10

					faces[k].udim = udim
					u.unwrap2.selectFaces (#(faces[k].index) as bitarray)
					u.unwrap2.moveSelected [-faces[k].x , -faces[k].y, 0 ]
					u.unwrap2.moveSelected [uidm_u + x ,udim_v + y , 0 ]
					faces[k].x = uidm_u + x
					faces[k].y = udim_v + y
				)
			)
	)
)

fn packFace &faces u =
(
	local n = 1000
	local listRect = getListRects faces
	listRect = Multiply10powerX listRect n false
	local enclosingRect = MRectangle width:n height: n
	local packedAll = false
	local r = packTile listRect enclosingRect 1000 &packedAll
	repos &faces r n u
)

fn getNumfaceSelected u =
(
		selectedFace = u.unwrap2.getSelectedFaces()
		local numface = selectedFace.numberSet
		return numface
)

function getIslandIndexes tempFaces u =
(
	local r = #()
	for i = 1 to tempFaces.count do
	(
			if tempFaces[i].flag != -1 do continue

			u.unwrap2.selectFaces (#(tempFaces[i].index) as bitarray)
			u.unwrap.expandSelection()
			local selectedFace = u.unwrap2.getSelectedFaces()
			local numSelectedFace = selectedFace.numberSet
			local count = 1

			while (numSelectedFace > count ) do
			(
				count = numSelectedFace
				u.unwrap.expandSelection()
				selectedFace = u.unwrap2.getSelectedFaces()
				numSelectedFace = selectedFace.numberSet
			)

			tempArray = selectedFace as array
			for j = 1 to tempArray.count do
				for k =1 to tempFaces.count do
				(
					if tempArray[j] == tempFaces[k].index do tempFaces[k].flag = 1
				)

			append r selectedFace
	)

	return r
)

function getIslandInfo listIndexes u =
(
	local x =1
	local y = 1
	local width =1
	local height =1
	local area_uvw = 1
	local f_area =1
	local islands = #()
	for i = 1 to listIndexes.count do
	(
		local indexes = listIndexes[i]
		u.unwrap4.getArea indexes &x &y &width &height &area_uvw &areaGeom
		local island = SIsland indexes:indexes f_area:areaGeom area_uvw: area_uvw fw:width fh:height x:x y:y
		append islands island
	)
	return islands
)

fn getSFace faces =
(
	local tempFaces = #()
	for i = 1 to faces.count do
	(
		local f = faces[i]
		local a = Sface index:f.index f_area:f.f_area area_uvw:f.area_uvw fw:f.fw fh:f.fh udim:f.udim x:f.x y:f.y flag:-1
		append tempFaces a
	)

	return tempFaces
)

fn getIsland &faces u =
(
	local tempFaces = getSFace faces
	local r = getIslandIndexes tempFaces u
	islands = getIslandInfo r u
	return islands
)

fn reposIslands islands pack n u beginTile=
(
	local udim_u
	local udim_v
	local udim
	for i = 1 to pack.count do
	(
		local rects = pack[i]
		for j = 1 to rects.count do
		(
			local r = rects[j]
			udim_u = mod (i+beginTile - 1) 10
			udim_v = (i+beginTile - 1) / 10
			u.unwrap2.selectFaces r.id
			local x = (r.x as float)/ n
			local y = (r.y as float)/ n
			x = x + udim_u
			y = y + udim_v
			u.unwrap2.moveSelected [-r.old_x, -r.old_y, 0]
			u.unwrap2.moveSelected [x,y,0]
		)
	)
)

fn packisland islands u beginTile n =
(
	local listRect = getListRectsForIsland islands
	listRect = Multiply10powerX listRect n false
	local enclosingRect = MRectangle width:n height: n
	local packedAll = false
	local r = packTile listRect enclosingRect 1000 &packedAll
	reposIslands islands r n u beginTile
	return r.count
)

fn rePosition &faces u =
(
	local islands = getIsland &faces u
	packIsland islands u 1 1
)

fn getIslandFitSizeAndScale islands tileSize density u =
(
	local tileWidth = tileSize[1]
	local tileHeight = tileSize[2]

	local areaTile = (tileWidth * tileHeight) /  density

	local islandsFit = #()
	local islandsNotFit = #()
	for i = 1 to islands.count do
	(
		local island = islands[i]
		local uvw_area_abs = island.area_uvw * areaTile
		local scaleArea = island.f_area/uvw_area_abs
		local scaleEdge = sqrt scaleArea
		local islandWidth = island.fw * scaleEdge
		local islandHeight = island.fh * scaleEdge

		format "----------------------------------------\n"
		format "island: %\n" island
		format "areatile: %, tileSize: %, density: %\n" areaTile tileSize density
		format "area_uvw: %, are_uvw_abs: %, f_area: %\n" island.area_uvw uvw_area_abs island.f_area
		format "scalse Area: %, scaleEdge: %, islandWidth: %, islandHeight: %\n" scaleArea scaleEdge islandWidth islandHeight
		format "-------------------****-----------------\n"

		if(islandWidth <=1  and islandHeight <=1) then
		(
			u.unwrap2.selectFaces island.indexes
			u.unwrap2.scaleSelectedXY scaleEdge scaleEdge [island.x,island.y,0]
			island.fw = islandWidth
			island.fh = islandHeight
			island.area_uvw = island.area_uvw * scaleArea
			append islandsFit island
		)else
		(
			append islandsNotFit island
		)
	)
	local r = #()
	append r islandsFit
	append r islandsNotFit
	return r
)

fn getTile i tileSize =
(
	format "tilesize: %" tileSize
	u = mod i 10
	v = i / 10
	return tileStruct u:u v:v sizeX:tileSize[1] sizeY:tileSize[2]
)

fn packIslandTile islands listTileSize density u =
(
	local beginTile = 1
	local tempIsland = islands
	sum = 0
	for i = 1 to listTileSize.count do
	(
		if(tempIsland.count == 0) do exit
		local tileSize = listTileSize[i]
		local r = getIslandFitSizeAndScale tempIsland tileSize density u
		local islandsFit = r[1]
		local numTile = packIsland islandsFit u beginTile 1000
		beginTile = beginTile+numTile
		tempIsland = r[2]
		for j = 1 to numTile do
		(
			sum = sum + 1
			tile = getTile sum tileSize
			append tileArray tile
		)
	)
)

fn scaleIslandInfo islands listTileSize density = 
(
	
	local maxTileSizeNeedIndex = 1
	local maxIndex = listTileSize.count
	local maxTileSize = 
	local listFit = #()
	local listUnFit = #()

	for j = 1 to maxIndex do
	(
		maxTileSizeNeedIndex = j
		local flag = true
		for i = 1 to islands.count do
		(
			local tile = listTileSize[j]
			local island = islands[i]
			
			local areaTile = (tile[1]*tile[2]) as float / density
			local uvw_area_abs = island.area_uvw * areaTile
			local scaleArea = island.f_area/uvw_area_abs
			local scaleEdge = sqrt scaleArea				
			local islandWidth = island.fw * scaleEdge				
			local islandheight 
			local area 
			if islandWidth>1 then
			(
				flag = false
				if (maxTileSizeNeedIndex < maxIndex) do exit
			)					
			else(
				local islandHeight = island.fh * scaleEdge
				if islandHeight>1 then
				(
					flag = false
					if (maxTileSizeNeedIndex < maxIndex) do exit						
				)						
			)
			
			if (islandWidth > 1 or islandheight > 1) then
			(
				if (islandWidth > islandheight) then
				(
					islandWidth = 1
					scaleEdge = (1 as float) / island.fw
					islandheight = island.fh * scaleEdge
				)else 
				(
					islandheight = 1
					scaleEdge = (1 as float) / island.fh
					islandWidth = island.fw * scaleEdge
				)
				data = #(scaleEdge, islandWidth,islandHeight,island.area_uvw*scaleEdge*scaleEdge)
				append listUnFit island
			)
			else
			(
				data = #(scaleEdge, islandWidth,islandHeight,island.area_uvw*scaleArea)
				append listFit island
			)
			
			island.data = data
			
		)
		
		if (flag == true and maxTileSizeNeedIndex < maxIndex) do
		(
			listFit = islands
			exit
		)				
	)

	return #(islands,listFit,listUnFit,listTileSize[maxTileSizeNeedIndex])
)

fn scaleIslandAndPack islands listTileSize maxSizeTile density u roundFactor= 
(
	print "list islands:"
	print islands
	
	local t = scaleIslandInfo islands listTileSize density
	
	print "list islands2:"
	print islands
	
	local a = t[1]
	local listFit = t[2]
	local listUnFit = t[3]
	local tileSize = t[4]
	
	for island in a do
	(
		u.unwrap2.selectFaces island.indexes
		scaleEdge = island.data[1]
		u.unwrap2.scaleSelectedXY scaleEdge scaleEdge [island.x,island.y,0]
		island.fw = island.data[2]
		island.fh = island.data[3]
		island.area_uvw = island.data[4]
	)
	
	print "list islands3:"
	print islands
	print "listFit"
	print listFit
	
	local numTile = packisland listFit u 1 roundFactor
	local numTile2 = 0
	if listUnFit.count != 0 do 
		numTile2 = packisland listUnFit u numTile roundFactor
	
	for i = 1 to numTile do
	(
		local tile = getTile i tileSize
		append TileArray tile
	)
	for i = 1 to numTile2 do
	(
		local tile = getTile i maxSizeTile
		append TileArray tile
	)

)

fn unwrapSelection u =
(
	local islands = getIsland importantFacesArray u
	local tileSize1 = #(512,512)
	local tileSize2 = #(1024,1024)
	local listTileSize = #(tileSize1,tileSize2)
	local maxSizeTile = #(2048,2048)
	local density = 64
	scaleIslandAndPack islands listTileSize maxSizeTile density u 1
)

fn test =
(
	print importantFacesArray
	print u1
	islands = getIsland importantFacesArray u1
	format "island : %\n" islands[1]
	print islands
	
	tileSize1 = #(512,512)
	listTileSize = #(tileSize1)
	maxSizeTile = #(2048,2048)
	density = 64
	
	packIsland islands u1 1 1
		
	scaleIslandAndPack islands listTileSize maxSizeTile density u1
	
	print islands
)

------- Old Pack ------------
-- Tim tap cac mat co tong dien tich lon nhat ma be hon tham so narea
fn findBestFit bSet narea importantFaces=
	(
		for k = 1 to importantFaces.count do
		(
			if (importantFaces[k].udim==1000)and(importantFaces[k].area_uvw<=narea) then (
				append bSet k
				narea = narea-importantFaces[k].area_uvw
			)
		)
		bestSet = bSet
		best = narea
	)
