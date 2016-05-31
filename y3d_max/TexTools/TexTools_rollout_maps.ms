close_texTools_bake = false;
if (rl_texTools_bake != undefined)then(
	if (rl_texTools_bake.open == false)then(
		close_texTools_bake = false;
	)else(
		close_texTools_bake = true;
		try (
			DestroyDialog rl_texTools_bake;
		)catch();
	)
)else(
	close_texTools_bake = false;
)
bake_save_settings;--function variable 
rl_texTools_bake;
if (close_texTools_bake == false)then(
fileIn "fn_01__bake_selectionMask.ms"
fileIn "fn_02__bake_uvLayout.ms"
fileIn "fn_27__bake_blockmap.ms"
fileIn "fn_29__bake_lighttracer.ms"
fileIn "fn_30__bake_hard_edges.ms"
fileIn "fn_03__bake_ao.ms"
fileIn "fn_28__bake_cavity.ms"
fileIn "fn_38__bake_camera_map_painting.ms"
fileIn "fn_31__bake_transfer_channels.ms"
fileIn "fn_42__bake_worn_edges.ms"
fileIn "fn_43__bake_vertexColors.ms"
)

function bake_wrapper bakeFn=(
	--does the actual bake operation and saves or stores result on disc, clipboard, material
	
	
	if (fn_gui_getInput != undefined and selection.count > 0)then(--see the main TexTools_x.xx.ms for this function
		local inp = fn_gui_getInput();--w,h,pad,channel
		
		local btmp = bitmap inp[1] inp[2] numframes:1 color:(color 120 120 120)
		
		local rl = rl_texTools_bake;
		---01.) get the Bitmap Bake
		case bakeFn of(
			fn_01__bake_selectionMask: (
				btmp = bakeFn inp[1] inp[2] inp[4];
			)
			fn_02__bake_uvLayout: (
				btmp = bakeFn inp[1] inp[2] inp[4];
			)
			fn_27__bake_blockmap: (
				btmp = bakeFn inp[1] inp[2] inp[3] inp[4] rl.rdb_block.state;
			)
			fn_03__bake_ao: (
				btmp = bakeFn inp[1] inp[2] inp[3] inp[4] rl.spn_ao_1.value rl.spn_ao_2.value rl.spn_ao_3.value;
			)
			fn_29__bake_lighttracer: (
				btmp = bakeFn inp[1] inp[2] inp[3] inp[4] (rl.spn_lt_1.value as integer) (rl.spn_lt_2.value as integer);
			)
			fn_30__bake_hard_edges: (
				btmp = bakeFn inp[1] inp[2] inp[3] inp[4] (rl.spn_edg_1.value as integer) rl.chk_edg_1.checked;
			)
			fn_28__bake_cavity: (
				btmp = bakeFn inp[1] inp[2] inp[3] inp[4] (rl.spn_cvt_1.value);
			)
			
			fn_38__bake_camera_map_painting: (
				local state = #(true,false)[(rl.rdb_cam.state)];--grabView true / false
				btmp = bakeFn state inp[1] inp[2] inp[3];
				if (btmp != false)then(--the operation was succesfull, switch to the other mode
					if (state)then(
						rl.rdb_cam.state = 2;
					)else(
						rl.rdb_cam.state = 1;
					)
				)
			)
			
			fn_31__bake_transfer_channels: (
				local ch_a = rl.ddl_trans_1.items[ rl.ddl_trans_1.selection ] as integer;
				local ch_b = rl.ddl_trans_2.items[ rl.ddl_trans_2.selection ] as integer;
				btmp = fn_31__bake_transfer_channels inp[1] inp[2] inp[3] ch_a ch_b;
			)
			
			fn_42__bake_worn_edges: (
				local contr = 10;
				local powr = 5;
				btmp = bakeFn inp[1] inp[2] inp[3] inp[4] contr powr;
			)
			fn_43__bake_vertexColors: (
				bakeFn();
			)
			
			
			
			default: (
				
			)

		)

		
		
		
		if (btmp != false)then(
			
			print("wrap bake function..."+bakeFn as string );
			
			if (rl_texTools_bake.chk_display.checked == true)then(
				display btmp;--display the bake
			)
			
			if (rl_texTools_bake.chk_clipboard.checked == true)then(--clipboard
				local version = (maxVersion())[1];
				if (version >= 10000) then( -- max 2008+
					setclipboardBitmap btmp;
				)else if (version >= 9000)then(--have to use the old dotNet code
					
					local theFileName = GetDir #image + "\\_renderToClipboard.bmp" --define a temp.file name
					local btmpB = bitmap inp[1] inp[2] numframes:1 color:(color 120 120 120) filename:theFileName;
					copy btmp btmpB;--have to copy a bitmap otherwise it is not saveable and assignable to the dotNet control
					save btmpB;
			
					local clipboardClass = dotNetClass "System.Windows.Forms.Clipboard" --create a Clipboard dotNetClass
					local theImage = dotNetClass "System.Drawing.Image" --create an Image dotNetClass
					local theBitmap = theImage.FromFile theFileName --get the saved image from file as bitmap
					clipboardClass.setImage theBitmap --copy the image to clipboard
					theBitmap.Dispose() --release the bitmap 
					deleteFile theFileName; --delete the temp. file
				)
			)
			
			if (rl_texTools_bake.chk_saveAs.checked == true)then(--save as
				local d_url = GetDir #image+"\\TexTools_map.bmp";--default save URL in the dialog
				local s_url = getSaveFileName caption:"Writing to clipboard failed, save instead and open the file"	filename:d_url		types:"Bitmap(*.bmp)|*.bmp|TIFF|*.tiff|All|*.*|"
				if (s_url != undefined)then(--user specified a save URL
					
					local btmpB = bitmap inp[1] inp[2] numframes:1 color:(color 120 120 120) filename:s_url;
					copy btmp btmpB;--have to copy a bitmap otherwise it is not saveable and assignable to the dotNet control
					save btmpB;
					print("saved to "+s_url);
				)
			)
			
			if (rl_texTools_bake.chk_setMaterial.checked == true)then(--as material
				local theFileName = GetDir #image + "\\_renderToClipboard.bmp" --define a temp.file name
				
				
				
				local btmpB = bitmap inp[1] inp[2] numframes:1 color:(color 120 120 120) filename:theFileName;
				copy btmp btmpB;--have to copy a bitmap otherwise it is not saveable and assignable to the dotNet control
				save btmpB;
				--selection[1].material = Standardmaterial ();
				
				selection[1].material = standard diffuseMap:(Bitmaptexture fileName:theFileName) showInViewport:true
				selection[1].material.selfIllumAmount = 100;
				/*
				selection[1].material.diffuseMap = Bitmaptexture fileName:theFileName;
				selection[1].material.showInViewport = true;*/
			)
			
		)
		
		
		freeSceneBitmaps();
	)
)


rcMenu rightClickAO(
	menuItem itm0 "Draft"
	menuItem itm1 "High and soft"
	menuItem itm2 "High and hard"
	
	on itm0 picked do (--openTexTools directory
		rl_texTools_bake.spn_ao_1.value = 16;
		rl_texTools_bake.spn_ao_2.value = 4.0;
	);
	on itm1 picked do (--openTexTools directory
		rl_texTools_bake.spn_ao_1.value = 512;
		rl_texTools_bake.spn_ao_2.value = 6.0;
	);
	on itm2 picked do (--openTexTools directory
		rl_texTools_bake.spn_ao_1.value = 512;
		rl_texTools_bake.spn_ao_2.value = 0.5;
	);
)
rcMenu rightClickLT(
	menuItem itm0 "Draft"
	menuItem itm1 "Medium"
	menuItem itm2 "High"
	
	on itm0 picked do (--openTexTools directory
		rl_texTools_bake.spn_lt_1.value = 32;
		rl_texTools_bake.spn_lt_2.value = 2.0;
	);
	on itm1 picked do (--openTexTools directory
		rl_texTools_bake.spn_lt_1.value = 512;
		rl_texTools_bake.spn_lt_2.value = 8.0;
	);
	on itm2 picked do (--openTexTools directory
		rl_texTools_bake.spn_lt_1.value = 1024;
		rl_texTools_bake.spn_lt_2.value = 16.0;
	);
)


rollout rl_texTools_bake "Bake Texture Maps" width:360 height:271(
	
	checkbox chk_clipboard "To Clipboard" pos:[4,3] width:77 height:18
	checkbox chk_setMaterial "Set As Material" pos:[100,3] width:90 height:18
	checkbox chk_saveAs "Save As..." pos:[210,3] width:77 height:18
	checkbox chk_display "Display" pos:[290,3] width:77 height:18
	
	on chk_clipboard changed arg do (
		bake_save_settings();
	)
	on chk_setMaterial changed arg do (
		bake_save_settings();
	)
	on chk_saveAs changed arg do (
		bake_save_settings();
	)
	on chk_display changed arg do (
		bake_save_settings();
	)
	
	local y_start = 32;
	local pos = [0,y_start];
	button btn_UV "UV" pos:[pos.x,pos.y] width:60 height:60;
	--checkbox chk_uv_1 "Smooth" pos:[pos.x,pos.y+60] width:59 height:15 checked:true
	on btn_UV pressed do(
		bake_wrapper fn_02__bake_uvLayout;
	)
	
 	local pos = [60,y_start];
	button btn_mask "Mask" pos:[pos.x,pos.y] width:60 height:60;
	on btn_mask pressed do(
		bake_wrapper fn_01__bake_selectionMask;
	)
	
	local pos = [120,y_start];
	button btn_block "Block" pos:[pos.x,pos.y] width:60 height:60;
	radiobuttons rdb_block "" pos:[pos.x,pos.y+60] width:61 height:48 labels:#("Element", "Smth.G.", "Mat.ID's") columns:1
	on btn_block pressed do(
		bake_wrapper fn_27__bake_blockmap;
	)
	
	local pos = [180,y_start];
	button btn_ao "AO" pos:[pos.x,pos.y] width:60 height:60;
	label lbl1 "smpl." pos:[pos.x,pos.y+60] width:26 height:15
	label lbl2 "sprd." pos:[pos.x,pos.y+60+16] width:26 height:15
	label lbl22 "dist." pos:[pos.x,pos.y+60+32] width:26 height:15
	
	spinner spn_ao_1 "" pos:[pos.x+25,pos.y+60] width:35 height:16 range:[1,512,16] type:#integer
	spinner spn_ao_2 "" pos:[pos.x+25,pos.y+60+16] width:35 height:16 range:[0,100,2.5]
	spinner spn_ao_3 "" pos:[pos.x+25,pos.y+60+32] width:35 height:16 range:[0.1,256,16]
	
	
	button btn_aoPreset "preset.." pos:[pos.x,pos.y+60+48] width:60 height:15
	on btn_ao pressed do(
		bake_wrapper fn_03__bake_ao;
	)
	on btn_aoPreset pressed do(
		popUpMenu rightClickAO pos:[btn_aoPreset.pos.x,btn_aoPreset.pos.y+16] rollout:rl_texTools_bake align:#align_topleft;
	)
	
	
	
	
	
	
	local pos = [240,y_start];
	button btn_lt "LT" pos:[pos.x,pos.y] width:60 height:60;
	label lbl3 "rays" pos:[pos.x,pos.y+60] width:26 height:15
	label lbl4 "bnc." pos:[pos.x,pos.y+60+16] width:26 height:15
	spinner spn_lt_1 "" pos:[pos.x+22,pos.y+60] width:38 height:16 range:[1,512,256] type:#integer
	spinner spn_lt_2 "" pos:[pos.x+25,pos.y+60+16] width:35 height:16 range:[0,100,8] type:#integer
	button btn_ltPreset "preset.." pos:[pos.x,pos.y+60+16+16] width:60 height:15
	on btn_lt pressed do(
		bake_wrapper fn_29__bake_lighttracer;
	)
	on btn_ltPreset pressed do(
		popUpMenu rightClickLT pos:[btn_ltPreset.pos.x,btn_ltPreset.pos.y+16] rollout:rl_texTools_bake align:#align_topleft;
	)
	
	
	
	local pos = [300,y_start];
	button btn_edges "Edges" pos:[pos.x,pos.y] width:60 height:60
	spinner spn_edg_1 "thickn." pos:[pos.x+28,pos.y+60] width:32 height:16 range:[1,16,1] type:#integer
	checkbox chk_edg_1 "borders" pos:[pos.x,pos.y+60+16] width:59 height:15 checked:true
	on btn_edges pressed do(
		bake_wrapper fn_30__bake_hard_edges;
	)
	
	
	
	local y_start = 150+8;
	local pos = [0,y_start];
	button btn_cavity "Cavity" pos:[pos.x,pos.y] width:60 height:60;
	label lbl6 "gain" pos:[pos.x,pos.y+60] width:22 height:15
	spinner spn_cvt_1 "" pos:[pos.x+20,pos.y+60] width:40 height:16 range:[0,4,0.05]--the lower the value the harder the contrast
	--button btn_cvtPreset "preset" pos:[pos.x,pos.y+60+16+16] width:60 height:15
	on btn_cavity pressed do(
		bake_wrapper fn_28__bake_cavity;
	)
	
	
	
	local pos = [60,y_start];
	button btn_worn "Worn" pos:[pos.x,pos.y] width:60 height:60;
	label lbl12 "contr." pos:[pos.x,pos.y+60] width:26 height:15
	label lbl21 "pwr^" pos:[pos.x,pos.y+60+16] width:26 height:15
	spinner spn_worn_1 "" pos:[pos.x+25,pos.y+60] width:35 height:16 range:[1,512,10] type:#integer
	spinner spn_worn_2 "" pos:[pos.x+25,pos.y+60+16] width:35 height:16 range:[1,100,5] type:#integer
	on btn_worn pressed do(
		bake_wrapper fn_42__bake_worn_edges;
	)
	
	
	local pos = [120,y_start];
	button btn_cam "Camera" pos:[pos.x,pos.y] width:60 height:60;
	radiobuttons rdb_cam "" pos:[pos.x,pos.y+60] width:60 height:32 labels:#("get View", "set Tex.") columns:1
	on btn_cam pressed do(
		bake_wrapper fn_38__bake_camera_map_painting;
	)
	
	
	
	
	
	
	
	
	
	
	
	
	
	local pos = [180,y_start];
	button btn_trans "Transfer" pos:[pos.x,pos.y] width:60 height:60;
	dropdownList ddl_trans_1 "" pos:[pos.x,pos.y+60] width:32 height:21 items:#("1", "2", "3", "4") selection:1
	dropdownList ddl_trans_2 "" pos:[pos.x+29,pos.y+60] width:32 height:21 items:#("1", "2", "3", "4") selection:2
	label lbl8 "base" pos:[pos.x,pos.y+60+20] width:26 height:12
	label lbl142 "target" pos:[pos.x+30,pos.y+60+20] width:31 height:12
	button btn_trans_swap "swap" pos:[pos.x,pos.y+60+16+20] width:60 height:15
	on btn_trans_swap pressed do(
		local a = ddl_trans_1.selection;
		local b = ddl_trans_2.selection;
		ddl_trans_1.selection = b;
		ddl_trans_2.selection = a;
	)
	on btn_trans pressed do(
		bake_wrapper fn_31__bake_transfer_channels;
	)
	
	
	
	local pos = [240,y_start];
	button btn_vertColor "2VertColr." pos:[pos.x,pos.y] width:60 height:60;
	label lbl16 "Obj. Map to Vtx Color.\nNeeds Dif. Material!!" pos:[pos.x+4,pos.y+60] width:60 height:90;
	on btn_vertColor pressed do(
		bake_wrapper fn_43__bake_vertexColors;
	)
	
	/*button btn_3d "3D Map" pos:[pos.x,pos.y] width:60 height:60;
	dropdownList ddl_3d "" pos:[pos.x,pos.y+60] width:60 height:21 items:#("Chipped","Wood","Smoke") selection:1
	button btn_3dMaterial "Set as Mat." pos:[pos.x,pos.y+60+20] width:60 height:15
	button btn_3dBake "Bake Mat." pos:[pos.x,pos.y+60+20+16] width:60 height:15
	
	
	
	local pos = [300,y_start];*/
	
	
)

function bake_save_settings =(
	
	
	--local tileSize = fn_00__save_load_settings_get "general" "CheckMapTiling";
	print("save settings ");
	fn_00__save_load_settings false;--save stuff
)


if (close_texTools_bake == false)then(
	clearListener();
	CreateDialog rl_texTools_bake;
	rl_texTools_bake.chk_clipboard.checked = true;
	rl_texTools_bake.chk_saveAs.checked = false;
	rl_texTools_bake.chk_setMaterial.checked = false;
	rl_texTools_bake.chk_display.checked = false;


	local _bmpUrl = ((pathConfig.GetDir #userScripts)+"\\TexTools\\TexTools_detail_maps_thb.bmp");
	local _sliceNr;
	---
	_sliceNr = 1;
	rl_texTools_bake.btn_Block.images = #(_bmpUrl, undefined, 12,_sliceNr,_sliceNr,_sliceNr,_sliceNr );
	_sliceNr = 3;
	rl_texTools_bake.btn_AO.images = #(_bmpUrl, undefined, 12,_sliceNr,_sliceNr,_sliceNr,_sliceNr );
	_sliceNr = 2;
	rl_texTools_bake.btn_LT.images = #(_bmpUrl, undefined, 12,_sliceNr,_sliceNr,_sliceNr,_sliceNr );
	_sliceNr = 4;
	rl_texTools_bake.btn_Cavity.images = #(_bmpUrl, undefined, 12,_sliceNr,_sliceNr,_sliceNr,_sliceNr );
	_sliceNr = 5;
	rl_texTools_bake.btn_worn.images = #(_bmpUrl, undefined, 12,_sliceNr,_sliceNr,_sliceNr,_sliceNr );
	
	_sliceNr = 9;
	rl_texTools_bake.btn_mask.images = #(_bmpUrl, undefined, 12,_sliceNr,_sliceNr,_sliceNr,_sliceNr );
	_sliceNr = 6;
	rl_texTools_bake.btn_Edges.images = #(_bmpUrl, undefined, 12,_sliceNr,_sliceNr,_sliceNr,_sliceNr );
	_sliceNr = 7;
	rl_texTools_bake.btn_trans.images = #(_bmpUrl, undefined, 12,_sliceNr,_sliceNr,_sliceNr,_sliceNr );
	_sliceNr = 8;
	rl_texTools_bake.btn_cam.images = #(_bmpUrl, undefined, 12,_sliceNr,_sliceNr,_sliceNr,_sliceNr );
	_sliceNr = 10;
	rl_texTools_bake.btn_UV.images = #(_bmpUrl, undefined, 12,_sliceNr,_sliceNr,_sliceNr,_sliceNr );
	_sliceNr = 11;
	rl_texTools_bake.btn_vertColor.images = #(_bmpUrl, undefined, 12,_sliceNr,_sliceNr,_sliceNr,_sliceNr );
	
	fn_00__save_load_settings true;--load stuff
	print("load stuff!!!");
)

