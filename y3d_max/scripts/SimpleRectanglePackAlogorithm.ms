struct MRectangle (x=-1.0,y=-1.0,width,height,id, old_x, old_y)
struct SAreaReg (x,y,width,height,isFree)
struct Pos (x,y)
aw = 3
ah = 4

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

fn hasNStepBottom Area step = 
(
	if step == 1 then return true
	else
	(	
		tArea = area
		while (tArea.bottom != undefined) do
			step -= 1
		if step == 1 do return true
	)
	return false
)

fn fIndex w h = 
(
	return h*aw + w
)

fn findHeightStep i j reg listArea area= 
(
	if area.height >= reg.height then return 0 else
	(		
		local needHeight = reg.height
		local currentHeight = area.height
		local t = i+1
		local bottomArea = undefined
		if t <= ah do bottomArea = listArea[t][j]
		local stepheight = 0	
		while ( bottomArea != undefined and bottomArea.isFree == true and currentHeight < needHeight) do
		(	
			stepHeight += 1
			currentHeight = currentHeight + bottomArea.height
			if(currentHeight >= needHeight) then 
			(
				return stepHeight
			)
			else
			(
				t += 1
				if t <= ah then bottomArea = listArea[t][j] else bottomArea = undefined
			)
		)
	)
	
	return -1
)

fn findWidthStep i j reg listArea area heightStep = 
(
	if area.width >= reg.width then return 0 else
	(		
		local needWidth = reg.width
		local currentWidth = area.width
		local t = j+1
		local stepWidth = 0
		local leftArea = undefined
		if t <= aw then leftArea = listArea[i][t] 
			
		while (leftArea != undefined and leftArea.isFree == true and currentWidth < needWidth and (hasStepHeight i t heightStep listArea) ) do
		(
			stepWidth += 1
			currentWidth += leftArea.width
			if(currentWidth >= needWidth) then return stepWidth
			else
			(
				t += 1
				if t <= aw then leftArea = listArea[i][t] else leftArea = undefined
			)
		)
	)
	
	return -1
)
	

fn hasStepHeight i j stepHeight listArea = 
(
	for step = 1 to stepHeight do
	(	
		if  listArea[i+step][j].isFree == false do return false
	)
	return true
)


fn findPlace reg listArea enclosingReg = 
(	
	for j = 1 to aw do
		for i = 1 to ah do
		(
			local area = listArea[i][j]
			if area.x + reg.width > enclosingReg.width do return #(-1,-1,-1,-1,-1,-1)
			if area.isFree == true do
			(
				local heightStep = findHeightStep i j  reg listArea area
				if heightStep >= 0 do 
				(
					local widthStep = findWidthStep i j reg listArea area heightStep
					if widthStep >=0 do return #(area.x,area.y,i,j,heightStep+i,widthStep+j)
				)
			)
		)
	return #(-1,-1,-1,-1,-1,-1)
)

fn breakEdgeWidth areaReg p = 
(	
	if (p.x > areaReg.x and p.x < (areaReg.width+areaReg.x)) do 
	(
		local width1 = p.x - areaReg.x
		local width2 = areaReg.width - width1
	
		local r1 = SAreaReg x:areaReg.x y:areaReg.y width:width1 height: areaReg.height isFree: areaReg.isFree leftA:areaReg.leftA rightA:areaReg.rightA topA:areaReg.topA bottomA:areaReg.bottomA 
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

fn place reg x y = 
(
	reg.x = x
	reg.y = y
)

fn updateListArea &listArea reg brow bcol erow ecol = 
(	
	local area = listArea[1][ecol]
	local maxWidthPos = area.x+area.width
	local mwp = reg.x+reg.width
	local w2 = maxWidthPos - mwp
	local w1 = mwp - area.x
	if w2 > 0 do
	(
		aw += 1
		for i = 1 to ah do
		(
			local area2 = listArea[i][ecol]
			area2.width = w1
			area =  SAreaReg x:(area2.x+w1) y:area2.y width:w2 height:area2.height isFree:area2.isFree
			insertItem area listArea[i] (ecol+1)
		)
	)
	area = listArea[erow][1]
	local maxHeightPos = area.y + area.height
	local heightPosRect = reg.y+reg.height
	local h2 = maxHeightPos - heightPosRect
	local h1 = heightPosRect - area.y
	if h2 > 0 do 
	(
		local newRow = #()
		ah += 1
		for i = 1 to aw do
		(
			local area2 = listArea[erow][i]
			area2.height = h1
			area = SAreaReg x:area2.x y:(area2.y+h1) width:area.width height:h2 isFree:area2.isFree
			append newRow area
		)
		
		insertItem newRow listArea (erow+1)
	)
	
	for i = brow to erow do
		for j = bcol to ecol do
			listArea[i][j].isFree = false
)

fn pack listReg enclosingReg = 
(
	qsort listReg invertCompareHeight	
	local listArea = #(#(SAreaReg x:0 y:0 width:enclosingReg.width height:enclosingReg.height isFree:true))
	aw = 1
	ah = 1
	for i=1 to listReg.count do
	(	
		reg = listReg[i]
		local p = findPlace reg listArea enclosingReg
		if ( p[1] >=0 ) do
		(
			place reg p[1] p[2]
			updateListArea &listArea reg p[3] p[4] p[5] p[6]
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


-- area11 = SAreaReg x:0 y:0 width:15 height:25 isFree:false
-- area12 = SAreaReg x:15 y:0 width:20 height:25 isFree:false
-- area13 = SAreaReg x:35 y:0 width:5 height: 25 isFree:true

-- area21 = SAreaReg x:0 y:25 width:15 height:5 isFree:false
-- area22 = SAreaReg x:15 y:25 width:20 height:5 isFree:true
-- area23 = SAreaReg x:35 y:25 width:5 height:5 isFree:true

-- area31 = SAreaReg x:0 y:30 width:15 height:10 isFree:true
-- area32 = SAreaReg x:15 y:30 width:20 height:10 isFree:true
-- area33 = SAreaReg x:35 y:30 width:5 height:10 isFree:true

-- area41 = SAreaReg x:0 y:40 width:15 height:8 isFree:true
-- area42 = SAreaReg x:15 y:40 width:20 height:8 isFree:true
-- area43 = SAreaReg x:35 y:40 width:5 height:8 isFree:true

-- aw = 3
-- ah = 4
-- listArea = #(#(area11,area12,area13),#(area21,area22,area23),#(area31,area32,area33),#(area41,area42,area43))
-- 	
-- 	
-- enclosingReg = MRectangle width:40 height:40
-- r3 = MRectangle x:0 y:30 width:38 height:12
-- p = findPlace r3 listArea enclosingReg

-- updateListArea  &listArea r3  3 1 4 3  
-- 	
-- print listArea.count
-- print listArea[4]

--------------------------------------- test 2-----------------------------------------

-- enclosingReg = MRectangle width:40 height:40
-- r1 = MRectangle width:15 height:30
-- listReg = #(r1)
-- listArea = #(#(SAreaReg x:0 y:0 width:enclosingReg.width height:enclosingReg.height isFree:true))
-- aw = 1 
-- ah = 1
-- p = findPlace r1 listArea enclosingReg
-- place r1 p[1] p[2]
-- print r1
-- updateListArea &ListArea r1 p[3] p[4] p[5] p[6]
-- print ListArea

-- r2 = MRectangle width: 20 height:25
-- p = findPlace r2 listArea enclosingReg
-- place r2 p[1] p[2]
-- print r2
-- updateListArea &ListArea r2 p[3] p[4] p[5] p[6]
-- print listArea

-- r5 =  MRectangle width: 10 height:10
-- p = findPlace r5 listArea enclosingReg
-- place r5 p[1] p[2]
-- print r5
-- updateListArea &listArea r5 p[3] p[4] p[5] p[6]
-- print listArea

-- print aw
-- print ah

-- r4 = MRectangle width:20 height:8
-- p = findPlace r4 listArea enclosingReg
-- place r4 p[1] p[2]
-- print r4
-- updateListArea &listArea r4 p[3] p[4] p[5] p[6]
-- print listArea

-- r3 = MRectangle width:9 height: 7
-- p = findPlace r3 listArea enclosingReg
-- place r3 p[1] p[2]
-- print r3
-- updateListArea &listArea r3 p[3] p[4] p[5] p[6]
-- print listArea

-- listReg = #(r1,r2,r3,r4,r5)
-- print listReg

-- pack listReg enclosingReg

-- print listReg

--------------------------------------example----------------
-- the rectangle will be holding all list specific regtangle
-- enclosingReg = MRectangle width:500 height:500
-- list regtangle to be packed 
-- r1 = MRectangle width:15 height:30 id:1
-- r2 = MRectangle width: 20 height:25 id:2
-- r3 = MRectangle width:9 height: 7 id:3
-- r4 = MRectangle width:20 height:8 id:4
-- r5 =  MRectangle width: 10 height:10 id:5
--  (if the x property of MRectable is -1 then this rectangle will not be packed)
-- listReg = #(r1,r2,r3,r4,r5)
-- call pack function

-- listReg = #()
-- for i = 1 to 500 do 
-- (
-- 	w = random 1 30
-- 	h = random 1 40
-- 	r = MRectangle width:w height:h 
-- 	append listReg r
-- )

-- print listReg.count

-- numTile = 10
-- packedAll = false
-- r = packTile listReg enclosingReg numTile &packedAll

-- print r[1]
-- print packedAll

-- print the result, the new place of each rectangle contain in x and y property
--print listreg

-- print "ok load file simplerectanglePackAlogorithm.ms"