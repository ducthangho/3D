fn fn_10__stack_shells noScaleMode= (
	undo on(
		if selection.count == 1 then (
			local obj = selection[ 1 ];
			local numFacesSelected = ( obj.modifiers[#unwrap_uvw].unwrap.getSelectedPolygons() ).numberSet;
					
			if (classOf obj.baseObject == Editable_Poly) and (classof (modPanel.getCurrentObject()) == Unwrap_UVW and numFacesSelected >0) then (
				local uv = obj.modifiers[ #unwrap_uvw ];
				obj.modifiers[ #unwrap_uvw ].unwrap2.faceToVertSelect();
				subObjectLevel = 1;
				--make out the selected shells
				local numVerts = uv.getSelectedVertices();
				local vertElemArray = #();
				for v in numVerts do (
					vertElemArray[ v ] = 0;
				)
				local elements = 0;
				local elemVerts = #{};

				local elem = #();

				with redraw off
				for v in numVerts do (
					-- If vertex has no element assigned yet, create a new element.
					if vertElemArray[ v ] == 0 then (
						uv.selectVertices #{ v }
						uv.selectElement()
						elemVerts = uv.getSelectedVertices();
						-- Ignore elements with less than 3 UV vertices.
						if ( elemVerts as array ).count > 2 then (
							elements += 1
							elem[elements] = elemVerts;
							for i in elemVerts do (
								vertElemArray[ i ] = elements -- Mark these vertices with their element number in vertElemArray.
							)
						)
					)
				)
				--
				if ( elements > 1)then(--only continue if more than one elements are catched

					
					function getCenter vertsBitArray =(
						local _array = #();
						--function to calculate the most center vert of the shell
						local uv = selection[1].modifiers[ #unwrap_uvw ];
						local cnt_x;
						local cnt_y;

						local cnt = 0;
						for i in vertsBitArray do (
							local pt = uv.getVertexPosition 1 i;
							
							if (cnt == 0)then(
								cnt_x = pt.x;
								cnt_y = pt.y;
								cnt+=1;
							)else(
								cnt_x+= pt.x;
								cnt_y+= pt.y;
							)
						)
						cnt_x = cnt_x / vertsBitArray.numberSet;
						cnt_y = cnt_y / vertsBitArray.numberSet;
						---
						local d_max;
						local d_min;
						cnt = 0;
						for i in vertsBitArray do (
							local pt = uv.getVertexPosition 1 i;
							local dx = pt.x - cnt_x;
							local dy = pt.y - cnt_y;
							local d = sqrt(dx*dx + dy*dy);
							local a = (atan2 dy dx);
							if (cnt == 0)then(
								d_max=[i,d,a];
								d_min=[i,d,a];
								cnt+=1;
							)else(
								if (d < d_min.y)then(
									d_min=[i,d,a];
								)
								if (d > d_max.y)then(
									d_max=[i,d,a];
								)
							)
						)
						
						_array[1] = [cnt_x,cnt_y];--center
						_array[2] = d_min;--d_min
						_array[3] = d_max;--d_max
						
						return _array;
					)
					
					
					--------------------------
					print("shells\t\t"+elements  as string);
					local diff_trans = #();
					local ct = 1;
					local vert_check;
					local _continue = true;
					for e = 1 to elements do (
						diff_trans[e]= getCenter(elem[e]);--array holding the attributes
						/*
						-- check if the shells have a equal amount of verts
						if (ct ==1)then(
							vert_check = elem[e].numberSet;
						)else(
							if ( vert_check != elem[e].numberSet)then(
								_continue = false;
								break;
							)
						)*/
						--print("chec elm: "+e as string+" /vts "+elem[e].numberSet as string);
						ct+=1;
					)
					---------------------------------------
					if (_continue)then(
						local dx = (uv.getVertexPosition 1 (diff_trans[1][3].x)).x - (uv.getVertexPosition 1 (diff_trans[1][2].x)).x;
						local dy = (uv.getVertexPosition 1 (diff_trans[1][3].x)).y - (uv.getVertexPosition 1 (diff_trans[1][2].x)).y;
						local base_a = (atan2 dy dx);
						local base_d = sqrt(dx*dx + dy*dy);
							
						for i = 2 to elements do (--base shell is the first one
							dx = (uv.getVertexPosition 1 (diff_trans[i][3].x)).x - (uv.getVertexPosition 1 (diff_trans[i][2].x)).x;
							dy = (uv.getVertexPosition 1 (diff_trans[i][3].x)).y - (uv.getVertexPosition 1 (diff_trans[i][2].x)).y;
							
							local trans_a = base_a - (atan2 dy dx);
							uv.selectVertices elem[i];
							uv.rotateSelectedVerticesCenter (trans_a* PI/180);
								
							if (not noScaleMode )then(
								local trans_s =  base_d / sqrt(dx*dx + dy*dy);
								print("scale offset: "+trans_s as string);
								for v in elem[i] do(
									--scale the verts on the bounding box center because scaleSelectedVertices is broken
									local pt = uv.getVertexPosition 1 v;
									dx =  pt.x - diff_trans[i][1].x;
									dy =  pt.y - diff_trans[i][1].y;
									local a = (atan2 dy dx);
									local d = sqrt(dx*dx + dy*dy) * trans_s;
									uv.SetVertexPosition 1 v [diff_trans[i][1].x+d*(cos a),diff_trans[i][1].y+d*(sin a),0];
								)
							)
							
							local trans_x = (uv.getVertexPosition 1 (diff_trans[1][2].x)).x - (uv.getVertexPosition 1 (diff_trans[i][2].x)).x;
							local trans_y = (uv.getVertexPosition 1 (diff_trans[1][2].x)).y - (uv.getVertexPosition 1 (diff_trans[i][2].x)).y;
							uv.moveSelectedVertices [trans_x,trans_y,0];
						)
					)
				)	
				subObjectLevel = 3;
			)
		)
	)
)