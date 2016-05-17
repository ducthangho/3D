(
struct MRectangle (x=-1.0,y=-1.0,width,height,id)
struct SAreaReg (x,y,width,height,isFree)
struct Pos (x,y)
global bestSet = #()
global best = -1

global rePosition
global findBestFit

fn compareOffsetXthenY regArea1 regArea2 =
(
	if (regArea1.x < regArea2.x) do return -1
	if (regArea1.x > regArea2.x) do return 1
	if (regArea1.y < regArea2.y) do return -1
	if (regArea1.y > regArea2.y) do return 1
	return 0
)

fn compareHeightReg reg1 reg2 =
(
	case of
	(
		(reg1.height <  reg2.height) : -1
		(reg1.height > reg2.height) : 1
		default : 0
	)
)

fn invertCompareHeight reg1 reg2  =
(
	-(compareHeightReg reg1 reg2)
)

fn isOverlap rect1 rect2 =
(
	local left1 = rect1.x
	local above1 = rect1.y
	local right1 = rect1.x + rect1.width
	local below1 = rect1.y + rect1.height

	local left2 = rect2.x
	local above2 = rect2.y
	local right2 = rect2.x + rect2.width
	local below2 = rect2.y + rect2.height

	if (right1 <= left2 or below1 <= above2 or right2 <= left1 or below2 <= above1) do
		return false

	return true
)


fn findPlace reg listArea enclosingReg =
(
	qsort listArea compareOffsetXthenY

	for i = 1 to listArea.count do
	(
		if listArea[i].isFree == false do continue

		local area = listArea[i]
		if reg.width > (enclosingReg.width - area.x) do return Pos x:-1 y:-1
		if reg.height > (enclosingReg.height - area.y) do continue

		local tempRect = MRectangle x:area.x y:area.y width:reg.width height:reg.height
		local flag = true
		for j = 1 to listArea.count do
		(
			area2 = listArea[j]
			if (area2.isFree == false and (isOverlap tempRect area2) == true) do
			(
				flag = false
				exit
			)
		)
		if flag == true do return Pos x:area.x y:area.y
	)
	return Pos x:-1 y:-1
)

fn breakEdgeWidth areaReg p =
(
	if (p.x > areaReg.x and p.x < (areaReg.width+areaReg.x)) do
	(
		local width1 = p.x - areaReg.x
		local width2 = areaReg.width - width1

		local r1 = SAreaReg x:areaReg.x y:areaReg.y width:width1 height: areaReg.height isFree: areaReg.isFree
		local r2 = SAreaReg x:p.x y:areaReg.y width:width2 height:areaReg.height isFree: areaReg.isFree
		return #(r1,r2)
	)
	return #(areaReg)
)

fn breakEdgeHeight areaReg p =
(

	if(p.y > areaReg.y and  p.y < (areaReg.height + areaReg.y)) do
	(
		local height1 = p.y - areaReg.y
		local height2 = areaReg.height - height1
		local r1 = SAreaReg x:areaReg.x y:areaReg.y width:areaReg.width height:height1 isFree:areaReg.isFree
		local r2 = SAreaReg x:areaReg.x y:p.y width:areareg.width height:height2 isFree:areaReg.isFree
		return #(r1,r2)
	)

	return #(areaReg)
)

fn breakAreaReg areaReg p =
(
	local r = #()
	local ar = breakEdgeWidth areaReg p
	if ar.count > 1 then
		for i = 1 to ar.count do
		(
			local ar2 = breakEdgeHeight ar[i] p
			join r ar2
		)
	else
	(
			local ar2 = breakEdgeHeight areaReg p
			return ar2
	)
	if r.count > 0 do return r
	return #(areaReg)
)

fn unplace reg =
(
	reg.x = reg.y = -1
)

fn place reg p =
(
	reg.x = p.x
	reg.y = p.y
)

fn updateListArea &listArea reg =
	(
		local r = #()
		local p = Pos x:(reg.x+reg.width) y:(reg.y + reg.height)
		for i = 1 to listArea.count do
		(
			local ar = breakAreaReg listArea[i] p
			join r ar
		)

		for i = 1 to r.count do
		(
			if( (r[i].isFree == true) and (isOverLap r[i] reg)==true) then
			(
				r[i].isFree = false
			)
		)
		listArea = r
	)

fn pack listReg enclosingReg =
	(
		qsort listReg invertCompareHeight
		local listArea = #(SAreaReg x:0 y:0 width:enclosingReg.width height:enclosingReg.height isFree:true)
		for i=1 to listReg.count do
		(
			local p = findPlace listReg[i] listArea enclosingReg
			if ( p.x >=0 ) do
			(
				place listReg[i] p
				updateListArea &listArea listReg[i]
			)
		)
		return listReg
	)

fn packTile listReg enclosingReg n &packedAll=
	(
		local r = #()
		packedAll = false

		local unpacked = listReg
		for i = 1 to n do
		(
			local a = pack unpacked enclosingReg
			local packed = #()
			unpacked = #()
			for j = 1 to a.count do
			(
				if (a[j].x != -1) then append packed a[j]
				else append unpacked a[j]
			)
			append r packed

			if unpacked.count == 0 do
			(
				packedAll = true
				exit
			)
		)
		return r
	)

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
			index = faces[i].index
			u.unwrap4.getArea (#(index) as bitarray) &x &y &width &height &areaUVW &areaGeom

			faces[i].x = x
			faces[i].y = y
			faces[i].fw = width
			faces[i].fh = height
			faces[i].area_uvw = areaUVW
			faces[i].f_area = areaGeom
		)
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

		tileArray = #()
		-- tileArray[1] = tileStruct u:0 v:0 sizeX:defaultSizeX sizeY:defaultSizeY
		-- tileArray[i+1] = tileStruct u:move_u v:move_v sizeX:defaultSizeX sizeY:defaultSizeY
		for i =1 to r.count do
		(
			local uidm_u = mod i 10
			local udim_v = i / 10
			local udim = 1001 + uidm_u + udim_v * 10

			local l = r[i]
			for j =1 to l.count do
				for k = 1 to faces.count do
				(
					if( l[j].id == faces[k].index) do
					(
						local x = l[j].x / s
						local y = l[j].y  / s
						format " x y = % % \n" x y
						faces[k].udim = udim
						u.unwrap2.selectFaces (#{faces[k].index})
						u.unwrap2.moveSelected [-faces[k].x , -faces[k].y, 0 ]
						u.unwrap2.moveSelected [uidm_u + x ,udim_v + y , 0 ]
						faces[k].x = uidm_u + x
						faces[k].y = udim_v + y
					)
				)
			tileArray[i] = tileStruct u:uidm_u v:udim_v sizeX:defaultSizeX sizeY:defaultSizeY
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

fn rePosition &faces u =
	(
		recalculate &faces u
		packFace &faces u
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



)
