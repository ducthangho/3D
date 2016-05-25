function fn_41__offset_overlapping_shells =(
	clearlistener();
	
	local uv = modPanel.getCurrentObject();
	if( classof(uv) == Unwrap_UVW)then(
		
		uv.unwrap2.setTVSubObjectMode 3;--face mode
		
		
		
		
		local totalFaces = uv.unwrap.numberPolygons();
		local faceElemArray = #();
		for f=1 to totalFaces do (
			faceElemArray[ f ] = 0;
		)
		local elem = #();
		--with redraw off;
		for f=1 to totalFaces do (
			if faceElemArray[ f ] == 0 then (
				uv.unwrap2.selectFaces  #{ f };
				uv.unwrap2.selectElement();
				local elemFaces = uv.unwrap2.getSelectedFaces() as array;
				append elem (uv.unwrap2.getSelectedFaces());
				for i in elemFaces do (
					faceElemArray[ i ] = elem.count; -- Mark these vertices with their element number in vertElemArray.
				)
			)
		)
		
		--get the overlapping shells
		uv.unwrap2.selectFaces #{};--select none faces, otherwise selecing overlapping faces doesn't work
		uv.unwrap5.selectOverlappedFaces();
		uv.unwrap2.selectElement();--extend to element selections
		local sel = uv.unwrap2.getSelectedFaces();
		
		
		function getBBox uv fcs =(--rethrieves a bounding box array of the passed through face bit array
			uv.unwrap2.selectFaces fcs;
			uv.unwrap2.faceToVertSelect();
			local verts =  uv.unwrap.getSelectedVertices();
			local bbox = #(0,0,0,0);
			local i = 1;
			for v in verts do(
				local pt = uv.getVertexPosition 1 v;
				if (i == 1) then(
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
			
			local x = bbox[1];
			local y = bbox[3];
			local w = bbox[2] - bbox[1];
			local h = bbox[4] - bbox[3];
			return #(x,y,w,h);--x,y,w,h
		)
		
		
		
		
		
		
		local allElemBBox = #();
		local ovrElem = #();
		for fcs in elem do(
			
			local bbox = (getBBox uv fcs);
			append allElemBBox bbox;
			
			
			local xor = sel - fcs;
			if (xor.numberset < sel.numberset)then(--this shell is part of the overlapping selection
				
				
				local pt = #(bbox[1]+bbox[3]/2.0, bbox[2]+bbox[4]/2.0);
				
				

				if (bbox[1] < 1.0 and bbox[1]+bbox[3] > 0.0)then(
					if (bbox[3] < 1.0 and bbox[3]+bbox[4] > 0.0)then(--only if within 0.0 - 1.0 box
						append ovrElem #(fcs,pt,bbox);
						Print("> "+sel.numberset as string+" , "+(xor.numberSet) as string+" = "+pt as string);
					)
				)
			)
		)
		
		print("ovrElem "+ovrElem.count as string);
		
		if (ovrElem.count == 0)then(
			
			print("stack back ");
			
			sel = #{};
			
			for e = 1 to elem.count do(
				local bbox = allElemBBox[e];
				local fcs = elem[e];
				
				
				
				if (bbox[1] > 1.0 or (bbox[1]+bbox[3]) < 0.0)then(
					sel+=fcs;
					
					local dir = bbox[1] / (abs bbox[1]) *-1;
					local shift = floor ((abs bbox[1]) / 1.0);
					if (bbox[1] < 0)then(
						shift+=1;
					)
					
					format "shift: % \n" (shift*dir)
										
					uv.unwrap2.selectFaces fcs;
					uv.unwrap2.MoveSelected [(shift*dir),0,0];
				)
				
			)
			
			uv.unwrap2.selectFaces sel;
			
			
			
			
			
			
		)else(
		
			
			
			grpElem = #();
			for i=1 to ovrElem.count do (
				
				local ovr = ovrElem[i];
				
				if (i == 1)then(
					grpElem[1] = #();--store position
					append grpElem[1] #(ovr[1],ovr[2]);--fcs, pt
				)else(
					local found = -1;
					for j=1 to grpElem.count do (
						
					--	format "dbg %,%,%,% \n" j grpElem.count grpElem[j].count 0
						local ptA = point2 ovrElem[i][2][1] ovrElem[i][2][2];
						local ptB = point2 grpElem[j][1][2][1] grpElem[j][1][2][2];
						local d = distance ptA ptB;
						if (d < 0.01)then(
							found = j;
							exit;
						)
					)
					if (found >0)then(--append to found group
						append grpElem[found] #(ovr[1],ovr[2]);
					)else(--create new group
						append grpElem #();--store position
						append grpElem[grpElem.count] #(ovr[1],ovr[2]);
					)
				)
			)
			
			sel = #{};
			for e  in grpElem do(
				
				if (e.count >= 2)then(
					for i=2 to e.count do(
						local fcs = e[i][1];
						sel+=fcs;
						uv.unwrap2.selectFaces fcs;
						uv.unwrap2.MoveSelected [1.0,0,0];
					)
				)
			)
			uv.unwrap2.selectFaces sel;
			
			print("num overlapping groups: "+grpElem.count as string);
		)	
		
	)
)

--fn_41__offset_overlapping_shells