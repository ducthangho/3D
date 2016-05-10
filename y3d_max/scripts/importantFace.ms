global numAutoFace = 5
global importantFacesArray = #()
global lbIFaceData = #("")
global tileCount = 4
global mapChannel = 4
struct yface (index, f_area, area_uvw, fw, fh, udim)
global s1 = undefined
global u1 = undefined
global bestSet = #()
global best = -1
global isolate_select = true

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

fn findBestFit3 bSet narea =
(

)


fn findBestFit bSet narea =
(	
	hasFit = false
	for k = 1 to importantFacesArray.count do 
	(
		kk = findItem bSet k
		if kk>0 then continue
		if (importantFacesArray[k].udim==1000)and(importantFacesArray[k].f_area<=narea) then (
			append bSet k
			kk = bSet.count
			hasFit = true
			importantFacesArray[k].udim += 1000 
			findBestFit bSet (narea-importantFacesArray[k].f_area)
			deleteItem bSet kk 
			importantFacesArray[k].udim -= 1000 
		) 
	)
	if hasFit==false then (
		if narea<best then (
			best = narea
			bestSet = bSet
		)
	)
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
	-- print "I Count:"
	-- print importantFacesArray.count
	if  importantFacesArray.count < 1 then 
	(
		MessageBox "No face selected!!"
		return false
	)
	for i = 1 to importantFacesArray.count do 
	(
		print importantFacesArray[i].area_uvw
		totalArea += importantFacesArray[i].area_uvw
	)
	tCount = tileCount-1
	avgAreaTile = totalArea/tCount
	maxBestTile = 0
	maxBest = -1
	-- print "AVG Area:"
	-- print avgAreaTile
	-- MessageBox (tileCount as String)
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
					print bestSet
					print totalArea
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
	print importantFacesArray
)
-- Di chuyen cac nhom mat quan trong sang tile rieng
fn moveByTile =
(
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
			u1.selectFaces theFacesToSelect 
			u1.flattenMapNoParams()
			u1.packNoParams()
			move_v = (i+1)/10
			move_u = i - move_v*10
			u1.unwrap2.MoveSelected [move_u,move_v,0]
		)
	)
	
-- 	print "Other:"
	otherFaces = #{}
	for i = 1 to (u1.numberPolygons()) do append otherFaces i
	for j = 1 to importantFacesArray.count do deleteItem otherFaces importantFacesArray[j].index
-- 	print otherFaces
	if otherFaces.count > 0 then 
	(	
		u1.selectFaces otherFaces 
		u1.flattenMapNoParams()
		u1.packNoParams()
	)
)

fn setMaterialIdFace = 
(
	if s1==undefined then (
		MessageBox "Something wrong!!!"
		return false
	)
	udim = 1001
	for i = 1 to tileCount-1 do (
		udim = udim+1
		for j = 1 to importantFacesArray.count do (
			if importantFacesArray[j].udim == udim then(
				polyOp.setFaceMatID s1 importantFacesArray[j].index (i+1)
				print j
			)
		)
	)
	
	print "Other:"
	otherFaces = #()
	for i = 1 to (u1.numberPolygons()) do append otherFaces i
	for j = 1 to importantFacesArray.count do 
	(
		k = findItem otherFaces importantFacesArray[j].index
		if (k>0) then deleteItem otherFaces k
	)	
	
	print otherFaces
	if otherFaces.count > 0 then 
	(	
		for i = 1 to otherFaces.count do
		polyOp.setFaceMatID s1 otherFaces[i] 1
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

-- fn y3dFlattenUV selFaces = 
-- (
-- 	uVFMPolyAngle = 45.0
-- 	uVFMSpacing = 0.02
-- 	uVFMNormalize = true
-- 	uVFMRotate = true
-- 	uVFMFillHoles = false
-- 	uVFMLayout = 1
-- 	uVFMPos = [400,400]
--     u1.setMapChannel mapChannel
-- 	u1.setPreventFlattening off
-- 	u1.selectFaces (selFaces as bitarray)
-- 	u1.flattenMap uVFMPolyAngle #([1,0,0], [-1,0,0], [0,1,0], [0,-1,0], [0,0,1], [0,0,-1]) uVFMSpacing uVFMNormalize (uVFMLayout-1) uVFMRotate uVFMFillHoles	
-- )

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
	-- u1 = s1.modifiers["y3d_unwrap"]
	-- allfaces = for i = 1 to (u1.numberPolygons()) collect i
 --    u1.setMapChannel mapChannel
	-- u1.setPreventFlattening off
	-- u1.selectFaces (allfaces  as bitarray)
	-- u1.flattenMapNoParams()
	-- u1.packNoParams()
	moveByTile()
-- 	y3dFlattenUV allfaces
	
)

fn doBake mapSize= 
(

)


rollout selectFaceRollout "Select Important Face" width:340 height:487
(
	button btnSelFace "Select Important Face" pos:[20,20] width:119 height:31
	listBox lbxSelectedFace "Selected Faces" pos:[20,68] width:212 height:10 enabled:true
	groupBox grpFDetail "Face Detail" pos:[20,233] width:300 height:95
	editText edtNumFace "" pos:[278,21] width:41 height:31
	button btnAutoSel "Auto Select" pos:[206,20] width:73 height:31
	button btnRun "Run" pos:[256,433] width:65 height:28
	button btnCancel "Cancel" pos:[185,433] width:65 height:28
	label lblArea "Area:" pos:[38,254] width:140 height:20
	label lblFaceNumber "Face number:" pos:[38,306] width:140 height:20
	button btnSelectAll "Select All" pos:[242,90] width:77 height:28
	button btnDelete "Delete" pos:[243,127] width:77 height:28
	button btnAddList "Add List" pos:[146,20] width:53 height:31
	groupBox grpOptions "Options" pos:[20,342] width:300 height:66
	spinner spnTileCount "Tile Count" pos:[60,369] width:80 height:16 range:[0,100,0] type:#integer scale:1
	dropDownList ddlMapChannel "" pos:[218,368] width:83 height:21 items:#("1", "2", "3", "4", "5", "6", "7", "8", "9")
	label lbl4 "Map channel" pos:[153,369] width:62 height:19
	label lblAreaUVW "AreaUVW:" pos:[38,278] width:140 height:20
	label lblWidth "Width:" pos:[180,254] width:120 height:20
	label lblHeight "Height:" pos:[180,278] width:120 height:20
	
	on selectFaceRollout open do (
		spnTileCount.value = tileCount
		ddlMapChannel.selection = mapChannel
	)

	on spnTileCount changed val do (
		tileCount = val
	)
		
	on btnSelFace pressed do (
		SelFaceButtonClick()
	)
	
	on btnCancel pressed do (
		DestroyDialog selectFaceRollout
	)
	
	on btnRun pressed do (
		runUnwrap()
		DestroyDialog selectFaceRollout
	)
	
	on lbxSelectedFace selected i do (
		if (lbxSelectedFace.items[i]=="") then return false
		idx = replace lbxSelectedFace.items[i] 1 7 ""
		idx = idx as integer
		ff = getIFaceByIndex idx
		if (ff!=false) then (
			lblArea.text = "Area:" + ff.f_area as String
			lblAreaUVW.text ="AreaUVW:#" + ff.area_uvw as String
			lblWidth.text ="Width:#" + ff.fw as String
			lblHeight.text ="Height:#" + ff.fh as String
			lblFaceNumber.text ="Face num:#" + ff.index as String
		)
-- 		n = ff.f_area as String
-- 		MessageBox n
-- 		MessageBox lbxSelectedFace.items[i]
	)
	
	on btnAutoSel pressed do (
		spnTileCount.value = tileCount
		if edtNumFace .text== "" then (
			edtNumFace .text = "5"
			numAutoFace = 5
		) else (
			numAutoFace = edtNumFace .text as Number
		)
		print numAutoFace
	)
	
	on btnAddList pressed do (
		if isolate_select == true then actionMan.executeAction 0 "261"  -- Tools: End Isolate
		if s1!=undefined then setIFacesFromSelect s1
		lbIFaceData = #()
		sortByArea()
		splitGroupByTileCount()
		for o in importantFacesArray do (
			lname = "Face :#"+(o.index as String)
			append lbIFaceData lname
		)
		lbxSelectedFace.items = lbIFaceData
		-- splitGroupByTileCount()
		
	)
	
	on ddlMapChannel changed text do (
		mapChannel = text as Integer
	)
)


macroScript SelectImportantFaces 
category:"Select Important Faces"
internalCategory:"Yeu 3D"
(
	lbIFaceData = #("")
	importantFacesArray = #()
	s1 = undefined
	u1 = undefined
	-- spnTileCount.value = tileCount

	createdialog selectFaceRollout
)

macroScript SetMaterialID 
category:"Set Material ID" --some macro script
internalCategory:"Yeu 3D"
(
	MessageBox "You have to bake object before run it!!"
	setMaterialIdFace()
)

fn createY3DMenu =
(
	theMainMenu = menuMan.getMainMenuBar()
	y3dMenu = menuMan.findMenu "Yeu 3D"
	if (y3dMenu != undefined) do (
		menuMan.unRegisterMenu y3dMenu
	)
	y3dMenu = menuMan.createMenu "Yeu 3D"
	y3dSubMenu = menuMan.createSubMenuItem "Yeu 3D" y3dMenu 
	theMainMenu.addItem y3dSubMenu (theMainMenu.numItems()+1) 
	theAction = menuMan.createActionItem "SelectImportantFaces" "Yeu 3D"
	theAction2 = menuMan.createActionItem "SetMaterialID" "Yeu 3D"
	y3dMenu.addItem theAction (y3dMenu.numItems()+1)
	y3dMenu.addItem theAction2 (y3dMenu.numItems()+1)
	menuMan.updateMenuBar()
)

createY3DMenu()