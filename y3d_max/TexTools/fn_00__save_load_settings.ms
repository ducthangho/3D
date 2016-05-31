function fn_00__save_load_settings doLoad =(
	--clearlistener();
	local url =(pathConfig.GetDir #userScripts)+"\\texTools.ini";
	
	if ( not doesFileExist url and not doLoad)then(--only if the file does not yet exists
		print("ini file does not exist...creating now");
		local f = createFile url;
		close f;
	)	
	if ( doesFileExist url)then(--only if the file does not yet exists
		function loadValue url sec1 sec2 dfault obj=(
			local a = getINISetting url sec1 sec2;--load the settings from the ini file and apply them to the UI
			if (a == "" or a == undefined)then(--not yet saved
				a = dfault;--apply default settings, because they are not yet stored
				print("default override for\t"+obj as string+ " = "+a as string);
			)else(
				print(classof( obj ) as string +"\t= "+a as string);
			)
			
			case classof( obj ) of(
				ColorPickerControl: 	(obj.color = execute a;)
				CheckBoxControl: 	(obj.checked = execute a;)
				EditTextControl: 		(obj.text = a as string;)
				SpinnerControl: 		(obj.value = a as Float;print("heyYa");)
				RolloutClass:(
					if (sec2 =="pos")then(SetDialogPos obj (execute a);)
				)
			)
		)
		
		
		function saveValue url sec1 sec2 dfault obj=(--save setttings from the UI to the ini file
			local val ="";
			case classof( obj ) of(
				ColorPickerControl: 	(val= obj.color as string)
				CheckBoxControl: 	(val= obj.checked as string)
				EditTextControl: 		(val= obj.text as string)
				SpinnerControl: 		(val= obj.value as string)
				RolloutClass:(
					if (sec2 =="pos")then(val= GetDialogPos rl_textureTools as string;)
				)
			)
			
			if (val == "" or val=="undefined")then(
				val = dfault;
				print("default override save for\t"+obj as string+ " = "+val as string);
			)
			format "set % | % \t\t= %\n" sec1 sec2 val
			setINISetting url sec1 sec2 val;
		)
		
		
		
		function wrap doLoad loadFn	saveFn	url sec1 sec2 dfault objStr=(
			
			--way of detecting if a rollout and a variable actually exist or are available
			local obj = undefined;
			try(
				obj = execute objStr;
			)catch();
			
			
			
			if (obj != undefined)then(--only if the rollout and variable exist in RAM
				if (doLoad)then(
					loadFn url sec1 sec2 dfault obj;
				)else(
					saveFn url sec1 sec2 dfault obj;
				)
			)
		)
		
		local l = loadValue;--load function
		local s = saveValue;--save function
		local d = doLoad;--do load boolean
		
		
		if (rl_TexTools_options != undefined)then(
			--Settings Panel
			wrap d l s	url "general" 	"bkg_color" 	"(color 81 81 81)"		"rl_TexTools_options.colorpicker_bkg_color";
			wrap d l s	url "general" 	"CheckMapTiling" 	"2"					"rl_TexTools_options.spn_checkMapTile";
			wrap d l s	url "general" 	"RoadKill_exe" 		"?"						"rl_TexTools_options.txt_rk_url";
			
			wrap d l s	url "panel" 	"snap2Editor" 		"true"						"rl_TexTools_options.chk_snapmove";
			wrap d l s	url "panel" 	"overrideColors" 	"true"						"rl_TexTools_options.chk_override";
			
		)
		
		if (rl_textureTools != undefined)then(
			wrap d l s	url "panel" 	"pos" 				"[160,81]"				"rl_textureTools";--special case
			-- TexTools toolbar
			wrap d l s	url "general" 	"tex_sizeW" 		"512"					"rl_textureTools.edt_tex_sizeW";
			wrap d l s	url "general" 	"tex_sizeH" 		"512"					"rl_textureTools.edt_tex_sizeH";
			wrap d l s	url "general" 	"padding_size" 	"2"					"rl_textureTools.edt_tex_padding";
			
			wrap d l s	url "general" 	"texel_texsize" 		"512"				"rl_textureTools.edt_texel_texsize";
			wrap d l s	url "general" 	"texel_unitsize" 		"256"				"rl_textureTools.edt_texel_perunit";
		)
		
		if (rl_texTools_bake != undefined)then(
			wrap d l s	url "bake" 	"toClipboard" 	"true"						"rl_texTools_bake.chk_clipboard";
			wrap d l s	url "bake" 	"setMaterial" 	"false"					"rl_texTools_bake.chk_setMaterial";
			wrap d l s	url "bake" 	"saveAsAsk" 	"false"					"rl_texTools_bake.chk_saveAs";
			wrap d l s	url "bake" 	"DisplayMap" 	"false"					"rl_texTools_bake.chk_display";
		)
	)
)

function fn_00__save_load_settings_get sec1 sec2 =(--get function
	local rt;--return value
	local url =(pathConfig.GetDir #userScripts)+"\\texTools.ini";
	if ( doesFileExist url)then(
		rt = getINISetting url sec1 sec2;
		if (rt != "")then(
			if ((findString rt ":\\") != undefined)then(
				rt = rt as string;
			)else if (rt != "" )then(--not yet saved
				rt = execute rt;
			)
		)else(
			rt =undefined;
		)
	)else(
		rt =undefined;
	)
	return rt;
)