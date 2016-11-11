(
global panelSelect = 1
global okCallback = ""

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

rollout yf_input_name "Set name" width:200 height:107
(
	editText edtInputName "" pos:[15,32] width:166 height:26
	label lbl1 "Name" pos:[21,13] width:117 height:18
	button btnOK "OK" pos:[36,74] width:60 height:22
	button btnCancel "Cancel" pos:[104,74] width:60 height:22
	on yf_input_name open do
		(
			edtInputName.value = gVarTmp
		)
	on btnOK pressed  do
		(

			-- ss = gVarTmp+"=\""+edtInputName.value+"\""
			-- execute ss
			print okCallback
			if okCallback!=undefined then
				(
					ss = okCallback+ " = \""+edtInputName.text+"\""
					execute ss
				)
			-- print "aaazzzaaa"
			-- execute "yf_job_list.doAddTask \"zzz\""
			try (--close if possible
				DestroyDialog yf_input_name;
			) catch();		
		)
	on btnCancel pressed do
		(
			okCallback="OK"
			try (--close if possible
				DestroyDialog yf_input_name;
			) catch();	
		)
)

rollout yf_main "Y3D Bake Tools" width:320 height:550
(
 	subRollout scriptBin width:320 height:500 pos:[-1,-1]
	button btnRunAll "Run All" pos:[160,508] width:65 height:26
	button btnYClose "Close" pos:[94,508] width:60 height:26

	fn openSetting indexPanel =
		(
			dpos = GetDialogPos yf_main
			dpos[1] += 330	
			dpos[2] += 35
			panelSelect = indexPanel
			createDialog yf_main_setting style:#(#style_border) modal:true pos:dpos
		)

	-- job main pannel
	rollout yf_attach_main "Attatch" width:320 height:38
	(
		button btnAddAndSelect "Select Important" pos:[108,9] width:100 height:22
		button btnAttach "Attach" pos:[14,9] width:83 height:22

		on btnAddAndSelect pressed do
			SelFaceButtonClick()
		on btnAttach pressed do
			doAttach()
	)


	-- unwrap main pannel
	rollout yf_unwrap_main "Unwrap" width:320 height:42
	(
		button btnUnwrapMO "Unwrap" pos:[12,10] width:80 height:22 enabled:true
		button btnUVEditor "Open UV Editor" pos:[97,10] width:96 height:22 enabled:true
		button btnUTypeSetting "Setting" pos:[198,10] width:80 height:22 enabled:true

		on yf_unwrap_main open do
			(
			)
		on btnUnwrapMO pressed do
			(

			)
		on btnUVEditor pressed do
			print "hi"
		on btnUTypeSetting pressed do
			(
				openSetting 2
			)
	)

	-- bake main panel
	rollout yf_bake_main "Bake" width:320 height:179
	(
		button btnBake "Bake" pos:[10,138] width:60 height:22 enabled:true
		listbox lbxListTex "Textures to bake" pos:[12,9] width:143 height:7
		spinner spnTWidth "Width" pos:[180,27] width:86 height:16 range:[0,10000,0] type:#integer scale:128
		spinner spnTHeight "Height" pos:[180,52] width:86 height:16 range:[0,10000,0] type:#integer scale:128
		button btn128 "128" pos:[168,78] width:30 height:20
		button btn256 "256" pos:[202,78] width:30 height:20
		button btn512 "512" pos:[236,78] width:30 height:20
		button btn1024 "1024" pos:[202,103] width:30 height:20
		button btn2048 "2048" pos:[236,103] width:30 height:20
		button btn768 "768" pos:[168,103] width:30 height:20

		button btnBakeSetting "Setting" pos:[157,138] width:80 height:22 enabled:true
		button btnAutoTex "Auto Texture" pos:[73,138] width:80 height:22 enabled:true
		
		fn updateAllTile =
			(
				items = #()
				for i=1 to TileBake.tiles.count do
					(
						name =	TileBake.getListItemText i
						append items name
					)
				lbxListTex.items = items
			)
		fn updateTexSelected sx:0 sy:0 =
			(
				i = lbxListTex.selection
				if i != 0 then
					(
						tmp = lbxListTex.items
						if sx>0 then spnTWidth.value = sx
						if sy>0 then spnTHeight.value = sy
						if sx>0 then TileBake.tiles[i].sizeX = sx
						if sy>0 then TileBake.tiles[i].sizeY = sy
						tmp[i] = TileBake.getListItemText i
						lbxListTex.items = tmp
					)
			)

		on yf_bake_main open do
			(
				updateAllTile()
				-- spnTileCount.value = tileCount
				-- spnMapChannel.value = mapChannel
				-- edtBakeFile.text = bakeFolder
				-- ds = defaultSizeX as string
				-- for i = 1 to ddlDSize.items.count do
				-- 	(
				-- 		if (ddlDSize.items[i]==ds) then
				-- 			(
				-- 				ddlDSize.selection = i
				-- 				exit
				-- 			)
				-- 	)
			)
		on btnBakeSetting pressed do
			(
				openSetting 3
			)

		on btnBake pressed do
			print "hi"
		on lbxListTex selected sel do
		(
			spnTWidth.value = TileBake.tiles[sel].sizeX
			spnTHeight.value = TileBake.tiles[sel].sizeY
		)
		on spnTWidth changed val do updateTexSelected sx:val
		on spnTHeight changed val do updateTexSelected sy:val
		on btn128 pressed  do updateTexSelected sx:128 sy:128
		on btn256 pressed  do updateTexSelected sx:256 sy:256
		on btn512 pressed  do updateTexSelected sx:512 sy:512
		on btn768 pressed  do updateTexSelected sx:768 sy:768
		on btn1024 pressed  do updateTexSelected sx:1024 sy:1024
		on btn2048 pressed  do updateTexSelected sx:2048 sy:2048
		on btnAutoTex pressed do
			(
				TileBake.autoTile()
				updateAllTile()
			)
	)

	-- material main pannel
	rollout yf_material_main "Set Bake Material" width:320 height:76
	(
		button btnSetBakeM "Set new bake material" pos:[5,13] width:120 height:22 enabled:true
		button btnRestoreM "Restore Old Material" pos:[131,12] width:120 height:22 enabled:true
		button btnSendBlender "Send to Blender" pos:[5,42] width:90 height:22 enabled:true
		on btnSetBakeM pressed do
			print "hi"
		on btnRestoreM pressed do
			print "hi"
		on btnSendBlender pressed do
			print "hi"
	)

	-- yf_main event
	on yf_main open do 
		(
			AddSubRollout scriptBin yf_attach_main
			AddSubRollout scriptBin yf_unwrap_main
			AddSubRollout scriptBin yf_bake_main
			AddSubRollout scriptBin yf_material_main
		)

	on yf_main close do 
		(
			try (--close if possible
				DestroyDialog yf_main_setting;
			) catch();
		)

	on btnYClose pressed do
		DestroyDialog yf_main

	on yf_main moved pos do
		(

		)
)

-- Main setting dialog
rollout yf_main_setting "Y3D Setting" width:320 height:393
(
	button btnMSCancel "Cancel" pos:[49,350] width:50 height:26
	button btnMSApply "Apply" pos:[191,350] width:80 height:26
	button btnSaveSetting "Save setting" pos:[104,350] width:82 height:26

	subRollout settingContainer width:320 height:350 pos:[-1,-1]

	local mapChannel
	local selFaceMode
	local packType
	local bakeMapType
	local defaultTileSize

	fn doApplySetting =
		(
			PSetting.mapChannel = mapChannel
			PSetting.selFaceMode = selFaceMode			
			PSetting.packType = packType			
			PSetting.bakeMapType = bakeMapType			
			PSetting.defaultTileSize = defaultTileSize			
		)

	fn reloadSetting =
		(
			mapChannel = PSetting.mapChannel
			selFaceMode = PSetting.selFaceMode			
			packType = PSetting.packType			
			bakeMapType = PSetting.bakeMapType			
			defaultTileSize = PSetting.defaultTileSize		
		)

	-- project setting
	rollout yf_project_setting "Project setting" width:320 height:95
	(
		button btnSetPFolder "Set project folder" pos:[16,61] width:100 height:22 enabled:true
		label lbl3 "OR" pos:[126,66] width:25 height:18
		button btnNewProject "New project" pos:[154,61] width:85 height:22 enabled:true
		editText edt2 "" pos:[13,31] width:226 height:22
		label lbl4 "Project folder" pos:[18,13] width:78 height:16
		on yf_project_setting open do
			(
		
		)
		on btnSetPFolder pressed  do
			print "hi"
		on btnNewProject pressed  do
			print "hi"
	)

	-- unwrap setting
	rollout yf_unwrap_setting "Unwrap Setting" width:320 height:139
	(
		radiobuttons rdoSelType "Select Type" pos:[10,10] width:176 height:30 labels:#("Face", "Island", "All") default:1 columns:3
		dropdownList ddlPackType "Packing Type" pos:[112,52] width:109 height:40 enabled:true items:#("Lazy Packing", "Rectangle Packing") selection:2
		spinner spnMapChannel "" pos:[11,73] width:67 height:16 enabled:true range:[0,100,0] type:#integer scale:1
		button btnUnwrapSetting "Setting" pos:[228,71] width:50 height:22 enabled:true
		label lbl2 "Map Channel" pos:[12,52] width:65 height:14
		button btnReselect "Reselect Faces" pos:[204,22] width:93 height:22 enabled:true


		button btnLoadSetting "Load saved setting" pos:[10,106] width:110 height:22

		fn openUSetting =
			(
				PackType1.closeSetting()
				PackType2.closeSetting()
				ss = "PackType"+(packType as String)+".openSetting()"
				execute ss
			)

		on yf_unwrap_setting open do
			(
				rdoSelType.state = selFaceMode as integer
				spnMapChannel.value = mapChannel as integer
				ddlPackType.selection = packType as integer
			)
		on rdoSelType changed stat do
			(
				selFaceMode = stat
			)
		on spnMapChannel changed val do
			mapChannel = val
		on btnUnwrapSetting pressed do
			(
				openUSetting()
			)
		on btnReselect pressed do
			print "hi"
		on ddlPackType selected sel do
			(
				packType = ddlPackType.selection
				openUSetting()

				-- mapBakeType = ddlMapType.selection
			)
	)

	-- bake setting
	rollout yf_bake_setting "Bake setting" width:320 height:120
	(
		edittext edtBakePath "" pos:[10,30] width:201 height:20
		button btnBakeOpenFile "Open" pos:[213,29] width:55 height:22 enabled:true
		label lbl3 "Folder to save bake image" pos:[13,9] width:137 height:21
		dropdownList ddlBakeMap "Bake map type" pos:[13,63] width:144 height:40 items:#("Vray Complete Map", "Diffuse Color")
		dropdownList ddlDSize "Default Size" pos:[181,62] width:87 height:40 enabled:true items:#("128", "256", "512", "768", "1024", "2048") selection:1
		
		on yf_bake_setting open do
		(

		)

		on edtBakePath changed text do
			(
				-- bakeFolder = edtBakeFile.text
			)
		on btnBakeOpenFile pressed do
			(
				print "hi"
			-- mypath = getSavePath()
			-- if mypath != undefined then
			-- 	(
			-- 	if mypath[mypath.count] != "\\" then mypath = mypath + "\\"
			-- 	bakeFolder = mypath
			-- 	edtBakeFile.text = mypath
			-- 	)
			-- else
			-- 	(
			-- 	bakeFolder = ""
			-- 	edtBakeFile.text = ""
			-- 	)
			)
		on ddlBakeMap selected sel do
			(
				-- mapBakeType = ddlMapType.selection
				print "hi"
			)
		on ddlDSize selected sel do
			print "hi"
	)

	-- open only selected pannel
	fn selectPanel indexPanel =
		(
			for i=1 to settingContainer.rollouts.count do
				(
					settingContainer.rollouts[i].open = false
				)
			settingContainer.rollouts[indexPanel].open = true
		)

	on yf_main_setting open do 
		(
			reloadSetting()
			AddSubRollout settingContainer yf_project_setting rolledUp:true
			AddSubRollout settingContainer yf_unwrap_setting rolledUp:true
			AddSubRollout settingContainer yf_bake_setting rolledUp:true
			selectPanel panelSelect
		)
	on yf_main_setting close do
		(
			PackType1.closeSetting()
			PackType2.closeSetting()
		)
	on btnMSCancel pressed  do
		(
			try (
				DestroyDialog yf_main_setting;
			) catch();	
		)
	on btnMSApply pressed  do
		(
			doApplySetting()
			try (
				DestroyDialog yf_main_setting;
			) catch();	
		)
	on btnSaveSetting pressed  do
		(
			doApplySetting()
			PSetting.saveAsDefault()
			try (
				DestroyDialog yf_main_setting;
			) catch();	
		)
		
)

try (--close if possible
	DestroyDialog yf_main;
) catch();
createDialog yf_main

)