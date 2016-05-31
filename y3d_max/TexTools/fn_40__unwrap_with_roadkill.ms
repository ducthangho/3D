--global variables
RoadKillApp;
OBJFilePath =  GetDir #export + "\\roadkillOBJ.obj"
EdgeFilePath = GetDir #export + "\\edgList.edg";


function fn_40__unwrap_with_roadkill useABFmode=(
	if (fn_00__save_load_settings == undefined)then(
		fileIn "fn_00__save_load_settings.ms"
	)
	RoadKillApp = (fn_00__save_load_settings_get "general" "RoadKill_exe") as string;--change this variable to your exe, if you are excluding this script for your own purposes
	
	fn killroad_OverwriteGWObjConfig =(
		local version = (maxVersion())[1];
		if version == 11000 then( -- max 2009 only
			
			fn fnWriteBinary dataFile data =(
				local d = fopen dataFile "wb"
				for i in data do(
					WriteByte d i
				)
				FClose  d;
			)
			
			local inputCfgPath = ((getDir #plugcfg) + "\\"+"gw_objimp.cfg")
			local outputCfgPath =  ((getDir #plugcfg) + "\\"+"gw_objexp.cfg")
			fnWriteBinary inputCfgPath #(5, 0, 0, 0, 39, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, -128, 63, 2, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 11, 0, 0, 0, 0, 0, 0, 0)
			fnWriteBinary outputCfgPath #(16, 0, 0, 0, 86, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 63, 0, 0, 1, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 46, 47, 109, 97, 112, 115, 47, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 60, 78, 79, 78, 69, 62)
			
		)else if version >= 12000 then( -- max 2010
			local iniPath_exportSettings = objExp.getIniName()
			setINISetting iniPath_exportSettings "Geometry" "FlipZyAxis" "1"
			setINISetting iniPath_exportSettings "Geometry" "Shapes" "0"
			setINISetting iniPath_exportSettings "Geometry" "ExportHiddenObjects" "0"
			setINISetting iniPath_exportSettings "Geometry" "FaceType" "2"--polygons
			setINISetting iniPath_exportSettings "Geometry" "TextureCoords" "1"
			setINISetting iniPath_exportSettings "Geometry" "Normals" "0"
			setINISetting iniPath_exportSettings "Geometry" "SmoothingGroups" "0"
			setINISetting iniPath_exportSettings "Geometry" "ObjScale" "1.000000"
			
			setINISetting iniPath_exportSettings "Output" "RelativeIndex" "0"
			setINISetting iniPath_exportSettings "Output" "Target" "0"
			setINISetting iniPath_exportSettings "Output" "Precision" "4"
			
			--new discovered settings by copypastepixel that are important for roadkill,see:
			--http://boards.polycount.net/showpost.php?p=1115492&postcount=16
			setINISetting iniPath_exportSettings "Optimize" "optVertex" "0"
			setINISetting iniPath_exportSettings "Optimize" "optNormals" "0"
			setINISetting iniPath_exportSettings "Optimize" "optTextureCoords" "0"
			
			
			local iniPath_importSettings = objImp.getIniName()
			setINISetting iniPath_importSettings "General" "ResetScene" "0"
			
			setINISetting iniPath_importSettings "Objects" "SingleMesh" "1"
			setINISetting iniPath_importSettings "Objects" "Retriangulate" "0"
			setINISetting iniPath_importSettings "Objects" "AsEditablePoly" "1"--get it back as a poly

			setINISetting iniPath_importSettings "Geometry" "SmoothingGroups" "0"
			setINISetting iniPath_importSettings "Geometry" "TextureCoords" "1"
		)
	)



	fn killroad_exportObj obj useABFmode= (
		select #(obj);
		exportFile OBJFilePath #noPrompt selectedOnly:true;
		local mode ="";
		if (useABFmode == true)then(
			mode = "-abf"
		)else(
			mode = "-lscm"
		)
		commandstring = (" \"" + OBJFilePath + "," + EdgeFilePath + ","+mode + ",-nofillholes" + ",-notlive"+"\"");--	-lscm or -abf

		local cmd = ("\""+Roadkillapp+"\""+commandstring) as string;
		format "DOSCOMMAND:\t%\n" cmd;	
			
		local val = DOSCommand ("\""+cmd+"\"");--Launch RoadKill
		--found fix @ http://forums.cgsociety.org/showthread.php?t=474829

		if val == 1 then(
			messageBox "Roadkill.exe didn't respond, is the file corrupted or URL defined in the Setup correct? Please check if the path is correct." title:"Roadkill not responding";
		)
	)

	fn killroad_importObj obj= (
		importFile OBJFilePath #noPrompt;
	)




	function killroad_saveOpenEdges obj uv=(
		uv.unwrap2.setTVSubObjectMode 2;
		--get a list of all Shells filled with the Bitarary selections for each shell
		uv.selectFaces #{1..uv.unwrap.numberPolygons()};
		uv.facetoedgeselect();--all edges are selected
		local allEdgesSelection = uv.unwrap2.getSelectedEdges();
		local edgeElemArray = #();
		for ed in allEdgesSelection do (
			edgeElemArray[ ed ] = 0;
		)
		local elem = #();
		with redraw off;
		for ed in allEdgesSelection do (
			if edgeElemArray[ ed ] == 0 then (
				uv.unwrap2.selectEdges  #{ ed };
				uv.unwrap2.selectElement();
				local elemEdges = uv.unwrap2.getSelectedEdges() as array;
				if elemEdges.count > 2 then (-- Ignore elements with less than 3 UV vertices.
					append elem (uv.unwrap2.getSelectedEdges());
					for i in elemEdges do (
						edgeElemArray[ i ] = elem.count; -- Mark these vertices with their element number in vertElemArray.
					)
				)
			)
		)

		--now get all the open Edges for all shells (elem = shell)
		local openEdgesSelection = #{};--the final open Edges selection
		for e in elem do(
			local selections = #();
			local selectionsNum = #();
			local maxNum = 0;
			for ed in e do(
				uv.unwrap2.selectEdges  #{ ed };
				uv.unwrap2.openEdgeSelect();
				if ((uv.getselectededges()).numberset > 1)then(
					local sel = ((uv.unwrap2.getSelectedEdges()) - #{ ed });--substract the original edge, kinda a bug in the openEdgeSelect method
					openEdgesSelection+= sel;--add it to the bitarray (only not yet switched bits will be drawn)
				)
			)	 
		)
		uv.unwrap2.selectEdges openEdgesSelection;

		if (openEdgesSelection.numberset > 0)then(
			local binFile = fOpen EdgeFilePath "wb";--write binary permission
			WriteShort binFile openEdgesSelection.count #unsigned
			WriteShort binFile 0 #signed -- seperator.
			uv.unwrap2.setTVSubObjectMode 1;--otherwise Geo Verts are not recognized
			for e in openEdgesSelection do(
				uv.unwrap2.selectEdges  #{ e };
				uv.edgeToVertSelect();
				local vertList = uv.unwrap5.getSelectedGeomVerts();
				if (vertList.numberSet == 2)then(
					for v in vertList do(
						writeString binFile ((v-1) as string)
					)
				)else(
					exit;--error, some messy faces somewhere, where 1 edge connect with more than 2 geo verts
				)
			)
			fClose binFile	
			print("edges saved : "+openEdgesSelection.numberset as string+"x");
		)
	)
		
	if (doesFileExist RoadKillApp)then(
		local version = (maxVersion())[1];
		if (version >= 11000) then( --at least max 2009, otherwise no support
			if (selection.count == 1)then(--at least an object selected
				
				if (classOf selection[1].baseObject == Editable_Poly)then(
					clearListener();
					if (getCommandPanelTaskMode() != #modify)then(--make sure we are in the modify panel section
						setCommandPanelTaskMode #modify;
					)
					local unwrap_modifier_before = false;
					if (classof (modPanel.getCurrentObject()) != Unwrap_UVW) then(
						modPanel.addModToSelection (Unwrap_UVW ()) ui:on;
						unwrap_modifier_before = false;
					)else(
						unwrap_modifier_before = true;--a modifier is already present
					)
					
					
					
					
					local obj = selection[1]; 
					local uv = modPanel.getCurrentObject();

					killroad_OverwriteGWObjConfig();
					killroad_saveOpenEdges obj uv;
					killroad_exportObj	obj useABFmode;
					
					if (not unwrap_modifier_before) then(
						deleteModifier obj 1;--remove the most upper modifier
					)
					killroad_importObj obj;
					
					local rObj = $roadKillObj;
					if IsValidNode rObj then(
						select #();
						select #(obj);
						addModifier obj (edit_poly());
						
						local mt = copy obj.material;
							
						obj.modifiers[#Edit_Poly].SetSelection #Face #{1..(polyop.getNumFaces obj)};
						obj.modifiers[#Edit_Poly].ButtonOp #DeleteFace;
						obj.modifiers[#Edit_Poly].Attach rObj editPolyNode:obj
						
						--set it back to the original material ID, so no confusion with the material
						obj.modifiers[#Edit_Poly].SetSelection #Face #{1..(polyop.getNumFaces obj)};
						obj.modifiers[#Edit_Poly].SetOperation #SetMaterial;
						obj.modifiers[#Edit_Poly].materialIDToSet = 0;
						obj.modifiers[#Edit_Poly].Commit();
						
						if (useABFmode == true)then(
							mode = "ABF"
						)else(
							mode = "LSCM"
						)
								
						obj.modifiers[#Edit_Poly].name =  "RoadKill UVs ["+mode+"]";
						
						obj.material = mt;--set back the wireframe color

						setCommandPanelTaskMode #create;--swap hack to update the modifier name
						setCommandPanelTaskMode #modify;
						
						print("Done. successfull!!!!");
					)else(
						messageBox "The roadkill application could not process the data from 3dsMax.\nPossible reason could be that the folder containing the Roadkill exe or the exe itself does not have priviliges to generate temporary files.\nFrom personal expierence under Windows 7 putting the roadKill folder under MyDocuments or any other user folder should do the trick." title:"Check Windows / User restrictions";
					)
				)
			)
		)	
	)else(
		messageBox "No RoadKill.exe found,\nplease define the exe location in the setup or settings dialog of TexTools.\n\nIf you don't have RoadKill you can download it free from:\n\nhttp://www.pullin-shapes.co.uk/page8.htm\n\nJust unzip the content to a folder you have access to and link to it from the Setup/ Setttings in TexTools\n" title:"No RoadKill.exe found";
	)
)

--fn_40__unwrap_with_roadkill(true);--start the RoadKill OpenEdge auto Unwrap test
