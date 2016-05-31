rl_TexTools_resize;--store in memory
tt_resize_pivot_pos;
try (--close if possible
	DestroyDialog rl_TexTools_resize;
) catch();
texTools_transform_sprite = openBitMap ((pathConfig.GetDir #userScripts)+"\\TexTools\\TexTools_transform_pivot.bmp");

function gui_bmpSlice src x y w h=(
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


function fn_texTools_resize_pivot_hit pos =(
	--26 x 26
	local x = floor (pos.x / (26.0/2.0)) as integer;
	local y = floor (pos.y / (26.0/2.0)) as integer;
	local nr = (x + y*2) as integer;
	local sx = amin #(nr*26, 234-26);--shift X on the slice
	
	
	local spr = gui_bmpSlice texTools_transform_sprite sx 0 26 26;--"TexTools_transform_pivot.bmp"
	rl_TexTools_resize.btn_pivot.transparent = color 197 197 197;
	rl_TexTools_resize.btn_pivot.bitmap = spr;
	
	tt_resize_pivot_pos = nr;
		/*
	case nr of(
		--0: print("...");
		0: uv.unwrap2.snapPivot 8;--upper left
		1: uv.unwrap2.snapPivot 6;--upper right
		2: uv.unwrap2.snapPivot 2;--bottom left
		3: uv.unwrap2.snapPivot 4;--bottom right
		default: uv.unwrap2.snapPivot 8;
	)
		*/
	---
)


rcMenu rightClickSize1(
	menuItem itm1 "128"
	menuItem itm2 "256"
	menuItem itm3 "512"
	menuItem itm4 "1024"
	menuItem itm5 "2048"
	menuItem itm6 "4096"
	--actions
	on itm1 picked do (rl_TexTools_resize.spn_orgw.value = rl_TexTools_resize.spn_orgh.value = 128);
	on itm2 picked do (rl_TexTools_resize.spn_orgw.value = rl_TexTools_resize.spn_orgh.value = 256);
	on itm3 picked do (rl_TexTools_resize.spn_orgw.value = rl_TexTools_resize.spn_orgh.value = 512);
	on itm4 picked do (rl_TexTools_resize.spn_orgw.value = rl_TexTools_resize.spn_orgh.value = 1024);
	on itm5 picked do (rl_TexTools_resize.spn_orgw.value = rl_TexTools_resize.spn_orgh.value = 2048);
	on itm6 picked do (rl_TexTools_resize.spn_orgw.value = rl_TexTools_resize.spn_orgh.value = 4096);
)
rcMenu rightClickSize2(
	menuItem itm1 "128"
	menuItem itm2 "256"
	menuItem itm3 "512"
	menuItem itm4 "1024"
	menuItem itm5 "2048"
	menuItem itm6 "4096"
	--actions
	on itm1 picked do (rl_TexTools_resize.spn_w.value = rl_TexTools_resize.spn_h.value = 128);
	on itm2 picked do (rl_TexTools_resize.spn_w.value = rl_TexTools_resize.spn_h.value = 256);
	on itm3 picked do (rl_TexTools_resize.spn_w.value = rl_TexTools_resize.spn_h.value = 512);
	on itm4 picked do (rl_TexTools_resize.spn_w.value = rl_TexTools_resize.spn_h.value = 1024);
	on itm5 picked do (rl_TexTools_resize.spn_w.value = rl_TexTools_resize.spn_h.value = 2048);
	on itm6 picked do (rl_TexTools_resize.spn_w.value = rl_TexTools_resize.spn_h.value = 4096);
)



rollout rl_TexTools_resize "Extend UV's Canvas" width:164 height:143
(
	button btn_extend "Transform" pos:[71,106] width:82 height:24
	ImgTag btn_pivot "imgtag" pos:[10,73] width:26 height:26 style:#bmp_center
	spinner spn_x "x" pos:[46,70] width:48 height:16 range:[-4096,4096,0] type:#integer scale:8
	spinner spn_y "y" pos:[46,86] width:48 height:16 range:[-4096,4096,0] type:#integer scale:8
	spinner spn_h "h" pos:[105,86] width:48 height:16 range:[0,4096,128] type:#integer scale:8
	spinner spn_w "w" pos:[104,70] width:49 height:16 range:[0,4096,128] type:#integer scale:8
	spinner spn_orgw "org. w" pos:[18,15] width:69 height:16 range:[0,4096,128] type:#integer scale:8
	spinner spn_orgh "org. h" pos:[21,31] width:66 height:16 range:[0,4096,128] type:#integer scale:8
	button btn_tmpl1 "Size..." pos:[92,15] width:57 height:24
	groupBox grp3 "New UV Canvas Size" pos:[4,55] width:155 height:82
	groupBox grp4 "Current Size" pos:[4,0] width:155 height:53
	button btn_tmpl2 "Size..." pos:[10,106] width:57 height:24
	--
	on btn_pivot lbuttondown pos flags do(
		fn_texTools_resize_pivot_hit pos;
	)
	
	on btn_tmpl1 pressed do(
		popUpMenu rightClickSize1 pos:btn_tmpl1.pos rollout:rl_TexTools_resize align:#align_topleft;
	)
	on btn_tmpl2 pressed do(
		popUpMenu rightClickSize2 pos:btn_tmpl2.pos rollout:rl_TexTools_resize align:#align_topleft;
	)
	
	
	
	
	
	
	
	
	
	on btn_extend pressed do(
		--add UVW transform modifier
		local sel = deepCopy (selection as array);
		if (sel.count > 0)then(
			for obj in sel do(
		
				local _w = spn_orgw.value as float;--the original resolution
				local _h = spn_orgh.value as float;
				
				local w = spn_w.value as float;
				local h = spn_h.value as float;
				local x = spn_x.value as float;
				local y = spn_x.value as float;
				
				select #(obj);
				modPanel.addModToSelection (UVW_Xform ()) ui:on
				obj.modifiers[#UVW_Xform].U_Tile = _w / w;
				obj.modifiers[#UVW_Xform].V_Tile = _h / h;
				
					print("tt_resize_pivot_pos" +tt_resize_pivot_pos as string);
					
				case tt_resize_pivot_pos of(
					0: (
						obj.modifiers[#UVW_Xform].U_Offset = x / w;
						obj.modifiers[#UVW_Xform].V_Offset = (y+(h-_h)) / h ;
						)
					1: (
						obj.modifiers[#UVW_Xform].U_Offset = (x+(w-_w)) / w ;
						obj.modifiers[#UVW_Xform].V_Offset = (y+(h-_h)) / h ;
						)
					2: (
						obj.modifiers[#UVW_Xform].U_Offset = x / w;
						obj.modifiers[#UVW_Xform].V_Offset = y / h;
						)
					3: (
						obj.modifiers[#UVW_Xform].U_Offset = (x+(w-_w)) / w ;
						obj.modifiers[#UVW_Xform].V_Offset = y / _h;
						)
					default: uv.unwrap2.snapPivot 8;
				)
					
				rl_textureTools.edt_tex_sizeW.text = (w as integer) as string;
				rl_textureTools.edt_tex_sizeH.text = (h as integer )as string;
			)
			try (--close if possible
				DestroyDialog rl_TexTools_resize;
			) catch();
			select sel;
		)	
	)
)
CreateDialog rl_TexTools_resize;
(
	fn_texTools_resize_pivot_hit [0,0];
	if (fn_gui_getInput != undefined)then(--if texTools is loaded this function should be available frm the TexTools_x.xx.ms
		local TT_Inp =fn_gui_getInput();--read out the TexTools inpt width and height
		rl_TexTools_resize.spn_w.value = TT_Inp[1];
		rl_TexTools_resize.spn_h.value = TT_Inp[2];
		rl_TexTools_resize.spn_orgw.value = TT_Inp[1];
		rl_TexTools_resize.spn_orgh.value = TT_Inp[2];
		
	)
)