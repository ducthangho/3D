fn fn_37__pack_uv tWidth tPadding = (
	local _space = tPadding as float / tWidth as float;
	print("space "+_space as string);
	local uv = modPanel.getCurrentObject();
	if classof (uv) == Unwrap_UVW then(
		if ((maxVersion())[1] >= 10000 and selection.count > 1)then(
			uv.unwrap2.pack 0 _space false false true;
		)else(				
			uv.unwrap2.pack 1 _space false false true;--appearently this does not work well with multi object UV mode
		)
	)
)
