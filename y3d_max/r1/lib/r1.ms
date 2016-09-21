
-- global variables and functions
if r1_init != true then
(
	

	struct bakeInfoItem (old_mat_id,new_mat_id,old_mats)
	global bakeInfos = #()
	global attachPoly
	global detachSelection
	global oldBakeGroup
	global newBakeGroup
	global originalObjects

	-- global form
	global yf_main
	global yf_main_setting
	global yf_input_name


	global TaskList
	global currentTask
	global currentJob

	global PSetting
	global TileBake
	global PackType1
	global PackType2

	global projectName

	struct pSettingClass
		(
			mapChannel=4,
			numAutoFace=5,
			tileCount=4,
			selFaceMode=1,
			packType=1,
			bakeMapType=1,
			defaultTileSize=128,

			-- fn resetSetting useIni=
			-- 	(
			-- 		if (useIni==false) then
			-- 			(
			-- 				mapChannel=4
			-- 				numAutoFace=5
			-- 				tileCount=4
			-- 				selFaceMode=1
			-- 				packType=1
			-- 			)
			-- 		else
			-- 			(

			-- 			)

			-- 	),

			fn saveAsDefault =
				(
					keys = #(
						"numAutoFace",
						"tileCount",
						"selFaceMode",
						"mapChannel",
						"packType",
						"bakeMapType",
						"defaultTileSize"
					)
					saveIniArray "\\r1.ini" keys
				),

			fn loadSetting =
				(
					keys = #(
						"numAutoFace",
						"tileCount",
						"selFaceMode",
						"mapChannel",
						"packType",
						"bakeMapType",
						"defaultTileSize"
					)
					loadIniArray "\\r1.ini" keys		
					-- mapChannel = mapChannel as integer
					-- selFaceMode = selFaceMode as integer
				)

		)
	PSetting = pSettingClass() -- project setting

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

	fn attachPoly objs keepOriginals centerPivot pivotPlacement =
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

	fn attachMesh objs keepOriginals centerPivot pivotPlacement =
		(
		allmesh = true
		for o in objs do
			(
			if (canConvertTo o TriMeshGeometry) == false then allmesh = false
			)
		if allmesh == false then
			(
			MessageBox "Not all of the selected objects can be converted to an EditableMesh. Please choose a different selection or run this script in a different mode." title:"objectAttacher"
			)
		else
			(
			disableSceneRedraw()
			try
				(
				-- copy objects
				oldObjs = #()
				newObjs = #()
				maxops.clonenodes objs cloneType:#copy actualNodeList:&oldObjs newNodes:&newObjs

				-- Convert Objects
				for o in newObjs do (if (classOf o != Editable_mesh) do (convertToMesh o))
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
						attach attachTo newObjs[i-1]
						deleteItem newObjs (i-1)
						progressUpdate m
						)
					)
				progressEnd()

				-- Convert
				convertTo newObjs[1] TriMeshGeometry
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

	fn detachGeometry obj deleteOriginal keepWireColor CenterPivot PivotPlacement =
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
				append newBakeGroup newDetachedObj
				if centerPivot == true then
					(
					if pivotPlacement == 1 then newDetachedObj.pivot = [(newDetachedObj.max.x+newDetachedObj.min.x)/2, (newDetachedObj.max.y+newDetachedObj.min.y)/2, (newDetachedObj.max.z+newDetachedObj.min.z)/2]
					else if pivotPlacement == 2 then newDetachedObj.pivot = [(newDetachedObj.max.x+newDetachedObj.min.x)/2, (newDetachedObj.max.y+newDetachedObj.min.y)/2, newDetachedObj.min.z]
					else if pivotPlacement == 3 then newDetachedObj.pivot = [0,0,0]
					)
				)
			delete newObj
			if deleteOriginal == true then delete obj
			else
				(
					oldBakeGroup = obj
					hide oldBakeGroup
				)
			)
		catch (MessageBox "An error has occured. You may want to undo." title:"objectDetacher")
		enableSceneRedraw()
		completeRedraw()
		)

	fn detachSelection oDTDeleteOriginal oDTKeepWireColor oDTSelectResult oDTCenterPivot oDTPivotPlacement =
		(
		objs = for i in selection collect i
		print objs
		if objs.count < 1 then MessageBox "Please select an object." title:"objectDetacher"
		else
			(
			error = false
			newBakeGroup = #()
			for i in objs do
				(
				if classof i == Editable_mesh or classof i == Editable_Poly then detachGeometry i oDTDeleteOriginal oDTKeepWireColor oDTCenterPivot oDTPivotPlacement
				else if superclassof i == Shape then
					()
				else error = true
				)
			if newBakeGroup.count > 0 then
				(
				if oDTSelectResult == true then (select newBakeGroup;replacePrompt(""))
				else deselect selection
				-- newBakeGroup = #()
				)
			if error == true then MessageBox "At least one of your objects could not be affected by this script. Please convert to an Editable Mesh or Poly." title:"objectDetacher"
			)
		)

	fn beginSelect selectType =
		(

		)

	if ySetup==true then
		(
			PSetting.saveAsDefault()
		)
	else
		(
			PSetting.loadSetting()
		)	

	print "r1 has been loaded"
	r1_init = true
)
