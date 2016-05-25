fn fn_33__peel_pipe = (

	clearListener();
	/*
	* split the mesh by smoothing group elements intersected by mesh elements
	* so that hard edges of the mesh result into UV seams
	* works only with max 2009+
	*/
	local uv_channel = 1;
		
	if (selection.count > 0 and (maxVersion())[1] >= 11000 )then(--at least an object selected and only with max 2009
		local obj = selection[1]; 
		local uv = modPanel.getCurrentObject();
		if (classOf obj.baseObject == Editable_Poly	and  classof(uv) == Unwrap_UVW)then(

			format "continue. \n";
			--get a vert Loop
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
			---------------------------------------------------
			
			
			if (uv.gettvsubobjectmode() == 2)then(
				
				if ( (uv.unwrap2.getSelectedEdges()).numberset == 1)then(
					uv.unwrap2.uvEdgeSelect();
				)
				
				/*uv.edgeToVertSelect();
			
				vert_sel_a = uv.unwrap.getSelectedVertices();
				uv.unwrap2.setTVSubObjectMode 1;
				
				
				--local stdArray_A = bitArray_A as array;
				
				
				local edgeVertArray = #();
				--sort the edge order
				for e in bitArray_A do(
					uv.unwrap2.selectEdges #{e};
					uv.edgeToVertSelect();
					edgeVertArray[edgeVertArray.count+1] = uv.unwrap.getSelectedVertices();
				
					--polyop.getEdgeVerts <Poly poly> <int edge>
					--Returns the edge's vertices as a 2 element array.
					--format "edge -. % , verts = % \n" e vert_sel_a;
					--uv.unwrap2.setTVSubObjectMode 1;
				)
				
				
				---
				local edge_rings = #();
				for e in bitArray_A do(
					uv.unwrap2.selectEdges #{e};
					uv.unwrap5.geomEdgeRingSelection ();
					local a = edge_rings[edge_rings.count+1] = floor(((uv.unwrap2.getSelectedEdges()).numberset )/2) as integer;
				
					format "e -.  ring steps = % \n" a;
					--obj.EditablePoly.setRingShift 1 true false;
				)*/
				
				local bitArray_A = uv.unwrap2.getSelectedEdges();--the whole edge loop
				
				uv.unwrap2.selectEdges bitArray_A;
				uv.unwrap.getSelectedVertices();
				uv.edgeToVertSelect();
				
				local bitArray_B = uv.unwrap.getSelectedVertices();
				
				local bitArray_C = #{};
				for v in bitArray_B do(
					bitArray_C+= #{ (mapVerts2Verts[v])[1] };
				)
				--polyOp.getVert obj (mapVerts2Verts[t])[1];
				
				
				
				modPanel.addModToSelection (Edit_Poly ()) ui:on;
				--deleteModifier $ 1;--delete the top modifier	
				
				
				
				print("PolyVert selection "+bitArray_C as string);
				
				subobjectLevel = 1;
				ePoly = obj.modifiers[#Edit_Poly];
				ePoly.SetSelection #Vertex #{}
				ePoly.Select #Vertex bitArray_C;
				ePoly.ConvertSelectionToBorder #Vertex #Edge;
				subobjectLevel = 2;
				local bitArray_D = ePoly.GetSelection #Edge; 

				format "poly edge sel. %\n" bitArray_D.numberset;
				
				local msh = snapshot obj;



					
					
				local bitArray_E = #{}; 
				local splinePos = #();--the spline positions (unsorted)
					
				for e in 	bitArray_D do(
					local edgeRing = #{e};
					
					ePoly.SetSelection #Edge #{};
					ePoly.Select #Edge #{e};
					ePoly.ButtonOp #SelectEdgeRing;
					
				
					local num = floor ( (ePoly.GetSelection #Edge).numberset /3) as integer;
					--format "ring steps + - . % \n" num;
					
					ePoly.SetSelection #Edge #{};
					ePoly.Select #Edge #{e};
					ePoly.RingSelect (num*1) true false;
					edgeRing+=ePoly.GetSelection #Edge;
					
					ePoly.SetSelection #Edge #{};
					ePoly.Select #Edge #{e};
					ePoly.RingSelect (num*-1) true false;
					edgeRing+=ePoly.GetSelection #Edge;
					
					------
					--ePoly.SetSelection #Edge #{};
					--ePoly.Select #Edge edgeRing;
					avg = [0,0,0];--point 3d
					local cnt = 0;
					for ed in edgeRing do(
						/*local v1 = ePoly.GetVertex (ePoly.GetEdgeVertex ed 1);
						local v2 = ePoly.GetVertex (ePoly.GetEdgeVertex ed 2);
						*/
						local v1 = getVert msh (ePoly.GetEdgeVertex ed 1);
						local v2 = getVert msh (ePoly.GetEdgeVertex ed 2);
						
						--local v = v1 + v2;
						local v = v1;
						v.x+=v2.x;
						v.y+=v2.y;
						v.z+=v2.z;
						
						v.x/=2;
						v.y/=2;
						v.z/=2;

						avg.x+=v.x;
						avg.y+=v.y;
						avg.z+=v.z;
						
						cnt+=1;
					)	
					avg.x/=cnt;
					avg.y/=cnt;
					avg.z/=cnt;
					
					
					
					--print("...avg pos. "+avg as string+" num: "+edgeRing.numberset as string);
					splinePos[splinePos.count+1] = avg;
					bitArray_E+= edgeRing;
				)
				
				deleteModifier obj 1;--delete the top modifier	
				uv.unwrap2.setTVSubObjectMode 2;
				if (splinePos.count >= 2)then(
					local spline = SplineShape pos:splinePos[1];
					addNewSpline spline;
					for pos in splinePos do(
						addKnot spline 1 #corner #line pos;
					)
					updateShape spline;
					delete msh;
					
					
					
					uv.unwrap.edit();
					subobjectLevel = 2;
					uv.unwrap2.setTVSubObjectMode 2;
					uv.unwrap2.selectElement();
					uv.unwrap2.edgeToFaceSelect();
					uv.unwrap2.setTVSubObjectMode 3;
					---apply spline mapping
					uv.splinemap_node = spline;
					uv.unwrap6.splineMap();--only max 2009+
					
					delete spline;
				)
			)
		)
	)else(
		format "you need 3dsmax 2009+ for this feature because it builds ontop of the Spline Mapping feature\n";
	)
)