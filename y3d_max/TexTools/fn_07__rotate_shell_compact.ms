fn fn_07__rotate_shell_compact =(
	undo on(
		if classof (modPanel.getCurrentObject()) == Unwrap_UVW then(
			local obj = selection[1];
			
			local numFacesSelected = ( obj.modifiers[#unwrap_uvw].unwrap.getSelectedPolygons() ).numberSet;
			
			if (numFacesSelected > 0)then(
				
				print("num faces . "+numFacesSelected as string);
				
				function getBBox =(
					local uv1 = selection[1].modifiers[#unwrap_uvw].unwrap;
					local _verts = uv1.getSelectedVertices() as array;
					local minx;
					local maxx;
					local miny;
					local maxy;
					local cnt = 0;
					for i in _verts do (
						local _pt = uv1.getVertexPosition 1 i;
						
						if (cnt == 0)then(
							minx = _pt.x;
							miny = _pt.y;
							maxx = _pt.x;
							maxy = _pt.y;
							cnt+=1;
						)else(
							minx = amin #(_pt.x, minx);
							miny = amin #(_pt.y, miny);
							maxx = amax #(_pt.x, maxx);
							maxy = amax #(_pt.y, maxy);
						)
					)
					return #((maxx - minx),(maxy - miny));
				)
				
				--apply on all sub elements
				local uv1 = obj.modifiers[#unwrap_uvw].unwrap;
				local uv2 = obj.modifiers[#unwrap_uvw].unwrap2;
				uv2.faceToVertSelect();

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
					bbox = getBBox();
					dia_old = amin #( bbox[1] , bbox[2] );
					bestAngle = 0;
					
					with redraw off(
						
						for n = 1 to (sweepSteps-1) do (--rotate sweepRange
							uv1.rotateSelectedVerticesCenter (sweepAngle* PI/180);
							
							bbox = getBBox();
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
				bbox = getBBox();
				if (bbox[2] > bbox[1])then(
					uv1.rotateSelectedVerticesCenter (90* PI/180);
				)
			)
		)
	)
)