global yInit
if yInit != true then
(
	-- global sVer = #("v1","m1","r1","r2","newm")
	global sVer = #("newm")

	for i=1 to sVer.count do
		(
			execute("global "+sVer[i]+"_init")
		)

	global ypath
	global presetDir = ((getdir #plugcfg) + "\\y3d\\presets\\")
	global workspaceDir = ((getdir #scene) + "\\y3d\\")
	global yFile
	global gVarTmp -- use as a global tempory variable
	global verIndex = 1
	global loadIni
	global reset_script
	global createY3DMenu
	global yInclude
	global copy_override
	global y3dMenu
	global addMenuItem
	global saveIniArray
	global loadIniArray
	global initSetting
	

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
			print ss
			execute ss
		)

	fn loadIni =
		(
			if (getDirectories workspaceDir).count == 0 then makeDir workspaceDir
			local inputFName
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
		)

	fn loadIniArray fName keys =
		(
			if (getDirectories presetDir).count == 0 then makeDir presetDir
			inputFName = presetDir + fName
			if ((getfiles inputFName).count != 0) then
				(
					for i=1 to keys.count do
						(
							yFile = inputFName
							execute("gVarTmp=getINISetting yFile \"Y3D\" \""+keys[i]+"\"")
							execute("if gVarTmp != OK then PSetting."+keys[i]+" = gVarTmp")
						)
				)
		)

	fn saveIniArray fName keys =
		(
			local outputFile=""
			if (getDirectories presetDir).count == 0 then makeDir presetDir
			outputFile = presetDir + fName
			if ((getfiles outputFile).count != 0) then deleteFile outputFile
			for i=1 to keys.count do
				(
					local val
					yFile = outputFile
					val = execute("PSetting."+keys[i]) as string
					execute("setINISetting yFile \"Y3D\" \""+keys[i]+"\" \""+val+"\"")
				)
		)

	fn reset_script vIndex =
		(
			vname = sVer[vIndex]
			execute(vname+"_init=false")
			fFrom = ypath+vname+"\\macro\\"+vname+".mcr"
			fTo = (getdir #userMacros) +"\\y3d-"+vname+".mcr"
			copy_override fFrom fTo
			macros.load (ypath+vname+"\macro")
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

	print "ylib has been loaded"
	loadIni()
	yInit = true

	yms_init = false
	minh_init = false
	yInclude "yms\lib\\yms.ms"
	yInclude "newm\lib\\minh.ms"

	-- r2_init = false
	-- yInclude "r2\lib\\r2.ms"
)
