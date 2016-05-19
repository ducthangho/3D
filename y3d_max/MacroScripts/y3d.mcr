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

	Include "$userScripts/lib/y3d_lib.ms"
	Include "$userScripts/scripts/SimpleRectanglePackAlogorithm.ms"
	Include "$userScripts/scripts/y3d_bake.ms"
	Include "$userScripts/scripts/y3d_unwrap.ms"
	Include "$userScripts/scripts/y3d_pack.ms"
	Include "$userScripts/scripts/y3d_unwrap.ms"
	Include "$userScripts/scripts/y3d_material.ms"
	Include "$userScripts/scripts/y3d_ui.ms"

	resetAll()
	showMODialog()
)
