fn fn_30__bake_hard_edges _sizeW _sizeH _padding _channelNr _lineThickness _withBorders=(
	clearlistener();
	undo on(
		local btmp = bitmap _sizeW _sizeH;
		
		local obj = selection[1];
		if (superclassof obj == GeometryClass)then(
			
			--01.) create a copy triangle mesh
			local tmesh = copy obj;
			convertToMesh tmesh;
			update(tmesh);
			
			tmesh.pos.x = tmesh.pos.x + 16;
			
			
			
			resetxform tmesh;--reset any transformations
			addModifier tmesh ( unwrap_uvw() );
			maxOps.CollapseNode tmesh on;

			--channel juggling, can't directly swap Object channel with UV channel
			channelInfo.CopyChannel tmesh 1 0;--copy Geometry to #0
			channelInfo.PasteChannel tmesh 3 2;--paste UVmap to #2;
			
			channelInfo.CopyChannel tmesh 3 1;--copy UVmap to #1
			channelInfo.PasteChannel tmesh 3 3;--paste UVmap to #3;
			
			channelInfo.CopyChannel tmesh 3 2;--copy UVmap to #2
			channelInfo.PasteChannel tmesh 3 1;--paste UVmap to #1
			
			channelInfo.CopyChannel tmesh 3 3;--copy UVmap to #3
			channelInfo.PasteChannel tmesh 1 0;--paste Geometry to #0;
			
			channelInfo.ClearChannel tmesh 2;--clear channel 2
			channelInfo.ClearChannel tmesh 3;--clear channel 3
			
			tmesh.scale = [_sizeW,_sizeH,1];--world space scaling
			resetxform tmesh;--reset any transformations
			
			--
			local a = 45;--atan( (2*_sizeH)  /_sizeW )*2;--camera angle
			local d =  ( _sizeW/2) / tan(a/2) ;--camera distance
			
			
			local cam = Freecamera  pos:tmesh.pos targetDistance:d fov:a mpassEnabled:off mpassRenderPerPass:off isSelected:off;	
			cam.pos.y+=_sizeH/2;
			cam.pos.x+=_sizeW/2;
			cam.pos.z+=d;
			
			--print("angle. "+a as string+" / d: "+d as string);
			
			---------------
			tmesh.material = InkNPaint();
			tmesh.material.paint_color = color 0 0 0;--black fill
			tmesh.material.paint_levels = 1;

			tmesh.material.self_overlap_ink_on = off
			tmesh.material.out_ink_on = off
			tmesh.material.matid_ink_on = off
			tmesh.material.self_underlap_ink_on = off
			tmesh.material.smgroup_edge_ink_on = on;--hard edges
			tmesh.material.smgroup_edge_ink_color = color 255 255 255;
				
			tmesh.material.min_ink_width = _lineThickness+2;
			tmesh.material.ink_quality = 3;

			--super samplling
			tmesh.material.samplerUseGlobal = off;
			tmesh.material.samplerEnable = on;
			tmesh.material.sampler = 0;
			tmesh.material.samplerQuality = 1;
			--optional shell border
			if (_withBorders)then(
				tmesh.material.out_ink_on = on;
			)else(
				tmesh.material.out_ink_on = off;
			)
			tmesh.material.color1 = color 255 255 255
			
			select tmesh;
			--render command help:
			--http://www.kxcad.net/autodesk/Autodesk_MAXScript_Reference_9/Controlling_the_Renderer.htm
			
			local url_save_path = GetDir #image + "\\_renderToClipboard.bmp"
			render rendertype:#selection camera:cam outputfile:url_save_path outputwidth:_sizeW outputheight:_sizeH antiAliasing:true vfb:false progressBar:true;
			btmp = openBitMap url_save_path;
			deleteFile url_save_path; --delete the temp. file
			
			select obj;
			
			delete tmesh;
			delete cam;
		)
		return btmp;
	)
)

--
--fn_30__bake_hard_edges 128 128 4 1 1 true;