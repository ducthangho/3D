(
global oDTDetachSelection
global oDTGeometry
global oDTDetachedObjects

global aSOPoly
global showMODialog
global bakeMaterials = #()
global notOneSelect = true


-- include "$userScripts\y3d_max\scripts\y3d_unwrap.ms"
-- include "$userScripts\y3d_max\scripts\bakeTile.ms"

fn PadValue n pad =
	(
	local a = (n as string)
	for w = 1 to 99999 do
		(
		if a.count < pad then a = "0" + a
		else exit
		)
	return a
	)

fn CopyAndPasteLayerFromNodeToNode copyObj pasteObj =
	(
	myLayerName = copyObj.layer.name
	myLayer = layermanager.getLayerFromName myLayerName
	myLayer.addnode pasteObj
	return undefined
	)

fn aSOPoly objs keepOriginals centerPivot pivotPlacement =
	(
	allpoly = true
	for o in objs do
		(
		if (canConvertTo o PolyMeshObject) == false then allpoly = false
		)
	if allpoly == false then
		(
		MessageBox "Not all of the selected objects can be converted to an EditablePoly. Please choose a different selection or run this script in a different mode." title:"objectAttacher"
		)
	else
		(
		disableSceneRedraw()
		try
			(
			oldObjs = #()
			newObjs = #()
			maxops.clonenodes objs cloneType:#copy actualNodeList:&oldObjs newNodes:&newObjs

			-- Convert Objects
			for o in newObjs do (if (classOf o != Editable_Poly) do (convertToPoly o))
			InstanceMgr.MakeObjectsUnique newObjs #individual

			-- Prep Progressbar
			progressStart "objectAttacher"
			escapeEnable = false
			numOfItems = objs.count
			currentIteration = 0

			-- Start process
			while (newObjs.count > 1) do
				(
				for i = newObjs.count to 2 by -2 do
					(
					if getProgressCancel() == true then exit
					currentIteration += 1
					m = ((currentIteration as float)/(numOfItems as float))*100
					attachTo = newObjs[i]
					attachTo.EditablePoly.attach newObjs[i-1] attachTo
					deleteItem newObjs (i-1)
					progressUpdate m
					)
				)
			progressEnd()

			-- Convert
			convertTo newObjs[1] PolyMeshObject
			if centerPivot == true then
				(
				if pivotPlacement == 1 then newObjs[1].pivot = [(newObjs[1].max.x+newObjs[1].min.x)/2, (newObjs[1].max.y+newObjs[1].min.y)/2, (newObjs[1].max.z+newObjs[1].min.z)/2]
				else if pivotPlacement == 2 then newObjs[1].pivot = [(newObjs[1].max.x+newObjs[1].min.x)/2, (newObjs[1].max.y+newObjs[1].min.y)/2, newObjs[1].min.z]
				else if pivotPlacement == 3 then newObjs[1].pivot = [0,0,0]
				)
			if keepOriginals == false then for i in oldObjs do delete i
			select newObjs[1]
			)
		catch (MessageBox "An error has occured when trying to attach one of the objects. You may want to undo." title:"objectAttacher")
		enableSceneRedraw()
		completeRedraw()
		)
	)

fn oDTGeometry obj deleteOriginal keepWireColor CenterPivot PivotPlacement =
	(
	disableSceneRedraw()
	try
		(
		if keepWireColor == true then MyColor = obj.wirecolor
		newObj = copy obj
		doMesh = false
		if classof newObj == Editable_mesh then doMesh = true
		convertTo newObj (Editable_Poly)
		i = 1
		while newObj.getnumfaces() != 0 do
			(
			-- prep
			if getCommandPanelTaskMode() == #modify then max create mode
			newDetachedObjName = obj.name + "_Detached" + (PadValue i 2)

			-- do detach
			newObj.EditablePoly.SetSelection #Face #{1}
			newObj.selectElement()
			facesToDetach = polyop.getFaceSelection newObj
			polyop.detachFaces newObj facesToDetach asNode:true delete:true name:newDetachedObjName

			-- set new poly object properties
			newDetachedObj = getnodebyname newDetachedObjName
			CopyAndPasteLayerFromNodeToNode obj newDetachedObj
			if keepWireColor == true then newDetachedObj.wirecolor = MyColor else newDetachedObj.wirecolor = (color (random 0 255) (random 0 255) (random 0 255))
			if doMesh == true then convertTo newDetachedObj (Editable_Mesh)
			replacePrompt("Objects Detached: " + (i as string))
			i += 1
			append oDTDetachedObjects newDetachedObj
			if centerPivot == true then
				(
				if pivotPlacement == 1 then newDetachedObj.pivot = [(newDetachedObj.max.x+newDetachedObj.min.x)/2, (newDetachedObj.max.y+newDetachedObj.min.y)/2, (newDetachedObj.max.z+newDetachedObj.min.z)/2]
				else if pivotPlacement == 2 then newDetachedObj.pivot = [(newDetachedObj.max.x+newDetachedObj.min.x)/2, (newDetachedObj.max.y+newDetachedObj.min.y)/2, newDetachedObj.min.z]
				else if pivotPlacement == 3 then newDetachedObj.pivot = [0,0,0]
				)
			)
		delete newObj
		if deleteOriginal == true then delete obj
		)
	catch (MessageBox "An error has occured. You may want to undo." title:"objectDetacher")
	enableSceneRedraw()
	completeRedraw()
	)

fn oDTDetachSelection oDTDeleteOriginal oDTKeepWireColor oDTSelectResult oDTCenterPivot oDTPivotPlacement =
	(
	objs = for i in selection collect i
	if objs.count < 1 then MessageBox "Please select an object." title:"objectDetacher"
	else
		(
		error = false
		oDTDetachedObjects = #()
		for i in objs do
			(
			if classof i == Editable_mesh or classof i == Editable_Poly then oDTGeometry i oDTDeleteOriginal oDTKeepWireColor oDTCenterPivot oDTPivotPlacement
			else if superclassof i == Shape then
				()
			else error = true
			)
		if oDTDetachedObjects.count > 0 then
			(
			if oDTSelectResult == true then (select oDTDetachedObjects;replacePrompt(""))
			else deselect selection
			oDTDetachedObjects = #()
			)
		if error == true then MessageBox "At least one of your objects could not be affected by this script. Please convert to an Editable Mesh or Poly." title:"objectDetacher"
		)
	)

fn doAttach =
	(
		objs = for i in selection collect i
		if objs.count < 2 then MessageBox "Please select 2 objects or more." title:"objectAttacher"
		else
			(
				-- chon kieu la poly, mesh, .., tam thoi poly
				aSOPoly objs false true "Center"
				-- aSOPoly objs aSOKeepOriginals aSOCenterPivot aSOPivotPlacement

			)
	)
-- fn doAttach =
-- 	(
-- 		notOneSelect = true
-- 		objs = for i in selection collect i
-- 		if objs.count < 2 then
-- 			(
-- 				if objs.count == 1 then notOneSelect=false
-- 			)
-- 		else
-- 			(
-- 				-- chon kieu la poly, mesh, .., tam thoi poly
-- 				aSOPoly objs false true "Center"
-- 				-- aSOPoly objs aSOKeepOriginals aSOCenterPivot aSOPivotPlacement
-- 			)
-- 	)

fn doBake =
	(
		-- switch to original material IDs
		if oldMatId.count > 0 then setMatIdByArray s1 oldMatId
		prefixFName = ""
		for i = 1 to tileArray.count do
			bakeTile s1 tileArray[i].sizeX tileArray[i].sizeY mapChannel tileArray[i].u tileArray[i].v 1
	)

fn setDefaultSize sx sy =
	(
		defaultSizeX = sx
		defaultSizeY = sy
	)

fn setMatObject obj =
	(
		-- print obj.name
		select obj
		modPanel.addModToSelection (Unwrap_UVW ()) ui:on
		modPanel.setCurrentObject obj.baseObject
		subobjectLevel = 4
		uu = obj.modifiers[#Unwrap_UVW]
		allfaces = for i = 1 to (uu.numberPolygons()) collect i
		for t = 1 to tileArray.count do
			(
				local sFaces = #{}
				for i=1 to allfaces.count do
					if allfaces[i]!=-1 then
					(
						k = polyOp.getFaceMatID obj i
						if k==t then
							(
								append sFaces i
								allfaces[i] = -1
							)
					)
				-- polyOp.setFaceSelection obj sFaces
				if sFaces.count>0 then
					(
						obj.SetSelection #Face sFaces
						obj.material = bakeMaterials[t]
					)
				else
					(
						obj.material.mapEnabled[t] = off
						obj.material.materialList[t] = undefined
					)
			)
	)

fn makeBakeMaterial =
	(

		oname = s1.name
		setMaterialIdFace s1
		modPanel.setCurrentObject s1.baseObject
		oDTDetachSelection true true true true "Center"

		bakeMaterials = #()
		for i = 1 to tileArray.count do
			(
				dmap = BitmapTexture()
				fname = 1000+i
				dmap.filename = bakeFolder +"\\"+ prefixFName + (fname as string) + ".jpg"
				dmap.coords.mapChannel = mapChannel

				bm = Standard()
				bm.name = oname + "_bake_" + (i as string)
				bm.specularlevel = 0
				bm.Glossiness = 0
				bm.useSelfIllumColor = off
				bm.twoSided = on
				bm.selfIllumAmount = 100
				bm.Ambient = color 0 0 0
				bm.diffuse = color 128 128 128
				bm.specular = color 0 0 0
				bm.bumpMapAmount = 100
				bm.diffuseMap = dmap
				append bakeMaterials bm
			)
		-- chinh lai material ID theo ket qua bake
		-- setMaterialIdFace s1
		-- print bakeMaterials
		-- -- ungroup cac doi tuong
		-- modPanel.setCurrentObject s1.baseObject
		-- -- if (notOneSelect) then
		-- 	oDTDetachSelection true true true true "Center"

		objs = for i in selection collect i
		for o in objs do setMatObject o
	)

rollout y3dBakeRollout "Y3D Baker" width:320 height:751
(
	button btnUnwrapMO "Unwrap" pos:[51,189] width:80 height:32
	button btnBake "Bake" pos:[30,527] width:80 height:32
	button btnMakeMat "Make bake material" pos:[30,634] width:136 height:32
	button btnDoAll "Do all + CLose" pos:[198,702] width:106 height:32
	button btnReset "Reset" pos:[113,702] width:80 height:32
	groupBox grp1 "Step 1: Pre Bake" pos:[15,15] width:290 height:101
	groupBox grp2 "Step 3: Bake it" pos:[15,257] width:290 height:317
	groupBox grp3 "Step 4: Ungroup and set bake material" pos:[15,593] width:290 height:92
	editText edtBakeFile "" pos:[29,308] width:201 height:20
	button btnOpen "Open" pos:[231,308] width:55 height:22
	label lbl1 "Folder to save bake image" pos:[32,287] width:137 height:21
	-- comboBox ddlMapType "Bake map type" pos:[32,341] width:144 height:1 items:#("Vray Complete Map", "Diffuse Color")
	-- comboBox ddlDSize "Default Size" pos:[200,341] width:87 height:1 items:#("256", "512", "1024", "2048", "4096")
	listBox lbxListTex "Textures to bake" pos:[32,398] width:143 height:7
	button btnSelectImportant "Group + Select Important" pos:[37,77] width:152 height:32
	radioButtons rdoSelectType "Select Type" pos:[35,36] width:150 height:30 labels:#("Face", "Element") default:1 columns:2
	spinner spnTWidth "Width" pos:[200,416] width:86 height:16 range:[0,10000,0] type:#integer scale:128
	spinner spnTHeight "Height" pos:[200,441] width:86 height:16 range:[0,10000,0] type:#integer scale:128
	button btn128 "128" pos:[188,467] width:30 height:20
	button btn256 "256" pos:[222,467] width:30 height:20
	button btn512 "512" pos:[256,467] width:30 height:20
	button btn1024 "1024" pos:[222,492] width:30 height:20
	button btn2048 "2048" pos:[256,492] width:30 height:20
	button btn768 "768" pos:[188,492] width:30 height:20
	spinner spnTileCount "Tile Count" pos:[60,163] width:80 height:16 range:[0,100,0] type:#integer scale:1
	spinner spnMapChannel "Map Channel" pos:[180,163] width:110 height:16 enabled:true range:[0,100,0] type:#integer scale:1
	groupBox grp25 "Step 2: Unwrap" pos:[15,136] width:290 height:102
	dropDownList ddlMapType "Bake map type" pos:[32,341] width:144 height:40 items:#("Vray Complete Map", "Diffuse Color")
	dropDownList ddlDSize "Default Size" pos:[200,340] width:87 height:40 enabled:true items:#("128", "256", "512", "768", "1024", "2048") selection:3

	fn updateTexSelected sx:0 sy:0 =
		(
			i = lbxListTex.selection
			if i != 0 then
				(
					tmp = lbxListTex.items
					if sx>0 then spnTWidth.value = sx
					if sy>0 then spnTHeight.value = sy
					if sx>0 then tileArray[i].sizeX = sx
					if sy>0 then tileArray[i].sizeY = sy
					tmp[i] = "Tile "+(i as string)+":["+(tileArray[i].sizeX as string)+" x "+(tileArray[i].sizeY as string)+"]"
					lbxListTex.items = tmp
				)
		)
	fn unwrapStep =
		(
			doUnwrap()
			items = #()
			for i=1 to tileArray.count do
				(
					name =	"Tile "+(i as string)+":["+(tileArray[i].sizeX as string)+" x "+(tileArray[i].sizeY as string)+"]"
					append items name
				)
			lbxListTex.items = items
		)

	on y3dBakeRollout open do (
		spnTileCount.value = tileCount
		spnMapChannel.value = mapChannel
		edtBakeFile.text = bakeFolder
		ds = defaultSizeX as string
		for i = 1 to ddlDSize.items.count do
			(
				if (ddlDSize.items[i]==ds) then
					(
						ddlDSize.selection = i
						exit
					)
			)
	)

	on edtBakeFile changed text do
		(
			bakeFolder = edtBakeFile.text
		)

	on btnOpen pressed do
		(
		mypath = getSavePath()
		if mypath != undefined then
			(
			if mypath[mypath.count] != "\\" then mypath = mypath + "\\"
			bakeFolder = mypath
			edtBakeFile.text = mypath
			)
		else
			(
			bakeFolder = ""
			edtBakeFile.text = ""
			)
		)

	on btnUnwrapMO pressed  do unwrapStep()
	on btnBake pressed  do doBake()
	on btnMakeMat pressed do
		(
			-- oDTDetachSelection true true true true "Center"
			makeBakeMaterial()
		)
	on btnDoAll pressed  do
		(
			unwrapStep()
			doBake()
			makeBakeMaterial()
		)
	on btnReset pressed  do
		print "hi"
	on lbxListTex selected sel do
		(
			spnTWidth.value = tileArray[sel].sizeX
			spnTHeight.value = tileArray[sel].sizeY
		)
	on btnSelectImportant pressed  do
		(
			doAttach()
			doSelectImportant()
		)
	on rdoSelectType changed stat do (
			selFaceMode = stat
			print stat
		)
	on spnTWidth changed val do updateTexSelected sx:val
	on spnTHeight changed val do updateTexSelected sy:val
	on btn128 pressed  do updateTexSelected sx:128 sy:128
	on btn256 pressed  do updateTexSelected sx:256 sy:256
	on btn512 pressed  do updateTexSelected sx:512 sy:512
	on btn768 pressed  do updateTexSelected sx:768 sy:768
	on btn1024 pressed  do updateTexSelected sx:1024 sy:1024
	on btn2048 pressed  do updateTexSelected sx:2048 sy:2048

	on spnTileCount changed val do tileCount = val
	on spnMapChannel changed val do mapChannel = val
	on ddlMapType selected sel do
		(
			mapBakeType = ddlMapType.selection
		)
	on ddlDSize selected sel do	setDefaultSize (ddlDSize.items[sel] as Integer) (ddlDSize.items[sel] as Integer)
)


fn showMODialog =
	(
		createdialog y3dBakeRollout
	)
)
