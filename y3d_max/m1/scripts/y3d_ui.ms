
global selFaceMode = 1
global packType = 2
global notOneSelect = true

global showMODialog
global resetAll

fn doAttach =
	(
		notOneSelect = true
		objs = for i in selection collect i
		if objs.count < 2 then
			(
				if objs.count == 1 then notOneSelect=false
			)
		else
			(
				-- chon kieu la poly, mesh, .., tam thoi poly
				attachPoly objs false true "Center"
			)
	)

fn doBake =
	(
		-- switch to original material IDs
		if oldMatId.count > 0 then setMatIdByArray s1 oldMatId
		prefixFName = ""
		for i = 1 to tileArray.count do
			bakeTile s1 tileArray[i].sizeX tileArray[i].sizeY mapChannel tileArray[i].u tileArray[i].v 1
		stepDone = 3
	)

fn setDefaultSize sx sy =
	(
		defaultSizeX = sx
		defaultSizeY = sy
	)

rollout y3dBakeRollout "Y3D Baker" width:320 height:700
(
	button btnUnwrapMO "Unwrap" pos:[51,189] width:80 height:32
	button btnBake "Bake" pos:[22,479] width:80 height:32
	button btnMakeMat "Make bake material" pos:[30,547] width:136 height:32
	button btnDoAll "Do all + CLose" pos:[197,594] width:106 height:32
	button btnReset "Reset" pos:[87,596] width:80 height:32
	GroupBox grp1 "Step 1: Pre Bake" pos:[15,15] width:290 height:100
	GroupBox grp2 "Step 3: Bake it" pos:[14,234] width:290 height:285
	GroupBox grp3 "Step 4: Ungroup and set bake material" pos:[14,519] width:290 height:66
	edittext edtBakeFile "" pos:[20,270] width:201 height:20
	button btnOpen "Open" pos:[222,270] width:55 height:22
	label lbl1 "Folder to save bake image" pos:[23,249] width:137 height:21
		-- comboBox ddlMapType "Bake map type" pos:[32,341] width:144 height:1 items:#("Vray Complete Map", "Diffuse Color")
		-- comboBox ddlDSize "Default Size" pos:[200,341] width:87 height:1 items:#("256", "512", "1024", "2048", "4096")
	listbox lbxListTex "Textures to bake" pos:[23,360] width:143 height:7
	button btnSelectImportant "Group + Select Important" pos:[37,77] width:152 height:32
	radiobuttons rdoSelectType "Select Type" pos:[35,36] width:130 height:30 labels:#("Face", "Element") default:1 columns:2
	spinner spnTWidth "Width" pos:[191,378] width:86 height:16 range:[0,10000,0] type:#integer scale:128
	spinner spnTHeight "Height" pos:[191,403] width:86 height:16 range:[0,10000,0] type:#integer scale:128
	button btn128 "128" pos:[179,429] width:30 height:20
	button btn256 "256" pos:[213,429] width:30 height:20
	button btn512 "512" pos:[247,429] width:30 height:20
	button btn1024 "1024" pos:[213,454] width:30 height:20
	button btn2048 "2048" pos:[247,454] width:30 height:20
	button btn768 "768" pos:[179,454] width:30 height:20
	spinner spnTileCount "Tile Count" pos:[60,163] width:80 height:16 range:[0,1000,0] type:#integer scale:1
	spinner spnMapChannel "Map Channel" pos:[180,163] width:110 height:16 enabled:true range:[0,100,0] type:#integer scale:1
	GroupBox grp25 "Step 2: Unwrap" pos:[14,124] width:290 height:102
	dropdownList ddlMapType "Bake map type" pos:[23,303] width:144 height:40 items:#("Vray Complete Map", "Diffuse Color")
	dropdownList ddlDSize "Default Size" pos:[191,302] width:87 height:40 enabled:true items:#("128", "256", "512", "768", "1024", "2048") selection:1
	button btnUndo "Undo" pos:[196,545] width:86 height:32
	button btnUVEditor "Open UV Editor" pos:[139,189] width:96 height:32

		fn updateTexSelected sx:0 sy:0 =
			(
				i = lbxListTex.selection
				if i != 0 then
					(
						tmp = lbxListTex.items
						if sx>0 then spnTWidth.value = sx
						if sy>0 then spnTHeight.value = sy
						if sx>0 then tileArray[i].sizeX = sx
						if sy>0 then tileArray[i].sizeY = sy
						tmp[i] = "Tile "+(i as string)+":["+(tileArray[i].sizeX as string)+" x "+(tileArray[i].sizeY as string)+"]"
						lbxListTex.items = tmp
					)
			)
		fn unwrapStep =
			(
				doUnwrap selFaceMode packType
				-- MessageBox (stepDone as string)
				if stepDone == 1 then
					(
						items = #()
						for i=1 to tileArray.count do
							(
								name =	"Tile "+(i as string)+":["+(tileArray[i].sizeX as string)+" x "+(tileArray[i].sizeY as string)+"]"
								append items name
							)
						lbxListTex.items = items
						stepDone = 2
					)
			)






	on y3dBakeRollout open do
	(
				spnTileCount.value = tileCount
				spnMapChannel.value = mapChannel
				edtBakeFile.text = bakeFolder
				ds = defaultSizeX as string
				for i = 1 to ddlDSize.items.count do
					(
						if (ddlDSize.items[i]==ds) then
							(
								ddlDSize.selection = i
								exit
							)
					)
			)
	on btnUnwrapMO pressed do
		unwrapStep()
	on btnBake pressed do
		doBake()
	on btnMakeMat pressed do
	(
		makeBakeMaterial s1 notOneSelect mapChannel
	)
	on btnDoAll pressed do
	(
		if stepDone==1 then unwrapStep()
		if stepDone==2 then doBake()
		if stepDone==3 then makeBakeMaterial s1 notOneSelect mapChannel
	)
	on btnReset pressed do
	(
		resetAll()
		lbxListTex.items = #()
		MessageBox "D? li?u dã du?c reset"
	)
	on edtBakeFile changed text do
	(
		bakeFolder = edtBakeFile.text
	)
	on btnOpen pressed do
	(
	mypath = getSavePath()
	if mypath != undefined then
		(
		if mypath[mypath.count] != "\\" then mypath = mypath + "\\"
		bakeFolder = mypath
		edtBakeFile.text = mypath
		)
	else
		(
		bakeFolder = ""
		edtBakeFile.text = ""
		)
	)
	on lbxListTex selected sel do
	(
		spnTWidth.value = tileArray[sel].sizeX
		spnTHeight.value = tileArray[sel].sizeY
	)
	on btnSelectImportant pressed do
	(
		resetAll()
		doAttach()
		doSelectImportant selFaceMode
	)
	on rdoSelectType changed stat do
	(
					selFaceMode = stat
					print stat
				)
	on spnTWidth changed val do
		updateTexSelected sx:val
	on spnTHeight changed val do
		updateTexSelected sy:val
	on btn128 pressed do
		updateTexSelected sx:128 sy:128
	on btn256 pressed do
		updateTexSelected sx:256 sy:256
	on btn512 pressed do
		updateTexSelected sx:512 sy:512
	on btn1024 pressed do
		updateTexSelected sx:1024 sy:1024
	on btn2048 pressed do
		updateTexSelected sx:2048 sy:2048
	on btn768 pressed do
		updateTexSelected sx:768 sy:768
	on spnTileCount changed val do
		tileCount = val
	on spnMapChannel changed val do
		mapChannel = val
	on ddlMapType selected sel do
	(
		mapBakeType = ddlMapType.selection
	)
	on ddlDSize selected sel do
		setDefaultSize (ddlDSize.items[sel] as Integer) (ddlDSize.items[sel] as Integer)
	on btnUndo pressed do
		restoreOldMat()
	on btnUVEditor pressed do
		print "hi"
)

fn showMODialog =
	(
		createdialog y3dBakeRollout
	)

fn resetAll =
	(
		resetBakeFlow()
		s1 = undefined
		u1 = undefined
		oldMatId = #()
		newMatId = #()
	)
--end--
resetAll()
showMODialog()