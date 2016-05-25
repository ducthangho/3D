fn fn_36__explode_uv_faces tWidth tPadding = (
	local _space = tPadding as float / tWidth as float;
	if classof (modPanel.getCurrentObject()) == Unwrap_UVW then(
		--local _space = 0;
		local uv = modPanel.getCurrentObject();
		
		uv.unwrap2.setTVSubObjectMode 3;
		local faces = uv.unwrap2.getSelectedFaces();
		actionMan.executeAction 0 "40021"  -- Selection: Select All
		
		
		uv.unwrap2.flattenMap 60.0 #([1,0,0], [-1,0,0], [0,1,0], [0,-1,0], [0,0,1], [0,0,-1]) (_space) true 0 false false;
		
	
		actionMan.executeAction 0 "40021"  -- Selection: Select All
		local w = (rl_textureTools.edt_tex_sizeW.text as integer)
		local h =  (rl_textureTools.edt_tex_sizeH.text as integer) 
		local pd = (rl_textureTools.edt_tex_padding.text as integer);
		fn_09__crop_to_uv_area w h pd;--crop to all elements
		

		if ((maxVersion())[1] >= 10000 )then(--if max2008 = multi object support, clean up the selection
			for s in selection do(
				uv.unwrap6.selectFacesByNode #{} s;
			)
		)else(
			uv.unwrap2.selectFaces #{};--works perfectly with just 1 object selected and or max9
		)
	
		

	)
)