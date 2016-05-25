function fn_01__bake_selectionMask _sizeW _sizeH _channelNr = (
	undo on(
		local btmp = bitmap _sizeW _sizeH;
		
		if (selection.count > 0)then(
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
			local current_channel = uv.unwrap.getMapChannel();
			if (current_channel == 0)then(--UV faces are not yet created, so it should be 1 by default so that they will be created
				current_channel = 1;
			)
			if (current_channel != _channelNr)then(--switch to another channel
				uv.unwrap.setMapChannel _channelNr;--set the map channel number
				uv.unwrap.reset();
			)
		
			if (uv.unwrap2.getTVSubObjectMode() != 3)then(--check if the 
				uv.unwrap2.setTVSubObjectMode 3;
				max select all;--select all faces
			)
			if ((uv.getSelectedFaces()).count == 0)then(--no faces selected, so select them all
				max select all;--select all faces
			)
			
			
						
			--obj.modifiers[#unwrap_uvw].unwrap.getSelectedPolygons() )
		
			uv.unwrap.filterSelected();
			uv.unwrap.filterSelected();
			

			uv.renderuv_width =  _sizeW;
			uv.renderuv_height = _sizeH;
			uv.renderuv_fillmode = 1;--solid
			uv.renderuv_showoverlap = false;
			uv.renderuv_seamColor = color 255 255 255;
			uv.renderuv_fillColor = color 255 255 255;
			uv.renderuv_showframebuffer = false;

			theFileName = GetDir #image + "_renderToClipboard.bmp" --define a temp.file name
			uv.unwrap5.renderUV theFileName;--save UV layout to the temp folder
			btmp = openBitMap theFileName;
			deleteFile theFileName; --delete the temp. file
			
			uv.renderuv_showframebuffer = true;--reset defaults
			uv.unwrap.filterSelected();--turn back former filter
			
			
			
			if (temp_session == true) then(--check if we have to delete the UV modifier because it's not supposed to be there in the first place
				deleteModifier selection uv;
			)
		)
		return btmp;
	)
)
--fn_01__bake_uvfaces_2_clipboard 256 256 1;