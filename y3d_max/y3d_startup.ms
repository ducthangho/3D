ybarRollout;

try (--close if possible
	cui.UnRegisterDialogBar ybarRollout
	DestroyDialog ybarRollout;
) catch();

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

		addMenuItem ("y3d_"+sVer[i]) "y3d"
	)

CreateDialog (rci.end())
cui.RegisterDialogBar ybarRollout
cui.DockDialogBar ybarRollout #cui_dock_right