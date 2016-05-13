struct MRectangle (x=-1.0,y=-1.0,width,height)
struct SAreaReg (x,y,width,height,isFree)
struct Pos (x,y)

fn compareOffsetX regArea1 regArea2 = 
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


fn findPlace reg listArea enclosingReg = 
(	
	qsort listArea compareOffsetX	
	for i = 1 to listArea.count do 
	(	
		if listArea[i].isFree == false do continue
			
		local area = listArea[i]
		if reg.width > (enclosingReg.width - area.x) do return Pos x:-1 y:-1
		if reg.height <= area.height do return Pos x:area.x y:area.y
		for j = i+1 to listArea.count do
		(
			local area2 = listArea[j]
			if area2.isFree == false do continue
			if area2.x > area.x then 
			exit else
			(
				if ((area.height + area2.height) >= reg.height and (area.height+area.y == area2.y)) do return Pos x:area.x y:area.y
			)
		)
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

-- enclosingReg = MRectangle width:36 height:40
-- r1 = MRectangle width:15 height:30
-- listReg = #(r1)
-- listArea = #(SAreaReg x:0 y:0 width:enclosingReg.width height:enclosingReg.height isFree:true)
-- p = findPlace r1 listArea enclosingReg
-- place r1 p
-- print r1
-- updateListArea &ListArea r1
-- print ListArea

-- r2 = MRectangle width: 20 height:25
-- p = findPlace r2 listArea enclosingReg
-- place r2 p
-- print r2
-- updateListArea &ListArea r2
-- print listArea

-- r5 =  MRectangle width: 10 height:10
-- p = findPlace r5 listArea enclosingReg
-- place r5 p
-- print r5
-- updateListArea &listArea r5
-- print listArea

-- r4 = MRectangle width:20 height:8
-- p = findPlace r4 listArea enclosingReg
-- place r4 p
-- print r4
-- updateListArea &listArea r4
-- print listArea

-- r3 = MRectangle width:9 height: 7
-- p = findPlace r3 listArea enclosingReg
-- place r3 p
-- print r3
-- updateListArea &listArea r3
-- print listArea

-- listReg = #(r1,r2,r3,r4,r5)
-- print listReg

-- pack listReg enclosingReg

-- print listReg

--------------------------------------example----------------
-- the rectangle will be holding all list specific regtangle
enclosingReg = MRectangle width:36 height:40
-- list regtangle to be packed 
r1 = MRectangle width:15 height:30
r2 = MRectangle width: 20 height:25
r3 = MRectangle width:9 height: 7
r4 = MRectangle width:20 height:8
r5 =  MRectangle width: 10 height:10
--  (if the x property of MRectable is -1 then this rectangle will not be packed)
listReg = #(r1,r2,r3,r4,r5)
-- call pack function

numTile = 2
packedAll = false
r = packTile listReg enclosingReg numTile &packedAll

print r
print packedAll

-- print the result, the new place of each rectangle contain in x and y property
--print listreg