macroScript SelectImportantFaces 
category:"Select Important Faces" 
internalCategory:"Yeu 3D"
tooltip:"Yeu 3D"
Icon:#("y3d_001",1)
(
	Include "$scripts/y3d_max/scripts/y3d_ui.ms"
	lbIFaceData = #("")
	importantFacesArray = #()
	s1 = undefined
	u1 = undefined
	-- spnTileCount.value = tileCount

	createdialog selectFaceRollout
)

macroScript SetMaterialID category:"Yeu 3D" --some macro script
(
	MessageBox "You have to bake object before run it!!"
	setMaterialIdFace()
)
