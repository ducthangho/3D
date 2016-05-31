fn fn_25__peltmap =(
	clearListener();
	
	undo on(
		if (selection.count > 0)then(--at least an object selected
			local obj = selection[1]; 
			if (classOf obj.baseObject == Editable_Poly) then (--only if this 1st object is a poly at its base
			
				if (obj.modifiers[ #unwrap_uvw ] != undefined) then(
		
					local uv = obj.modifiers[ #unwrap_uvw ];
					
					case uv.gettvsubobjectmode() of(
						1: (uv.verttoedgeselect())
						2: (
							--if there is only 1 edge selected, loop select it
							if ( (uv.unwrap2.getSelectedEdges()).numberset == 1)then(
								uv.unwrap2.uvEdgeSelect();
							)
							
							--break the UV here
							local sel1 = uv.getselectededges();--the original cut selection
							
							--convert selection to face element because faces are the only constant array in UVW
							$.modifiers[#unwrap_uvw].unwrap2.selectElement();
							$.modifiers[#unwrap_uvw].edgetofaceselect();
							$.modifiers[#unwrap_uvw].settvsubobjectmode 3;	
							local sel2 = uv.unwrap2.getSelectedFaces();
							
							--select the original edge selection and break it there
							$.modifiers[#unwrap_uvw].settvsubobjectmode 2;
							uv.unwrap2.selectEdges sel1;	
							uv.breakSelected();	
							
							--get back the face element selection and convert it to the new edge array > bitarray
							$.modifiers[#unwrap_uvw].settvsubobjectmode 3;
							uv.unwrap2.selectFaces sel2;
							uv.facetoedgeselect();
							$.modifiers[#unwrap_uvw].settvsubobjectmode 2;

							--print("cut: new selection; "+(uv.getselectededges()).numberset as string+"\tbefore: "+sel1.numberset as string);
						)
						3: (
							uv.facetoedgeselect()
							--check if multiple elements, and if so - merge them together into 1 shell?
							-- this might be faster as the iron button and easier to repeat for the same goal = pelt mapping
							
						)
					)
					uv.unwrap2.selectElement();
					
					sel = (uv.getselectededges()) as array;
					if sel.count > 0 do(
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
							)/*else(
								--print("no "+e as string+",\t <= "+sel.count as string +" != "+(edges_sel as string));	
							)*/
						)

						
						if (found == true)then(
							
							
							--convert current openEdges to seam
							
							uv.settvsubobjectmode 2;
							uv.unwrap5.setPeltSelectedSeams #{};--clear pelt seam selection
							uv.unwrap5.peltEdgeSelToSeam true;--and replace any previous ones
							
							uv.unwrap2.selectElement();--select now the associated shell
							uv.edgetofaceselect();
							uv.settvsubobjectmode 3;
							
							
							uv.unwrap5.mappingMode 5;--enter pelt mapping mode
							uv.unwrap5.peltDialog ();--in max9 open the UV Pelt editor dialogue
							--try(
								uv.unwrap5.setPeltDialogSamples 30;--default 5
							--)catch();
							uv.unwrap5.peltDialogRun();--run for a certain time
							
							
							/*uv.unwrap3.setRelaxIteration 200;--deafult 100
							uv.unwrap3.setRelaxAmount 1;--default 0.1
							uv.unwrap3.relax2();*/
							uv.relaxByEdgeAngle 1000 0 1 false;
							
							--<boolean><Unwrap_UVW>.getPeltLockOpenEdges()
							--$.modifiers[#unwrap_uvw].unwrap5.setPeltLockOpenEdges true
							--$.modifiers[#unwrap_uvw].unwrap5.setPeltDialogSamples 5--the number of samples
							uv.unwrap5.setPeltSelectedSeams #{};--clear pelt seams


						)else(
							print("no did not work!, could not find the open edges of the shell");
						)
						
					)
					/*	
					oldsel = uv.getselectededges();
					uv.openedgeselect();
					newsel = (uv.getselectededges()) - oldsel;
					uv.selectedges newsel;
					uv.openedgeselect();
					uv.settvsubobjectmode 2;
					*/
					--
					--$.modifiers[#unwrap_uvw].unwrap5.peltEdgeSelToSeam off;

					print("ok...");

					
				)else(
					print("no UV editor present...");
				)
			)
		)
	)
)

--
--fn_25__peltmap();