function fn_02__bake_uvLayout _sizeW _sizeH _channelNr= (
	undo on(
		local btmp = bitmap _sizeW _sizeH;
				
		if (selection.count > 0)then(
			local objs = modPanel.getCurrentObject();--the selection in the mod panel,- the new way to select multiple objects in max2008+ to multi unwrap
			local del_uvModifier = false;
			if classof (modPanel.getCurrentObject()) != Unwrap_UVW then(--add a UVW unwrap modifier
				modPanel.addModToSelection (Unwrap_UVW ()) ui:on;
				objs = modPanel.getCurrentObject();--the selection in the mod panel,- the new way to select multiple objects in max2008+ to multi unwrap
				del_uvModifier = true;
			)
			
			--check if target channel is the current one
			local current_channel = objs.unwrap.getMapChannel();
			if (current_channel == 0)then(
				current_channel = 1;
			)
			if (current_channel != _channelNr)then(--switch to another channel
				objs.unwrap.setMapChannel _channelNr;--set the map channel number
				objs.unwrap.reset();
			)
			
			objs.renderuv_width =  _sizeW;
			objs.renderuv_height = _sizeH;
			objs.renderuv_fillmode = 0;--solid
			objs.renderuv_seamColor = color 255 255 255;
			objs.renderuv_showframebuffer = false;
			
			theFileName = GetDir #image + "\\_renderToClipboard.bmp" --define a temp.file name
			objs.unwrap5.renderUV theFileName;--save UV layout to the temp folder
			
			btmp = openBitMap theFileName;

			deleteFile theFileName; --delete the temp. file
			objs.renderuv_showframebuffer = true;--reset defaults
			
			if (del_uvModifier == true)then(
				for obj in selection do(
					deleteModifier obj 1;--delete the top one that has been temporarly created
				)
			)
		)
		return btmp;
	)
)