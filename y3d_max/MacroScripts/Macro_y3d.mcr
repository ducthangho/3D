
	-- macroScript SelectImportantFaces
	-- category:"y3d"
	-- internalCategory:"Yêu 3D"
	-- tooltip: "Test Menu Item"
	-- (
	--  MessageBox "sac"
	-- )

	-- macroScript SetMaterialID
	-- category:"y3d" --some macro script
	-- internalCategory:"Yêu 3D"
	-- (
	--  MessageBox "sac"
	-- )


macroScript y3dbake
category:"y3d"
internalCategory:"Yêu 3D"
tooltip:"Y3D Baker"
Icon:#("cws",3)
(

	fn createY3DMenu =
	(
		theMainMenu = menuMan.getMainMenuBar()
		y3dMenu = menuMan.findMenu "Yêu 3D"
		if (y3dMenu != undefined) do (
			print "tim thay"
			menuMan.unRegisterMenu y3dMenu
		)
		y3dMenu = menuMan.createMenu "Yêu 3D"
		y3dSubMenu = menuMan.createSubMenuItem "Yêu 3D" y3dMenu
		theMainMenu.addItem y3dSubMenu -1
		theAction = menuMan.createActionItem "y3dbake" "Yêu 3D"

		y3dMenu.addItem theAction -1
		menuMan.updateMenuBar()
	)

	createY3DMenu()

	Include "$userScripts/scripts/SimpleRectanglePackAlogorithm.ms"
	Include "$userScripts/scripts/y3d_unwrap.ms"
	Include "$userScripts/scripts/bakeTile.ms"
	Include "$userScripts/scripts/y3d_bake.ms"
	importantFacesArray = #()
	s1 = undefined
	u1 = undefined
	showMODialog()
)


-- macroScript y3d_menu
-- category:"y3d"
-- (

-- )
