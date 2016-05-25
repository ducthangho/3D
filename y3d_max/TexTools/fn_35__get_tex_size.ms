fn fn_35__get_tex_size = (
	local _w=512;--default faall back values
	local _h=512;
	
	fn fileExists fname = (getfiles fname).count != 0
	
	--local _result = 256;
	if (selection.count ==1)then(
		if (selection[1].material != undefined and (classof selection[1].material.maps[2]) == bitmaptexture)then(--the object material difuse bitmap width
			if (fileExists selection[1].material.maps[2].filename)then(--only if the bitmap exists
				_w = selection[1].material.maps[2].bitmap.width;
				_h = selection[1].material.maps[2].bitmap.height;
			)
		)else(
			if classof (modPanel.getCurrentObject()) == Unwrap_UVW then(
				_w = selection[1].modifiers[1].unwrap.getRenderWidth();
				_h = selection[1].modifiers[1].unwrap.getRenderHeight();
				--_result = amax #(_w, _h);
			)
		)
	)
	--return _result;
	return #(_w, _h, 4);
)