macroScript y3dtoolset
category:"y3d"
tooltip:"Switch Script"
internalCategory:"Yêu 3D"
Icon:#("cws",3)
(
	Include "$scripts\y3d\lib\ylib.ms"
	rollout y3dInstallRollout "Y3D toolsets" width:148 height:257
		(

			bitmap bmpLogo "Bitmap" pos:[10,10] width:128 height:128 fileName:"y3d_160.jpg"
			button btnRunScript "Reload script" pos:[10,207] width:128 height:30
			dropDownList ddlSelectScript "Select script" pos:[10,158] width:128 height:40 items:sVer
			
			-- on y3dInstallRollout open do 
			-- 	(
			-- 		ddlSelectScript.items = sVer
			-- 	)
			on ddlSelectScript selected sel do
				(
					verIndex = sel
					print(sel)
				)

			on btnRunScript pressed  do
				(
					reset_script verIndex
				)	
		)
	CreateDialog y3dInstallRollout
)