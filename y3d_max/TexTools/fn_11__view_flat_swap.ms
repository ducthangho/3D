settings_prv_bkgview_color;
function fn_11__view_flat_swap = (
		--flat
	if (viewport.getrenderlevel() != #flat)then(
		viewport.SetRenderLevel #flat;
		/*
		max 2010 seems to be broken in this area, going to touch this again as soon as I found a work around
		
		settings_prv_bkgview_color = getVPortBGColor();
		local c = fn_00__save_load_settings_get "general" "bkg_color"
		
		if ((maxVersion())[1] >= 12000 )then(--max2010 changed its way of GUI changes
			print("c "+c as string)
			SetUIColor 41 c;
			colorMan.repaintUI #repaintAll;
		)else(
			setVPortBGColor c;
		)
		*/
		
		viewport.SetShowEdgeFaces false;
		viewport.SetTransparencyLevel 0;
		displaySafeFrames = false;
	)else(
		/*if (settings_prv_bkgview_color != undefined)then(
			setVPortBGColor settings_prv_bkgview_color;
		)*/
		viewport.setrenderlevel #smooth;
		viewport.SetShowEdgeFaces true;
	)
	enableSceneRedraw();
	completeRedraw();
)