rl_TexTools_options;
if (fn_00__save_load_settings == undefined)then(
	fileIn "fn_00__save_load_settings.ms"
)

try (--close if possible
	DestroyDialog rl_TexTools_options;
) catch();



rollout rl_TexTools_options "TexTools Settings" width:151 height:124
(
	colorPicker colorpicker_bkg_color "" pos:[2,5] width:16 height:12 color:[81,81,81]
	
	checkbox chk_override "" pos:[5,33] width:12 height:18
	checkbox chk_snapmove "" pos:[5,18] width:12 height:18
	label lbl2 "Background Color" pos:[24,4] width:96 height:12
	label lbl3 "Override 'Edit UV' Colors" pos:[24,34] width:120 height:12
	label lbl4 "Dock next to UV Editor" pos:[24,20] width:130 height:12

	spinner spn_checkMapTile "" pos:[101,57] width:44 height:16 scale:1 type:#integer range:[1,32,1]

	label lbl6 "CheckerMap tiling:" pos:[6,56] width:93 height:15
	
	label lbl5 "'Roadkill.exe' url:" pos:[6,79] width:160 height:15
	button btn_browsRK "Browse" pos:[100,78] width:46 height:18
	
	editText txt_rk_url "" pos:[2,97] width:144 height:18
	----------------------
	--checkboxes
	on rl_TexTools_options close do(
		fn_00__save_load_settings false;--save stuff
	)
	on chk_override changed arg do(
		fn_00__save_load_settings false;--save stuff
	)
	on chk_snapmove changed arg do(
		fn_00__save_load_settings false;--save stuff
	)
	---
	on spn_checkMapTile changed arg do(
		fn_00__save_load_settings false;--save stuff
	)
	--
	
	on btn_browsRK pressed do(
		f = getOpenFileName types:"RoadKill1_1.exe(*.exe)|*.exe|";
		if (f !=undefined)then(
			txt_rk_url.text = f;
			fn_00__save_load_settings false;--save stuff
		)
	)
	
)
--
CreateDialog rl_TexTools_options;
fn_00__save_load_settings true;--load it