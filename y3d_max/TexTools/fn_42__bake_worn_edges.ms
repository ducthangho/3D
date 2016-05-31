function fn_42__bake_worn_edges _sizeW _sizeH _padding _channelNr _contrast _power= (
	clearListener();
	local btmp = bitmap _sizeW _sizeH;
	
	
	if (selection.count > 0)then(
		local contrast = _contrast;
		local power = _power;
		local inp = selection[1];
		
		local obj = convertTo (copy inp) editable_poly;--make a clone that will be deleted later, so that we don't touch the original
		
		if(classOf obj == Editable_Poly)do ( --cancel if wrong object is selected
		
		---local tmesh = copy obj;
		--convertToMesh tmesh;
			
			
			------------ CACHED FUNCTIONS ---------------
			local OP_getNumFaces = polyOp.getNumFaces
			local OP_getNumVerts = polyOp.getNumVerts
			local OP_getNumEdges = polyOp.getNumEdges
			local OP_getFaceArea = polyOp.getFaceArea	
			local OP_getFacesUsingEdge = polyOp.getFacesUsingEdge
			local OP_getFaceNormal = polyOp.getFaceNormal
			local OP_getVertsUsingEdge = polyOp.getVertsUsingEdge
			local OP_getVert = polyOp.getVert
			local OP_getFaceCenter = polyOp.getFaceCenter
			local OP_getFaceArea = polyOp.getFaceArea
			local OP_getEdgesUsingVert = polyOp.getEdgesUsingVert
			local OP_setVertColor = polyOp.setVertColor
				
			if contrast == undefined do( contrast = 1 )
			if power == undefined do( power = 1 )

			local model = obj.baseobject

			local TotalPolys = OP_getNumFaces model
			local TotalVerts = OP_getNumVerts model
			local TotalEdges = OP_getNumEdges model
				
			local nVerts = #()
			local nFaces = #()

			local pSmallest = 10000.0;--power smallest
			local pLargest = 0.0;--power largest
			local pTotalArea = 0.0;--power total area
			
			for i=1 to TotalPolys do(--sort out the biggest and smallest face (range wise)
				pArea = OP_getFaceArea model i
				if pArea > pLargest do(	pLargest = pArea )
				if pArea < pSmallest do( pSmallest = pArea )
			)

			Edge_Angle_array = #();

			for i=1 to TotalEdges do(--loop through all edges
				nFaces = (OP_getFacesUsingEdge model i) as array;--Get Neighboring Faces
				local theAngle = 0;
			
				if (nFaces.count == 2) then(
					theAngle = acos(dot (normalize (OP_getFaceNormal model nFaces[1]) ) (normalize (OP_getFaceNormal model nFaces[2])))
						
					if( theAngle != 0)do(--Get Edge Normal
						local normal = ((OP_getFaceNormal model nFaces[1])+(OP_getFaceNormal model nFaces[2])) / 2;
				
						nVerts = (OP_getVertsUsingEdge model i) as array
						local edgeCenter = (OP_getVert model nVerts[1] + OP_getVert model nVerts[2])/2;--3d point position of the center of that edge
						local n1Center = OP_getFaceCenter model nFaces[1]
						local n2Center = OP_getFaceCenter model nFaces[2]
			
						local avgDistance = (( distance edgeCenter n1Center) +( distance edgeCenter n2Center )) / 2;
						local edgeCenterNew = edgeCenter + (normal * .001);
						local avgDistanceNew = (( distance edgeCenterNew n1Center ) + ( distance edgeCenterNew n2Center )) / 2;
						
						if (avgDistanceNew < avgDistance) do(--reverse angle
							theAngle =  -theAngle;
						)

						local pAreaMultiply = pLargest - pSmallest
			
						-- is this the average of all faces, or just the two in question? 	the following call is redundant! (x+x)/2 = x
						--avgArea = ((polyOp.getFaceArea model nFaces[1]) + (polyOp.getFaceArea model nFaces[1]))/2
						local avgArea = ((OP_getFaceArea model nFaces[1]) + (OP_getFaceArea model nFaces[2])) / 2;
						local areaPower = 1 - ((avgArea - pSmallest) / pAreaMultiply);
												
						areaPower = areaPower^power;
						theAngle = theAngle * areaPower * contrast;
					)
				) 
				
				if (theAngle as string) == "-1.#IND" do(--infinite
					theAngle = 0;
				)
				theAngle += 180
				append Edge_Angle_array theAngle;--append this final angle to the edge angle array
			)

			--testoutput1L.text = "Assigning vertex colors"
			local angleMultiply = (255.0/360.0) as float
			
			for i=1 to TotalVerts do( 	
				local theAngle = 0;
				local nEdges = (OP_getEdgesUsingVert model i) as array;--Get Neighboring Verts
			
				
				for j=1 to nEdges.count do(--collect the edge angles that are connected with this vert
					theAngle += Edge_Angle_array[nEdges[j]];
				)
				theAngle = theAngle / nEdges.count;--get average value
				
				local vertAlpha = (angleMultiply*theAngle) as Integer;
			
				if vertAlpha > 250 do(--clamp alpha color values between 250 and 5
					vertAlpha = 250;
				)
				if vertAlpha < 5 do(
					vertAlpha = 5;
				)
				local vertColor = [vertAlpha, vertAlpha, vertAlpha] as color;
				OP_setVertColor model 3 i vertColor ;--assign computed vertex color;
			)
			
			
			
			--setup gradient material to be baked
			
			local mat = Standardmaterial ();
			mat.diffuseMap = Gradient_Ramp ();
			mat.diffuseMap.Gradient_Type = 4;--linear
			mat.diffuseMap.Coordinates.mapChannel = 3;--vertex color
			mat.diffuseMap.Gradient_Ramp.flag__1.color = [0,0,0];
			mat.diffuseMap.Gradient_Ramp.flag__3.color = [0,0,0];--the middle one
			mat.diffuseMap.Gradient_Ramp.flag__2.color = [255,255,255];
			mat.diffuseMap.Gradient_Ramp.flag__3.position = 64;		
			obj.material = mat;		
					
					
			obj.removeAllBakeElements();
			local url_save_path = GetDir #image + "\\_renderToClipboard.bmp"
			local dif_bake = DiffuseMap();
			dif_bake.outputSzX    =    _sizeW;
			dif_bake.outputSzY    =    _sizeH;
			dif_bake.autoSzOn     = false;
			dif_bake.filterOn	 = true;
			
			dif_bake.elementName  = "DiffuseMap";
			dif_bake.enabled      = true;
			
			dif_bake.fileName = url_save_path;--just in case for 64-bit machines
			dif_bake.fileType = url_save_path;--appearently the target filename/path
			
			-- Baking job
			bake = obj.INodeBakeProperties;
			bake.bakeChannel = _channelNr;--_channelNr;
			bake.nDilations  = _padding;-- (Pading)
			bake.flags       = 1; --bit 1 of flag will be set to signify map channel conflict
			bake.bakeEnabled  = true;
			bake.addBakeElement dif_bake;
			
			select obj;
			render rendertype:#bakeSelected outputwidth:_sizeW outputheight:_sizeH antiAliasing:true vfb:false progressBar:true;		
			btmp = openBitMap url_save_path;
			deleteFile url_save_path; --delete the temp. file

			--remove traces
			bake.removeBakeElement dif_bake;--remove the dif bake entry
					
			clearSelection();
			select inp;
			obj.name = "tmp_bake";
			delete obj;
		)--end if editablePoly
	)--end if sleection.count > 0
	
	format "worn edges\n";
	
	return btmp;
)
/*
(
local b2 = fn_42__bake_worn_edges 256 256 2 1 10 5;--render worn edges
display b2;
)
*/