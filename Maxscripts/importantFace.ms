global numAutoFace = 5
global importantFacesArray = #()
global lbIFaceData = #("")
global tileCount = 4
global mapChannel = 4
struct yface (index, f_area, unwrap_type)
global s1 = undefined
global u1 = undefined

fn getIFaceByIndex idx = 
(
	for i = 1 to importantFacesArray.count do
	(
		if (idx == importantFacesArray[i].index) then return importantFacesArray[i]
	)
	return false
)

fn y3dFlattenUV selFaces = 
(
	uVFMPolyAngle = 45.0
	uVFMSpacing = 0.02
	uVFMNormalize = true
	uVFMRotate = true
	uVFMFillHoles = false
	uVFMLayout = 1
	uVFMPos = [400,400]

-- 	u1 = s1.modifiers["y3d_unwrap"]
    u1.setMapChannel mapChannel
	u1.setPreventFlattening off
	u1.selectFaces (selFaces as bitarray)
	u1.flattenMap uVFMPolyAngle #([1,0,0], [-1,0,0], [0,1,0], [0,-1,0], [0,0,1], [0,0,-1]) uVFMSpacing uVFMNormalize (uVFMLayout-1) uVFMRotate uVFMFillHoles	
)

fn setIFacesFromSelect obj = 
(
	u = obj.modifiers["y3d_unwrap"]
	sp = u.getSelectedPolygons()
	if sp.count < 1 then (
		MessageBox "No face selected" 
	)else(
		for i in sp do
		(
			tmp = yface()
			tmp.index = i
			u.getArea #{i} &myx &myy &mywidth &myheight &myareaUVW &g
			tmp.f_area = g
			tmp.unwrap_type = 1
			append importantFacesArray tmp
-- 			append lbxSelectedFace .items i
		)
-- 		print lbxSelectedFace
-- 		lbxSelectedFace	items:(for o in importantFacesArray collect o.index)
	)
		
)

fn SelFaceButtonClick = 
(
	if selection.count == 0 then (
		MessageBox "Please select at least one object!" title: "Warning!"
	) else (
		max modify mode
		s1 = selection[1]	
		if (classof s1.baseobject) != Editable_Poly then convertto s1 editable_poly
		subobjectLevel = 0
		s1.ignoreBackfacing  = true
		
		if (s1.modifiers["y3d_unwrap"]==undefined) then (
			uu = unwrap_UVW()
			uu.name = "y3d_unwrap"
			addModifier s1 uu
		)
		uu = s1.modifiers["y3d_unwrap"]
		modPanel.setCurrentObject s1.modifiers[#y3d_unwrap]
		subobjectLevel = 3
		
	)
)

fn runUnwrap =
(
	u1 = s1.modifiers["y3d_unwrap"]
	allfaces = for i = 1 to (u1.numberPolygons()) collect i
	y3dFlattenUV allfaces
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
	label lblArea "Area:" pos:[34,254] width:140 height:20
	label lblFaceNumber "Face numbe" pos:[34,276] width:140 height:20
	radioButtons rdoProjection "Projection" pos:[226,244] width:70 height:78 labels:#("Planar", "Cylindrical", "Spherical", "Box") default:1 columns:1
	button btnSelectAll "Select All" pos:[242,90] width:77 height:28
	button btnDelete "Delete" pos:[243,127] width:77 height:28
	button btnAddList "Add List" pos:[146,20] width:53 height:31
	groupBox grpOptions "Options" pos:[20,342] width:300 height:66
	spinner spnTileCount "Tile Count" pos:[60,370] width:80 height:16 type:#integer scale:1
	dropDownList ddlMapChannel "" pos:[218,368] width:83 height:21 items:#("1", "2", "3", "4", "5", "6", "7", "8", "9")
	label lbl4 "Map channel" pos:[153,369] width:62 height:19
	
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
			lblFaceNumber.text ="Face num:#" + ff.index as String
		)
-- 		n = ff.f_area as String
-- 		MessageBox n
-- 		MessageBox lbxSelectedFace.items[i]
	)
	
	on btnAutoSel pressed do (
		if edtNumFace .text== "" then (
			edtNumFace .text = "5"
			numAutoFace = 5
		) else (
			numAutoFace = edtNumFace .text as Number
		)
		print numAutoFace
	)
	
	on btnAddList pressed do (
		if s1!=undefined then setIFacesFromSelect s1
		lbIFaceData = #()
		for o in importantFacesArray do (
			lname = "Face :#"+(o.index as String)
			append lbIFaceData lname
		)
		lbxSelectedFace.items = lbIFaceData
	)
	
	on ddlMapChannel changed text do (
		mapChannel = text as Integer
	)
)


macroScript SelectImportantFaces category:"Yeu 3D" --some macro script
(
	lbIFaceData = #("")
	createdialog selectFaceRollout
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
	y3dMenu.addItem theAction (y3dMenu.numItems()+1)
	menuMan.updateMenuBar()
)

createY3DMenu()