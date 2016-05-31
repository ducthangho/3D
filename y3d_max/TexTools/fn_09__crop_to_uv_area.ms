fn fn_09__crop_to_uv_area tex_sizeW tex_sizeH pix_space =(
	undo on(
		--local obj = selection[1];
		if classof (modPanel.getCurrentObject()) == Unwrap_UVW then(
			local uv = modPanel.getCurrentObject();--obj.modifiers[#unwrap_uvw];

			clearlistener();
			--convert to vertex selection
			local _mode = uv.unwrap2.getTVSubObjectMode();
			if (_mode == 2)then(
				uv.unwrap2.edgeToVertSelect();
			)else if (_mode == 3)then(
				uv.unwrap2.faceToVertSelect();
			)
			---check if the selection is empty,- in that case try to select all non-dead tVerts
			--
			local verts = uv.unwrap.getSelectedVertices();
			if (verts.numberset == 0)then(
				uv.unwrap2.setTVSubObjectMode 1;--_mode
				actionMan.executeAction 0 "40021"  -- Selection: Select All
				verts = uv.unwrap.getSelectedVertices();
				print("count :"+verts.count as string)
			) 

			
			
			
			
			function getBounds uv=(
				local _maxX;
				local _maxY;
				local _minX;
				local _minY;
				local i = 1;
				if ((maxVersion())[1] >= 10000 )then(--if max2008 = multi object support, go through multiple objects
					print("multi obj mode");
					i = 1;
					for sel in selection do(
						local verts = uv.unwrap6.getSelectedVerticesByNode sel;
						
						for v in verts do(
							local pt = uv.unwrap6.GetVertexPositionByNode 1 v sel;
							
							if (i == 1) then(
								_maxX = pt.x;
								_maxY = pt.y;
								_minX = pt.x;
								_minY = pt.y;
							)else(
								_maxX = amax #(_maxX,pt.x);
								_minX = amin #(_minX,pt.x);
								_maxY = amax #(_maxY,pt.y);
								_minY = amin #(_minY,pt.y);
							)
							i+=1;
						)
					)
				)else(--Max 9 and earlier
					i = 1;
					local verts = uv.unwrap.getSelectedVertices();
					for v in verts do(
						local pt = uv.getVertexPosition 1 v;
						
						if (i == 1) then(
							_maxX = pt.x;
							_maxY = pt.y;
							_minX = pt.x;
							_minY = pt.y;
						)else(
							_maxX = amax #(_maxX,pt.x);
							_minX = amin #(_minX,pt.x);
							_maxY = amax #(_maxY,pt.y);
							_minY = amin #(_minY,pt.y);
						)
						i+=1;
					)
				)
				return #(_minX,_minY,_maxX,_maxY);
			)
				
			local _maxX;
			local _maxY;
			local _minX;
			local _minY;
			
			local bounds = getBounds(uv);
			_minX = bounds[1];
			_minY = bounds[2];
			_maxX = bounds[3];
			_maxY = bounds[4];
			
			
			
			
			
			
			
			
			
			
			
			
			-------------------------------------------------------
			local w = (_maxX - _minX) as Float ;
			local h =  (_maxY - _minY) as Float;

			local shift_valX = ((pix_space as float)/ (tex_sizeW as float)) as Float;--margin shift
			local shift_valY = ((pix_space as float)/ (tex_sizeH as float)) as Float;--margin shift
			
			local max_side = amax #(w, h) as Float;
			local sX = 1.0 /(max_side + (2.0*shift_valX));--scale value
			local sY = 1.0 /(max_side + (2.0*shift_valY));--scale value

			print("scale : "+sX as string);	
				
			uv.scaleSelectedVerticesCenter sX 1;--dit not yet got the regular version to work (wihtout center)
			uv.scaleSelectedVerticesCenter sY 2;--dit not yet got the regular version to work (wihtout center)

			-------------------------------------------------------

			local bounds = getBounds(uv);
			_minX = bounds[1];
			_minY = bounds[2];
			_maxX = bounds[3];
			_maxY = bounds[4];

			print("move: "+(-_minX) as string)

			local x = -_minX;
			local y = 1.0-_maxY;--*-1;
			uv.moveSelectedVertices [x,y, 0];
			uv.moveSelectedVertices [shift_valX,-shift_valY, 0];
			
			uv.selectVertices #{};--continue with nothing selected
			uv.unwrap2.setTVSubObjectMode _mode;--
		)
	)
)
--
--fn_09__crop_to_uv_area 512 512 16; 