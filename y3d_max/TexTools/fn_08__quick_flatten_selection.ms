fn fn_08__quick_flatten_selection _relative=(

		
	
	function flattenSelForNode obj uv=(
		local numFacesSelected;-- = ( uv.unwrap.getSelectedPolygons() ).numberSet;
		if ((maxVersion())[1] >= 10000 )then(--max2008+ version
			numFacesSelected = (uv.unwrap6.getSelectedFacesByNode obj).numberSet;
		)else(
			numFacesSelected = ( uv.unwrap.getSelectedPolygons() ).numberSet;
		)
		
		--print("facenum "+numFacesSelected as string);
		if (numFacesSelected > 0)then(
			local var_a = uv.unwrap3.getRelaxIteration();
			
			uv.unwrap5.quickPlanarMap();--quick flatten
			uv.unwrap3.setRelaxIteration 1000;
			uv.unwrap3.relax2();
			
			---restore previous settings
			uv.unwrap3.setRelaxIteration var_a;
		)
	)
	
	
	
		
	if (selection.count > 0)then(--at least an object selected
		local uv = modPanel.getCurrentObject();
		if( classof(uv) == Unwrap_UVW)then(
			undo on(
				if ((maxVersion())[1] >= 10000 )then(
					for sel in selection do(
						flattenSelForNode sel uv;
					)
				)else(
					flattenSelForNode selection[1] uv;--only do this with the first object in the selection
				)
			)
		)
	)	
				
)