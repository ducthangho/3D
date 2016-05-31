fn fn_12__extend_selection = (
	undo on(
		local objs = modPanel.getCurrentObject();
			if (classof (objs) == Unwrap_UVW ) then (
				local _mode =objs.unwrap2.getTVSubObjectMode();
				if (_mode == 2)then(--edges
					objs.unwrap2.uvEdgeSelect();
					--$.modifiers[#unwrap_uvw].unwrap5.geomEdgeLoopSelection ()
				)else if(_mode == 3)then(
					--faces
					objs.unwrap2.selectElement();
				)
			)
		--)
	)
)