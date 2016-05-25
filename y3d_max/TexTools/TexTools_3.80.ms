--new TexTools 3.0 Main GUI
TexTools_version_nr = "4.10";--the version number
bmp_spr_gui;
rightClickSize;
rightClickTools;
rl_textureTools;
rl_TexTools_options;
rl_texTools_transform;
rl_texTools_bake;--reserve here otherwise it wont catc hwit the load stuff
fn_00__save_load_settings;
fn_00__align_texTools_panel;
fn_texTools_transform_openClose;


fileIn "fn_00__save_load_settings.ms"
--fileIn "fn_01__bake_uvfaces_2_clipboard.ms"
fileIn "fn_02__bake_uvLayout.ms"
fileIn "fn_04__align_uv_verts.ms"
fileIn "fn_06__align_uv_shell_to_edge.ms"
fileIn "fn_07__rotate_shell_compact.ms"
fileIn "fn_08__quick_flatten_selection.ms"
fileIn "fn_09__crop_to_uv_area.ms"
--fileIn "fn_10__stack_shells.ms"
fileIn "fn_11__view_flat_swap.ms"
fileIn "fn_12__extend_selection.ms"
fileIn "fn_13__open_uv_editor.ms"
fileIn "fn_14__checker_swap.ms"
fileIn "fn_17__open_in_ps.ms"
fileIn "fn_16__relax.ms"
fileIn "fn_18__swap_UVW2XYZ.ms"
fileIn "fn_19__texel_set.ms"
fileIn "fn_20__texel_get.ms"
fileIn "fn_21__normalize_uv_shells.ms"
fileIn "fn_22__sort_in_row.ms"
fileIn "fn_23__rectify.ms"
fileIn "fn_24__stitch.ms"
fileIn "fn_25__peltmap.ms"
fileIn "fn_32__symmetry.ms"
fileIn "fn_33__peel_pipe.ms"
fileIn "fn_34__split_uv_by_smoothing_groups.ms"
fileIn "fn_35__get_tex_size.ms";
filein "fn_36__explode_uv_faces.ms"
filein "fn_37__pack_uv.ms";
filein "fn_39__smoothGroupsByUvShells.ms";
filein "fn_40__unwrap_with_roadkill.ms";

fileIn "TexTools_rollout_transform.ms"


try (
	DestroyDialog rl_textureTools;
) catch();
try (
	DestroyDialog rl_texTools_bake;
)catch();
try (
	DestroyDialog rl_TexTools_options;
) catch();
try (
	DestroyDialog rl_TexTools_help;
) catch();
-------
function fn_00__align_texTools_panel =(
	local do_snapToolbar 		= fn_00__save_load_settings_get "panel" "snap2Editor";
	if (do_snapToolbar == true)then(--only if this setting is checked
		if classof (modPanel.getCurrentObject()) == Unwrap_UVW then(
			local uv = modpanel.getcurrentobject();
			local uvX =  uv.GetWindowX();
			local uvY =  uv.GetWindowY();
			local uvH =  uv.GetWindowH();
			local uvW =  uv.GetWindowW();
			local TTpos = GetDialogPos rl_textureTools;--GetDialogPos rl_textureTools
			local tx = 0;--target X
			local ty = uvY;
			
			if (uvX < systemTools.GetScreenWidth() and uvY < systemTools.GetScreenHeight())then(
				if (TTpos.x < (uvX+uvW/2))then(
					tx = uvX-60-6;
				)else(
					tx = uvX+uvW;
				)
				if (uvX < 64/2)then(--no free place on this side
					tx = uvX+uvW;
				)else if (uvX > systemTools.GetScreenWidth()-64/2)then(
					tx = uvX-60-6;
				)
				
				tx = amax #(tx, 0);
				tx = amin #(tx, systemTools.GetScreenWidth()-60-6);
				ty = amax #(ty, 0);
				ty = amin #(ty, systemTools.GetScreenHeight() - rl_textureTools.height-24);
				
				local pos = getDialogPos rl_textureTools;
				if (pos.x != tx or pos.y != ty)then(
					SetDialogPos rl_textureTools [tx,ty];
				)
			)
		)
	)
)
-------
function fn_gui_bmpSlice src x y w h=(
	if ((maxVersion())[1] >= 10000 )then(--max2008+ only, 20x+ faster
		local btmp = bitmap w h color:(color 0 0 0);
		pasteBitmap src btmp (box2 x y w h) [0,0];
		return btmp;
	)else(--max 9, a hell lot slower but at least it works
		local btmp = bitmap w h color:(color 0 0 0);
		if (src == undefined)then(
			return btmp;
		)else(
			local _w = src.width;
			local _h = src.height;
			local pixels = #();
			for r=0 to _h-1 do(
				pixels[r+1] = #();
				local pixRow=getpixels src [0,r] _w -- read in the column of pixels
				for c=1 to _w do(
					pixels[r+1][c] = pixRow[c];
				)
			)
			for r=1 to h do(
				for c=1 to w do(
					setpixels btmp [c-1,r-1] #(pixels[r+y][c+x])
				)
			)
			return btmp;
		)
	)
)





array_gui_buttons = #();--store button settings in this array
gui_img_slice1 = undefined;




function fn_gui_button_click1 nr=(--regular click
	array_gui_buttons[nr][3]();
)
function fn_gui_button_click2 nr=(--context click
	array_gui_buttons[nr][4]();
)
function fn_gui_button_over nr=(
	array_gui_buttons[nr][1].bitmap = array_gui_buttons[nr][6];
)
function fn_gui_button_out nr=(
	array_gui_buttons[nr][1].bitmap = array_gui_buttons[nr][5];
)
function fn_gui_getInput =(
	--w,h,p,c
	return #((rl_textureTools.edt_tex_sizeW.text as integer),(rl_textureTools.edt_tex_sizeH.text as integer), (rl_textureTools.edt_tex_padding.text as integer),(rl_textureTools.ddl_uvChannel.items[rl_textureTools.ddl_uvChannel.selection] as integer) );
)
--------------
function fn_gui_build_tt_rollout =(--generates the rollout dynamicly because max doesn't allow to loop add buttons
	--local url = "E:\\Projects 2009\\TexTools 3.00\\TexTools_gui.bmp";--url to the GUI icons
	local url = (pathConfig.GetDir #userScripts)+"\\TexTools\\TexTools_gui.bmp";--url to the GUI icons
	bmp_spr_gui = openBitMap url;
	array_gui_buttons = #();--array to hold anything for the buttons
	
	function pshBtn a bbBtn bbBtm label fnCall1 fnCall2=(--this method will push a button to the rollout and the arrays that control the click, rollover and rollout states
		local btmpA = fn_gui_bmpSlice bmp_spr_gui bbBtm.x bbBtm.y bbBtm.w bbBtm.h;
		local btmpB = fn_gui_bmpSlice bmp_spr_gui bbBtm.x (bbBtm.y+bbBtm.h) bbBtm.w bbBtm.h;
		append a #(undefined,label,fnCall1,fnCall2,btmpA,btmpB);
		format "functions: %\t% \n" fnCall1 fnCall2

		local nr = a.count as string;
		local cd="\nimgTag btn_"+nr+" pos:["+bbBtn.x as string+","+bbBtn.y as string+"] bitmap:array_gui_buttons["+nr+"][5] tooltip:\""+label+"\" transparent:(color 0 255 0);";
		if (fnCall1 != undefined)then(
			cd+="\non btn_"+nr+" click a b do 	fn_gui_button_click1 "+nr+";";
		)
		if (fnCall2 != undefined)then(
			cd+="\non btn_"+nr+" rightclick a b do 	fn_gui_button_click2 "+nr+";";
		)
		cd+="\non btn_"+nr+" mouseover a b do 	fn_gui_button_over "+nr+";";
		cd+="\non btn_"+nr+" mouseout a b do 	fn_gui_button_out "+nr+";";
		return cd;
	)

	
	rcMenu rightClickSize(
		menuItem itm1 "Pick"
		seperator menuSep01
		menuItem itm3 "128"
		menuItem itm4 "256"
		menuItem itm5 "512"
		menuItem itm6 "1024"
		menuItem itm7 "2048"
		menuItem itm8 "4096"
		--actions
		on itm1 picked do (
			local size = fn_35__get_tex_size();
			rl_textureTools.edt_tex_sizeW.text = size[1] as string;
			rl_textureTools.edt_tex_sizeH.text = size[2] as string;
			rl_textureTools.edt_tex_padding.text = size[3] as string;
		)
		on itm3 picked do (rl_textureTools.edt_tex_sizeW.text = rl_textureTools.edt_tex_sizeH.text = "128");
		on itm4 picked do (rl_textureTools.edt_tex_sizeW.text = rl_textureTools.edt_tex_sizeH.text = "256");
		on itm5 picked do (rl_textureTools.edt_tex_sizeW.text = rl_textureTools.edt_tex_sizeH.text = "512");
		on itm6 picked do (rl_textureTools.edt_tex_sizeW.text = rl_textureTools.edt_tex_sizeH.text = "1024");
		on itm7 picked do (rl_textureTools.edt_tex_sizeW.text = rl_textureTools.edt_tex_sizeH.text = "2048");
		on itm8 picked do (rl_textureTools.edt_tex_sizeW.text = rl_textureTools.edt_tex_sizeH.text = "4096");
	) -- end of rcTest rcmenu
	
	rcMenu rightClickTools(
		--menuItem itm1 "Copy/Swap UV Channels..."
		menuItem itm0 "Resize UV Canvas"
		menuItem itm4 "Smoothing Groups from UV shells"
		menuItem itm3 "Create UV Shape / Vector"
		seperator menuSep01
		menuItem itm6 "RoadKill: LSCM unwrap"
		menuItem itm7 "RoadKill: ABF unwrap"
		
		
		seperator menuSep02
		menuItem itm2 "UV Channel Listener"
		menuItem itm1 "TexTools Directory"
		menuItem itm5 "MacroScript Directory"
		
		
		on itm1 picked do (--openTexTools directory
			local url =getdir #userScripts+"\\TexTools";
			shellLaunch "explorer.exe" url;
		);
		on itm2 picked do (--UV Channel Listener
			ChannelInfo.dialog();
		);
		on itm0 picked do (--UV Channel Listener
			fileIn "TexTools_rollout_resize.ms"
		);
		on itm3 picked do (--Convert to shape
			fileIn "TexTools_rollout_uvToShape.ms"
		);
		on itm4 picked do (--Convert to shape
			fn_39__smoothGroupsByUvShells();
		);
		on itm5 picked do (--macro scripts directory
			local url =pathConfig.GetDir #userMacros;
			shellLaunch "explorer.exe" url;
		);
		on itm6 picked do (--RoadKill LSCM
			fn_40__unwrap_with_roadkill(false);
		);
		on itm7 picked do (--RoadKill ABF
			fn_40__unwrap_with_roadkill(true);
		);
	) -- end of rcTest rcmenu
	
	
	
	
	---01.)	Fetch all the buttons and functions
	local yOff = 0;
	
	local c = "rollout rl_textureTools \"v."+TexTools_version_nr+"\" width:60 height:600(\n";--code to be executed at the end
	c+="local b1 = bitmap 30 30 color:black;\nlocal b2 = bitmap 60 15 color:black;\nlocal b3 = bitmap 30 15 color:black;";
	

	c+="on rl_textureTools open do (fn_00__align_texTools_panel();\n";
	c+=")\n";
	c+="Timer enterFrame \"\" pos:[0,0] width:10 height:10 interval:500\non enterFrame tick do fn_00__align_texTools_panel();\n";

	yOff = 0;--System menu
	c+= pshBtn array_gui_buttons (box2 0 yOff 30 15) (box2 0 0 30 15) "Help" (function btn_fn=(
		local is_open = false;
		if (rl_TexTools_help != undefined)then(
			if (rl_TexTools_help.open == true)then(
				try (--close if possible
					DestroyDialog rl_TexTools_help;
					is_open = true;
				) catch();
			)
		)
		if (not is_open)then(
			fileIn "TexTools_rollout_help.ms";--open the help dialogue
		)
		
	)) undefined;
	c+= pshBtn array_gui_buttons (box2 30 yOff 30 15) (box2 30 0 30 15) "Setup" (function btn_fn=(
		local is_open = false;
		if (rl_TexTools_options != undefined)then(
			if (rl_TexTools_options.open == true)then(
				is_open = true;
			)
		)
		if (not is_open)then(
			fn_00__save_load_settings false;--save before opening the floater
			fileIn "TexTools_rollout_options.ms";--open the options dialogue
			try(
				SetDialogPos rl_TexTools_options [(GetDialogPos rl_textureTools).x+64,(GetDialogPos rl_textureTools).y];
			)catch();
		)else(
			fn_00__save_load_settings false;--save settings
			DestroyDialog rl_TexTools_options;
		)
	)) undefined;
	
	yOff = 15;--Resolution input + channel selection
	c+="edittext edt_tex_sizeW \"\" pos:[-8,"+yOff as string+"] width:40 height:16 toolTip:\"Width\"\n";
	c+="edittext edt_tex_sizeH \"\" pos:[-8,"+(yOff+17) as string+"] width:40 height:16 toolTip:\"Height\"\n";
	c+="dropdownList ddl_uvChannel \"\" pos:[32,"+(yOff) as string+"] width:30 height:21 items:#(\"1\", \"2\", \"3\", \"4\") selection:1\n";
	c+="edittext edt_tex_padding \"\" pos:[28,"+(yOff+17) as string+"] width:32 height:16 toolTip:\"padding size in pixels\"\n";
	yOff+= 32;--Resize and Size templates
	
	--3.5 new Context Tools
	c+= pshBtn array_gui_buttons (box2 0 yOff 30 15) (box2 60 0 30 15) "Size" (function btn_fn=(
		popUpMenu rightClickSize pos:[30,60] rollout:rl_textureTools align:#align_topright;
	)) undefined;
	c+= pshBtn array_gui_buttons (box2 30 yOff 30 15) (box2 90 0 30 15) "Tools" (function btn_fn=(
		popUpMenu rightClickTools pos:[30,60] rollout:rl_textureTools align:#align_topleft;
	)) undefined;
	yOff+= 16;--
	
	--Main button
	c+= pshBtn array_gui_buttons (box2 0 yOff 60 30) (box2 120 0 60 30) "Open UV editor" (function btn_fn_editor=(
		local inp = fn_gui_getInput();
		fn_13__open_uv_editor inp[1] inp[2] inp[4];
	)) undefined;
	
	yOff+=30;--Visual related
	c+= pshBtn array_gui_buttons (box2 0 yOff 30 30) (box2 180 0 30 30) "Checker Map Swap, LMB: Cylce Checker Maps, RMB: Reset to former Material" (function btn_fn_checker1=(
		fn_14__checker_swap(false);
	)) (function btn_fn_checker2=(
		fn_14__checker_swap(true);--force reset
	))
	c+= pshBtn array_gui_buttons (box2 30 yOff 30 30) (box2 210 0 30 30) "Shading, LMB: Swap Flat/Shaded, RMB: Reset to Shaded" (function btn_fn_difuse=(
		fn_11__view_flat_swap();
	)) (function btn_fn_difuseOff=(
		viewport.setrenderlevel #smooth;
	));
	c+= pshBtn array_gui_buttons (box2 0 (yOff+30) 30 30) (box2 240 0 30 30) "Open associated Textures of Object selection in Photoshop" (function btn_fn_ps=(
		fn_17__open_in_ps();
	)) undefined;
	
	yOff+= 30;	
	--3.5 RoadKill button
	c+= pshBtn array_gui_buttons (box2 30 yOff 30 30) (box2 270 0 30 30) "Swap UV with XYZ channel" (function btn_fn_swapUV=(
		fn_18__swap_UVW2XYZ();
	)) undefined;
	
	yOff+= 30;	
	
	
	
	c+= pshBtn array_gui_buttons (box2 0 yOff 30 30) (box2 300 0 30 30) "Render UVLayout to your Clipboard" (function btn_fn_renderUV=(
		local version = (maxVersion())[1];
		if (version >= 10000) then( -- max 2008+
			local inp = fn_gui_getInput();
			local btmp = fn_02__bake_uvLayout inp[1] inp[2] inp[4];
			if (btmp != false)then(
				setclipboardBitmap btmp;
				print("done storing UV wireframe to clipboard");
			)
		)else(
			messagebox "You need 3dsMax 2008+ to access this feature from here. Use otherwise the Texture Map Floater to bake your Wireframe in Max 9." title:"Max2008+ required"
		)
	)) undefined;
	c+= pshBtn array_gui_buttons (box2 30 yOff 30 30) (box2 330 0 30 30) "Render Texture Maps Floater" (function btn_fn_renderMaps=(
		fileIn "TexTools_rollout_maps.ms";--open the detail map floater
	)) undefined;

	
	
	yOff+= 30;	
	c+= pshBtn array_gui_buttons (box2 0 yOff 30 30) (box2 360 0 30 30) "Iron UV faces to 1 shell" (function btn_fn_splitSmooth=(
		fn_08__quick_flatten_selection false;
	)) undefined;
	c+= pshBtn array_gui_buttons (box2 30 yOff 30 30) (box2 390 0 30 30) "Split UV LMB: using smoothing groups, RMB: Explode UVs angle based" (function btn_fn_splitSmoothingGrps=(
		fn_34__split_uv_by_smoothing_groups();
	)) (function btn_fn_explodeUV=(
		local inp = fn_gui_getInput();
		fn_36__explode_uv_faces inp[1] inp[3];
	))
	

	yOff+= 30;	
	c+= pshBtn array_gui_buttons (box2 0 yOff 30 30) (box2 540 0 30 30) "Relax UV selection" (function btn_fn_splitSmooth=(
		fn_16__relax();
	)) undefined;
	c+= pshBtn array_gui_buttons (box2 30 yOff 30 30) (box2 570 0 30 30) "Linear, LMB: no snap align, RMB: 90 degr. axis snap" (function btn_fn_align_edgs_LB=(
		fn_04__align_uv_verts 1;--0 degree snaps
	))(function btn_fn_align_edgs_RB=(
		fn_04__align_uv_verts 90;--90 degree snaps
	));
	c+= pshBtn array_gui_buttons (box2 0 (yOff+30) 30 30) (box2 600 0 30 30) "Rectify" (function btn_fn_ps=(
		fn_23__rectify();
	)) undefined;
	c+= pshBtn array_gui_buttons (box2 30 (yOff+30) 30 30) (box2 630 0 30 30) "LMB: Pelt map, RMB: Peel map" (function btn_fn_pelt=(
		fn_25__peltmap();
	)) (function btn_fn_peel=(
		fn_33__peel_pipe();
	));
	c+= pshBtn array_gui_buttons (box2 30 (yOff+60) 30 30) (box2 660 0 30 30) "Stitch, LMB:Stitch, RMB: just isolate selection of stitchable edges" (function btn_fn_stitch1=(
		fn_24__stitch false;
	)) (function btn_fn_stitch2=(
		fn_24__stitch true;--select only the edges
	));
	c+= pshBtn array_gui_buttons (box2 0 (yOff+60) 30 30) (box2 690 0 30 30) "Symmetry accross edge mirror or over selected face half of the shell" (function btn_fn_swapUV=(
		fn_32__symmetry();
	)) undefined;
	
	
	
	yOff+= 90;	
	c+= pshBtn array_gui_buttons (box2 0 yOff 30 30) (box2 420 0 30 30) "Pack UV's" (function btn_fn_packUV=(
		local inp = fn_gui_getInput();
		fn_37__pack_uv inp[1] inp[3];
	)) undefined;
	
	c+= pshBtn array_gui_buttons (box2 30 yOff 30 30) (box2 450 0 30 30) "Sort Shells in row, LMB: Horizontal, RMB: Vertical" (function btn_fn_sortFraces=(
		local inp = fn_gui_getInput();
		fn_22__sort_in_row true inp[1] inp[2] inp[3];
	)) (function btn_fn_sortFraces2=(
		local inp = fn_gui_getInput();
		fn_22__sort_in_row false inp[1] inp[2] inp[3];
	));
	
	c+= pshBtn array_gui_buttons (box2 0 (yOff+30) 30 30) (box2 480 0 30 30) "Align Shell to edge" (function btn_fn_ps=(
		fn_06__align_uv_shell_to_edge();
	)) undefined;
	c+= pshBtn array_gui_buttons (box2 30 (yOff+30) 30 30) (box2 510 0 30 30) "Crop UV canvas to active UV's" (function btn_fn_swapUV=(
		local inp = fn_gui_getInput();
		fn_09__crop_to_uv_area inp[1] inp[2] inp[3];
	)) undefined;

	yOff+= 60;	

	c+= "edittext edt_texel_texsize \"\" pos:[26,"+(yOff) as string+"] width:37 height:14 toolTip:\"the texture size in ² pixels\"\n";
	c+= "edittext edt_texel_perunit \"\" pos:[26,"+(yOff+16) as string+"] width:37 height:14 toolTip:\"padding size in pixels\"\n";
	gui_img_slice1 = fn_gui_bmpSlice bmp_spr_gui 0 30 30 30;
	c+= "ImgTag img_texel_label \"Bitmap\" pos:[0,"+(yOff) as string+"] width:30 height:30 bitmap:gui_img_slice1\n";
	c+= pshBtn array_gui_buttons (box2 0 (yOff+30) 30 30) (box2 720 0 30 30) "Normalize UV shells" (function btn_fn_ps=(
		fn_21__normalize_uv_shells();
	)) undefined;
	c+= pshBtn array_gui_buttons (box2 30 (yOff+30) 30 30) (box2 750 0 30 30) "Get/Set Texel ratio, LMB: Pick ratio from selection (only one), RMB: Assign to object selection (can be multiple)" (function btn_fn_getTexel=(
		rl_textureTools.edt_texel_texsize.text = "";
		rl_textureTools.edt_texel_perunit.text = "";
		local inp = fn_gui_getInput();
		local meta = fn_20__texel_get(inp[1] as float);
		rl_textureTools.edt_texel_texsize.text = (meta.x as float) as string;
		rl_textureTools.edt_texel_perunit.text = (meta.y as float) as string;
		
	)) (function btn_fn_setTexel=(
		
		local texture_res = (rl_textureTools.edt_tex_sizeW.text as integer) as float;
		local texl_res = (rl_textureTools.edt_texel_texsize.text as float);
		local texl_unit = (rl_textureTools.edt_texel_perunit.text as float);
		
		if (texl_res > 0 and texl_unit > 0 and texture_res > 0)then(
			fn_19__texel_set texture_res texl_res texl_unit;
		)
	));
	c+="\n)";
	execute c;--02.)	Create rollout Base		
	CreateDialog rl_textureTools 60 (407+16) pos:(point2 16 100) bgcolor:(color 158 158 158) style:#(#style_border,#style_toolwindow,#style_sysmenu);

	for i=1 to array_gui_buttons.count do(--02.) link rollout buttons to Array pointers
		array_gui_buttons[i][1] = execute ("rl_textureTools.btn_"+i as string)
	)
)

---init the TexTools toolbar
print("init");
fn_gui_build_tt_rollout();
fn_00__save_load_settings true;--doLoad = true






--| 	macroscripts for quick access	 |--
macroScript TexTools	category:"TexTools"	toolTip:"TexTools"	ButtonText:"TexTools"	icon:#("TexTools",1)
(
	-- thx for IN_human for the swapping input
	-- http://www.scriptspot.com/3ds-max/textools-uv-toolbox-for-the-texture-artist#comment-4100
	on execute do(
		if (rl_textureTools != undefined) then(--the rollout and the functions already exist in the RAM, most likely we can just reopen it
			if (rl_textureTools.open == true)then(
				try(
					destroyDialog rl_textureTools;
					destroyDialog rl_texTools_detail_maps;
				)catch();
			)else if(rl_textureTools.open == false)then(
				fn_gui_build_tt_rollout();
				fn_00__save_load_settings true;--doLoad = true
			)
		)else(
			filein ((pathConfig.GetDir #userScripts)+"\\TexTools\\TexTools_3.80.ms");--launch the script
		)
	)
)

macroScript render_to_clipboard_uv_layout	category:"TexTools"	 ButtonText:"Render:UVLayout" toolTip:"Render:UVLayout"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		local inp = fn_gui_getInput();
		fn_02__bake_uv_2_clipboard inp[1] inp[2] inp[4];
	)
)
macroScript render_to_clipboard_uv_selected_faces	category:"TexTools" ButtonText:"Render:Mask" toolTip:"Render:Mask"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		local inp = fn_gui_getInput();
		fn_01__bake_uvfaces_2_clipboard inp[1] inp[2] inp[4];
	)
)

macroScript align_verts_straight	category:"TexTools"	ButtonText:"Linear Edge Flow" toolTip:"Linear Edge Flow"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		fn_04__align_uv_verts 90;
	)
)
macroScript quick_flatten_selection_to_shell	category:"TexTools"	ButtonText:"Iron to Shell" toolTip:"Iron to Shell"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		fn_08__quick_flatten_selection false;
	)
)
macroScript context_extend_selection	category:"TexTools"	ButtonText:"Extend Selection" toolTip:"Extend Selection"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		fn_12__extend_selection();
	)
)

macroScript line_up_in_a_row	category:"TexTools"	ButtonText:"LineUp Shells" toolTip:"LineUp Shells"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		local inp = fn_gui_getInput();
		fn_22__sort_in_row true inp[1] inp[2] inp[3];
	)
)

macroScript rectify_quads	category:"TexTools"	ButtonText:"Rectify Quad Faces" toolTip:"Rectify Quad Faces"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		fn_23__rectify();
	)
)
macroScript normalize_selection	category:"TexTools"	ButtonText:"Normalize UV" toolTip:"Normalize UV"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		fn_21__normalize_uv_shells();
	)
)
macroScript align_shell_to_edge	category:"TexTools"	ButtonText:"Edge Align Shell" toolTip:"Edge Align Shell"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		fn_06__align_uv_shell_to_edge();
	)
)
macroScript swap_3d_uv	category:"TexTools"	ButtonText:"Swap UV/3D" toolTip:"Swap UV/3D"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		fn_18__swap_UVW2XYZ();
	)
)
macroScript relax_selection	category:"TexTools"	ButtonText:"Relax"	toolTip:"Relax"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		fn_16__relax();
	)
)
macroScript stitch_selection	category:"TexTools"	ButtonText:"Stitch" 	toolTip:"Stitch"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		fn_24__stitch false;
	)
)
macroScript peltmap_selection	category:"TexTools"	ButtonText:"Peltmap" 	toolTip:"Peltmap"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		fn_25__peltmap();
	)
)
macroScript open_close_editor	category:"TexTools"	ButtonText:"Open/Close Editor" 	toolTip:"Open/Close Editor"
(
	if (rl_textureTools != undefined and rl_textureTools.open == true) then(
		local inp = fn_gui_getInput();
		fn_13__open_uv_editor inp[1] inp[2] inp[4];
	)
)
--| 	macroscripts for quick access	 |--