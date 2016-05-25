rl_TexTools_uvSpline;--store in memory
try (--close if possible
	DestroyDialog rl_TexTools_uvSpline;
) catch();



function export_uv2splines _w _h dpi outlineMode addBoundBox=(

	if (selection.count > 0)then(
		local inpObj = selection[1];
		/*local _w = amax #(selection[1].max.x - selection[1].min.x, selection[1].max.y - selection[1].min.y)
		local _h = _w;*/
		local _w = _w as float;
		local _h = _h as float;
		
		
		If SuperClassof inpObj == GeometryClass then(
		
			local obj = copy inpObj;
			resetxform obj;--reset any transformations

			--addModifier obj ( unwrap_uvw() );
			maxOps.CollapseNode obj on;

			--channel juggling, can't directly swap Object channel with UV channel
			channelInfo.CopyChannel obj 1 0;--copy Geometry to #0
			channelInfo.PasteChannel obj 3 2;--paste UVmap to #2;
			channelInfo.CopyChannel obj 3 1;--copy UVmap to #1
			channelInfo.PasteChannel obj 3 3;--paste UVmap to #3;
			channelInfo.CopyChannel obj 3 2;--copy UVmap to #2
			channelInfo.PasteChannel obj 3 1;--paste UVmap to #1
			channelInfo.CopyChannel obj 3 3;--copy UVmap to #3
			channelInfo.PasteChannel obj 1 0;--paste Geometry to #0;
			channelInfo.ClearChannel obj 2;--clear channel 2
			channelInfo.ClearChannel obj 3;--clear channel 3
			
			convertToMesh obj;--will create a proper mesh with the right UV data
			convertToPoly obj;--need to convert it back to a Poly so we can access edge border mode
				
			obj.EditablePoly.SetSelection #Face #{1..obj.EditablePoly.GetNumFaces()};
			if (outlineMode == true)then(
				obj.EditablePoly.ConvertSelection #Face #Border;
			)else(
				obj.EditablePoly.ConvertSelection #Face #Edge;
			)
			local edgeSel = obj.EditablePoly.GetSelection #Edge;
			polyop.createShape obj edgeSel smooth:false name:"TexToolsUVShape" ;--node:uvShape;
			local shp = $TexToolsUVShape;--create shortcut
			
			
			--add UV bounding box
			if (addBoundBox)then(
				addNewSpline shp;
				addKnot shp (numSplines shp) #corner #line [shp.pos.x,shp.pos.y,0]
				addKnot shp (numSplines shp) #corner #line [shp.pos.x+1,shp.pos.y,0]
				addKnot shp (numSplines shp) #corner #line [shp.pos.x+1,shp.pos.y+1,0]
				addKnot shp (numSplines shp) #corner #line [shp.pos.x,shp.pos.y+1,0]
				close shp (numSplines shp);
				updateShape shp;
			)
			
			local dpiSze = units.decodeValue (dpi as string+"'");--inch units to sytem units
			local sw = _w / dpiSze;--scale width
			local sh = _h / dpiSze;--scale height
			
			shp.scale = [sw,sh,0];--UVW scale it to the desired size
			
			delete obj;
			select #(shp);
		)
	)
)





rollout rl_TexTools_uvSpline "Create UV splines" width:165 height:72
(
	button btn_create "Create Shape" pos:[85,44] width:76 height:24
	radiobuttons rdo_mode "" pos:[4,4] width:71 height:32 columns:1 labels:#("Shell Outlines", "Wireframe")
	dropdownList ddl_dpi "" pos:[90,19] width:51 height:21 items:#("75", "150", "300", "600", "1200")
	label lbl1 "dpi" pos:[145,21] width:20 height:19
	checkbox chk_bounds "Bound. Box" pos:[90,2] width:79 height:17 checked:true;
	
	on btn_create pressed do(
		if (fn_gui_getInput != undefined)then(--if texTools is loaded this function should be available frm the TexTools_x.xx.ms
		
			local TT_Inp =fn_gui_getInput();--read out the TexTools inpt width and height
			
			local dpi = ddl_dpi.items[ddl_dpi.selection] as Number;
			local bnd = chk_bounds.checked;
			local outl = false;
			if (rdo_mode.state == 1)then(
				outl = true;
			)
			--export_uv2splines _w _h dpi outlineMode addBoundBox
			export_uv2splines TT_Inp[1] TT_Inp[2] dpi outl bnd;
		)
		
	)
)
CreateDialog rl_TexTools_uvSpline;