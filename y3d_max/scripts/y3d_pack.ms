struct SIsland (indexes,f_area,area_uvw,fw,fh,udim,x,y)
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
		format "??? ---% %----??\n" islands.count islands[i]
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

fn MultiplyAsInt listRect n invert=
(
	if invert == false then
	(
		for i = 1 to listRect.count do
		(
			local r = listRect[i]
			format "here % - %\n" r.width n
			listRect[i].width = floor (n * r.width + 0.5)
			listRect[i].height = floor (n * r.height+0.5)
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
	listRect = MultiplyAsInt listRect n false
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
-- struct SIsland (indexes,f_area,area_uvw,fw,fh,udim,x,y)
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
			format "r.index %\n" r.id
			udim_u = mod (i+beginTile - 1) 10
			udim_v = (i+beginTile - 1) / 10
-- 			udim = 1001 + udim_u + udim_v*10
			u.unwrap2.selectFaces r.id
			local x = r.x / n
			local y = r.y / n
			x = x + udim_u
			y = y + udim_v
			u.unwrap2.moveSelected [-r.old_x, -r.old_y, 0]
			u.unwrap2.moveSelected [x,y,0]
		)
	)
)

fn packisland islands u beginTile =
(
	local n = 1000
	local listRect = getListRectsForIsland islands
	listRect = MultiplyAsInt listRect n false
	local enclosingRect = MRectangle width:n height: n
	local packedAll = false
	local r = packTile listRect enclosingRect 1000 &packedAll
	reposIslands islands r n u beginTile
	return r.count
)

fn rePosition &faces u =
(
-- 	recalculate &faces u
	islands = getIsland &faces u
	packIsland islands u 1
)
-- struct SIsland (indexes,f_area,area_uvw,fw,fh,udim,x,y)



fn getIslandFitSize islands tileSize density u =
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
	print listTileSize
	local beginTile = 1
	local tempIsland = islands
	sum = 0
	for i = 1 to listTileSize.count do
	(
		if(tempIsland.count == 0) do exit
		local tileSize = listTileSize[i]
		local r = getIslandFitSize tempIsland tileSize density u
		local islandsFit = r[1]
		local numTile = packIsland islandsFit u beginTile
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

fn unwrapSelection u =
(
	local islands = getIsland importantFacesArray u

	local tileSize1 = #(512,512)
	local tileSize2 = #(1024,1024)
	local tileSize3 = #(2048,2048)
	local listTileSize = #(tileSize1,tileSize2,tileSize3)
	local density = 128

	packIslandTile islands listTileSize density u
)

fn test =
(
	print u1
	print importantFacesArray
	islands = getIsland importantFacesArray u1
	print islands

	tileSize1 = #(512,512)
	tileSize2 = #(1024,1024)
	tileSize3 = #(2048,2048)
	listTileSize = #(tileSize1,tileSize2,tileSize3)
	density = 128

	r = getIslandFitSize islands tileSize1 density u1
	print r[1].count
	print r[2].count


	r = getIslandFitSize r[2] tileSize2 density u1
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
