(
global s1 = undefined
global u1 = undefined

global isolate_select = true

global defaultSizeX = 128
global defaultSizeY = 128
global defaultMapType = 1

struct yface (index, f_area, area_uvw, fw, fh, udim, x, y)
struct tileStruct (u,v,sizeX,sizeY)

--function
global setIFacesFromSelect
global doSelectImportant
global doUnwrap

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
						findBestFit #() nowArea importantFacesArray
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
				if (nowArea <= 0) then exit
			)
		)
		-- cho cac mat chua duoc gan udim vao 1 tile con trong nhieu nhat
		for j = 1 to importantFacesArray.count do
		(
			if importantFacesArray[j].udim == 1000 then importantFacesArray[j].udim = 1001+maxBestTile
		)
		-- print "vua chia xong ----------------------"
		-- print importantFacesArray
	)
-- Di chuyen cac nhom mat quan trong sang tile rieng
fn moveByTile uu =
	(
		print importantFacesArray
		tileArray = #()
		tileArray[1] = tileStruct u:0 v:0 sizeX:defaultSizeX sizeY:defaultSizeY
		udim = 1001
		for i = 2 to tileCount do (
			udim = udim+1
			theFacesToSelect = #{}
			for j = 1 to importantFacesArray.count do (
				if importantFacesArray[j].udim == udim then(
					append theFacesToSelect importantFacesArray[j].index
				)
			)
			-- chon face
	-- 		s1.selectedFaces #

			if theFacesToSelect.numberSet  >0 then
			(
				
				uu.selectFaces theFacesToSelect
				uu.flattenMapNoParams()
				-- uu.packNoParams() 1
				move_v = (i-1)/10
				-- move_u = i - move_v*10
				-- move_u = mod (i-1) 10
				move_u = (i-1) - move_v*10
				uu.unwrap2.MoveSelected [move_u,move_v,0]
				tileArray[i] = tileStruct u:move_u v:move_v sizeX:defaultSizeX sizeY:defaultSizeY
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
			-- uu.flattenMapNoParams()
			-- uu.packNoParams()
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

fn updateGlobalVariable =
(
	global s1 = $
	--addModifier s1 (unwrap_UVW())
	max modify mode
	modPanel.addModToSelection (Unwrap_UVW ()) ui:on
	subobjectLevel = 3
	s1.modifiers[1].setMapChannel 2
	global u1 = s1.modifiers[1]
)


fn setIFacesFromSelect obj =
	(
		u = obj.modifiers["y3d_unwrap"]
		sp = u.getSelectedPolygons()
		if sp.count < 1 then (
			importantFacesArray = #()
			stepDone = 0
			-- MessageBox "No face selected" +(stepDone as string)
			-- print stepDone
		)else(
			importantFacesArray = #()
			for i in sp do
			(
				tmp = yface()
				tmp.index = i
				-- u.getArea #{i} &myx &myy &my_width &my_height &myareaUVW &g
				-- tmp.f_area = g
				-- tmp.area_uvw = myareaUVW
				-- tmp.udim = 1000
				-- tmp.fw = my_width
				-- tmp.fh = my_height
				append importantFacesArray tmp
			)
-- 			getUVFromBlender obj importantFacesArray  -- goi Ham cua Anh Dang
			-- tinh lai uv moi
--			u = obj.modifiers["y3d_unwrap"]  --comment Ham Anh Dang
			getUVFromBlenderFBX obj importantFacesArray -- goi Ham cua Minh
			
			-------------begin: Minh Them -----------------
			updateGlobalVariable()
			obj = $
			u = u1
			-------------end: Minh Them -----------------
			for i=1 to importantFacesArray.count do
			(
				u.getArea #{importantFacesArray[i].index} &myx &myy &my_width &my_height &myareaUVW &g
				importantFacesArray[i].f_area = g
				importantFacesArray[i].area_uvw = myareaUVW
				importantFacesArray[i].udim = 1000
				importantFacesArray[i].fw = my_width
				importantFacesArray[i].fh = my_height
				importantFacesArray[i].x = myx
				importantFacesArray[i].y = myy

				format "posx: %\n" myx
			)
		)
	)

fn SelFaceButtonClick =
	(
		if selection.count == 0 then (
			stepDone = 0
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
			stepDone = 1
		)
	)

fn doAddList =
	(
		if s1==undefined then return false
		if isolate_select == true then actionMan.executeAction 0 "261"  -- Tools: End Isolate
		setIFacesFromSelect s1

		if (stepDone == 1) then
			(
				sortByArea()
				splitGroupByTileCount()
			)
		else
			(
				-- MessageBox "No face selected"
			)
	)

fn doSelectImportant selectMode =
	(
		if selectMode == 1 then
			(
				SelFaceButtonClick()
			)
		else
			(
				MessageBox "Zzz.."
			)
	)

fn doUnwrap1 selectMode =
	(
		if selectMode == 1 then
			(
				doAddList()
				if stepDone == 1 then
					(
						if queryBox "Do you want to continue?" beep:false then
							(
								u1 = s1.modifiers["y3d_unwrap"]
								if u1!=undefined then moveByTile u1
							)
					)
			)
		else
			(
				MessageBox "Che do chon mat chua support.."
				stepDone = 0
			)
	)

-----------------#ham cu anh Dang --------------------------------
fn doUnwrap3 selectMode =
	(
		if selectMode == 1 then
			(
				-- u1.unwrap2.flattenMapNoParams()
				if isolate_select == true then actionMan.executeAction 0 "261" 
				setIFacesFromSelect s1
				if queryBox "Do you want to continue?" beep:false then
					(
						u1 = s1.modifiers["y3d_unwrap"]
						unwrapSelection u1 	
					)
			)
		else
			(
				MessageBox "Che do chon mat chua support.."
				stepDone = 0
			)
	)
---------------------#begin: ham cua Minh --------------------------------------

fn copyMaterial oldObj newObj = 
(
	newObj.material = oldObj.material
)

fn doUnwrap2 selectMode =
	(		
		if selectMode == 1 then
			(
				--u1.unwrap2.flattenMapNoParams()
				local oldObj = s1
				setIFacesFromSelect s1

				local newObj = s1
				format "OldObject: %, new OBJ: %\n" oldObj newObj
				unwrapSelection u1
				
				copyMaterial oldObj newObj
				print "--------------------------------------------------------------------"
				format "newObjMaterial: %,\n oldObjMaterial: %\n" newObj.material oldObj.material
			)
		else
			(
				MessageBox "Che do chon mat chua support.."
				stepDone = 0
			)
	)


---------------------------------end : ham Minh them --------------------------------------
fn doUnwrap selectMode packType =
	(
		if packType == 1 then doUnwrap1 selectMode
		else if packType == 2 then doUnwrap2 selectMode
	)
)
