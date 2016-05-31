function fn_27__bake_blockmap _sizeW _sizeH _padding _channelNr _splitType=(
	local btmp = bitmap _sizeW _sizeH;
	
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
		--02.) collect shell arrays of the faces
		
		local elem = #();
		
		
		max create mode;--speed improvement 1
		with redraw off(--speed improvement 2
			undo off(--speed improvement 3
				
				
				local numFaces = getNumFaces tmesh;
				
				if (_splitType == 1)then(--split by Mesh Elements
					local faceElemArray = #();
					for f=1 to numFaces do (
						faceElemArray[ f ] = 0;
					)
					
					local faceBitArray = #{1..numFaces};
					for f in faceBitArray do (
						if faceElemArray[ f ] == 0 then (
							local elements = (meshOp.getElementsUsingFace tmesh f) as array;--faces as array
							elem[elem.count+1] = meshOp.getElementsUsingFace tmesh f;--faces as bitArray
							for i in elements do (
								faceElemArray[i] = elem.count;
								setFaceMatID tmesh i elem.count;--set material face ID
							)
						)
					)
				)else if (_splitType == 2)then(--split by smoothin groups
					--got input from: 
					--http://forums.cgsociety.org/showpost.php?p=2869667&postcount=7
					local numSmoothGroups = 0;
					local notfound = true;
					for i in 1 to numFaces while notfound do(
						local face_SG = getFaceSmoothGroup tmesh i;
						if face_SG == 0 do(
							messagebox "Every face must have a smoothing group!"
							notfound = false;
						)
						numSmoothGroups = amax #(face_SG, numSmoothGroups);
					)
					--Apply a matrial ID to each group of faces in the array based on what smoothing group they have.
					for f in 1 to numFaces while notfound do(
						SG = getFaceSmoothGroup tmesh f;
						for i = 1 to 32 do(
							ID = bit.get SG i;
							if ID == true then(
								setFaceMatID tmesh f i;
							)
						)
					)
					--create empty elements array so that the following script can create the 32 colors for this type
					for i=1 to numSmoothGroups do(
						elem[i] = #{};
					)
				)else if (_splitType == 3)then(--split by material ID's
					
					local numMatGroups = 0;
					for i in 1 to numFaces do(
						local id = getFaceMatID tmesh i;
						numMatGroups = amax #(id, numMatGroups);
					)
					for i=1 to numMatGroups do(
						elem[i] = #{};
					)
				)
				--
			)
		)
		--print("elements: "+elem.count as string);
		--03.) assign random color material to each element face collection
		function rainbowColor p =(
			local clr = color (random 0 255) (random 0 255) (random 0 255);
			clr.h = p*255;
			clr.s = 255;
			clr.v = 255;
			return 	clr;	
		)

		tmesh.material = Multimaterial();
		tmesh.material.numsubs = elem.count;
		tmesh.material.materialList.count = elem.count;
		with redraw off(--speed improvement 1
			undo off(--speed improvement 2
				for i=1 to elem.count do (
					local clor = rainbowColor ((i as float)*(1.0/elem.count));
					tmesh.material.materialList[i].Diffuse = clor;
					tmesh.material.materialList[i].ambient = clor;
					tmesh.material.materialList[i].selfIllumAmount = 100
				)
			)
		)
		--04.) start a RTT bake of just difuse colors and the selected object

		tmesh.removeAllBakeElements();
		local url_save_path = GetDir #image + "\\_renderToClipboard.bmp"
		local dif_bake = DiffuseMap();
		dif_bake.outputSzX    =    _sizeW;
		dif_bake.outputSzY    =    _sizeH;
		dif_bake.autoSzOn     = false;
		dif_bake.filterOn	 = false;
		
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
		render rendertype:#bakeSelected outputwidth:_sizeW outputheight:_sizeH antiAliasing:false filterMaps:false vfb:false progressBar:true;		

		btmp = openBitMap url_save_path;
		deleteFile url_save_path; --delete the temp. file
		--remove traces
		bake.removeBakeElement dif_bake;--remove the dif bake entry
		
		select obj;
		delete tmesh;
		--print("done!");
		gc();
	)
	return btmp;
)
---
--fn_27__bake_blockmap 320 320 2 1 3;