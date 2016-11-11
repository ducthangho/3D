(
	struct yface (index, f_area, area_uvw, fw, fh, udim, x, y)

	-- packing setting UI
	rollout yf_packtype_2 "Rectangle Packing" width:230 height:161
	(
		spinner spnDensity "Density" pos:[22,11] width:80 height:16 enabled:true range:[0,100000,0] type:#integer scale:1
		button btn128 "128" pos:[12,35] width:30 height:20
		button btn256 "256" pos:[46,35] width:30 height:20
		button btn512 "512" pos:[80,35] width:30 height:20
		button btn1024 "1024" pos:[151,34] width:30 height:20
		button btn2048 "2048" pos:[185,35] width:30 height:20
		button btn768 "768" pos:[117,35] width:30 height:20
		dropdownList ddlMaxSize "Maxsize Tile" pos:[12,66] width:80 height:40 items:#("512", "1024", "2048", "2560", "4096") selection:3
		button btnPT2Close "Close" pos:[91,126] width:47 height:20
	
		on yf_packtype_2 open do
		(
			spnDensity.value = PackType2.density
			ddlMaxSize.selection = PackType2.maxSizeId
		)
		on spnDensity changed val do
			PackType2.density = val
		on btn128 pressed do
			spnDensity.value = 128
		on btn256 pressed do
			spnDensity.value = 256
		on btn512 pressed do
			spnDensity.value = 512
		on btn1024 pressed do
			spnDensity.value = 1024
		on btn2048 pressed do
			spnDensity.value = 2048
		on btn768 pressed do
			spnDensity.value = 768
		on ddlMaxSize selected sel do
			PackType2.maxSizeId = sel
		on btnPT2Close pressed do
		(
			try (--close if possible
				DestroyDialog yf_packtype_2;
			) catch();
		)
	)

	-- Type 1 Class
	struct PackingType2Class  --lazy packing
		(
			importantFaces = #(),
			oldObjectId = -1,
			density = 512,
			maxSizeId = 3, -- 6=2048

			fn openSetting =
				(
					dpos = GetDialogPos yf_main_setting
					dpos[1] += 330	
					createDialog yf_packtype_2 style:#(#style_border) modal:false pos:dpos
				),
			fn closeSetting =
				(
					try (--close if possible
						DestroyDialog yf_packtype_2;
					) catch();
				),

			fn initData bitFaceAr objId =
				(

				),

			fn doUnwrap =
				(

				),

			on create do 
				(

				)
		)

	PackType2 = PackingType2Class()
)
