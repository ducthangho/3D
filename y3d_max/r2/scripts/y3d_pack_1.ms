(
	struct yface (index, f_area, area_uvw, fw, fh, udim, x, y)

	-- packing setting UI
	rollout yf_packtype_1 "Lazy packing setting" width:230 height:82
	(
		spinner spnTileCount "Tile Count" pos:[50,15] width:80 height:16 range:[0,1000,0] type:#integer scale:1
		button btnPT1Close "Close" pos:[91,47] width:47 height:20

		on yf_packtype_1 open do
		(
			spnTileCount.value = PackType1.tileCount
		)
		on spnTileCount changed val do
			PackType1.tileCount = val
		on btnPT1Close pressed do
		(
			try (--close if possible
				DestroyDialog yf_packtype_1;
			) catch();
		)
	)
	-- Type 1 Class
	struct PackingType1Class  --lazy packing
		(
			importantFaces = #(),
			oldObjectId = -1,
			tileCount = 4,
			bestSet,
			best,

			fn openSetting =
				(
					dpos = GetDialogPos yf_main_setting
					dpos[1] += 330	
					createDialog yf_packtype_1 style:#(#style_border) modal:false pos:dpos
				),

			fn closeSetting =
				(
					try (--close if possible
						DestroyDialog yf_packtype_1;
					) catch();
				),

			fn findBestFit bSet narea=
				(
					for k = 1 to importantFaces.count do
					(
						if (importantFaces[k].udim==1000)and(importantFaces[k].area_uvw<=narea) then (
							append bSet k
							narea = narea-importantFaces[k].area_uvw
						)
					)
					bestSet = bSet
					best = narea
				),

			fn sortByArea = -- co the dung qsort
				(
					listed = sort(for i in importantFaces collect i.area_uvw)
					tmp = #()
					for i =  listed.count to 1 by -1 do (
						for j = 1 to importantFaces.count do
							if importantFaces[j].area_uvw == listed[i] then (
								append tmp importantFaces[j]
								deleteItem importantFaces j
								exit
							)
					)
					importantFaces = tmp
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

	PackType1 = PackingType1Class()
)
