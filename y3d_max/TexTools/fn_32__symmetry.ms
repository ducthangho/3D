function fn_32__symmetry =(
	--support for editable polys only for now because of 3d edge distance math stuff (polyOp methods)
	local mirror_mode = 0;--( -1 = left to right, 0 = average, +1 = right to left, will be determined in the code later on
	local uv_channel = 1;
	
	clearListener();
	if (selection.count > 0)then(--at least an object selected
		undo on(
			local obj = selection[1]; 
			
			local uv = modPanel.getCurrentObject();
			local vert_sel_a;--used for filtering out 1/2 face selection to edge mirror
			local vert_sel_b;--used for filtering out 1/2 face selection to edge mirror
			local vert_sel_c;--mirror edge selection
			
			if (classOf obj.baseObject == Editable_Poly	and  classof(uv) == Unwrap_UVW)then(

				---------{  build up the uvVerts and meshVerts relation array  }----------
				local numMapVerts = uv.unwrap.NumberVertices();--just from the base, not the current edit!!!!
				local mapVerts2Verts = for mv=1 to numMapVerts collect #();
				local numMapFaces = uv.unwrap.numberPolygons();
				for f = 1 to numMapFaces do (
					local polyFace = polyOp.getFaceVerts obj f;
					for mv=1 to (uv.unwrap.numberPointsInFace f) do (
						local mapVert = uv.unwrap.getVertexIndexFromFace f mv;--face, sub vert
						if findItem mapVerts2Verts[mapVert] polyFace[mv] == 0 do(
							append mapVerts2Verts[mapVert] polyFace[mv];
						)
					)
				)
				---------------------------------------------------------
				
				---------{  select mirror edge  }----------
				case uv.gettvsubobjectmode() of(
					2: (
						if ( (uv.unwrap2.getSelectedEdges()).numberset == 1)then(
							uv.unwrap2.uvEdgeSelect();
						)
						uv.edgeToVertSelect();
					)
					3: (uv.faceToVertSelect())
				)
				--convert 1/2 face selection (converted to verts now) to the symmetry edge selection as vertex
				vert_sel_a = uv.unwrap.getSelectedVertices();
				uv.unwrap2.setTVSubObjectMode 1;
				uv.unwrap.contractSelection();
				vert_sel_b = uv.unwrap.getSelectedVertices();
				uv.unwrap.selectVertices (vert_sel_a - vert_sel_b);
				--convert to edge selection
				case uv.gettvsubobjectmode() of(
					1: (uv.verttoedgeselect())
					3: (uv.facetoedgeselect())
				)
				vert_sel_c = uv.unwrap2.getSelectedEdges();
				---------{  select mirror edge  }----------

				
				
				
				

				---------{  loop through the edges and offset rotate the element to the x-axis  }----------
				uv.unwrap2.setTVSubObjectMode 2;
				
				local angle_offsets = 0;
				for e in vert_sel_c do(
					uv.unwrap2.selectEdges #{e};--select single sub edge
					uv.edgeToVertSelect();
					local vts = uv.unwrap.getSelectedVertices() as array;
					local ptA = uv.getVertexPosition uv_channel vts[1];
					local ptB = uv.getVertexPosition uv_channel vts[2];
					
					local dx = ptA.x - ptB.x;
					local dy = ptA.y - ptB.y;
					local _a_abs = mod ((atan2 dy dx)+4*360) 360;--the angle in closed 360 degrees
					local _a_off = (mod _a_abs 90);--angle offset
					if (_a_off > 45)then(
						_a_off = -(90 - _a_off);
					)
					angle_offsets+= (_a_off);
				)
				angle_offsets = angle_offsets / vert_sel_c.numberset;
				uv.unwrap2.selectEdges vert_sel_c;
				uv.unwrap2.selectElement();
				uv.unwrap2.rotateSelectedCenter (-angle_offsets * PI/180);
				angle_offsets = 0.0;
				for e in vert_sel_c do(
					uv.unwrap2.selectEdges #{e};--select single sub edge
					uv.edgeToVertSelect();
					local vts = uv.unwrap.getSelectedVertices() as array;
					local ptA = uv.getVertexPosition uv_channel vts[1];
					local ptB = uv.getVertexPosition uv_channel vts[2];
					local dx = ptA.x - ptB.x;
					local dy = ptA.y - ptB.y;
					local _a_abs = mod ((atan2 dy dx)+8*360) 180;--the angle in closed 360 degrees
					if (_a_abs < 45 )then(
						_a_abs+=180;
					)
					angle_offsets+= _a_abs;
				)
				angle_offsets = (angle_offsets / vert_sel_c.numberset) as float;
				if (angle_offsets > (90+45))then(--is aligned horizontal,- but needs to be vertical!!!
					uv.unwrap2.selectElement();
					uv.unwrap2.rotateSelectedCenter (-90 * PI/180);
				)
				---------{  loop through the edges and offset rotate the element to the x-axis  }----------

				
				
				
				---------{  straighten the mirror edge  }----------
				uv.unwrap2.selectEdges vert_sel_c;
				uv.edgeToVertSelect();
				local vts = uv.unwrap.getSelectedVertices();
				local x_max = 0;
				local x_min = 9999.0;
				for v in vts do(
					local x = (uv.getVertexPosition uv_channel v).x;
					if (x_min == 9999)then(
						x_min = x;
						x_max = x;
					)else(
						x_min =amin #(x_min, x);
						x_max =amax #(x_min, x);
					)
				)
				local x_mirror_pos = (x_min+(x_max - x_min)/2);
				for v in vts do(
					uv.unwrap.selectVertices #{v};
					uv.moveX x_mirror_pos;
				)
				--check which side or mode (i.e half face selection, or just edge selection)
				local avg_min = x_mirror_pos;
				local avg_max = x_mirror_pos;
				for v in vert_sel_a do(
					local pt = uv.getVertexPosition uv_channel v;
					avg_min = amin #(pt.x, avg_min);
					avg_max = amax #(pt.x, avg_max);
				)
				avg_min = x_mirror_pos - avg_min;
				avg_max = avg_max - x_mirror_pos;
				if ( avg_min > avg_max )then(
					mirror_mode = -1;
				)else if( avg_min < avg_max )then(
					mirror_mode = 1;
				)else(
					mirror_mode = 0;
				)
				---------{  straighten the mirror edge  }----------

				
				
				---------{  mirror parts experiment  }----------
				uv.unwrap2.selectEdges vert_sel_c;--select mirror axis edges
				uv.unwrap2.setTVSubObjectMode 1;
				uv.edgeToVertSelect();

				local flt_a = uv.unwrap.getSelectedVertices();uv.unwrap2.selectElement();--just the mirror axis flow
				local flt_b = uv.unwrap.getSelectedVertices();--the whole element
				local flt_c = copy flt_a;--the peeling count array

				local t = (flt_a as array)[1];--one vert on the mirror axis to compare distances towards
				mirror_cntVtx = polyOp.getVert obj (mapVerts2Verts[t])[1];

				
				function fn_grow_selection _uv _sel _btShell _btPeel =(--grow the selection, substract and interesect shell
					_uv.unwrap.selectVertices #{_sel};--just one side
					_uv.unwrap.expandSelection();--UV view expanding
					local bt_limit = _uv.unwrap.getSelectedVertices();
					_uv.unwrap.selectVertices #{_sel};--just one side
					_uv.unwrap5.expandGeomVertexSelection();--3d view expanding
					local expanded = (_uv.unwrap.getSelectedVertices() * (_btShell-_btPeel)* bt_limit )  - #{_sel};--intersecting B - A			
					return expanded;
				)
				
				function fn_sort_mirror_verts	_mapVerts2Verts	_obj	_uv	_mirror_cntVtx		_sideL _sideR=(
					local ret_pairs = #();
					if (_sideL.numberset == _sideR.numberset)then(
						local sort_arrays = #();
						
						local i_count = 1;
						for i in _sideL do(
							sort_arrays[ sort_arrays.count+1 ] = #();--push the distances value in here
							
							local a_v = _mapVerts2Verts[i][1];--side Left Vert XYZ
							local a_p = polyOp.getVert _obj a_v;
							local a_dis = distance a_p _mirror_cntVtx;
							local j_count = 1;
							for j in _sideR do(
								local b_v = _mapVerts2Verts[j][1];--side Right Vert XYZ
								local b_p = polyOp.getVert _obj b_v;
								local b_dis = distance b_p _mirror_cntVtx;
								
								local diff = abs(b_dis -  a_dis);--otherwise it seems to be a bit unpreciese
								sort_arrays[i_count][j_count] = diff;
								j_count+=1;
							)
							i_count+=1;
						)
						local sort_arrays_copy = deepCopy sort_arrays;
						for i=1 to sort_arrays.count do(
							sort sort_arrays_copy[i];
							--print("sort. "+sort_arrays_copy[i] as string);
						)
						for i=1 to sort_arrays.count do(
							a = (_sideL as array)[i];
							local id = findItem (sort_arrays[i]) (sort_arrays_copy[i][1]);--find the closest match
							b = (_sideR as array)[id];
							ret_pairs[ret_pairs.count+1] = #(a,b);
						)
					)	
					return ret_pairs;	
				)
				
				
				
				
				
				function fn_do_mirror_tVerts _uv _ch _mirror_mode	x_mirror_pos _vertL _vertR =(
					--right to left
					case _mirror_mode of(
						(-1): (
							--Left to Right (left face selection)
							local pos = _uv.getVertexPosition _ch _vertL;
							_uv.unwrap.selectVertices #{_vertR};
							_uv.moveX (x_mirror_pos + (x_mirror_pos - pos.x));
							_uv.moveY pos.y;
						)
						0: (
							--average (booth sides)
							local posR = _uv.getVertexPosition _ch _vertR;
							local posL = _uv.getVertexPosition _ch _vertL;
							local avgrX = ((x_mirror_pos - posL.x) + (posR.x - x_mirror_pos ))/2;
							local avgrY = (posL.y + posR.y)/2;
							
							_uv.unwrap.selectVertices #{_vertL};
							_uv.moveX (x_mirror_pos - avgrX);
							_uv.moveY avgrY;
							_uv.unwrap.selectVertices #{_vertR};
							_uv.moveX (x_mirror_pos + avgrX);
							_uv.moveY avgrY;
						)
						1: (
							--Right to Left (right face selection)
							local pos = _uv.getVertexPosition _ch _vertR;
							_uv.unwrap.selectVertices #{_vertL};
							_uv.moveX (x_mirror_pos + (x_mirror_pos - pos.x));
							_uv.moveY pos.y;
						)
					)
				)
				
				
				
				
				-- create starting peel vertArray set
				local last_pairs = #();--empty array
				for v in flt_a do(
					local grow = fn_grow_selection uv v flt_b flt_c;--grow this selection;
					if ( grow.numberset == 2)then(
						flt_c= flt_c+ grow;
						grow = grow as array;		
						local p1 = uv.getVertexPosition uv_channel grow[1];
						local p2 = uv.getVertexPosition uv_channel grow[2];
						if (p1.x > p2.x)then(
							fn_do_mirror_tVerts  uv uv_channel mirror_mode	x_mirror_pos grow[2] grow[1];
							last_pairs[last_pairs.count+1] = #(grow[2],grow[1]);--2d array, hold in each container a pair of 2 verts, left > right
						)else(
							fn_do_mirror_tVerts  uv uv_channel mirror_mode	x_mirror_pos grow[1] grow[2];
							last_pairs[last_pairs.count+1] = #(grow[1],grow[2]);
						)
					)else if (grow.numberset > 2)then(
						--not yet solved
					)
				)
				
				local loop_count = 0;
				local while_exit = false;
				while (loop_count < (flt_b.numberset/2) and while_exit == false)do(
					if (flt_c.numberset < flt_b.numberset)then(--there is still place to expand/ grow
						
						local peel_pairs = copy last_pairs #nomap;
						last_pairs = #();--empty the array and fill it up in the loops with new peel pairs
						
						if ( peel_pairs.count == 0)then(
							while_exit = true;--nothing left
						)else(
							
							for pair in peel_pairs do(
								
								
								local grow_L = fn_grow_selection uv pair[1] flt_b flt_c;--grow this selection on the Left
								local grow_R = fn_grow_selection uv pair[2] flt_b flt_c;--grow this selection in the Right

								if (grow_L.numberset > 0 and grow_R.numberset > 0)then(--continue only if booth sides have the same amount of grown verts (symmetrical UV faces + geometry check)
									concat_pair = fn_sort_mirror_verts 	mapVerts2Verts	obj	uv	mirror_cntVtx		grow_L	grow_R;
									
									for m in concat_pair do(--for the pairs we found, do the mirror
										fn_do_mirror_tVerts  uv uv_channel mirror_mode	x_mirror_pos m[1] m[2];
									)
									
									local b = #{};--temporary bitarray, converts the concatenate pair to bitarray selection
									for p in concat_pair do(
										b=b+(p as bitArray);
									)
									flt_c = flt_c+b;--extend the substraction filter
									
									last_pairs = last_pairs+ concat_pair;--update the last pairs array with the current results, so we can build upon them in the next loop (i.e grow selection,...)
								)
							)	
						)
					)
					loop_count+=1;
				)
				uv.unwrap2.setTVSubObjectMode 2;
				uv.unwrap2.selectEdges vert_sel_c;--select the mirror axis,- also as visial feedback on what was mirrored
				
				local a = flt_c.numberset;
				local b = flt_b.numberset;
				format "\nSymmetry--> done.@ (% / %) Tverts ( % pct.)\n"a b ((a/b*100) as integer) ;
			)
		)
	)
)
--fn_32__symmetry();