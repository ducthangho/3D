ypath = undefined
yInit = false
ySetup = true
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

		folderTo = pathConfig.GetDir #userScripts + "\\y3d"
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

fFrom = ypath+"y3d_160.jpg"
fTo = (getdir #userMacros) +"\\y3d_160.jpg"

copy_override fFrom fTo


fFrom = ypath+"y3d_main.mcr"
fTo = (getdir #userMacros) +"\\y3d_main.mcr"

copy_override fFrom fTo

includefTo = "include \""+fTo+"\""
execute includefTo
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
