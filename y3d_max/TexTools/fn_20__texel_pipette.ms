fn fn_20__texel_pick_up _textureSize =(
	clearListener();
	undo on(
		local obj; 
		if (selection.count == 0)then(
			obj = pickObject message:"pick the object" prompt:"pick the object"
		)else(
			obj = selection[1];
		)
		
		if (classOf obj.baseObject == Editable_Poly) then (--classof (modPanel.getCurrentObject()) == Unwrap_UVW
			
			local elem_3d_area = 0.;
			local elem_uv_area = 0.;
			local faces = #{1..(polyop.getNumFaces obj)}
			
			theTMesh = snapshotAsMesh obj --snapshot as TriMesh to memory
			theNewMesh = Editable_mesh() --create an empty EMesh
			theNewMesh.mesh = theTMesh --assign TriMesh value to the EMesh
			convertTo theNewMesh Editable_Poly --collapse to EPoly
			delete theTMesh --free up memory
			resetxform theNewMesh;--reset any transformations
			maxOps.CollapseNode theNewMesh on;
			for i in faces do (--for each face in this shell
				elem_3d_area+= (polyop.getFaceArea theNewMesh i);
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
			
			for i in faces do (
				elem_uv_area+= (polyop.getFaceArea theNewMesh i) as float;
			)
			delete theNewMesh;
			local _units = (sqrt elem_3d_area);
			local _pixels = (sqrt elem_uv_area);
			
			format "pixel. % \t\t units % \n" _pixels _units;
			return [_pixels as float, _units as float];
		)else(
			return [0.0, 0.0];
		)
	)
)