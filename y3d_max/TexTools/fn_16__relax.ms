fn fn_16__relax=(
	if classof (modPanel.getCurrentObject()) == Unwrap_UVW then(
		undo on(
			local uv = modPanel.getCurrentObject();
			
			local gRi = uv.unwrap3.getRelaxIteration();
			local gRa = uv.unwrap3.getRelaxAmount();
			
			case uv.gettvsubobjectmode() of(
				1: (
					uv.unwrap3.setRelaxIteration 200;--deafult 100
					uv.unwrap3.setRelaxAmount 1;--default 0.1
					uv.unwrap3.relax2();
				)
				2: (
					--edge selection
					uv.relaxByEdgeAngle 1000 0 1 false;
				)
				3: (
					
					uv.unwrap5.relaxByFaceAngle 1000 0 1 false;
				)
			)
			
			--restore previous settings
			uv.unwrap3.setRelaxIteration gRi;
			uv.unwrap3.setRelaxAmount gRa;
		)
	)
)