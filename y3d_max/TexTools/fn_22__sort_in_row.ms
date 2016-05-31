fn fn_22__sort_in_row useXaxis texSizeW texSizeH texPad =(
	clearListener();
	if (selection.count > 0)then(
		undo on(
			
			

			function sortRow obj uv useXaxis texSizeW texSizeH texPad=(
				
				if (uv != undefined) then(
					local _mode =uv.unwrap2.getTVSubObjectMode();
					
					if (classOf obj.baseObject == Editable_Poly ) then (
							
						--local uv = obj.modifiers[ #unwrap_uvw ];
						uv.unwrap2.selectElement();	

						local _mode = uv.unwrap2.getTVSubObjectMode();
						if (_mode == 2)then(
							uv.unwrap2.edgeToVertSelect();
						)else if (_mode == 3)then(
							uv.unwrap2.faceToVertSelect();
						)
						
						if ((maxVersion())[1] >= 10000 )then(--Max 2008+
							local original_vertex_selection = uv.unwrap6.getSelectedVerticesByNode obj;
							local numVertexSelected = #{1..(uv.unwrap6.numberVerticesByNode obj)};
						)else(
							local original_vertex_selection = uv.unwrap.getSelectedVertices();
							local numVertexSelected = #{1..(uv.unwrap.NumberVertices())};
						)
						
						/*
						local numVertexSelected = #{1..(polyop.getNumMapVerts obj 1)};
						
						--local numVertexSelected = #{1..(uv.unwrap.NumberVertices())};
						local numVertexSelected = #{1..(uv.unwrap6.numberVerticesByNode obj)};
						
						*/
						
					--		numberVerticesByNode 
							
						--NumberVertices()
-->.

						
						/*
						uv.unwrap2.setTVSubObjectMode 1;--vert mode
						actionMan.executeAction 0 "40021";--select all
						numVertexSelected = uv.unwrap6.getSelectedVerticesByNode obj;
						uv.unwrap6.selectVerticesByNode original_vertex_selection obj;
*/
						
						
						
						
						
						
						
						
						local vertElemArray = #();
						for v in numVertexSelected do (
							vertElemArray[ v ] = 0;
						)
						local elementsCount = 0;
						local elemVerts = #{};

						uv.unwrap2.setTVSubObjectMode 1;
						
						local elem = #();
						with redraw off;
						for v in original_vertex_selection do (
							if vertElemArray[ v ] == 0 then (
								if ((maxVersion())[1] >= 10000 )then(--Max 2008+
									uv.unwrap6.selectVerticesByNode #{ v } obj;
								)else(
									uv.selectVertices #{ v }
								)
								uv.unwrap2.selectElement();
								if ((maxVersion())[1] >= 10000 )then(--Max 2008+
									elemVerts = (uv.unwrap6.getSelectedVerticesByNode obj) as array;
								)else(
									elemVerts = uv.unwrap.getselectedvertices() as array;
								)
								
								if elemVerts.count > 2 then (-- Ignore elements with less than 3 UV vertices.
									elementsCount += 1;
									
									
									
									if ((maxVersion())[1] >= 10000 )then(--Max 2008+
										elem[elementsCount] = (uv.unwrap6.getSelectedVerticesByNode obj);
									)else(
										elem[elementsCount] = uv.unwrap.getselectedvertices();--elemVerts;
									)
									
									for i in elemVerts do (
										vertElemArray[ i ] = elementsCount; -- Mark these vertices with their element number in vertElemArray.
									)
								)
							)
						)


						if ( elementsCount > 0)then(--only continue if more than one elements are catched
							--continue

							function compress_rotate _obj uv usexflow = (
							--	local uv = _obj.modifiers[ #unwrap_uvw ];
								local uv1 = uv.unwrap;
								local uv2 = uv.unwrap2;
								function getBBox uv _obj=(
									if ((maxVersion())[1] >= 10000 )then(--Max 2008+
										local _verts = (uv.unwrap6.getSelectedVerticesByNode _obj) as array;
									)else(
										local _verts = uv.unwrap.getSelectedVertices() as array;
									)
									local minx;
									local maxx;
									local miny;
									local maxy;
									local cnt = 0;
									for i in _verts do (
										local _pt = uv.unwrap.getVertexPosition 1 i;
										if (cnt == 0)then(
											minx = _pt.x;
											miny = _pt.y;
											maxx = _pt.x;
											maxy = _pt.y;
											cnt+=1;
										)else(
											minx = amin #(_pt.x, minx) as float;
											miny = amin #(_pt.y, miny) as float;
											maxx = amax #(_pt.x, maxx) as float;
											maxy = amax #(_pt.y, maxy) as float;
										)
									)
									return #((maxx - minx),(maxy - miny),minx,maxy);
								)
								
								--settings
								local sweepSteps = 8;
								local iterations = 4;
								local sweepRange = 90 as float;
								local sweepAngle = (sweepRange/ sweepSteps) as float;
								
								local dia_old;--diameter of the bounding box ( w +h);
								local dia_new;
								local bestAngle;
								local correctionAngle;
								local bbox;
								
								for i= 1 to iterations do (
									bbox = getBBox uv _obj;
									dia_old = amin #( bbox[1] , bbox[2] );
									bestAngle = 0;
									with redraw off(
										for n = 1 to (sweepSteps-1) do (--rotate sweepRange
											uv1.rotateSelectedVerticesCenter (sweepAngle* PI/180);
											
											bbox = getBBox uv _obj;
											dia_new = amin #( bbox[1] , bbox[2] );
											
											if (dia_new < dia_old) then(
												dia_old = dia_new;
												bestAngle = ((n) * sweepAngle);
											)
										)
									)
									correctionAngle = bestAngle - sweepAngle*(sweepSteps - 1);
									if (i != iterations)then(
										sweepRange = sweepAngle;
										sweepAngle = sweepRange/sweepSteps;
										correctionAngle -= sweepRange * 0.5;
									)
									uv1.rotateSelectedVerticesCenter (correctionAngle* PI/180);
								)
								--make sure its always horizontal!!!
								bbox = getBBox uv _obj;
								if (usexflow== false)then(
									if (bbox[2] > bbox[1])then(
										uv1.rotateSelectedVerticesCenter (90* PI/180);
										bbox = getBBox uv _obj;
									)
								)else(
									if (bbox[2] < bbox[1])then(
										uv1.rotateSelectedVerticesCenter (90* PI/180);
										bbox = getBBox uv _obj;
									)
								)
								return bbox;
							)

							
							
							local min_x;
							local min_y;
							local max_x;
							local max_y;
							
							local widths = #();--used to sort later
							local bbox = #();
							for e = 1 to elementsCount do (
								
								if ((maxVersion())[1] >= 10000 )then(--Max 2008+
									uv.unwrap6.selectVerticesByNode elem[e] obj;
								)else(
									uv.selectVertices elem[e];
								)	
								
								bbox[e] = compress_rotate obj uv useXaxis;
								
								if (useXaxis == true)then(
									widths[e] = bbox[e][2];
								)else(
									widths[e] = bbox[e][1];
								)
								
								if (e == 1)then(--first loop
									min_x = bbox[e][3];
									min_y = bbox[e][4];
									max_x = bbox[e][3];
									max_y = bbox[e][4];
								)else(
									min_x = amin #( min_x , bbox[e][3] );
									min_y = amin #( min_y , bbox[e][4] );
									max_x = amax #( min_x , bbox[e][3] );
									max_y = amax #( min_y , bbox[e][4] );
								)
							)
							
							--sort on their widths or heights
							sort_array = deepCopy widths;
							sort sort_array;
							
							
							local stepper;
							if (useXaxis == true)then(
								stepper = min_x;
							)else(
								stepper = max_y;--start at the top of the former selection
							)
							with redraw off(
								for i = 1 to elementsCount do(
									local idx = findItem (widths) (sort_array[elementsCount-i+1]);
									--idx = elementsCount  - idx+1;			
									if (idx > 0)then(
										local sx;
										local sy;
										if (useXaxis)then(
											--sx = stepper - bbox[idx][3];
											sx = (stepper - bbox[idx][3]) as float;
											sy = (max_y - bbox[idx][4]) as float;
											stepper+= bbox[idx][1]+(texPad as Float / texSizeW as Float);
										)else(
											sx = min_x - bbox[idx][3];
											sy = stepper - bbox[idx][4];
											stepper-=bbox[idx][2]+(texPad as Float / texSizeH as Float);
										)
										
										
										if ((maxVersion())[1] >= 10000 )then(--Max 2008+
											uv.unwrap6.selectVerticesByNode elem[idx] obj;
										)else(
											uv.selectVertices elem[idx];
										)
										
										uv.moveSelectedVertices [sx,sy, 0];
									)
								)
							)
						)
						uv.unwrap2.setTVSubObjectMode 3;
					)
				)
			)
			
			
			local uv = modPanel.getCurrentObject();
			if( classof(uv) == Unwrap_UVW)then(
				if ((maxVersion())[1] >= 10000 )then(
					local orgFaceSel = #();
					for i = 1 to selection.count do(--go through multiple objects
						orgFaceSel[i] = uv.unwrap6.getSelectedFacesByNode selection[i];
					)
					for i = 1 to selection.count do(--go through multiple objects
						uv.unwrap6.selectFacesByNode orgFaceSel[i] selection[i];--put back origininal selection
						sortRow selection[i] uv useXaxis texSizeW texSizeH texPad;
					)
				)else(
					sortRow selection[1] uv useXaxis texSizeW texSizeH texPad;--only do this with the first object in the selection
				)
			)
		)--end undo
	)
)