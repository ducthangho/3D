fn fn_18__swap_UVW2XYZ =(
	undo on(
		If SuperClassof $ == GeometryClass and selection.count == 1 then(
			
			local org = selection[1];
			local obj = snapshot org;

			local w = obj.max.x - obj.min.x;
			local h = obj.max.y - obj.min.y;
			local a_sze; 

			
			local _scaled = getUserProp org "texTools_swapped";
			/*if (org.modifiers[#Edit_Poly] == undefined and _scaled == true)then(
				_scaled = false;
			)*/
			if (_scaled == undefined or _scaled == false)then(
				a_sze = (w+h)/2.0;
				obj.scale = [1.0/a_sze,1.0/a_sze,1.0/a_sze];--UVW scale
				
				setUserProp org "texTools_swapped" true;
				setUserProp org "texTools_SwapScale" a_sze;
			)else(
				a_sze = (getUserProp org "texTools_SwapScale") as float;
				obj.scale = [1.0/a_sze,1.0/a_sze,1.0/a_sze];--UVW scale
				
				setUserProp org "texTools_swapped" false;
				setUserProp org "texTools_SwapScale" 1.0;--back to normal
			)
			
			
			resetxform obj;--reset any transformations
			--modPanel.addModToSelection (Unwrap_UVW ()) ui:on;
			addModifier obj ( unwrap_uvw() );
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
			
			obj.scale = [a_sze,a_sze,a_sze];--world space scaling
			convertToMesh obj;
			
			modPanel.addModToSelection (Edit_Poly ()) ui:on
			subobjectLevel = 1;
			actionMan.executeAction 0 "40021"  -- Selection: Select All
			actionMan.executeAction 0 "40020"  -- Edit: Delete Objects
			org.modifiers[#Edit_Poly].Attach obj editPolyNode:org;	
			subobjectLevel = 0;
		)
	)
)