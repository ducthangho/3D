fn fn_34__split_uv_by_smoothing_groups =(

	function split_smoothingGroups obj uv=(
		print("process. "+(classOf obj.baseObject) as string);
		if (classOf obj.baseObject == Editable_Poly)then(
			--format "continue. \n";
			function getSmoothGroups _obj =(
				local numFaces = _obj.EditablePoly.GetNumFaces();
				local numSmoothGroups = 0;
				local notfound = true;
				for i in 1 to numFaces while notfound do(
					local face_SG = polyop.getFaceSmoothGroup _obj i;
					if face_SG == 0 then(
						notfound = false;
					)
					numSmoothGroups = amax #(face_SG, numSmoothGroups);
				)
				local SmoothElements = #();
				if (notfound == true)then(
					--setup the group arrays
					for i in 1 to numSmoothGroups do(
						SmoothElements[i]= #{};
					)
					----
					for i in 1 to numFaces do(
						local face_SG = polyop.getFaceSmoothGroup _obj i;
						SmoothElements[face_SG]+= #{i};
					)
				)
				return SmoothElements;
			)
			
			--set the smoothing groups
			local fcs = obj.EditablePoly.GetNumFaces();
			local groups= getSmoothGroups obj;
			
			if (groups.count == 0)then(
				--no existing smoothing groups, so define some in the editAble Poly Object
				messagebox "You dont have smoothing groups on your model to split into. So it will be split with a default of 60 degrees"
				--format "no smoothing groups!\n";
				
				modPanel.addModToSelection (Edit_Poly ()) ui:on;
				local ePoly = obj.modifiers[#Edit_Poly];
					
				fcs = ePoly.GetNumFaces();
				ePoly.SetSelection #Face #{1..fcs}
				ePoly.autoSmoothThreshold = 60.00;
				ePoly.ButtonOp #Autosmooth
				ePoly.SetSelection #Face #{}--select nothing	
					
				groups= getSmoothGroups obj;	
				
				deleteModifier obj 1;--delete the top modifier	
				uv.unwrap.edit();--back to the UV editor
				subobjectLevel = 3;
				uv.unwrap2.setTVSubObjectMode 3;
			)	
				
			for sel in groups do(
				if ((maxVersion())[1] >= 10000 )then(
					uv.unwrap6.selectFacesByNode sel obj;
				)else(
					uv.unwrap2.selectFaces sel;
				)
				
				uv.unwrap5.quickPlanarMap();--quick flatten
				--uv.relaxByEdgeAngle 1 0 1 false;
				uv.unwrap5.relaxByFaceAngle 1 0 1 false;
				if ((maxVersion())[1] >= 10000 )then(--de select any faces, max2008 multi obj bug I guess
					uv.unwrap6.selectFacesByNode #{} obj;
				)
			)
			
			uv.unwrap2.selectFaces #{1..fcs};

			--fn_21__normalize_uv_shells();
			
			uv.unwrap2.pack 1 (0.01) true true false;
			uv.unwrap2.selectFaces #{};
		)
	)
	
	if (selection.count > 0)then(--at least an object selected
		--local obj = selection[1]; 
		local uv = modPanel.getCurrentObject();
		if( classof(uv) == Unwrap_UVW)then(
			if ((maxVersion())[1] >= 10000 )then(
				for sel in selection do(
					split_smoothingGroups sel uv;
				)
			)else(
				split_smoothingGroups selection[1] uv;--only do this with the first object in the selection
			)
		)
	)
)
--fn_34__split_uv_by_smoothing_groups();