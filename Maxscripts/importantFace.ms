global numAutoFace = 5
global importantFacesArray = #()
global lbIFaceData = #("")
struct yface (index, f_area, unwrap_type)
global s1 = undefined

fn y3dFlattenUV obj selFaces = 
(
	uVFMPolyAngle = 45.0
	uVFMSpacing = 0.02
	uVFMNormalize = true
	uVFMRotate = true
	uVFMFillHoles = false
	uVFMLayout = 1
	uVFMPos = [400,400]

	u = obj.modifiers["y3d_unwrap"]
-- 	u.setMapChanel 3
	u.setPreventFlattening off
	u.selectFaces (selFaces as bitarray)
	u.flattenMap uVFMPolyAngle #([1,0,0], [-1,0,0], [0,1,0], [0,-1,0], [0,0,1], [0,0,-1]) uVFMSpacing uVFMNormalize (uVFMLayout-1) uVFMRotate uVFMFillHoles	
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
			uu.getArea #{i} &myx &myy &mywidth &myheight &myareaUVW &g
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

rollout selectFaceRollout "Select Important Face" width:340 height:390
(
	button btnSelFace "Select Important Face" pos:[20,22] width:130 height:31
	listBox lbxSelectedFace "Selected Faces" pos:[20,69] width:300 height:10 enabled:true items:lbIFaceData
	groupBox grpFaceDetail "Face Detail" pos:[20,233] width:300 height:95
	editText edtNumFace "" pos:[278,22] width:41 height:32
	button btnAutoSel "Auto Select" pos:[160,22] width:119 height:31
	button btnSave "Save" pos:[254,345] width:65 height:28
	button btnCancel "Cancel" pos:[183,344] width:65 height:28
	label lbl1 "Area: " pos:[38,268] width:0 height:0
	label lblArea "Area:" pos:[34,254] width:140 height:20
	label lblFaceNumber "Face number:" pos:[34,276] width:140 height:20
	radioButtons rdoProjection "Projection" pos:[226,244] width:140 height:46 enabled:true labels:#("Planar", "Cylindrical", "Spherical", "Box") columns:1

	on btnSelFace pressed do (
		SelFaceButtonClick()
	)
	
	on btnCancel pressed do (
		DestroyDialog selectFaceRollout
	)
	
-- 	on btnAutoSel pressed do (
-- 		if edtNumFace .text== "" then (
-- 			edtNumFace .text = "5"
-- 			numAutoFace = 5
-- 		) else (
-- 			numAutoFace = edtNumFace .text as Number
-- 		)
-- 		print numAutoFace
-- 	)
	on btnAutoSel pressed do (
		if s1!=undefined then setIFacesFromSelect s1
		lbIFaceData = #()
		for o in importantFacesArray do (
			lname = "Face :#"+(o.index as String)
			append lbIFaceData lname
		)
		lbxSelectedFace.items = lbIFaceData
	)
)


macroScript SelectImportantFaces category:"Yeu 3D" --some macro script
(
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


createY3DMenu()