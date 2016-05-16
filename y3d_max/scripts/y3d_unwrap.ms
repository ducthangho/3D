(

-- fileIn "SimpleRectanglePackAlogorithm.ms"

global numAutoFace = 5

global tileCount = 4
global mapChannel = 4

global s1 = undefined
global u1 = undefined
global bestSet = #()
global best = -1
global isolate_select = true

global selFaceMode = 1
global defaultSizeX = 512
global defaultSizeY = 512
global defaultMapType = 1

global tileArray = #()
global bakeInfos = #()
global oldMatId = #()
global newMatId = #()
global importantFacesArray = #()

struct yface (index, f_area, area_uvw, fw, fh, udim, x, y)
struct tileStruct (u,v,sizeX,sizeY)
struct bakeInfoItem (old_mat_id,new_mat_id,old_mats)

--function
global setIFacesFromSelect
global doSelectImportant
global doUnwrap
global selectFaceRollout
global setMaterialIdFace
global setMatIdByArray

-- Tim tap cac mat co tong dien tich lon nhat ma be hon tham so narea

fn findBestFit2 bSet narea =
(
	for k = 1 to importantFacesArray.count do
	(
		if (importantFacesArray[k].udim==1000)and(importantFacesArray[k].area_uvw<=narea) then (
			append bSet k
			narea = narea-importantFacesArray[k].area_uvw
		)
	)
	bestSet = bSet
	best = narea
)

-- Sap xep cac mat uu tien theo do lon dien tich tu to den nho
fn sortByArea = -- co the dung qsort
(
	listed = sort(for i in importantFacesArray collect i.area_uvw)
	tmp = #()
	for i =  listed.count to 1 by -1 do (
		for j = 1 to importantFacesArray.count do
			if importantFacesArray[j].area_uvw == listed[i] then (
				append tmp importantFacesArray[j]
				deleteItem importantFacesArray j
				exit
			)
	)
	importantFacesArray = tmp
)
-- Chia deu cac mat quan trong theo so nhom sinh ra
fn splitGroupByTileCount =
(
	totalArea = 0
	if  importantFacesArray.count < 1 then
	(
		MessageBox "No face selected!!"
		return false
	)
	for i = 1 to importantFacesArray.count do
	(
		totalArea += importantFacesArray[i].area_uvw
	)
	tCount = tileCount-1
	avgAreaTile = totalArea/tCount
	maxBestTile = 0
	maxBest = -1

	iTile = 0
	for i = 1 to tileCount-1 do (
		iTile = iTile+1
		nowArea = avgAreaTile
		for j = 1 to importantFacesArray.count do (
			if importantFacesArray[j].udim == 1000 then(
				if (importantFacesArray[j].area_uvw <=nowArea) then
				(
					nowArea = nowArea - importantFacesArray[j].area_uvw
					totalArea = totalArea - importantFacesArray[j].area_uvw
					importantFacesArray[j].udim = 1001+iTile
					best = nowArea
					bestSet = #()
					findBestFit2 #() nowArea
					if bestSet.count > 0 then
					(
						for b = 1 to bestSet.count do
						(
							importantFacesArray[bestSet[b]].udim = 1001+iTile
							totalArea = totalArea - importantFacesArray[bestSet[b]].area_uvw
						)
					)
					if best > maxBest then
					(
						maxBest = best
						maxBestTile = iTile
					)
					tCount = tCount - 1
					avgAreaTile = totalArea/tCount
					exit
				) else if nowArea == avgAreaTile then (
						importantFacesArray[j].udim = 1001+iTile
						tCount = tCount - 1
						totalArea = totalArea - importantFacesArray[j].area_uvw
						avgAreaTile = totalArea/tCount
						exit
				)
			)
			-- if (nowArea <= 0) and (iTile<tileCount-1) then exit
			if (nowArea <= 0) then exit
		)
	)
	-- cho cac mat chua duoc gan udim vao 1 tile con trong nhieu nhat
	for j = 1 to importantFacesArray.count do
	(
		if importantFacesArray[j].udim == 1000 then importantFacesArray[j].udim = 1001+maxBestTile
	)
	-- print importantFacesArray
)
-- Di chuyen cac nhom mat quan trong sang tile rieng
fn moveByTile uu =
(
	tileArray = #()
	tileArray[1] = tileStruct u:0 v:0 sizeX:defaultSizeX sizeY:defaultSizeY
	udim = 1001
	for i = 1 to tileCount-1 do (
		udim = udim+1
		theFacesToSelect = #{}
		for j = 1 to importantFacesArray.count do (
			if importantFacesArray[j].udim == udim then(
				append theFacesToSelect importantFacesArray[j].index
			)
		)
		-- chon face
-- 		s1.selectedFaces #{}
		if theFacesToSelect.count >0 then
		(
			uu.selectFaces theFacesToSelect
			uu.flattenMapNoParams()
			-- uu.packNoParams()
			move_v = (i+1)/10
			move_u = i - move_v*10
			uu.unwrap2.MoveSelected [move_u,move_v,0]
			tileArray[i+1] = tileStruct u:move_u v:move_v sizeX:defaultSizeX sizeY:defaultSizeY
		)
	)

-- 	print "Other:"
	otherFaces = #{}
	for i = 1 to (uu.numberPolygons()) do append otherFaces i
	for j = 1 to importantFacesArray.count do deleteItem otherFaces importantFacesArray[j].index
-- 	print otherFaces
	if otherFaces.count > 0 then
	(
		uu.selectFaces otherFaces
		uu.flattenMapNoParams()
		-- uu.packNoParams()
	)
)

fn setMaterialIdFace obj =
(
	if obj==undefined then (
		MessageBox "Something wrong!!!"
		return false
	)
	local otherFaces = #()
	oldMatId = #()
	newMatId = #()
	for i = 1 to (obj.modifiers["y3d_unwrap"].numberPolygons()) do
		(
			oldMatId[i] = 0
			newMatId[i] = 0
			append otherFaces i
		)

	for j = 1 to importantFacesArray.count do
		(
			oldMatId[importantFacesArray[j].index] = -1
		)

	if otherFaces.count > 0 then
	(
		for i = 1 to otherFaces.count do
			(
				-- k = findItem otherFaces importantFacesArray[j].index
				-- if (k>0) then deleteItem otherFaces k
				if (oldMatId[i]!=-1) then
					(
						oldMatId[i] = polyOp.getFaceMatID obj otherFaces[i]
						newMatId[i] = 1
						-- polyOp.setFaceMatID s1 otherFaces[i] 1
					)
			)

	)

	udim = 1001
	for i = 1 to tileCount-1 do (
		udim = udim+1
		for j = 1 to importantFacesArray.count do (

			-- k = findItem otherFaces importantFacesArray[j].index
			-- if (k>0) then (
			-- 	oldMatId[i] = -1
			-- )
			if importantFacesArray[j].udim == udim then
				(
					oldMatId[importantFacesArray[j].index] = polyOp.getFaceMatID obj importantFacesArray[j].index
					newMatId[importantFacesArray[j].index] = i+1
					-- polyOp.setFaceMatID obj importantFacesArray[j].index (i+1)
				)
		)
	)
	setMatIdByArray obj newMatId
)

fn setMatIdByArray obj arr=
	(
		if arr.count < 1 then
			(
				MessageBox "Du lieu sai!"
				return false
			)
		for i = 1 to arr.count do
			(
				polyOp.setFaceMatID obj i arr[i]
			)
	)

fn getIFaceByIndex idx =
	(
		for i = 1 to importantFacesArray.count do
		(
			if (idx == importantFacesArray[i].index) then return importantFacesArray[i]
		)
		return false
	)

fn setIFacesFromSelect obj =
	(
		u = obj.modifiers["y3d_unwrap"]
		sp = u.getSelectedPolygons()
		if sp.count < 1 then (
			MessageBox "No face selected"
		)else(
			importantFacesArray = #()
			for i in sp do
			(
				tmp = yface()
				tmp.index = i
				u.getArea #{i} &myx &myy &my_width &my_height &myareaUVW &g
				tmp.f_area = g
				tmp.area_uvw = myareaUVW
				tmp.udim = 1000
				tmp.fw = my_width
				tmp.fh = my_height
				append importantFacesArray tmp
			)
		)
	)

fn SelFaceButtonClick =
	(
		if selection.count == 0 then (
			MessageBox "Please select at least one object!" title: "Warning!"
		) else (
			max modify mode
			if isolate_select == true then
			(
				actionMan.executeAction 0 "197"  -- Tools: Isolate Selection
				max tool zoomextents all
			)
			s1 = selection[1]
			if (classof s1.baseobject) != Editable_Poly then convertto s1 editable_poly
			subobjectLevel = 0
			s1.ignoreBackfacing  = true

			if (s1.modifiers["y3d_unwrap"]==undefined) then (
				u1 = unwrap_UVW()
				u1.name = "y3d_unwrap"
				addModifier s1 u1
			)
			u1 = s1.modifiers["y3d_unwrap"]
			modPanel.setCurrentObject s1.modifiers[#y3d_unwrap]
			subobjectLevel = 3
			u1.setMapChannel mapChannel
			u1.setPreventFlattening off
			u1.selectFaces #{}
			u1.flattenMapNoParams()
			u1.packNoParams()
		)
	)

fn runUnwrap =
	(
		if u1!=undefined then moveByTile u1
	)

fn doAddList =
	(
		if s1==undefined then return false
		if isolate_select == true then actionMan.executeAction 0 "261"  -- Tools: End Isolate
		setIFacesFromSelect s1
		sortByArea()
		splitGroupByTileCount()
	)

fn doSelectImportant =
	(
		if selFaceMode == 1 then
			(
				SelFaceButtonClick()
			)
		else
			(
				MessageBox "Zzz.."
			)
	)

---------------------- ham cu: Dang --------------
fn doUnwrap2 =
	(
		if selFaceMode == 1 then
			(
				doAddList()
				if u1!=undefined then moveByTile u1
			)
		else
			(
				MessageBox "Zz.."
			)
	)

)
---- begin: function minh them -----
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

fn rePosition &faces u = 
(
	recalculate &faces u
	packFace &faces u
)

fn test = 
(
	n = 1000
	print importantFacesArray
	recalculate &importantFacesArray u1
	listRect = getListRects importantFacesArray 
	print listRect
	listRect = MultiplyAsInt listRect n false
	
	
	packedAll = true
	enclosingRect = MRectangle width:n height: n
	
	r = packTile listRect enclosingRect 1000 &packedAll
	print r[1]
	
	r2 = #()
	for i = 1 to r.count do
	(
		ri = MultiplyAsInt r[i] n true
		append r2 ri
	)
	r = r2
	faces = importantFacesArray
	u = u1
	
	for i =1 to r.count do
	(
		local l = r[i]
		for j =1 to l.count do
			for k = 1 to faces.count do
			(
				if( l[j].id == faces[k].index) do
				(
					local x = l[j].x / n
					local y = l[j].y / n
					
					format " x y = % % \n" x y
					
					local uidm_u = mod i 10
					local udim_v = i / 10
					local udim = 1001 + uidm_u + udim_v * 10
					
					faces[k].udim = udim
					u.unwrap2.selectFaces (#(faces[k].index) as bitarray)
					u.unwrap2.moveSelected [-faces[k].x , -faces[k].y, 0 ]
					u.unwrap2.moveSelected [uidm_u + x ,udim_v + y , 0 ]
					faces[k].x = x
					faces[k].y = y
				)
			)
	)
	
	reposition &importantFacesArray u1
	
	print u1
	print listRect
	print r.count
	print r[1]
)
----------------------------------------------------------------------------------------------------
-------------main function: is the first call function, it call other function -----------------
fn doUnwrap = 
	(
		if selFaceMode == 1 then 
			(
				doAddList()
				if u1!=undefined do
				(
					moveByTile u1
					rePosition importantFacesArray u1
				)
			)
		else 
			(
				MessageBox "Zz.."
			)
	)
-----end: function minh them----------------------
