fn fn_19__fixTexelDensity __textureSize __texelTexSize __texelUnitSize=(

	function texelScale obj _vertexSelection _textureSize _texelTexSize _texelUnitSize =(
		if (classOf obj.baseObject == Editable_Poly and _vertexSelection.numberSet >0) then (--classof (modPanel.getCurrentObject()) == Unwrap_UVW
			
			local uv = obj.modifiers[ #unwrap_uvw ];
			local original_vertex_selection = _vertexSelection;--uv.getSelectedFaces();
			
			uv.selectVertices original_vertex_selection;
			local vertElemArray = #();
			for v in original_vertex_selection do (
				vertElemArray[ v ] = 0;
			)
			local elementsCount = 0;
			local elemVerts = #{};

			local elem = #();
			with redraw off
			for v in original_vertex_selection do (
				-- If vertex has no element assigned yet, create a new element.
				if vertElemArray[ v ] == 0 then (
					uv.selectVertices #{ v }
					uv.selectElement();
					elemVerts = uv.getselectedvertices() as array;
					uv.unwrap2.vertToFaceSelect();--convert shell to face selection
					
					if elemVerts.count > 2 then (-- Ignore elements with less than 3 UV vertices.
						elementsCount += 1;
						elem[elementsCount] = uv.unwrap2.getSelectedFaces();--elemVerts;
						for i in elemVerts do (
							vertElemArray[ i ] = elementsCount; -- Mark these vertices with their element number in vertElemArray.
						)
					)
				)
			)

			--
			if ( elementsCount > 0)then(--only continue if more than one elements are catched
				local elem_3d_area = #();
				local elem_uv_area = #();

				theTMesh = snapshotAsMesh obj --snapshot as TriMesh to memory
				theNewMesh = Editable_mesh() --create an empty EMesh
				theNewMesh.mesh = theTMesh --assign TriMesh value to the EMesh
				convertTo theNewMesh Editable_Poly --collapse to EPoly
				delete theTMesh --free up memory
				resetxform theNewMesh;--reset any transformations
				maxOps.CollapseNode theNewMesh on;

				for e = 1 to elementsCount do (
					elem_3d_area[e]= 0;
					for i in elem[e] do (--for each face in this shell
						elem_3d_area[e] += (polyop.getFaceArea theNewMesh i);
					)
				)
				---------------------------------------
				convertToMesh theNewMesh;
				--swap UV faces
				channelInfo.CopyChannel theNewMesh 1 0;--copy Geometry to #0
				channelInfo.PasteChannel theNewMesh 3 2;--paste UVmap to #2;
				channelInfo.CopyChannel theNewMesh 3 1;--copy UVmap to #1
				channelInfo.PasteChannel theNewMesh 3 3;--paste UVmap to #3;
				channelInfo.CopyChannel theNewMesh 3 2;--copy UVmap to #2
				channelInfo.PasteChannel theNewMesh 3 1;--paste UVmap to #1
				channelInfo.CopyChannel theNewMesh 3 3;--copy UVmap to #3
				channelInfo.PasteChannel theNewMesh 1 0;--paste Geometry to #0;
				channelInfo.ClearChannel theNewMesh 2;--clear channel 2
				channelInfo.ClearChannel theNewMesh 3;--clear channel 3
				theNewMesh.scale = [_textureSize,_textureSize,_textureSize];
				resetxform theNewMesh;--reset any transformations
				maxOps.CollapseNode theNewMesh on;
				convertTo theNewMesh Editable_Poly --collapse to EPoly
				
				for e = 1 to elementsCount do (
					elem_uv_area[e]= 0;
					for i in elem[e] do (
						elem_uv_area[e] += (polyop.getFaceArea theNewMesh i);
					)
				)
				delete theNewMesh;
				subObjectLevel = 3;
				
				--scale elements
				for e = 1 to elementsCount do (
					local G = ((sqrt elem_3d_area[e] as Float) / _texelUnitSize as Float) * _texelTexSize as Float;--what it should be in this area
					local W = sqrt elem_uv_area[e] as Float;--
					uv.unwrap2.selectFaces elem[e];
					print(e as string+"#. scale @" +(G / W) as string+"x");
					uv.unwrap2.scaleSelectedCenter (G / W) 0;--x and y = proportional
				)
				uv.selectVertices original_vertex_selection;
				clearSelection();
				
			)	
			subObjectLevel = 0;--no sub mode
		)
	)
	
	undo on(
		local _selection = selection as array;--copy of the current selection as array
		
		if (_selection.count > 1)then(--multiple objects
			
			clearSelection();
			clearNodeSelection redraw:true;
			disableSceneRedraw();
			
			clearListener();
			for _obj in _selection do(
				try(
					if (_obj.scale != [1,1,1])then(
						ResetXForm _obj;
					)
					addModifier _obj ( unwrap_uvw() );
					if _obj.modifiers[ #unwrap_uvw ] != undefined then(
						clearSelection();
						clearNodeSelection redraw:true;
						select _obj;
						
						modPanel.setCurrentObject _obj.modifiers[#unwrap_uvw] node:_obj ui:true;
						max modify mode;
						subObjectLevel = 1;
						actionMan.executeAction 0 "40021"  -- Selection: Select All

						local numVertexSelected = #{1..(polyop.getNumMapVerts _obj 1)};	
						
						format "..texel job @ $verts: $\n" _obj.name numVertexSelected;
						texelScale _obj numVertexSelected __textureSize __texelTexSize __texelUnitSize;
					)
				)catch(
					print("error occoured @"+_obj as string);
					enableSceneRedraw();
				)	
			)
			clearSelection();
			clearNodeSelection redraw:true;
			select _selection;--back to the original object selection
			
			
		)else if(_selection.count == 1)then(--just 1 object
			print("single object...");
			clearListener();
			
			local _obj = _selection[1];--this very first object
			try(
				
				if classOf _obj.baseObject == Editable_Poly then (
					max modify mode;
					if _obj.modifiers[ #unwrap_uvw ] == undefined then (
						addModifier _obj ( unwrap_uvw() );
					)

					local face_selection = #{};
					local uv = _obj.modifiers[ #unwrap_uvw ];--the UV modifier
					if (subObjectLevel == 3)then(
						face_selection= uv.unwrap2.getSelectedFaces();
					)else(--not in any sub mode thus select all FACES!
						face_selection= #{1..(polyop.getNumFaces _obj)};
					)
					uv.unwrap6.selectFacesByNode face_selection _obj;
					uv.unwrap2.faceToVertSelect();
					subObjectLevel = 1;
					
					local numVertexSelected = uv.unwrap.getSelectedVertices();
					format "..texel job @ %verts: %\n" _obj.name numVertexSelected;
					texelScale _obj numVertexSelected __textureSize __texelTexSize __texelUnitSize;
					
					select _obj;
					subObjectLevel = 3;
					uv.unwrap6.selectFacesByNode face_selection _obj;
				)
			)catch(
				print("error occoured in single object @"+_obj as string);
				enableSceneRedraw();
			)
		)
	)
	enableSceneRedraw();
	
	
	
	completeRedraw();
	print "done processing";
)