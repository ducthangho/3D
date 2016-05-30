ybarRollout;

try (--close if possible
	cui.UnRegisterDialogBar ybarRollout
	DestroyDialog ybarRollout;
) catch();

-- rollout ybarRollout "YBar" width:34 height:192
-- (
-- 	button btn1 "y1" pos:[5,5] width:24 height:24
-- 	-- on ybarRollout open do 
-- 	-- 	(
-- 	-- 		ybarRollout.addControl #button #btn2 "z2" "pos:[5,34] width:24 height:24"
-- 	-- 		ybarRollout.addHandler #btn2 #pressed codeStr: "MessageBox @hihi@" filter:on
-- 	-- 	)
-- )
Include "$scripts\y3d\lib\ylib.ms"

rci = rolloutCreator "ybarRollout" "YBar" width:34 height:192
rci.begin() 
for i=1 to sVer.count do
	(
		vname = "y" + (i as string)
		bname = "btn_"+(i as string)
		rci.addControl #button bname vname pos:[5,5+(i-1)*29] width:24 height:24
		-- codeS = "MessageBox @"+sVer[i]+"@"
		codeS = "macros.run @y3d@ @y3d_"+(sVer[i] as string)+"@"
		rci.addHandler bname #pressed codeStr: codeS
	)

-- rci.addControl #button #btn2 "z2" pos:[5,34] width:24 height:24
-- rci.addHandler #btn_1 #pressed codeStr: "MessageBox @hihi@" filter:on


CreateDialog (rci.end())

cui.RegisterDialogBar ybarRollout
cui.DockDialogBar ybarRollout #cui_dock_right 