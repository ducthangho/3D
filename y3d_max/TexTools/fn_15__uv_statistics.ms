fn fn_15__uv_statistics =(
	undo on(
		local hadUnwrap = true;
		local obj = selection[1];
		local rating = #("\t(bad)","\t(ok)","\t(good)");
		if classOf obj.baseObject == Editable_Poly then (
			
			if obj.modifiers[ #unwrap_uvw ] == undefined then (
				hadUnwrap = false;
				addModifier obj ( unwrap_uvw() )
			)
			
			if classof (modPanel.getCurrentObject()) == Unwrap_UVW then(
				--
				
				
				local msg = "name:\t\t"+obj.name+"\n";
				
				--UV channels
				
				local a = polyOp.getNumMaps obj;
				msg+="uv channels:\t"+a as string+"\n";
				
				--UV faces
				local a = polyop.getNumMapFaces obj 1;
				local b = polyop.getNumFaces obj;
				msg+="uv/poly fcs:\t"+a as string+"/ "+b as string;
				if (a < b)then(
					msg+=rating[3]+"\n";
				)else(
					msg+=rating[2]+"\n";
				)

				--UV shells
				subObjectLevel = 1;
				max select all;
				local numVerts = obj.modifiers[ #unwrap_uvw ].numberVertices()
				local vertElemArray = #()
				for i = 1 to numVerts do ( vertElemArray[ i ] = 0 )
				local elements = 0
				local elemVerts = #{}
				
				with redraw off(
					for v = 1 to numVerts do (
						-- If vertex has no element assigned yet, create a new element.
						if vertElemArray[ v ] == 0 then (
							obj.modifiers[ #unwrap_uvw ].selectVertices #{ v }
							obj.modifiers[ #unwrap_uvw ].selectElement()
							elemVerts = obj.modifiers[ #unwrap_uvw ].getSelectedVertices()
							-- Ignore elements with less than 3 UV vertices.
							if ( elemVerts as array ).count > 2 then (
								elements += 1
								for i in elemVerts do (
									vertElemArray[ i ] = elements -- Mark these vertices with their element number in vertElemArray.
								)
							)
						)
					)
				)
				msg+="uv shells:\t"+elements as string;
				
				local a = polyop.getNumFaces obj;
				if (elements <= ceil(a*0.05))then(
					msg+="\t"+rating[3]+"\n";
				)else if(elements < ceil(a*0.2)) then(
					msg+="\t"+rating[2]+"\n";
				)else(
					msg+="\t"+rating[1]+"\n";
				)
				
				msg+="\n";
				
				
				
				
				--overlapping faces
				subObjectLevel = 3;
				obj.Unwrap_Uvw.selectOverlappedFaces();
				local _overlapCount = (obj.Unwrap_Uvw.getSelectedFaces()).numberSet;
				msg+= "overlap. fcs:\t"+_overlapCount as string;
				
				if (_overlapCount == 0)then(
					msg+="\t"+rating[3]+"\n";
				)else(
					msg+="\t"+rating[1]+"\n";
				)	
					
					
				--wasted space
				local _size = 32;-- 32*32 = 1024 pixels to check
				obj.modifiers[#unwrap_uvw].renderuv_width =  _size;
				obj.modifiers[#unwrap_uvw].renderuv_height = _size;
				obj.modifiers[#unwrap_uvw].renderuv_fillmode = 1;--solid fill mode
				obj.modifiers[#unwrap_uvw].renderuv_showoverlap = false;
				obj.modifiers[#unwrap_uvw].renderuv_seamColor = color 255 255 255;
				obj.modifiers[#unwrap_uvw].renderuv_fillColor = color 255 255 255;
				obj.modifiers[#unwrap_uvw].renderuv_showframebuffer = false;
					
				theFileName = GetDir #image + "_renderToClipboard.bmp" --define a temp.file name
				obj.modifiers[#unwrap_uvw].unwrap5.renderUV theFileName;--save UV layout to the temp folder
				obj.modifiers[#unwrap_uvw].renderuv_showframebuffer = true;--reset defaults
				
				theBitmap = openBitMap theFileName;	
				local countFill = 0;	--filled pixel area	
				try(
					for y = 0 to (theBitmap.height-1) do(
						local pixRow = getpixels theBitmap [0,y] _size;
						for x = 1 to theBitmap.width do(
							if (pixRow[x] == (color 255 255 255)) then (
								countFill+=1;
							)
						)
					)
					deleteFile theFileName --delete the temp. file
				)catch(
					msg = msg+"bmp error!! \n";
					deleteFile theFileName --delete the temp. file
				)
				local p= (countFill as float / (_size*_size) as float *100) as integer;
				msg+= "used space:\t"+p as string+"%";	

				if (p > 90)then(
					msg+="\t"+rating[3]+"\n";
				)else if(p > 70) then(
					msg+="\t"+rating[2]+"\n";
				)else(
					msg+="\t"+rating[1]+"\n";
				)
				
				--tiling uv`s
				subObjectLevel = 1;
				max select all;
				local numVerts = obj.modifiers[ #unwrap_uvw ].numberVertices()
				local vertElemArray = #()
				for i = 1 to numVerts do ( vertElemArray[ i ] = 0 )
				local elements = 0
				local elemVerts = #{}
				local overlapping_elements = 0;
				with redraw off(
					for v = 1 to numVerts do (
						-- If vertex has no element assigned yet, create a new element.
						if vertElemArray[ v ] == 0 then (
							obj.modifiers[ #unwrap_uvw ].selectVertices #{ v }
							obj.modifiers[ #unwrap_uvw ].selectElement()
							elemVerts = obj.modifiers[ #unwrap_uvw ].getSelectedVertices()
							-- Ignore elements with less than 3 UV vertices.
							if ( elemVerts as array ).count > 2 then (
								elements += 1
								local hit = false;
								for i in elemVerts do (
									local pt = $.modifiers[#unwrap_uvw].getVertexPosition 1 i;
									
									if (hit == false)then(
										if (pt.x < 0 or pt.x > 1 or pt.y < 0 or pt.y > 1)then(
											overlapping_elements+=1;
											hit = true;
										)
									)
									vertElemArray[ i ] = elements -- Mark these vertices with their element number in vertElemArray.
								)
							)
						)
					)
				)
				
				msg+= "tiled shells:\t"+overlapping_elements as string+" / "+elements as string;	
				if (overlapping_elements == 0)then(
					msg+="\t"+rating[3]+"\n";
				)else(
					msg+="\t"+rating[1]+"\n";
				)
				
				
				messageBox msg title:"UV statistics                                                                " beep:true;
				
			)else(
				print("you are not in UV-edit mode!");
			)
			--
			if not hadUnwrap then deleteModifier obj (obj.modifiers[ #unwrap_uvw ]);
		)
	)
)