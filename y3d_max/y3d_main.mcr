macroScript y3dtoolset
category:"y3d"
tooltip:"Reload Script"
internalCategory:"Yêu 3D"
Icon:#("cws",3)
(
	Include "$userScripts\y3d\lib\ylib.ms"
	rollout y3dInstallRollout "Y3D toolsets" width:148 height:257
		(

			bitmap bmpLogo "Bitmap" pos:[10,10] width:128 height:128 fileName:((getdir #userMacros) +"\\y3d_160.jpg")
			button btnRunScript "Reload script" pos:[10,207] width:128 height:30
			dropDownList ddlSelectScript "Select script" pos:[10,158] width:128 height:40 items:sVer
			
			on ddlSelectScript selected sel do
				(
					verIndex = sel
				)

			on btnRunScript pressed  do
				(

					print "verIndex"
					reset_script verIndex
				)	
		)
	CreateDialog y3dInstallRollout
)