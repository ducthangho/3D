fn fn_06__align_uv_shell_to_edge =(
--fn align_uv_edge_to_axis =(
	print("align uv shell to axis");
	undo on(
		local uv = modPanel.getCurrentObject();
		--local objs = modPanel.getCurrentObject();
		if classof (uv) == Unwrap_UVW then(
			local _mode = uv.unwrap2.getTVSubObjectMode();
			if (_mode == 2)then(

				local _array = uv.unwrap2.getSelectedEdges();
				
				if (_array.numberSet == 1)then(--only 1 edge selected!!!
					
					
					--calculate the angle difference between 2 deltas
					function get2VertsAngleDiff dx dy= (
						local _a_abs = mod ((atan2 dy dx)+4*360) 360;--the angle in closed 360 degrees
						local _a_off = (mod _a_abs 90);--angle offset
						if (_a_off > 45)then(
							_a_off = -(90 - _a_off);
						)
						return _a_off;
					)
					
					local _edge = (_array as array)[1] as integer;

					--get the transformation info
					uv.unwrap2.edgeToVertSelect();
					_points = uv.unwrap.getSelectedVertices() as array;
					local ptA = uv.getVertexPosition 1 _points[1];
					local ptB = uv.getVertexPosition 1 _points[2];
					local dx = ptB.x - ptA.x;
					local dy = ptB.y - ptA.y;
					
					local a_off = get2VertsAngleDiff dx dy;

					uv.unwrap2.selectElement();
					uv.unwrap2.RotateSelected (-a_off  * PI/180) [(ptA.x + dx/2),(ptA.y + dy/2),0]
					uv.unwrap2.selectEdges #{_edge};

					local ptA = uv.getVertexPosition 1 _points[1];
					local ptB = uv.getVertexPosition 1 _points[2];
					local dx = ptB.x - ptA.x;
					local dy = ptB.y - ptA.y;
					local a_off = get2VertsAngleDiff dx dy;
					
					if (a_off != 0)then(--stupid angle snap is on
						print("shut off the angle snap..");
						max angle snap toggle ;--shut it up
						uv.unwrap2.selectElement();
						uv.unwrap2.RotateSelected (-a_off  * PI/180) [(ptA.x + dx/2),(ptA.y + dy/2),0]
						uv.unwrap2.selectEdges #{_edge};
					)
				)else(
					print("select just 1 edge!");
				)
			)
		)
	)
)