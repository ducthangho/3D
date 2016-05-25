fn fn_13__open_uv_editor textureWidth textureHeight channelNr= (
	--try(
	if (selection.count > 0)then(
		disableSceneRedraw();
		
		local close_editor = false;
		local unwrap_modifier_before = false;
		if (getCommandPanelTaskMode() != #modify)then(--make sure we are in the modify panel section
			setCommandPanelTaskMode #modify;
		)
		
		
		if (classof (modPanel.getCurrentObject()) != Unwrap_UVW) then(
			modPanel.addModToSelection (Unwrap_UVW ()) ui:on;
			unwrap_modifier_before = false;
		)else(
			unwrap_modifier_before = true;--a modifier is already present
			local w =  (modpanel.getcurrentobject()).GetWindowW();
			if ( w > 0 and w < 1600)then(--determine if the edit window is open
				--CLOSE THE UV UNWRAP EDITOR

				local uv = modPanel.getCurrentObject();--the selection in the mod panel,- the new way to select multiple objects in max2008+ to multi unwrap
				
				--uv.unwrap2.setBackgroundColor settings_prv_bkgUV_color;

				local org_selection = getCurrentSelection();
				clearSelection();
				select org_selection;
				close_editor = true;
				
				enableSceneRedraw();
				completeRedraw();
				if (rl_texTools_transform != undefined and fn_texTools_transform_openClose != undefined)then(
					fn_texTools_transform_openClose 2;--close the UV Transform floater
				)	
			)
		)
		if (close_editor == false)then(
		
			local uv = modPanel.getCurrentObject();--the selection in the mod panel,- the new way to select multiple objects in max2008+ to multi unwrap
			
			local do_override_colors 	= fn_00__save_load_settings_get "panel" "overrideColors";
			local do_snapToolbar 		= fn_00__save_load_settings_get "panel" "snap2Editor";
				
			
			
			--uv.unwrap5.setShowMapSeams on;
			--uv.unwrap5.setPeltAlwaysShowSeams off
			uv.unwrap4.setViewportOpenEdges true;
			uv.unwrap4.setThickOpenEdges false;
			
			subobjectLevel = 3;

			local current_channel = uv.unwrap.getMapChannel();
			if (current_channel == 0)then(
				current_channel = 1;
			)
			print("open the UV editor !!!!! @"+current_channel as string);
			
			
			if (current_channel != channelNr)then(--switch to another channel
				print ("switch channel");
				if ( unwrap_modifier_before )then(
					modPanel.addModToSelection (Unwrap_UVW ()) ui:on;--add another UVunwrap modifer as we are about to change to a new channel
				)
				uv = modPanel.getCurrentObject();--the selection in the mod panel,- the new way to select multiple objects in max2008+ to multi unwrap
				uv.unwrap.setMapChannel channelNr;--set the map channel number
				uv.unwrap.reset();
			)
			
			--open
			uv.unwrap.edit();
			uv.unwrap.edit();
			
			uv.unwrap2.setFreeFormMode on;
			
			uv.unwrap2.setGeomSelectElementMode(false); -- select viewport element mode
			uv.unwrap2.setGeomPlanarThreshold(20);
			uv.unwrap2.setGeomPlanarThresholdMode(true);
			
			--$.unwrap_uvw.unwrap2.setTVElementMode(true);--select element UV view
			uv.unwrap2.setTVSubObjectMode(3);--face selection more
			
			uv.unwrap.setWeldThreshold 0.001;
			
			settings_prv_bkgUV_color = (uv.unwrap2.getBackgroundColor() );
			settings_prv_bkgUV_color.x=255 - settings_prv_bkgUV_color.x*255;
			settings_prv_bkgUV_color.y=255 - settings_prv_bkgUV_color.y*255;
			settings_prv_bkgUV_color.z=255 - settings_prv_bkgUV_color.z*255;
			
			
			
			
			if (do_override_colors == true)then(
				uv.unwrap2.setShowMap(false);--disable tex view
				
				uv.unwrap2.setOpenEdgeColor [256-190,256-230,256-119];
				uv.unwrap2.setSharedColor [256-255,256-132,256-255];
				uv.unwrap.setLineColor [256-255,256-132,256-0];
				uv.unwrap.setSelectionColor [256-255, 256-108, 256-0];
				
				local colr = fn_00__save_load_settings_get "general" "bkg_color";
				
				uv.unwrap2.setBackgroundColor ([255-colr.r, 255-colr.g, 255-colr.b] as Point3);

				--grid colors
				uv.unwrap2.setGridVisible true;
				uv.unwrap2.setGridSize 0.5;
				uv.unwrap2.setGridColor [256-47,256-55,256-80];
			)
			-- new stuff
			--set unwrap editor texture size for appropriate proportion presentation			
			uv.unwrap.setRenderWidth textureWidth;
            uv.unwrap.setRenderHeight textureHeight;

			if (do_snapToolbar == true)then(
				fn_00__align_texTools_panel();--align the texTools panel aside to the uvUnwrap editor
			)
			
			
			if (rl_texTools_transform != undefined and fn_texTools_transform_openClose != undefined)then(
				fn_texTools_transform_openClose 1;--open the UV Transform floater
			)	
			
			enableSceneRedraw()
			completeRedraw();
		)
	)
	/*)catch(
		print("error opening the UV editor...");
		enableSceneRedraw()
		completeRedraw();
	)*/
	enableSceneRedraw()
	completeRedraw();
)