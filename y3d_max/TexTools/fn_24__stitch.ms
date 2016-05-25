fn fn_24__stitch selectOnly=(
	clearListener();
	undo on(
		if (selection.count > 0)then(--at least an object selected
			local obj = selection[1]; 
			--if (classOf obj.baseObject == Editable_Poly) then (--only if this 1st object is a poly at its base
			if (superclassof obj == GeometryClass)then(
				
				if (obj.modifiers[ #unwrap_uvw ] != undefined) then(
					local uv = obj.modifiers[ #unwrap_uvw ];
					if (selectOnly== true)then(
						case uv.gettvsubobjectmode() of(--convert selection
							1: (
								uv.verttoedgeselect()
							)
							3: (
								uv.facetoedgeselect()
							)
						)
						sel_org = uv.getselectededges();
						sel = (uv.getselectededges()) as array;
						if sel.count > 0 do(
							--isolate to just border edges
							local found = false;
							for e=1 to sel.count do(
								uv.unwrap2.selectEdges #{sel[e]};
								uv.unwrap2.openEdgeSelect();
								
								local edges_sel = (uv.getselectededges()).numberset;
								if (edges_sel > 1)then(
									print("done: !!\t"+edges_sel as string+".."+(uv.getselectededges()) as string);
									--uv.unwrap2.selectEdges ((uv.getselectededges()) - #{sel[e]});
									found = true;
									exit;
								)
							)
							if (found == true)then(
								print("yes I can continue...");
								
								--uv.unwrap2.selectEdges #{1.. uv.}
								sel_border = uv.getselectededges();
									
								--select all
								uv.selectFaces #{1.. uv.numberPolygons()};
								uv.facetoedgeselect();
								sel_all =  uv.getselectededges();
									
								uv.unwrap2.selectEdges (sel_org-(sel_all-sel_border));--now contract all edges that are not part of the border
								
								uv.unwrap2.setTVSubObjectMode 2;
								uv.selectFaces #{};
							)
						)
					)else(
						uv.unwrap2.stitchVertsNoParams();
					)
				)else(
					print("no UV editor present...");
				)
			)
		)
	)
)
--fn_24__stitch false;