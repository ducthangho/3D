global y3dInit
if y3dInit != true then
(
	struct bakeInfoItem (old_mat_id,new_mat_id,old_mats)

	global tileArray = #()
	global bakeInfos = #()
	global importantFacesArray = #()

	global numAutoFace = 5
	global tileCount = 4
	global mapChannel = 4

	global resetBakeFlow
	global attachPoly
	global detachSelection
	global oldBakeGroup
	global newBakeGroup
	global stepDone = 0

	fn resetBakeFlow =
		(
			tileArray = #()
			importantFacesArray = #()
			if mapChannel == undefined then mapChannel = 4
			if tileCount == undefined then tileCount = 4
			oldBakeGroup = undefined
			newBakeGroup = #()
			stepDone = 0
		)

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

	fn clearNewBake =
		(
			for o in newBakeGroup do delete o
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

	print "y3d v 1.0 has been loaded"
	y3dInit = true
)
