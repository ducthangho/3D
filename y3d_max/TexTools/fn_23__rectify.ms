fn fn_23__rectify =(
	clearListener();
	undo on(
		if (selection.count > 0)then(--at least an object selected
			local obj = selection[1]; 
			if (classOf obj.baseObject == Editable_Poly) then (--only if this 1st object is a poly at its base
			
				if (obj.modifiers[ #unwrap_uvw ] != undefined) then(
		
					local uv = obj.modifiers[ #unwrap_uvw ];
					local faces = uv.unwrap2.getSelectedFaces();
	
					
					--print("faces: "+faces.numberSet as string);
					
					
					local loops =8;
					
					
					
					try(
						with redraw off;
						
						--1.) sort the faces , and exclude any non quads
						local score_sort = #();--new array for storing the sort order
						local score_face_track = #();
						for f in faces do(--sort the faces from top left to down right
							uv.selectFaces #{f};
							uv.unwrap2.faceToVertSelect();
							vtx = uv.unwrap.getSelectedVertices();

							function getCentroidIndex uvSet vtxArray =(
								local x;
								local y;
								local cnt = 0;
								for v in vtxArray do(
									local pt = uvSet.getVertexPosition 1 v;
									if (cnt == 0)then(--the first vertex of this face
										x = pt.x;
										y = pt.y;
									)else(
										x+= pt.x;
										y+= pt.y;
									)
									cnt+=1.0;
								)
								x = (x / cnt);
								y = (y / cnt);
								--local r = ((1-y)*100000 + x*10000) as integer;--absolute sort ID
								local r = ((y)*100000 + (1-x)*10000) as integer;--absolute sort ID
								
								--print(r as string+"\tx/y/" + x as string +" / "+y as string);
								return r;
							)
							if (vtx.numberSet == 4)then(--only if it is a quad
								score_sort[score_sort.count+1] = getCentroidIndex uv vtx;
								score_face_track[score_face_track.count+1] = f;
							)
							
						)
						sort_array = deepCopy score_sort;
						sort sort_array;

						local quads = #();
						for f in sort_array do(
							local key = findItem (score_sort) (f);
							quads[quads.count+1] = score_face_track[key];
						)
						print("sort. "+quads as string);
						
						
						
						for lo = 1 to loops do(
						
							local interp = (loops-(lo-1)) as Float;--interpolate getting closer to 1:1 at the end

							for f in quads do(
								uv.selectFaces #{f};
								uv.unwrap2.faceToVertSelect();
								
								vtx = uv.unwrap.getSelectedVertices();
								
								--if (vtx.numberSet == 4)then(--only if it is a quad
									
								--determine the upper left vert
								local vtx_UL;
								local bb_x = #();
								local bb_y = #();
								local c = 0;
								for v in vtx do(
									local pt = uv.getVertexPosition 1 v;
									if (c == 0)then(--the first vertex of this face
										bb_x[1] = pt.x;
										bb_x[2] = pt.x;
										bb_y[1] = pt.y;
										bb_y[2] = pt.y;
										
									)else(
										if(pt.x < bb_x[1])then(
											bb_x[1] = pt.x;
										)
										if(pt.x > bb_x[2])then(
											bb_x[2] = pt.x;
										)
										if(pt.y < bb_y[1])then(
											bb_y[1] = pt.y;
										)
										if(pt.y > bb_y[2])then(
											bb_y[2] = pt.y;
										)
									)
									c+=1;
								)
								
								local vtx_CNTR = [(bb_x[1] + (bb_x[2] - bb_x[1])*0.5),(bb_y[1] + (bb_y[2] - bb_y[1])*0.5)];--2d coordinate
								
								c = 0;
								for v in vtx do(
									local pt = uv.getVertexPosition 1 v;
									if (c == 0)then(--the first vertex of this face
										vtx_UL = uv.getVertexPosition 1 v;--first vertex
									)else(
										if (pt.x < vtx_CNTR.x and pt.y > vtx_CNTR.y)then(
											vtx_UL = uv.getVertexPosition 1 v;--first vertex
										)
									)
									c+=1;
								)
	
	
								--sort in clockwise order
								
								
								local angles = #();--simple array
								local vtx_ids = #();
								
								local dx = vtx_UL.x - vtx_CNTR.x;
								local dy = -vtx_UL.y +vtx_CNTR.y;
								local ang_UL = mod ((atan2 dy dx)+4.0*360.0) 360.0;--the angle from the center
		
								
								for v in vtx do(
									local pt = uv.getVertexPosition 1 v;
									if (pt != vtx_UL)then(
										local dx = pt.x - vtx_CNTR.x;
										local dy = -pt.y +vtx_CNTR.y;
										local a = mod (((atan2 dy dx) - ang_UL) +4.0*360.0) 360.0;--the angle from the center
										--local d = sqrt (dx*dx+dy*dy);
	
										angles[angles.count+1]= a;-- - (360-ang_UL));--*-1.0;--invert for clockwise sort order
										vtx_ids[vtx_ids.count+1] = v;
									)
								)
								local sort_array = deepCopy angles;
								sort sort_array;
								
								
								function get2VertsAngleDiff dx dy= (
									local _a_abs = mod ((atan2 dy dx)+4*360) 360;--the angle in closed 360 degrees
									local _a_off = (mod _a_abs 90);--angle offset
									if (_a_off > 45)then(
										_a_off = -(90 - _a_off);
									)
									return _a_off;
								)
								function getVectorDistance dx dy=(--determine the distance
									return sqrt(dx*dx+dy*dy);
								)
		
		
		
								if (lo <= 1)then(
									--rotate by average
									local dist = #();
									
									local vtx_1 = (vtx_ids[ (findItem angles (sort_array[1])) ])
									local pt_1 = uv.getVertexPosition 1 vtx_1;
									local dx = pt_1.x - vtx_UL.x;
									local dy = pt_1.y - vtx_UL.y;
									local ang_a = get2VertsAngleDiff dx dy;
									dist[1] = getVectorDistance dx dy;
									
										
									local vtx_2 = (vtx_ids[ (findItem angles (sort_array[2])) ])
									local pt_2 = uv.getVertexPosition 1 vtx_2;
									local dx = pt_2.x - pt_1.x;
									local dy = pt_2.y - pt_1.y;
									local ang_b = get2VertsAngleDiff dx dy;
									dist[2] = getVectorDistance dx dy;
										
									local vtx_3 = (vtx_ids[ (findItem angles (sort_array[3])) ])
									local pt_3 = uv.getVertexPosition 1 vtx_3;
									local dx = pt_3.x - pt_2.x;
									local dy = pt_3.y - pt_2.y;
									local ang_c = get2VertsAngleDiff dx dy;
									dist[3] = getVectorDistance dx dy;
										
									local dx = vtx_UL.x - pt_3.x;
									local dy = vtx_UL.y - pt_3.y;
									local ang_d = get2VertsAngleDiff dx dy;
									dist[4] = getVectorDistance dx dy;
									
									local dist_tot = dist[1] + dist[2] + dist[3] + dist[4];
									local ang_diff = (ang_a*(dist[1]/dist_tot) + ang_b*(dist[2]/dist_tot) + ang_c*(dist[3]/dist_tot) + ang_d*(dist[4]/dist_tot))/1;

									uv.selectFaces #{f};
									uv.RotateSelected (-ang_diff * PI/180) [vtx_CNTR.x,vtx_CNTR.y,0];
								)else(
								
									
									local vtx_id = (vtx_ids[ (findItem angles (sort_array[1])) ])
									local pt_a = uv.getVertexPosition 1 vtx_id;
									local dx = pt_a.x - vtx_UL.x;
									local dy = pt_a.y - vtx_UL.y;
									local a_off = get2VertsAngleDiff dx dy;
									
									uv.unwrap2.selectFaces #{f};
									uv.RotateSelected (-a_off/interp * PI/180) [vtx_UL.x,vtx_UL.y,0]
	
									local vtx_id = (vtx_ids[ (findItem angles (sort_array[3])) ])
									local pt_b = uv.getVertexPosition 1 vtx_id;
									local dx = pt_b.x - vtx_UL.x;
									local dy = pt_b.y - vtx_UL.y;
									local a_off = get2VertsAngleDiff dx dy;
									uv.selectVertices #{vtx_id};
									uv.RotateSelectedVertices (-a_off/interp * PI/180) [vtx_UL.x,vtx_UL.y,0]
			
									--fix last vertex of the face
									uv.selectVertices #{(vtx_ids[ (findItem angles (sort_array[1]))]), (vtx_ids[ (findItem angles (sort_array[2])) ]), (vtx_ids[ (findItem angles (sort_array[3])) ])};

									local vtx_id = (vtx_ids[ (findItem angles (sort_array[2])) ])
									uv.selectVertices #{vtx_id};
									local pt = uv.getVertexPosition 1 vtx_id;
									local x = pt.x + (amax #(pt_a.x,pt_b.x) - pt.x)/interp;
									local y = pt.y + (amin #(pt_a.y,pt_b.y) - pt.y)/interp;
									--interp
									uv.SetVertexPosition 1 vtx_id [x,y,0];
								)
							)
							
						)
						uv.selectFaces faces;
					)catch(print("some error in the rectify process..."));
					
				)else(
					print("no UV editor present...");
				)
			)
		)
	)
)