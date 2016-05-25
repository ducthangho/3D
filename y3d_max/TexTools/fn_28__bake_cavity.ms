function fn_28__bake_cavity _sizeW _sizeH _padding _channelNr gain_value=(

	local btmp = bitmap _sizeW _sizeH;
	
	-- original code and idea:
	-- Pits & Peaks 1.13 for 3DS MAX v4.x
	-- (C) 2002 Jeff Stewart
	-- http://object01.go.dyndns.org/
	-- object01@hotmail.com
	-- modified by renderhjs.net
	--gain_value = 0.05;--the lower this value, the more contrast
	
	local obj = selection[1];
	clearListener();
	
	--check if ScanlineRenderer is the default renderer
	if ((classOf renderers.current) != Default_Scanline_Renderer)then(
		if queryBox "Scanline Renderer is advised for basic RTT rendering,\nswitch the current Renderer to:\nDefault Scanline Renderer?" title:"Assign Scanline Renderer?" beep:true == true then(
			renderers.current = Default_Scanline_Renderer();
		)
	)

	
	
	
	
	if (superclassof obj == GeometryClass)then(
		--01.) create a copy triangle mesh
		local tmesh = copy obj;
		convertToMesh tmesh;
		update(tmesh);
		--tmesh.pos.x = tmesh.pos.x+(random 16 64);
		--02.) collect shell arrays of the faces
		
		local dirtObject = tmesh;
		
		fn getVertFaces _obj =(
			numverts = getNumVerts _obj
			local vertFaceArray = #()
			local faceVerts
			vertFaceArray = for i = 1 to _obj.numverts collect #()
			for f = 1 to _obj.numfaces do (
				faceVerts = getFace _obj f
				append vertFaceArray[faceVerts.x] f
				append vertFaceArray[faceVerts.y] f
				append vertFaceArray[faceVerts.z] f
			)--end for
			return vertFaceArray
		)
		
		fn avgAngles dirtObject vertex faceArray = (
			local normalArray = #()
			local normalSum = [0,0,0]
			local angleSum = 0
			-- Collect normals of all faces sharing 'vertex'
			for i = 1 to faceArray.count do(
				tmpNormal = normalize ( getFaceNormal dirtObject faceArray[i] )
				if finditem normalArray tmpNormal == 0 do append normalArray tmpNormal
			)
			-- Find average normal of all faces sharing 'vertex'.
			-- This vector will be compared with the edge vectors to determine concavity.
			for i = 1 to normalArray.count do normalSum += normalArray[i]
			avgNormal = normalize normalSum
			-- Find all edges sharing 'vertex'.
			edgeArray = meshop.getEdgesUsingVert dirtObject vertex
			-- Create an array of edge vectors for comparison with avgNormal.
			edgeVerts = #{}
			edgeVectorArray = #()
			edgeVector = point3
			for e in edgeArray do(
				edgeVerts = meshop.getVertsUsingEdge dirtObject e
				edgeVerts[vertex] = false	-- "this" vertex not needed
				for v in edgeVerts do edgeVector = normalize ( getVert dirtObject v - getVert dirtObject vertex )
				if edgeVector == Point3 do continue	-- if EdgeVector == Point3, that means
													-- edge e only had one vertex.  This
													-- is actually possible; a default
													-- Teapot's edge 2305 has only one
													-- vertex.  ???
													-- Can't add 'Point3' to the
													-- edgeVectorArray.  Will crash the
													-- script.
				append edgeVectorArray edgeVector
			)
			-- Determine average angle between all edge vectors and avgNormal
			angleSum = 0
			for ev in edgeVectorArray do angleSum += acos ( dot ev avgNormal )
			avgAngle = angleSum / edgeVectorArray.count
		)
		
		fn colorize dirtObject VertArray angleArray gain_value =(
		
			fn gain gain_value value =(
				if gain_value < 0.0001 do gain_value = 0.0001
				if gain_value > 0.9999 do gain_value = 0.9999
				if value < 0.0001 do value = 0.0001
				if value > 0.9999 do value = 0.9999
			
				if value < 0.5 then(
					value = value / ((1.0/gain_value-2.0)*(1.0-2.0*value)+1.0)
				)
				else(
					value = ((1.0/gain_value-2.0)*(1.0-2.0*value)-value) / ((1.0/gain_value-2.0)*(1.0-2.0*value)-1.0)
				)
			
				return value
			)

		
			-- Assign vertex colors based on concavity
			
			local peak_color = white;
			local pit_color = black;	
			defaultVCFaces dirtObject	-- "prepares" the object for vertex colors
										-- (I don't completely understand all the CPV
										--  stuff in the MAXScript Reference.)
			progressStart "Assigning vertex colors..."
			for i = 1 to VertArray.count do(
				percentagePeak = angleArray[i] / 180		-- 0% = pit, 100% = peak
				percentagePeak = gain gain_value percentagePeak
				vertColor = ( percentagePeak * peak_color ) + ( ( 1 - percentagePeak ) * pit_color )	-- do appropriate blending between pit & peak colors
				setVertColor dirtObject i vertColor			-- apply blended color to vertex
				progressUpdate ( i as float / VertArray.count * 100 )
			)
			progressEnd()
		)

		
		----------------------------------------------
		VertArray = getVertFaces dirtObject;--check
		angleArray = #();
		progressStart "Finding pits & peaks..."
		for i = 1 to VertArray.count do (
			angleArray[i] = avgAngles dirtObject i VertArray[i];
			progressUpdate ( i as float / VertArray.count * 100 )
		)
		progressEnd()
		-- Determine concavity angles for every vertex
		colorize dirtObject VertArray angleArray gain_value;-- apply color to verticies based on concavity
		-- Set node property flags
		dirtObject.showVertexColors = true;
		dirtObject.vertexColorsShaded = false;
		
		update dirtObject;
		gc()	-- garbage collect
		
		--04.) start a RTT bake of just difuse colors and the selected object
		tmesh.material =  Standardmaterial ();
		tmesh.material.diffuseMap = Mix ()
		tmesh.material.diffuseMap.Mask = Vertex_Color ();
	
		tmesh.removeAllBakeElements();
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
		bake = tmesh.INodeBakeProperties;
		bake.bakeChannel = _channelNr;--_channelNr;
		bake.nDilations  = _padding;-- (Pading)
		bake.flags       = 1; --bit 1 of flag will be set to signify map channel conflict
		bake.bakeEnabled  = true;
		bake.addBakeElement dif_bake;
		
		select tmesh;
		render rendertype:#bakeSelected outputwidth:_sizeW outputheight:_sizeH antiAliasing:true vfb:false progressBar:true;		
		btmp = openBitMap url_save_path;
		deleteFile url_save_path; --delete the temp. file

		--remove traces
		bake.removeBakeElement dif_bake;--remove the dif bake entry
		
		select obj;
		delete tmesh;
	)
	
	return btmp;
)
----
--fn_28__bake_cavityMap 58 58 2 1;