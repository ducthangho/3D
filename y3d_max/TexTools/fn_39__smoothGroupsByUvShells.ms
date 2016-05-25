function fn_39__smoothGroupsByUvShells=(
	clearListener();
	if (getCommandPanelTaskMode() != #modify)then(--make sure we are in the modify panel section
		setCommandPanelTaskMode #modify;
	)
	if (selection.count == 1)then(--at least an object selected
		local obj = selection[1]; 
		local uv = modPanel.getCurrentObject();
		
		if (classof(uv) != Unwrap_UVW)then(
			modPanel.addModToSelection (Unwrap_UVW ()) ui:on;
			uv = modPanel.getCurrentObject();
		)
		
		uv.unwrap.edit();
		uv.unwrap.edit();
		uv.unwrap2.setTVSubObjectMode(3);

		local totalFaces = uv.unwrap.numberPolygons();
		
		local faceElemArray = #();
		for f=1 to totalFaces do (
			faceElemArray[ f ] = 0;
		)
		local elem = #();
		--with redraw off;
		for f=1 to totalFaces do (
			if faceElemArray[ f ] == 0 then (
				uv.unwrap2.selectFaces  #{ f };
				uv.unwrap2.selectElement();
				local elemFaces = uv.unwrap2.getSelectedFaces() as array;
				
				append elem (uv.unwrap2.getSelectedFaces());
				for i in elemFaces do (
					faceElemArray[ i ] = elem.count; -- Mark these vertices with their element number in vertElemArray.
				)
			)
		)
		
		print("num shells: "+elem.count as string+"\t"+totalFaces as string);
		
		modPanel.addModToSelection (Edit_Poly ()) ui:on;
		obj.modifiers[#Edit_Poly].autoSmoothThreshold = 180	
		for e in elem do(
			obj.modifiers[#Edit_Poly].SetSelection #Face e;	
			obj.modifiers[#Edit_Poly].ButtonOp #Autosmooth	
		)	
	)
)
--fn_39__smoothGroupsByUvShells();