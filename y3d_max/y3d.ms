ypath = undefined
fn saveIni =
	(
		local path = getThisScriptFilename() 
		path = getFilenamePath path
		presetDir = ((getdir #plugcfg) + "\\y3d\\presets\\")
		if (getDirectories presetDir).count == 0 then makeDir presetDir
		outputFile = presetDir + "y3d.ini"
		if ((getfiles outputFile).count != 0) then deleteFile outputFile
		setINISetting outputFile "General" "ypath" path
		setINISetting outputFile "General" "verIndex" "1"
		ypath = path
		verIndex = 1

		folderTo = pathConfig.GetDir #scripts + "\\y3d"
		y3dDir = makeDir folderTo
		libDir = makeDir (folderTo+"\\lib")
		fileTo = folderTo+"\\lib\\ylib.ms"
		if ((getfiles fileTo).count != 0) then deleteFile fileTo
		copyF = copyFile (path+"\\lib\ylib.ms") fileTo
	)

saveIni()

ss = ypath + "lib\ylib.ms"
ss =  "include \""+ss+"\""
execute ss

fFrom = ypath+"y3d_main.mcr"
fTo = (getdir #userMacros) +"\\y3d_main.mcr"

copy_override fFrom fTo

includefTo = "include \""+fTo+"\""
execute includefTo
-- category:"y3d"
-- tooltip:"Switch Script"
-- internalCategory:"Yêu 3D"
-- Icon:#("cws",3)
-- (
-- 	rollout y3dInstallRollout "Y3D toolsets" width:148 height:257
-- 		(

-- 			bitmap bmpLogo "Bitmap" pos:[10,10] width:128 height:128 fileName:"y3d_160.jpg"
-- 			button btnRunScript "Run script" pos:[10,207] width:128 height:30
-- 			dropDownList ddlSelectScript "Select script" pos:[10,158] width:128 height:40 items:#()
			
-- 			on y3dInstallRollout open do 
-- 				(
-- 					ss = (pathConfig.GetDir #scripts) + "\y3d\lib\ylib.ms"
-- 					ss =  "include \""+ss+"\""
-- 					execute ss
-- 					ddlSelectScript.items = sVer
-- 				)
-- 			on btnRunScript pressed  do
-- 				(
-- 					reset_script 1
-- 				)	
-- 		)
-- 	CreateDialog y3dInstallRollout
-- )

createY3DMenu()

for i=1 to sVer.count do
	(
		reset_script i
	)
		
fFrom = ypath+"y3d_startup.ms"
fTo = (getdir #userStartupScripts) +"\\y3d_startup.ms"
copy_override fFrom fTo
includefTo = "include \""+fTo+"\""
execute includefTo
