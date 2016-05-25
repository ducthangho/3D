fn fn_26__texel_calc_texture_size __texelTexSize __texelUnitSize __padding=(
	clearListener();
	
	--input and script template for texel calculate texture size: Mario Röske, Piranha-Bytes 
	--http://www.piranha-bytes.com/
	
	--under construction!!!!
	undo on(
	
		local obj; 
		if (selection.count == 0)then(
			obj = pickObject message:"pick the object" prompt:"pick the object"
		)else(
			obj = selection[1];
		)
		--if classof (modPanel.getCurrentObject()) == Unwrap_UVW then(--only if there is existing UV view open, or modifier present
		if (classOf obj.baseObject == Editable_Poly) then (--classof (modPanel.getCurrentObject()) == Unwrap_UVW
			
			select obj;--just in case the user selected multiple objects before
			--modPanel.addModToSelection (Unwrap_UVW ()) ui:on;--add a new modifier and store only here the changes
			
			local elem_3d_area = 0.;
			local faces = #{1..(polyop.getNumFaces obj)}
			
			theTMesh = snapshotAsMesh obj --snapshot as TriMesh to memory
			theNewMesh = Editable_mesh() --create an empty EMesh
			theNewMesh.mesh = theTMesh --assign TriMesh value to the EMesh
			convertTo theNewMesh Editable_Poly --collapse to EPoly
			delete theTMesh --free up memory
			resetxform theNewMesh;--reset any transformations
			maxOps.CollapseNode theNewMesh on;
			for i in faces do (--for each face in this shell
				elem_3d_area+= (polyop.getFaceArea theNewMesh i);--add the square size of this triangle to the elem_3d_area variable
			)
			delete theNewMesh;--we dont need it anymore, because we alredy estimated the total square size of the mesh surface







			local TexSquaredSize = __texelUnitSize / (sqrt elem_3d_area) * __texelTexSize;
			print(" squared size: "+nTexSizeSquared as string);
			
			--do UV modifier stuff	
			local _uv0 = obj.modifiers[#unwrap_uvw];
			
			--get UV bounding box
			local _maxX;
			local _maxY;
			local _minX;
			local _minY;
			for i = 1 to _uv0.NumberVertices() do(
				_pt = _uv0.getVertexPosition 1 i;
				if (i > 1) then(
					if (_pt.x > _maxX)then(
					_maxX = _pt.x;
					)
					if (_pt.y > _maxY)then(
						_maxY = _pt.y;
					)
					if (_pt.x < _minX)then(
						_minX = _pt.x;
					)
					if (_pt.y < _minY)then(
						_minY = _pt.y;
					)
				)else(
					_maxX = _pt.x;
					_maxY = _pt.y;
					_minX = _pt.x;
					_minY = _pt.y;
				)
			)
			--calculate the Texture space and margin space in pixels
			local w = ((_maxX - _minX) as Float ) * TexSquaredSize + (2 * __padding);--width in pixels
			local h = ((_maxY - _minY) as Float ) * TexSquaredSize + (2 * __padding);
			w = ceil w;--round up (ie 2.87 = 3.0)
			h = ceil h;--round up
			
			--scale the UV's so that they match the new UV distortion
			_uv0.scaleSelectedVerticesCenter ( TexSquaredSize / w) 1;
			_uv0.scaleSelectedVerticesCenter ( TexSquaredSize / h) 2;
			print("new size. "+(w as integer) as string+" / "+(h as integer) as string);
			
			--set the width and height within the UV editor
			_uv0.unwrap.setRenderWidth w;
				_uv0.unwrap.setRenderHeight h;
			--recalculate the bounding box because scaleSelectedVertices method is fucked up and we have to scale based on the center(it uses local UV editor pixel screen coordinates with a inverted axis,- well done autodesk)
			for i = 1 to _uv0.NumberVertices() do(
					_pt = _uv0.getVertexPosition 1 i;
					if (i > 1) then(
						if (_pt.x > _maxX)then(
							_maxX = _pt.x;
						)
						if (_pt.y > _maxY)then(
							_maxY = _pt.y;
						)
						if (_pt.x < _minX)then(
							_minX = _pt.x;
						)
						if (_pt.y < _minY)then(
							_minY = _pt.y;
						)
					)else(
						_maxX = _pt.x;
						_maxY = _pt.y;
						_minX = _pt.x;
						_minY = _pt.y;
					)
				)
			_uv0.moveSelectedVertices [-_minX + __padding/w ,1-_maxY - __padding/h , 0];
			
			--open the editor
			(modPanel.getCurrentObject()).unwrap.edit();
			(modPanel.getCurrentObject()).unwrap.edit();
		)	
	)
)
--fn_26__texel_calc_texture_size 256 256 8;