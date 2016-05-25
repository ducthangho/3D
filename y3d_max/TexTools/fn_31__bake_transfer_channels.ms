fn fn_31__bake_transfer_channels _sizeW _sizeH _padding _channelNrA _channelNrB=(
	clearlistener();
	local btmp = bitmap _sizeW _sizeH;
	if (selection.count == 1)then(--sorry just 1 Object supported for now
		if (getCommandPanelTaskMode() != #modify)then(--make sure we are in the modify panel section
			setCommandPanelTaskMode #modify;
		)
		local uv = modPanel.getCurrentObject();--the selection in the mod panel,- the new way to select multiple objects in max2008+ to multi unwrap
		local temp_session = false;--temp as in temporary session meaning this UV modfier and its changes will be undone later
		if classof (uv) != Unwrap_UVW then(--add a UVW unwrap modifier
			modPanel.addModToSelection (Unwrap_UVW ()) ui:on;
			uv = modPanel.getCurrentObject();
			temp_session = true;
		)
		--check if target channel is the current one
	
		local org_material = copy selection[1].material;
		local num_channels = (polyOp.getNumMaps selection[1])-1;
		if (_channelNrA <= num_channels and _channelNrB <= num_channels)then(
			if( selection[1].material != undefined)then(--check for difuse material and change if accessable the display UV-channel number
				for m in (selection[1].material.maps) where (m!= undefined) do(
					--loop through all material slots and check for coordinate properties
					if (classof m.coords == StandardUVGen)then(
						m.coords.mapChannel = _channelNrA;--input channel
					)
				)
			)
			------
			local url_save_path = GetDir #image + "\\_renderToClipboard.bmp"
			local dif_bake = DiffuseMap();
			dif_bake.outputSzX    =    _sizeW;
			dif_bake.outputSzY    =    _sizeH;
			dif_bake.autoSzOn     = false;
			dif_bake.filterOn	 = true;
			dif_bake.elementName  = "DiffuseMap";
			dif_bake.enabled      = true;
			dif_bake.fileName = url_save_path;--just in case for 64-bit machines
			dif_bake.fileType = url_save_path;--appearently the target filename/path
			
			-- Baking job
			bake = selection[1].INodeBakeProperties;
			bake.bakeChannel = _channelNrB;--_channelNr;
			bake.nDilations  = _padding;-- (Pading)
			bake.flags       = 1; --bit 1 of flag will be set to signify map channel conflict
			bake.bakeEnabled  = true;
			bake.addBakeElement dif_bake;
			
			render rendertype:#bakeSelected outputwidth:_sizeW outputheight:_sizeH antiAliasing:true vfb:false progressBar:true;		
			btmp = openBitMap url_save_path;
			deleteFile url_save_path; --delete the temp. file
			
			--remove traces
			bake.removeAllBakeElements();
			bake.bakeEnabled = false;
			selection[1].removeAllBakeElements();
			selection[1].material = org_material;--replace back the original material
			
			if (temp_session == true) then(--check if we have to delete the UV modifier because it's not supposed to be there in the first place
				deleteModifier selection uv;
			)
		)else(
			messageBox "You don't have that many UV-channels on the selected object" title:"Error" beep:false;
		)
	)else(
		messageBox "Sorry only 1 object a time for now supported. Tip: copy the objects instead and merge them and then retry this script" title:"Error" beep:false;
	)
	return btmp;
)
--fn_31__bake_transfer_channels 128 128 4 2 1;
