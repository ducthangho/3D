fn fn_03__bake_ao _sizeW _sizeH _padding _channelNr _samples _spread _maxDistance=(
	undo on(
		local btmp = bitmap _sizeW _sizeH;
		
		local _continue = false;--checking if the script should continue
		if ((classOf renderers.current) != mental_ray_renderer)then(--check if mentalRay is the default renderer
			if queryBox "mentalRay as current renderer is required\nauto assign it and continue?" title:"assign mentalRay?" beep:true == true then(               
				renderers.current = mental_ray_renderer();
				_continue = true;
			)else(
				_continue = false;
			)
		)else(
			_continue = true;
		)

		if (_continue)then(
			
			obj = selection[1];
			--tex_size = _size;
			
			obj.removeAllBakeElements();

			renderers.current = mental_ray_renderer();
			
			local url_save_path = GetDir #image + "\\_renderToClipboard.bmp"
			--local url_save_path = "c:\\_renderToClipboard.bmp"
			
			ao_bake = AmbientOcclusionBakeElement();	
			ao_bake.outputSzX    =    _sizeW;
			ao_bake.outputSzY    =    _sizeH;
			ao_bake.autoSzOn              = false;
			ao_bake.bright                      = color 255 255 255 0;
			ao_bake.dark                      = color 0 0 0 0;
			try(
				--crashed for Polycount member Kovac on max9 SP 2, winXP -> Unknown property: "backgroundColor" in ReferenceTarget:Ambient_Occlusion__MR
				ao_bake.backgroundColor		= color 0 0 0 0;
			)catch();
			ao_bake.elementName          = "Ambient Occlusion";
			ao_bake.falloff                      = 1.0;
			ao_bake.maxDistance          = _maxDistance;
			ao_bake.samples                  = _samples;--16
			ao_bake.spread                  = _spread;--0.8
			ao_bake.enabled                  = true;
			
			ao_bake.fileName = url_save_path;--just in case for 64-bit machines
			ao_bake.fileType = url_save_path;--appearently the target filename/path
			
			
			-- Baking job
			bake = obj.INodeBakeProperties;
			bake.bakeChannel = _channelNr;
			bake.nDilations         = _padding; -- (Pading)
			bake.flags             = 1; --bit 1 of flag will be set to signify map channel conflict
			bake.bakeEnabled     = true;
			bake.addBakeElement ao_bake;
			--select obj
			render rendertype:#bakeSelected outputwidth:_sizeW outputheight:_sizeH vfb:false progressBar:true;
			
			btmp = openBitMap url_save_path;
			deleteFile url_save_path; --delete the temp. file
			
			--remove traces
			bake.removeBakeElement ao_bake;--remove the AO bake entry
		)
		return btmp;
	)
)
-- __
--fn_03__bake_ao_2_clipboard 64 64 2 1 16 0.8;