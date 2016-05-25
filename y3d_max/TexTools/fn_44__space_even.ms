function fn_44__space_even= (
	clearlistener();
	
	
	
	if (selection.count > 0)then(--at least an object selected
		undo on(
			local obj = selection[1]; 
			local uv = modPanel.getCurrentObject();	
			if (classof(uv) == Unwrap_UVW)then(
				local _mode = uv.unwrap2.getTVSubObjectMode();
				if (_mode == 3)then(--align shells
					
					uv.unwrap2.selectElement();	
					uv.unwrap2.faceToVertSelect();
					local original_vertex_selection = uv.unwrap.getSelectedVertices();
					local numVertexSelected = #{1..(polyop.getNumMapVerts obj 1)};
					
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
							uv.selectVertices #{ v }
							uv.unwrap2.selectElement();
							elemVerts = uv.unwrap.getselectedvertices() as array;
							if elemVerts.count > 2 then (-- Ignore elements with less than 3 UV vertices.
								elementsCount += 1;
								elem[elementsCount] = uv.unwrap.getselectedvertices();--elemVerts;
								for i in elemVerts do (
									vertElemArray[ i ] = elementsCount; -- Mark these vertices with their element number in vertElemArray.
								)
							)
						)
					)
					
					if (elem.count > 2)then(--only if there are more than 2 shells
						
						local centroids = #();--get centroids
						local bboxes = #();
						
						local totalShellsW = 0;
						local totalShellsH = 0;
						
						
						for e in elem do(
							uv.selectVertices e;--select the element as tVerts
							local vts = uv.unwrap.getselectedvertices();
							
							local first = true;
							local minX;
							local maxX;
							local minY;
							local maxY;
							
							cntr = [0, 0, 0];
							
							for v in vts do(
								local pt = uv.unwrap6.GetVertexPositionByNode 1 v obj;
								if (first)then(
									minX = pt.x;
									maxX = pt.x;
									minY = pt.y;
									maxY = pt.y;
									first = false;
								)else(
									minX = amin #(minX,pt.x);
									maxX = amax #(maxX,pt.x);
									minY = amin #(minY,pt.y);
									maxY = amax #(maxY,pt.y);
								)
								cntr.x+=pt.x;
								cntr.y+=pt.y;
								cntr.z+=pt.z;
							)
							cntr.x/=vts.numberSet;
							cntr.y/=vts.numberSet;
							cntr.z/=vts.numberSet;
							
							append centroids cntr;
							append bboxes #(minX,minY,maxX-minX,maxY-minY);--append centroid to shell centroids array
							
							totalShellsW=totalShellsW+(maxX-minX);
							totalShellsH=totalShellsH+(maxY-minY);
						)
						
						--determine if it is a vertical or horizontal flow
						local minX = centroids[1].x;
						local maxX = centroids[1].x;
						local minY = centroids[1].y;
						local maxY = centroids[1].y;
						local centroidSortY = #();
						local centroidSortX = #();
						
						for pt in centroids do(
							minX = amin #(minX,pt.x);
							maxX = amax #(maxX,pt.x);
							minY = amin #(minY,pt.y);
							maxY = amax #(maxY,pt.y);
							
							append centroidSortX pt.x;--sorting array for later so we know which shell comes first, later,...
							append centroidSortY pt.y;
						)
						
						local dx = maxX - minX;
						local dy = maxY - minY;
						
						local sortX = deepCopy centroidSortX;
						local sortY = deepCopy centroidSortY;
						sort sortX;
						sort sortY;
						
						
						print("flow U or V? "+dy as string+", "+dx as string+", // > "+totalShellsH as string);
						
						if (dy > dx)then(--vertical flow
							
							local idxA = findItem centroidSortY (sortY[1]);--first shell
							local idxB = findItem centroidSortY (sortY[sortY.count]);--last shell in this flow
							
							local yA  = bboxes[idxA][2];--minY
							local yB  = bboxes[idxB][2]+ bboxes[idxB][4];--minY + height
							
							local stepY = yA;
							local step = ((yB - yA)-totalShellsH) / (elem.count-1);-- / (elem.count -1);
		
							for i=1 to elem.count do(
								local idx = findItem centroidSortY (sortY[i]);
								local e = elem[idx];
								
								local shft = stepY - bboxes[idx][2];
								uv.selectVertices e;--select the element as tVerts
								uv.unwrap2.moveSelected [0,shft,0];

								stepY+= (bboxes[idx][4]+ step);		
							)
						)else(
							local idxA = findItem centroidSortX (sortX[1]);--first shell
							local idxB = findItem centroidSortX (sortX[sortX.count]);--last shell in this flow
							
							local xA  = bboxes[idxA][1];--minY
							local xB  = bboxes[idxB][1]+ bboxes[idxB][3];--minY + height
							
							local stepX = xA;
							local step = ((xB - xA)-totalShellsW) / (elem.count-1);-- / (elem.count -1);
		
							for i=1 to elem.count do(
								local idx = findItem centroidSortX (sortX[i]);
								local e = elem[idx];
								
								local shft = stepX - bboxes[idx][1];
								uv.selectVertices e;--select the element as tVerts
								uv.unwrap2.moveSelected [shft,0,0];

								stepX+= (bboxes[idx][3]+ step);		
							)
						)
						
						
						
					)	
					
					
					--format "min//max, %,%,\n" dx dy;
					
					
					
					uv.selectVertices original_vertex_selection;
					uv.unwrap2.setTVSubObjectMode 3;--back to face selection
					
				)else(--align verts/ edges
					if (_mode == 2)then(
						uv.unwrap2.edgeToVertSelect();
					)
				)				
			)
		)
	)
	
	print("space evenly ");
)

--fn_44__space_even();