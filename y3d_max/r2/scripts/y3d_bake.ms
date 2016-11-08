(

	struct tileStruct (u,v,sizeX,sizeY)
	struct tileClass 
		(

			tiles = #(),
			fn findTileUV =
				(

				),
			fn autoTile =
				(
					if tiles.count<1 then
						(
							tmp = tileStruct()
							tmp.u = 1
							tmp.v = 1
							tmp.sizeX = PSetting.defaultTileSize
							tmp.sizeY = PSetting.defaultTileSize
							append tiles tmp
						)
				),
			fn getListItemText idx =
				(
					return "Tile "+(idx as string)+":["+(tiles[idx].sizeX as string)+" x "+(tiles[idx].sizeY as string)+"]"
				)
		)
	if TileBake == undefined then TileBake = tileClass()		

)