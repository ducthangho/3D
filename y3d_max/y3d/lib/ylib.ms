global yInit
if yInit != true then
(
	print "new m"
	global sVer = #("v1","v2","release1","m1","newm")
	global ypath = undefined
	global verIndex = 1
	global loadIni
	global reset_script
	global createY3DMenu
	global yInclude
	global copy_override
	global y3dMenu
	global addMenuItem

	fn yInclude fpath =
		(
			local path
			path = ypath
			if path==undefined then 
				(
					path = (pathConfig.GetDir #scripts) + "\y3d\\"
				) 
			ss = path + fpath
			r = ss.count
			for i = 1 to r do
				(
					if ss[i] == "\\" then
					(
						ss = replace ss i 1 "/"
					)
				)
			ss =  "include \""+ss+"\""
			execute ss
		)

	fn loadIni =
		(
			presetDir = ((getdir #plugcfg) + "\\y3d\\presets\\")
			inputFName = presetDir + "y3d.ini"
			if ((getfiles inputFName).count != 0) then
				(
					print "start read ini"
					ypath = getINISetting inputFName "General" "ypath"
					verIndex = getINISetting inputFName "General" "verIndex"
					-- ypath = execute (getINISetting inputFName "General" "ypath")
					-- verIndex = execute (getINISetting inputFName "General" "verIndex")
					-- if ypath == OK then ypath = getThisScriptFilename()
					-- if verIndex == OK then verIndex = 1
				)
			else 
				(
					ypath = getThisScriptFilename()
					ypath = getFilenamePath ypath
					verIndex = 1
				)
			print ypath
		)

	fn reset_script vIndex =
		(
			vname = sVer[vIndex]
			fFrom = ypath+vname+"\\macro\\"+sVer[vIndex]+".mcr"
			fTo = (getdir #userMacros) +"\\y3d-"+sVer[vIndex]+".mcr"
			-- if ((getfiles dTo).count != 0) then deleteFile dTo
			-- copyFile dFrom dTo
			copy_override fFrom fTo
			macros.load (ypath+sVer[vIndex]+"\macro")
		)

	fn copy_override fFrom fTo =
		(
			if ((getfiles fTo).count != 0) then deleteFile fTo
			copyFile fFrom fTo
		)

	fn createY3DMenu =
		(
			theMainMenu = menuMan.getMainMenuBar()
			y3dMenu = menuMan.findMenu "Yêu 3D"
			if (y3dMenu != undefined) do (
				menuMan.unRegisterMenu y3dMenu
			)
			y3dMenu = menuMan.createMenu "Yêu 3D"
			y3dSubMenu = menuMan.createSubMenuItem "Yêu 3D" y3dMenu
			theMainMenu.addItem y3dSubMenu -1
			theAction = menuMan.createActionItem "y3dtoolset" "Yêu 3D"

			y3dMenu.addItem theAction -1
			menuMan.updateMenuBar()
		)


	fn addMenuItem iName iType=
		(
			theAction = menuMan.createActionItem iName iType
			y3dMenu.addItem theAction -1
			menuMan.updateMenuBar()
		)

	y3dMenu = menuMan.findMenu "Yêu 3D"
	if (y3dMenu != undefined) do (
		menuMan.unRegisterMenu y3dMenu
	)
	y3dMenu = menuMan.createMenu "Yêu 3D"
	print "y3d v 1.0 has been loaded"
	loadIni()
	yInit = true
)
