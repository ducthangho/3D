gui_bmpSlice
fn_texTools_transform_pivot_hit
fn_texTools_transform_moveFloater
fn_texTools_transform_read_values
fn_texTools_transform_selection
fn_texTools_transform_align_selection
fn_texTools_transform_snap_rotate
fn_texTools_transform_pixel_snap
texTools_transform_last;--the array for storing the UV's

filein "fn_41__offset_overlapping_shells.ms";--for the shift overlapping button
filein "fn_44__space_even.ms";--for the space evenly button

try (
	DestroyDialog rl_texTools_transform;
)catch();
texTools_transform_sprite = openBitMap ((pathConfig.GetDir #userScripts)+"\\TexTools\\TexTools_transform_pivot.bmp");
---------------------------------------------

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




function fn_texTools_transform_pivot_hit pos =(
	--26 x 26
	local x = floor (pos.x / (26.0/2.0)) as integer;
	local y = floor (pos.y / (26.0/2.0)) as integer;
	local nr = (x + y*2) as integer;
	local sx = amin #(nr*26, 234-26);--shift X on the slice
	
	
	local spr = gui_bmpSlice texTools_transform_sprite sx 0 26 26;--"TexTools_transform_pivot.bmp"
	rl_texTools_transform.btn_pivot.transparent = color 197 197 197;
	rl_texTools_transform.btn_pivot.bitmap = spr;
	
	----
	--local uv_channel = 1;
	if (selection.count > 0)then(--at least an object selected
		local obj = selection[1]; 
		local uv = modPanel.getCurrentObject();	
		if (classof(uv) == Unwrap_UVW)then(
			case nr of(
				--0: print("...");
				0: uv.unwrap2.snapPivot 8;--upper left
				1: uv.unwrap2.snapPivot 6;--upper right
				2: uv.unwrap2.snapPivot 2;--bottom left
				3: uv.unwrap2.snapPivot 4;--bottom right
				default: uv.unwrap2.snapPivot 8;
			)
			texTools_transform_last[6] = nr;--store this one
		)
	)
	---
	fn_texTools_transform_read_values();
)
---
function fn_texTools_transform_moveFloater = (--move aside to the edit UNwrap window and checks if it needs to be closed
	if (getCommandPanelTaskMode() == #modify)then(--make sure we are in the modify panel section
			
		if classof (modPanel.getCurrentObject()) == Unwrap_UVW then(
			local uv = modpanel.getcurrentobject();
			local uvX =  uv.GetWindowX();
			local uvY =  uv.GetWindowY();
			local uvH =  uv.GetWindowH();
			local uvW =  uv.GetWindowW();
			
			local tx = uvX;--target X
			local ty = uvY - 39;
			
			
			local pos = getDialogPos rl_texTools_transform;
			if (pos.x != tx or pos.y != ty )then(
				if (uvW > 0 and uvW < 1600) then(
					SetDialogPos rl_texTools_transform [tx,ty];
				)else(
					print("!! KILL TexTools Selection Transform Floater");
					fn_texTools_transform_openClose 2;
				)
			)
		)else(
			print("!! KILL TexTools Selection Transform Floater");
			fn_texTools_transform_openClose 2;
		)
	)else(
		print("!! KILL TexTools Selection Transform Floater");
		fn_texTools_transform_openClose 2;
	)
)





function fn_texTools_transform_read_values =(--update and read values from the current selection and place them in the floater
	--local uv_channel = 1;
	if (selection.count > 0)then(--at least an object selected
		--local obj = selection[1]; 
		
		
		function read_transform_perNode uv obj firstRun bbox rl_texTools_transform texTools_transform_last mode=(
			
			--local obj = selection
			local uvW =  uv.GetWindowW();
			if ( uvW > 0 and uvW < 1600)then(--determine if the edit window is open
				local verts;
				if ((maxVersion())[1] >= 10000 )then(--Max 2008+
					verts = uv.unwrap6.getSelectedVerticesByNode obj;
				)else(
					verts = uv.unwrap.getSelectedVertices();
				)
				print("verts >>"+verts as string+"<<");
				--
				if ( verts != texTools_transform_last[5]  and verts != undefined)then(
					texTools_transform_last[5] =verts;

					local i = 1;
					for v in verts do(
					
						if ((maxVersion())[1] >= 10000 )then(--Max 2008+
							local pt = uv.unwrap6.GetVertexPositionByNode 1 v obj
						)else(
							local pt = uv.getVertexPosition 1 v;
						)

						if (i == 1 and firstRun == true) then(
							print("first");
							bbox[1] = bbox[2] = pt.x;
							bbox[3] = bbox[4] = pt.y;
						)else(
							bbox[1] = amin #(bbox[1],pt.x);--minX
							bbox[2] = amax #(bbox[2],pt.x);--maxX
							bbox[3]  = amin #(bbox[3],pt.y);--minY
							bbox[4]  = amax #(bbox[4],pt.y);--maxY
						)
						i+=1;
					)
				)else(
					uv.unwrap2.setTVSubObjectMode mode;
				)
				---------------------------
			)
		)

		
		
		local uv = modPanel.getCurrentObject();	
		if (classof(uv) == Unwrap_UVW)then(
			local bbox = #(0.0,0.0,0.0,0.0);--bounding box
			local rt = rl_texTools_transform;--rollout
			local width = rl_textureTools.edt_tex_sizeW.text as integer;
			local height = rl_textureTools.edt_tex_sizeH.text as integer;
			local mode = uv.unwrap2.getTVSubObjectMode();
			if (mode == 2)then(
				uv.unwrap2.edgeToVertSelect();
			)else if (mode == 3)then(
				uv.unwrap2.faceToVertSelect();
			)
			uv.unwrap2.setTVSubObjectMode 1;
			if ((maxVersion())[1] >= 10000 )then(--Max 2008+ multi object UV support
				local first = true;
				for s in selection do(
					read_transform_perNode uv s first bbox rl_texTools_transform texTools_transform_last mode;
					first = false;
				)
			)else(
				read_transform_perNode uv selection[1] true bbox rl_texTools_transform texTools_transform_last mode;
			)
			

			
			
			local w = (bbox[2] - bbox[1]) as Float;
			local h =  (bbox[4] - bbox[3]) as Float;
			case (texTools_transform_last[6]) of(
				0: (
					--default Bitmap/ Screen Orientation (i.e Photoshop & Flash)
					rt.spn_x.value = texTools_transform_last[1]= bbox[1]*width;--left-x
					rt.spn_y.value = texTools_transform_last[2]= (1.0-bbox[4])*height;--top
				)
				1: (
					rt.spn_x.value = texTools_transform_last[1]= (1-bbox[2])*width;--right-x
					rt.spn_y.value = texTools_transform_last[2]= (1.0-bbox[4])*height;--top
				)
				2: (
					--default max UV setup, lower left corner
					rt.spn_x.value = texTools_transform_last[1]= bbox[1]*width;--left-x
					rt.spn_y.value = texTools_transform_last[2]= bbox[3]*height;--bottom
				)
				3: (
					rt.spn_x.value = texTools_transform_last[1]= (1-bbox[2])*width;--right-x
					rt.spn_y.value = texTools_transform_last[2]= bbox[3]*height;--bottom
				)
				default:()
			)
			rt.spn_w.value = texTools_transform_last[3]= w*width;
			rt.spn_h.value = texTools_transform_last[4]=  h*height;

			----------------------
			uv.unwrap2.setTVSubObjectMode mode;
			
			case (texTools_transform_last[6]) of(
				0: uv.unwrap2.snapPivot 8;--upper left
				1: uv.unwrap2.snapPivot 6;--upper right
				2: uv.unwrap2.snapPivot 2;--bottom left
				3: uv.unwrap2.snapPivot 4;--bottom right
				default: uv.unwrap2.snapPivot 8;
			)
		)
	)
)






function fn_texTools_transform_pixel_snap texSizeW texSizeH = (
	clearlistener();

	if (selection.count >= 1)then(
	
		function snapToPixelsNode obj uv texSizeW texSizeH= (
			
			
			local verts = uv.unwrap6.getSelectedVerticesByNode obj;
			
			for v in verts do(
				local pt = uv.unwrap6.GetVertexPositionByNode 1 v obj;
				
				
				local modX = mod  ((pt.x as float+80.0) *texSizeW as float ) 1.0;
				if (modX < 0.5)then(
					pt.x = floor ((pt.x*texSizeW as float) ) / texSizeW as float;
				)else(
					pt.x = ceil ((pt.x*texSizeW as float) ) / texSizeW as float;
				)
				
				local modY = mod  ((pt.y as float+80.0) *texSizeH as float ) 1.0;
				if (modY < 0.5)then(
					pt.y = floor ((pt.y*texSizeH as float) ) / texSizeH as float;
				)else(
					pt.y = ceil ((pt.y*texSizeH as float) ) / texSizeH as float;
				)

				uv.unwrap6.setVertexPositionByNode 1 v pt obj;
			)
		)
		
		--run on all selected objects
		undo on(
			local uv = modPanel.getCurrentObject();
			
			local _mode = uv.unwrap2.getTVSubObjectMode();
			if (_mode == 2)then(
				uv.unwrap2.edgeToVertSelect();
			)else if (_mode == 3)then(
				uv.unwrap2.faceToVertSelect();
			)
			
			
			for o in selection do(
				snapToPixelsNode o uv texSizeW texSizeH;
			)
		)
		--fn_texTools_transform_read_values();
	)
)



function fn_texTools_transform_selection val type=(--absolute Position of the bounding box to be
	
	local rt = rl_texTools_transform;--rollout

	--local uv_channel = 1;
	if (selection.count > 0)then(--at least an object selected
		local obj = selection[1]; 
		local uv = modPanel.getCurrentObject();	
		if (classof(uv) == Unwrap_UVW)then(
			--print("continue...");
			
			local width = rl_textureTools.edt_tex_sizeW.text as float;
			local height = rl_textureTools.edt_tex_sizeH.text as float;
			
			----------------------
			
			case type of(
				1: (
					--transform X-slider
					local x = ((val as float) - (texTools_transform_last[1])) ;
					texTools_transform_last[1]+=x;
					
					case (texTools_transform_last[6]) of(
						0: (uv.unwrap2.moveSelected  [x/width,0,0];)
						1: (uv.unwrap2.moveSelected  [-x/width,0,0];)
						2: (uv.unwrap2.moveSelected  [x/width,0,0];)
						3: (uv.unwrap2.moveSelected  [-x/width,0,0];)
						default:()
					)
				)
				2: (
					--transform Y-slider
					fn_texTools_transform_read_values();--read out exact values
					local y = ((val as float) - (texTools_transform_last[2]));
					
					case (texTools_transform_last[6]) of(
						0: (uv.unwrap2.moveSelected  [0,-y/height,0];)
						1: (uv.unwrap2.moveSelected  [0,-y/height,0];)
						2: (uv.unwrap2.moveSelected  [0,(y/height),0];)
						3: (uv.unwrap2.moveSelected  [0,(y/height),0];)
						default:()
					)
					
				)
				3: (
					local s = (val as float) / (texTools_transform_last[3] as float ) ;
					texTools_transform_last[3]=val;
					
					uv.unwrap2.scaleSelectedCenter ( s  ) 1;--just x axis scale
					local tmp_x = texTools_transform_last[1] as float;
					fn_texTools_transform_read_values();--read out exact values
					
					case (texTools_transform_last[6]) of(
						0: (
							local x = (tmp_x - texTools_transform_last[1] )/width;
							uv.unwrap2.moveSelected  [ x,0,0];
						)
						1: (
							local x = (tmp_x - texTools_transform_last[1] )/width;
							uv.unwrap2.moveSelected  [ -x,0,0];
						)
						2: (
							local x = (tmp_x - texTools_transform_last[1] )/width;
							uv.unwrap2.moveSelected  [ x,0,0];
						)
						3: (
							local x = (tmp_x - texTools_transform_last[1] )/width;
							uv.unwrap2.moveSelected  [ -x,0,0];
						)
						default:()
					)
				)
				4: (
					local s = (val as float / texTools_transform_last[4] as float ) ;
					texTools_transform_last[4]=val;
					
					uv.unwrap2.scaleSelectedCenter ( s  ) 2;--just x axis scale
					local tmp_y = texTools_transform_last[2] as float;
					fn_texTools_transform_read_values();--read out exact values

					case (texTools_transform_last[6]) of(
						0: (
							local y = (tmp_y - texTools_transform_last[2] )/height;
							uv.unwrap2.moveSelected  [ 0,-y,0];
						)
						1: (
							local y = (tmp_y - texTools_transform_last[2] )/height;
							uv.unwrap2.moveSelected  [ 0,-y,0];
						)
						2: (
							local y = (tmp_y - texTools_transform_last[2] )/height;
							uv.unwrap2.moveSelected  [ 0,y,0];
						)
						3: (
							local y = (tmp_y - texTools_transform_last[2] )/height;
							uv.unwrap2.moveSelected  [ 0,y,0];
						)
						default:()
					)
				)
				default:()
			)
			fn_texTools_transform_read_values();
		)
	)
)


function fn_texTools_transform_align_selection type=(
	if (selection.count > 0)then(--at least an object selected
		local uv = modPanel.getCurrentObject();	
		if (classof(uv) == Unwrap_UVW)then(
			local width = rl_textureTools.edt_tex_sizeW.text as float;
			local height = rl_textureTools.edt_tex_sizeH.text as float;
			
			--undo on(
			--texTools_transform_last[2]
			local obj = selection[1]; 
			local _mode = uv.unwrap2.getTVSubObjectMode();
			
			if (_mode == 3)then(--align shells
				
				
				
				
				
				
				
				
				
				
				uv.unwrap2.selectElement();	
				uv.unwrap2.faceToVertSelect();
				local original_vertex_selection = uv.unwrap.getSelectedVertices();
				local numVertexSelected = #{1..(polyop.getNumMapVerts obj 1)};
				
				local vertElemArray = #();
				for v in numVertexSelected do (
					vertElemArray[ v ] = 0;
				)
				local elementsCount = 0;
				local elemVerts = #{};
				uv.unwrap2.setTVSubObjectMode 1;
				local elem = #();
				with redraw off;
				for v in original_vertex_selection do (
					if vertElemArray[ v ] == 0 then (
						uv.selectVertices #{ v }
						uv.unwrap2.selectElement();
						elemVerts = uv.unwrap.getselectedvertices() as array;
						if elemVerts.count > 2 then (-- Ignore elements with less than 3 UV vertices.
							elementsCount += 1;
							elem[elementsCount] = uv.unwrap.getselectedvertices();--elemVerts;
							for i in elemVerts do (
								vertElemArray[ i ] = elementsCount; -- Mark these vertices with their element number in vertElemArray.
							)
						)
					)
				)
				
				uv.selectVertices original_vertex_selection;
				
				case type of(
					1: (
						print("Align to the left");
						
						fn_texTools_transform_pivot_hit [0,0];
						fn_texTools_transform_read_values();--read out exact values
						local x = texTools_transform_last[1];
						for el in elem do(
							uv.selectVertices el;
							fn_texTools_transform_read_values();--read out exact values
							fn_texTools_transform_selection x 1;
						)
					)
					2: (
						print("Align to the right");
						
						fn_texTools_transform_pivot_hit [25,0];
						fn_texTools_transform_read_values();--read out exact values
						local x = texTools_transform_last[1];
						for el in elem do(
							uv.selectVertices el;
							fn_texTools_transform_read_values();--read out exact values
							fn_texTools_transform_selection x 1;
						)
					)
					3: (
						print("Align to the top");
						
						fn_texTools_transform_pivot_hit [0,0];
						fn_texTools_transform_read_values();--read out exact values
						local y = texTools_transform_last[2];
						for el in elem do(
							uv.selectVertices el;
							fn_texTools_transform_read_values();--read out exact values
							fn_texTools_transform_selection y 2;
						)
					)
					4: (
						print("Align to the bottom");
						
						fn_texTools_transform_pivot_hit [0,25];
						fn_texTools_transform_read_values();--read out exact values
						local y = texTools_transform_last[2];
						for el in elem do(
							uv.selectVertices el;
							fn_texTools_transform_read_values();--read out exact values
							fn_texTools_transform_selection y 2;
						)
					)
					default:()
				)
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				

			)else(--align verts/ edges
				if (_mode == 2)then(
					uv.unwrap2.edgeToVertSelect();
				)
				case type of(
					1: (
						print("Align to the left");
						fn_texTools_transform_pivot_hit [0,0];
						fn_texTools_transform_read_values();--read out exact values
						fn_texTools_transform_selection 0 3;
					)
					2: (
						print("Align to the right");
						fn_texTools_transform_pivot_hit [25,0];
						fn_texTools_transform_read_values();--read out exact values
						fn_texTools_transform_selection 0 3;
					)
					3: (
						print("Align to the top");
						fn_texTools_transform_pivot_hit [0,0];
						fn_texTools_transform_read_values();--read out exact values
						fn_texTools_transform_selection 0 4;
					)
					4: (
						print("Align to the bottom");
						fn_texTools_transform_pivot_hit [0,25];
						fn_texTools_transform_read_values();--read out exact values
						fn_texTools_transform_selection 0 4;
					)
					default:()
				)
			)
			uv.unwrap2.setTVSubObjectMode _mode;
			
				
			--fn_texTools_transform_read_values();	
				
				
				
				
				
				
				
				
				
			--)
		)
	)
)


function fn_texTools_transform_snap_rotate direction=(--rotate the selection while keeping the Pivots Bounding Box selection position
	clearlistener();
	if (selection.count > 0)then(
		local uv = modPanel.getCurrentObject();--the selection in the mod panel,- the new way to select multiple objects in max2008+ to multi unwrap
		if classof (uv) == Unwrap_UVW then(
			undo on(
				fn_texTools_transform_read_values();--get bounding box Information
				
				local x = texTools_transform_last[1];
				local y = texTools_transform_last[2];
				
				local a = (90*direction) * PI / 180;
				uv.unwrap2.rotateSelectedCenter a;
				
				fn_texTools_transform_read_values();--update BBox values 
				
				fn_texTools_transform_selection x 1;--move X
				fn_texTools_transform_selection y 2;--move Y
				
				fn_texTools_transform_read_values();
			)
		)
	)
)




rl_texTools_transform;
texTools_transform_last = #();
----------

rollout rl_texTools_transform "TexTools Transform Selection" width:432 height:37
(
	ImgTag btn_pivot "imgtag" pos:[3,4] width:26 height:26 style:#bmp_center

	spinner spn_x "" pos:[39,1] width:48 height:16 range:[-4096,4096,0] type:#integer
	spinner spn_y "" pos:[39,17] width:48 height:16 range:[-4096,4096,0] type:#integer
	spinner spn_h "" pos:[98,17] width:48 height:16 range:[0,4096,128] type:#integer
	spinner spn_w "" pos:[98,1] width:48 height:16 range:[0,4096,128] type:#integer
	-------
	label lbl3 "h" pos:[90,17] width:8 height:13
	label lbl5 "w" pos:[89,1] width:8 height:13
	label lbl1 "x" pos:[33,1] width:5 height:13
	label lbl2 "y" pos:[33,17] width:5 height:13

	label lbl6 "Align Shells / Verts" pos:[150,-1] width:120 height:13
	button btn_align_1 "" pos:[149,12] width:22 height:22 toolTip:"Align shells (sel. faces) or just selected verts to the left"
	button btn_align_2 "" pos:[218,12] width:22 height:22
	button btn_align_3 "" pos:[172,12] width:22 height:22
	button btn_align_4 "" pos:[195,12] width:22 height:22
	---------
	Timer enterFrame "testClock" pos:[250,6] width:24 height:24 interval:500 --tick once a second
	---------
	label lbl7 "Rotate" pos:[249,-1] width:88 height:13
	button btn_snapRotate1 "" pos:[248,12] width:22 height:22
	button btn_snapRotate2 "" pos:[271,12] width:22 height:22
	---------
	label lbl8 "Spc." pos:[249+52,-1] width:88 height:13
	button btn_spaceEvenly "" pos:[248+52,12] width:22 height:22

	label lbl9 "PixSnap" pos:[295+30,-1] width:88 height:13
	button btn_snapPixels "" pos:[295+30,12] width:22 height:22
	button btn_shiftOver "Shift Overlap" pos:[342+12,12] width:74 height:22
	---------
	

	
	on rl_texTools_transform moved pos do
	(
			--when the dialog is moved
			fn_texTools_transform_read_values();
		)
	on btn_spaceEvenly pressed do
		fn_44__space_even();--space evenly
	on btn_pivot lbuttondown pos flags do
		fn_texTools_transform_pivot_hit pos;
	on spn_x changed val do
		fn_texTools_transform_selection val 1;
	on spn_x buttondown do
		fn_texTools_transform_read_values();
	on spn_x buttonup do
		fn_texTools_transform_read_values();
	on spn_y changed val do
		fn_texTools_transform_selection val 2;
	on spn_y buttondown do
		fn_texTools_transform_read_values();
	on spn_y buttonup do
		fn_texTools_transform_read_values();
	on spn_h changed val do
		fn_texTools_transform_selection val 4;
	on spn_h buttondown do
		fn_texTools_transform_read_values();
	on spn_h buttonup do
		fn_texTools_transform_read_values();
	on spn_w changed val do
		fn_texTools_transform_selection val 3;
	on spn_w buttondown do
		fn_texTools_transform_read_values();
	on spn_w buttonup do
		fn_texTools_transform_read_values();
	on btn_align_1 pressed do
	(
		fn_texTools_transform_align_selection 1;
	)
	on btn_align_2 pressed do
	(
		fn_texTools_transform_align_selection 2;
	)
	on btn_align_3 pressed do
	(
		fn_texTools_transform_align_selection 3;
	)
	on btn_align_4 pressed do
	(
		fn_texTools_transform_align_selection 4;
	)
	on enterFrame tick do
	(
		fn_texTools_transform_moveFloater();
	)
	on btn_snapRotate1 pressed do
	(
		fn_texTools_transform_snap_rotate 1;--rotate counter clockwise 90
	)
	on btn_snapRotate2 pressed do
	(
		fn_texTools_transform_snap_rotate -1;--rotate counter clockwise 90
	)
	on btn_snapPixels pressed do
	(
		fn_texTools_transform_pixel_snap (rl_textureTools.edt_tex_sizeW.text as integer) (rl_textureTools.edt_tex_sizeH.text as integer);
	)
	on btn_shiftOver pressed do
	(
		fn_41__offset_overlapping_shells();--shift double faces +1.0 U
	)
	

)
-------



clearListener();
function fn_texTools_transform_openClose actionState =(
	
	/*
	* actionState:
	* 0 = auto swap state
	* 1 = force open
	* 2 = force close
	*/
	
	local autoOpen = false;
	if (rl_texTools_transform != undefined)then(
		if (rl_texTools_transform.open == false)then(
			autoOpen = true;
		)else(
			autoOpen = false;
			try (
				DestroyDialog rl_texTools_transform;
			)catch();
		)
	)else(
		autoOpen = true;
	)
	
	
	if ((actionState == 0 and autoOpen) or actionState == 1)then(
	
		--rl_texTools_detail_maps.open = true;
		if (rl_textureTools != undefined)then(
			
			texTools_transform_last[1] = 0.0;--x
			texTools_transform_last[2] = 0.0;--y
			texTools_transform_last[3] = 0.0;--w
			texTools_transform_last[4] = 0.0;--h
			texTools_transform_last[5] = #{};--bit array selection to check if the selection changed to reduce loops
			texTools_transform_last[6] = 0;--last pivot 4-slice position (0-3, 0-based)
			
			
			local TTpos = GetDialogPos rl_textureTools;--GetDialogPos rl_textureTools
			local _x = TTpos.x+64;
			local _y = TTpos.y+400;

			--CreateDialog rl_texTools_transform 295 34 pos:[_x,_y] style:#(#style_border);
			CreateDialog rl_texTools_transform pos:[_x,_y] style:#(#style_border);
			SetDialogPos rl_texTools_transform [_x,_y];

			--map the buttons:
			--texTools_transform_sprite
			local rl = rl_texTools_transform;
		
			if ((maxVersion())[1] < 12000)then(--below max 2010
				local sprite = (gui_bmpSlice texTools_transform_sprite 104 0 20 20) as bitmap;
				rl.btn_align_1.images = #(sprite, undefined, 1,1,1,1,1 );

				local sprite = (gui_bmpSlice texTools_transform_sprite (104+20) 0 20 20) as bitmap;
				rl.btn_align_2.images = #(sprite, undefined, 1,1,1,1,1 );

				local sprite = (gui_bmpSlice texTools_transform_sprite (104+40) 0 20 20) as bitmap;
				rl.btn_align_3.images = #(sprite, undefined, 1,1,1,1,1 );

				local sprite = (gui_bmpSlice texTools_transform_sprite (104+60) 0 20 20) as bitmap;
				rl.btn_align_4.images = #(sprite, undefined, 1,1,1,1,1 );
				
				local sprite = (gui_bmpSlice texTools_transform_sprite (104+80) 0 20 20) as bitmap;
				rl.btn_snapRotate2.images = #(sprite, undefined, 1,1,1,1,1 );
				local sprite = (gui_bmpSlice texTools_transform_sprite (104+100) 0 20 20) as bitmap;
				rl.btn_snapRotate1.images = #(sprite, undefined, 1,1,1,1,1 );
				
				local sprite = (gui_bmpSlice texTools_transform_sprite (104+120) 0 20 20) as bitmap;
				rl.btn_spaceEvenly.images = #(sprite, undefined, 1,1,1,1,1 );
				local sprite = (gui_bmpSlice texTools_transform_sprite (104+140) 0 20 20) as bitmap;
				rl.btn_snapPixels.images = #(sprite, undefined, 1,1,1,1,1 );
				
				
			)else(
				local sprite = (gui_bmpSlice texTools_transform_sprite 104 0 20 20) as bitmap;
				rl.btn_align_1.images = #(sprite, undefined, 1,1,1,1,1,true );

				local sprite = (gui_bmpSlice texTools_transform_sprite (104+20) 0 20 20) as bitmap;
				rl.btn_align_2.images = #(sprite, undefined, 1,1,1,1,1,true );

				local sprite = (gui_bmpSlice texTools_transform_sprite (104+40) 0 20 20) as bitmap;
				rl.btn_align_3.images = #(sprite, undefined, 1,1,1,1,1,true );

				local sprite = (gui_bmpSlice texTools_transform_sprite (104+60) 0 20 20) as bitmap;
				rl.btn_align_4.images = #(sprite, undefined, 1,1,1,1,1,true );
				
				local sprite = (gui_bmpSlice texTools_transform_sprite (104+80) 0 20 20) as bitmap;
				rl.btn_snapRotate2.images = #(sprite, undefined, 1,1,1,1,1,true );
				local sprite = (gui_bmpSlice texTools_transform_sprite (104+100) 0 20 20) as bitmap;
				rl.btn_snapRotate1.images = #(sprite, undefined, 1,1,1,1,1,true );
				
				local sprite = (gui_bmpSlice texTools_transform_sprite (104+120) 0 20 20) as bitmap;
				rl.btn_spaceEvenly.images = #(sprite, undefined, 1,1,1,1,1,true );
				local sprite = (gui_bmpSlice texTools_transform_sprite (104+140) 0 20 20) as bitmap;
				rl.btn_snapPixels.images = #(sprite, undefined, 1,1,1,1,1,true );
				
			)
			
			fn_texTools_transform_pivot_hit [0,0];
			fn_texTools_transform_read_values();
			fn_texTools_transform_moveFloater();
		)
	)else(
	)
	---check if it needs to be closed or restarted
)
--fn_texTools_transform_openClose 2;--close at all costs shoudl it already exist

fn_texTools_transform_openClose 1;--close at all costs shoudl it already exist