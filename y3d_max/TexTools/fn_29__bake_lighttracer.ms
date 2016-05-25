fn fn_29__bake_lighttracer _sizeW _sizeH _padding _channelNr _rays _bounces=(
	local btmp = bitmap _sizeW _sizeH;
	
	local _continue = false;--checking if the script should continue
	--check if Scanline Renderer  is the default renderer
	if ((classOf renderers.current) != Default_Scanline_Renderer)then(
		if queryBox "Scanline Renderer as current renderer is required\nauto assign it and continue?" title:"assign Scanline Renderer?" beep:true == true then(               
			renderers.current = Default_Scanline_Renderer();
			_continue = true;
		)else(
			_continue = false;
		)
	)else(
		_continue = true;
	)
	local obj = selection[1];
	if (_continue and superclassof obj == GeometryClass)then(
		sceneradiosity.radiosity = Light_Tracer();--make sure that the Light Tracer is active in the advanced lightning (render settings)
		
		--01.) create a copy triangle mesh
		local tmesh = copy obj;
		convertToMesh tmesh;
		update(tmesh);
		---
		tmesh.pos.x = tmesh.pos.x+256;
		--02.) setup skylight + material
		local _skylight = Skylight pos:tmesh.pos isSelected:off;
		_skylight.color = color 242 242 242;--make sure the image will end up as greyscale and not blue tinted
		
		tmesh.material = Standardmaterial ();
		tmesh.material.Diffuse = color 255 255 255;
		tmesh.material.ambient = color 255 255 255;
		tmesh.material.selfIllumAmount = 0;
		--more info: 
		--	http://forums.cgsociety.org/showpost.php?p=1643337&postcount=3
		sceneRadiosity.radiosity.rays = _rays;--1000 for high
		sceneRadiosity.radiosity.sky_lights = 1.0;
		sceneRadiosity.radiosity.bounces = _bounces;--integer
		sceneRadiosity.radiosity.object_multiplier = 1.0 / (_bounces as float);
		sceneRadiosity.radiosity.filter_size = 2.0;--pixels to be melted/ blurred together
	
		--03.) RTT bake the map
		
		
		tmesh.removeAllBakeElements();
		local url_save_path = GetDir #image + "\\_renderToClipboard.bmp"
		local dif_bake = LightingMap();
		dif_bake.outputSzX    =    _sizeW;
		dif_bake.outputSzY    =    _sizeH;
		dif_bake.autoSzOn     = false;
		dif_bake.filterOn	 = true;
		dif_bake.elementName  = "LightingMap";--CompleteMap

		dif_bake.enabled      = true;
		dif_bake.directOn = true;--add lightning, very important
		dif_bake.indirectOn = true;
		dif_bake.shadowsOn = false;

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
		delete _skylight;
	)
	return btmp;
)
--fn_29__bake_lighttracer_dirt_map 64 64 4 1 256 4;