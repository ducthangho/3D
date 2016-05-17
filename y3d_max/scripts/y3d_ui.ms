(
global selFaceMode = 1
global packType = 1
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
	)

fn setDefaultSize sx sy =
	(
		defaultSizeX = sx
		defaultSizeY = sy
	)

rollout y3dBakeRollout "Y3D Baker" width:320 height:750
	(
		button btnUnwrapMO "Unwrap" pos:[51,189] width:80 height:32
		button btnBake "Bake" pos:[30,527] width:80 height:32
		button btnMakeMat "Make bake material" pos:[30,634] width:136 height:32
		button btnDoAll "Do all + CLose" pos:[198,702] width:106 height:32
		button btnReset "Reset" pos:[113,702] width:80 height:32
		groupBox grp1 "Step 1: Pre Bake" pos:[15,15] width:290 height:101
		groupBox grp2 "Step 3: Bake it" pos:[15,257] width:290 height:317
		groupBox grp3 "Step 4: Ungroup and set bake material" pos:[15,593] width:290 height:92
		editText edtBakeFile "" pos:[29,308] width:201 height:20
		button btnOpen "Open" pos:[231,308] width:55 height:22
		label lbl1 "Folder to save bake image" pos:[32,287] width:137 height:21
		-- comboBox ddlMapType "Bake map type" pos:[32,341] width:144 height:1 items:#("Vray Complete Map", "Diffuse Color")
		-- comboBox ddlDSize "Default Size" pos:[200,341] width:87 height:1 items:#("256", "512", "1024", "2048", "4096")
		listBox lbxListTex "Textures to bake" pos:[32,398] width:143 height:7
		button btnSelectImportant "Group + Select Important" pos:[37,77] width:152 height:32
		radioButtons rdoSelectType "Select Type" pos:[35,36] width:150 height:30 labels:#("Face", "Element") default:1 columns:2
		spinner spnTWidth "Width" pos:[200,416] width:86 height:16 range:[0,10000,0] type:#integer scale:128
		spinner spnTHeight "Height" pos:[200,441] width:86 height:16 range:[0,10000,0] type:#integer scale:128
		button btn128 "128" pos:[188,467] width:30 height:20
		button btn256 "256" pos:[222,467] width:30 height:20
		button btn512 "512" pos:[256,467] width:30 height:20
		button btn1024 "1024" pos:[222,492] width:30 height:20
		button btn2048 "2048" pos:[256,492] width:30 height:20
		button btn768 "768" pos:[188,492] width:30 height:20
		spinner spnTileCount "Tile Count" pos:[60,163] width:80 height:16 range:[0,100,0] type:#integer scale:1
		spinner spnMapChannel "Map Channel" pos:[180,163] width:110 height:16 enabled:true range:[0,100,0] type:#integer scale:1
		groupBox grp25 "Step 2: Unwrap" pos:[15,136] width:290 height:102
		dropDownList ddlMapType "Bake map type" pos:[32,341] width:144 height:40 items:#("Vray Complete Map", "Diffuse Color")
		dropDownList ddlDSize "Default Size" pos:[200,340] width:87 height:40 enabled:true items:#("128", "256", "512", "768", "1024", "2048") selection:1

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
				items = #()
				for i=1 to tileArray.count do
					(
						name =	"Tile "+(i as string)+":["+(tileArray[i].sizeX as string)+" x "+(tileArray[i].sizeY as string)+"]"
						append items name
					)
				lbxListTex.items = items
			)

		on y3dBakeRollout open do (
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

		on btnUnwrapMO pressed  do unwrapStep()
		on btnBake pressed  do doBake()
		on btnMakeMat pressed do
			(
				makeBakeMaterial s1 notOneSelect mapChannel
			)
		on btnDoAll pressed  do
			(
				unwrapStep()
				doBake()
				makeBakeMaterial s1 notOneSelect mapChannel
			)
		on btnReset pressed  do
			(
				resetAll()
				lbxListTex.items = #()
				MessageBox "Dữ liệu đã được reset"
			)
		on lbxListTex selected sel do
			(
				spnTWidth.value = tileArray[sel].sizeX
				spnTHeight.value = tileArray[sel].sizeY
			)
		on btnSelectImportant pressed  do
			(
				resetAll()
				doAttach()
				doSelectImportant selFaceMode
			)
		on rdoSelectType changed stat do (
				selFaceMode = stat
				print stat
			)
		on spnTWidth changed val do updateTexSelected sx:val
		on spnTHeight changed val do updateTexSelected sy:val
		on btn128 pressed  do updateTexSelected sx:128 sy:128
		on btn256 pressed  do updateTexSelected sx:256 sy:256
		on btn512 pressed  do updateTexSelected sx:512 sy:512
		on btn768 pressed  do updateTexSelected sx:768 sy:768
		on btn1024 pressed  do updateTexSelected sx:1024 sy:1024
		on btn2048 pressed  do updateTexSelected sx:2048 sy:2048

		on spnTileCount changed val do tileCount = val
		on spnMapChannel changed val do mapChannel = val
		on ddlMapType selected sel do
			(
				mapBakeType = ddlMapType.selection
			)
		on ddlDSize selected sel do	setDefaultSize (ddlDSize.items[sel] as Integer) (ddlDSize.items[sel] as Integer)
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
)
