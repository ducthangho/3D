check_swap_former_materials_idx = 0;--stepper index if things loop later using modulo %16
check_swap_former_materials_idxs = #();--smal index for the materials, abs ID using getId()
check_swap_former_materials = #();--save former materials in this 2d array container

function fn_14__checker_swap forceOrginMaterial= (
	clearListener();
	local tileSize = fn_00__save_load_settings_get "general" "CheckMapTiling";
	if (tileSize == undefined)then(
		tileSize = 1;
	)
	
	local maxCacheMat  = 24;--store materials in arrays, this is the maximum limit to which the script remembers node id's associated materials

	function getId obj=(--try to get a absolute ID for this node based on node name and node color
		local n = ((obj.wireColor).r as integer) as string;
		local n= n+","+((obj.wireColor).g as integer) as string;
		local n= n+","+ ((obj.wireColor).b as integer) as string;
		local n= n+","+obj.name;
		return n;
	)
	
	function getMode obj=(
		if (obj.material == undefined)then(
			return -1;
		)else(
			local texFileName;
			try(
				texFileName = getFilenameFile(obj.material.diffuseMap.fileName);
			)catch(
				texFileName = "";
			)
			
			if ( (obj.material) as string == "Standardmaterial:Standard" and ((classof obj.material.diffuseMap) as string== "Checker"))then(
				return 1;
			)else if( (obj.material) as string == "Standardmaterial:Standard" and classof(obj.material.diffuseMap) as string == "Bitmaptexture" and texFileName=="texTools_checkermap_a")then(
				return 2;
			)else if( (obj.material) as string == "Standardmaterial:Standard" and classof(obj.material.diffuseMap) as string == "Bitmaptexture" and texFileName=="texTools_checkermap_b")then(
				return 3;
			)else(
				return 0;--its own material
			)
		)
	)
	
	function setMode obj nr tileSize=(
		
		if (nr == 1)then(
			obj.material = standard diffuseMap:(checker()) showInViewport:true;
			obj .material.diffusemap.coords.U_Tiling = tileSize+4;
			obj .material.diffusemap.coords.V_Tiling = tileSize+4;
			obj .material.diffusemap.coords.realWorldScale = off;
			obj .material.diffusemap.color2 = color 154 173 186;
			obj .material.diffusemap.color1 = color 74 77 90;
		)else if (nr == 2)then(
			--local tex = openBitmap "checkermap_a.png";
			local tex = openBitmap ((pathConfig.GetDir #userScripts)+"\\TexTools\\texTools_checkermap_a.png");
			obj .material = standard diffuseMap:(Bitmaptexture bitmap:tex) showInViewport:true;
			obj .material.diffusemap.coords.U_Tiling = tileSize;
			obj .material.diffusemap.coords.V_Tiling = tileSize;
			obj .material.diffusemap.coords.realWorldScale = off;
		)else if (nr == 3)then(
			local tex = openBitmap ((pathConfig.GetDir #userScripts)+"\\TexTools\\texTools_checkermap_b.png");
			obj .material = standard diffuseMap:(Bitmaptexture bitmap:tex) showInViewport:true;
			obj .material.diffusemap.coords.U_Tiling = tileSize;
			obj .material.diffusemap.coords.V_Tiling = tileSize;
			obj .material.diffusemap.coords.realWorldScale = off;
		)else if (nr == 0)then(
			--set original material
			local idx = findItem check_swap_former_materials_idxs obj;--check if it is already stored, in that case get the index
			if (idx != 0)then(--yes a former material was found in the databse
				obj.material = check_swap_former_materials[idx];
			)else(
				obj.material = undefined;
			)
		)
	)

	function pushMaterialToCache obj getMode maxCacheMat=(
		print(" chec "+check_swap_former_materials as string);
		local m = (getMode obj);
		if (m == 0)then(
			
			local idx1 = findItem check_swap_former_materials_idxs obj;--check if it is already stored, in that case get the index
			
			if (idx1 == 0)then(--does not yet exist in the database
				check_swap_former_materials_idx+=1;
				if (check_swap_former_materials_idx <= maxCacheMat)then(
					append check_swap_former_materials obj.material;--append the material to the 
					append check_swap_former_materials_idxs obj;--which obj
				)else(--reuse old ones
					local idx2 = mod check_swap_former_materials_idx maxCacheMat;
					print("idx2 "+idx2 as string+" / "+check_swap_former_materials.count as string);
					check_swap_former_materials[idx2] = obj.material;
					check_swap_former_materials_idxs[idx2] = obj;
				)
			)else(--reuse the indecies and update the material associated with the obj
				check_swap_former_materials[idx1] = obj.material;
				check_swap_former_materials_idxs[idx1] = obj;
			)
		)
	)
	

	--check_swap_former 
	if (selection.count > 0)then(

		if (forceOrginMaterial)then(
			for obj in selection do(
				setMode obj 0 tileSize;--set original material if possible, otherwise undefined
			)
			
		)else(
			
			for obj in selection do(--try store the original maps that are about to change
				pushMaterialToCache obj getMode maxCacheMat;
			)
			
			------{...determine the flow for the whole selection group...}---
			local modes = #();--all current modes for the selected objects
			local idxs = #();--short array for all indexies
			
			for obj in selection do(
				local m = (getMode obj);
				local idx = findItem idxs m;
				if (idx == 0)then(--not yet in array
					append idxs m;
					idx = modes.count+1;
					modes[idx] = 1;
				)else(
					modes[idx]+=1;
				)
			)
			local max_mode = #(1, modes[1]);--which idx and which count
			for i=1 to modes.count do(--get the one with the highest counting
				if (modes[i] > max_mode[2])then(
					max_mode = #(i,modes[i]);
				)
			)
			max_mode = idxs[ max_mode[1] ];--this is actually the higest mode (-1,0,1,2,3)
			------{...determine the flow for the whole selection group...}---
			
			
			format ("getMode range:"+idxs as string+", counts: "+modes as string+"\nhighest: "+max_mode as string+"\n");
			
			for obj in selection do(
				if (max_mode == -1)then(--most of them don't have yet any material
					setMode obj 1 tileSize;--first checker material
				)else if(max_mode == 0)then(--most of them have their unique material
					setMode obj 1 tileSize;--first checker material
				)else if(max_mode ==1)then(--most of them have their unique material
					setMode obj 2 tileSize;--first checker material
				)else if(max_mode == 2)then(--most of them have their unique material
					setMode obj 3 tileSize;--first checker material
				)else if(max_mode == 3)then(--most of them have their unique material
					setMode obj 0 tileSize;--first checker material
				)
			)
		)
	)

)