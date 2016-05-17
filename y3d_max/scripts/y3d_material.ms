(
global bakeMaterials = #()
global oldMaterials = #()
global oldMatId = #()
global newMatId = #()

global makeBakeMaterial
global setMatIdByArray
global setMaterialIdFace
global restoreOldMat

-- fn emptyCurrentMatLib =
-- 	(
-- 		while currentMaterialLibrary.count > 0 do
-- 		(
-- 			for mat in currentMaterialLibrary do deleteItem currentMaterialLibrary mat.name
-- 		)--end while
-- 	)

-- fn saveOldMat obj=
-- 	(
-- 		oldMaterials = #()
-- 		mclass = classof obj.material
-- 		if (mclass != Multimaterial) then
-- 			(
-- 				append oldMaterials obj.material
-- 			)
-- 		else
-- 			(
-- 				oldMaterials = obj.material.materialList
-- 			)
-- 	)

fn restoreOldMat =
	(
		if oldBakeGroup==undefined then
			(
				MessageBox "Dữ liệu cũ đã mất không thể khôi phục!"
				return false
			)
		-- xoa cac doi tuong vua duoc detach va gan bake material
		print newBakeGroup
		for o in newBakeGroup do delete o

		-- hien lai object luc chua detach
		unhide oldBakeGroup
		select oldBakeGroup
		s1 = oldBakeGroup
		u1 = oldBakeGroup.modifiers["y3d_unwrap"]
		if oldMatId.count > 0 then setMatIdByArray s1 oldMatId
		print "Undo OK"
	)

fn setMatObject obj notOne mChanel =
	(
		if not notOne then convertToPoly obj
		select obj
		modPanel.addModToSelection (Unwrap_UVW ()) ui:on
		uu = obj.modifiers[obj.modifiers.count]
		uu.unwrap.setMapChannel mChanel
		modPanel.setCurrentObject obj.baseObject
		subobjectLevel = 4
		allfaces = #()
		mid = #()
		for i = 1 to (uu.numberPolygons()) do
			(
				append allfaces i
				append mid (polyOp.getFaceMatID obj i)
			)
		for t = 1 to tileArray.count do
			(
				local sFaces = #{}
				for i=1 to allfaces.count do
					if allfaces[i]!=-1 then
					(
						k = mid[i]
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

fn makeBakeMaterial obj notOne mChanel=
	(
		oname = obj.name
		setMaterialIdFace obj importantFacesArray
		modPanel.setCurrentObject obj.baseObject
		-- detach neu co nhieu object duoc chon
		if notOne then detachSelection false true true true "Center"

		bakeMaterials = #()
		for i = 1 to tileArray.count do
			(
				dmap = BitmapTexture()
				fname = 1000+i
				dmap.filename = bakeFolder +"\\"+ prefixFName + (fname as string) + ".jpg"
				dmap.coords.mapChannel = mChanel

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
		if notOne then
			(
				objs = for i in selection collect i
				for o in objs do setMatObject o notOne mChanel
			)
		else
			(
				setMatObject obj notOne mChanel
			)

	)

fn setMaterialIdFace obj importantFaces =
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

		for j = 1 to importantFaces.count do
			(
				oldMatId[importantFaces[j].index] = -1
			)

		if otherFaces.count > 0 then
		(
			for i = 1 to otherFaces.count do
				(
					if (oldMatId[i]!=-1) then
						(
							oldMatId[i] = polyOp.getFaceMatID obj otherFaces[i]
							newMatId[i] = 1
						)
				)

		)
		local udim = 1001
		for i = 1 to tileCount-1 do (
			udim = udim+1
			for j = 1 to importantFaces.count do (
				if importantFaces[j].udim == udim then
					(
						oldMatId[importantFaces[j].index] = polyOp.getFaceMatID obj importantFaces[j].index
						newMatId[importantFaces[j].index] = i+1
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


)
