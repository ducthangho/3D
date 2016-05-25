function fn_43__bake_vertexColors= (
	clearListener();


	if (selection.count > 0)then(
		obj = selection[1];
		
		if (getCommandPanelTaskMode() != #modify)then(--make sure we are in the modify panel section
			setCommandPanelTaskMode #modify;
		)
		
		clearSelection;
		select obj;
		modPanel.addModToSelection (VertexPaint ()) ui:on
		obj.modifiers[#VertexPaint].useMaps = on;
		--obj.modifiers[#VertexPaint].colorBy = 1;--xolor by vertex
		obj.modifiers[#VertexPaint].colorBy = 0--by face
		obj.modifiers[#VertexPaint].lightingModel = 2;--diffuse only
		obj.modifiers[#VertexPaint].mapchannel=0;--vertex color
		--AssignVertexColors.ApplyNodes #(obj) vertexPaint:obj.modifiers[#VertexPaint];
		
		--vertex colors are not updated (bug in max, so the button has to be pressed manually
		fn pressAssignVColor = if getCommandPanelTaskMode() == #modify and iskindof (modpanel.getCurrentObject()) VertexPaint do(
			name = "Assign" 
			if (win = windows.getChildHWND #max "Assign Vertex Colors") != undefined do (
				if (bt = windows.getChildHWND (UIAccessor.GetParentWindow win[1]) name) != undefined do (
					UIAccessor.PressButton bt[1]
					format "pressed: %\n" bt 
				) 
			)
		)

		pressAssignVColor();
	)
)

--fn_43__bake_vertexColors();
