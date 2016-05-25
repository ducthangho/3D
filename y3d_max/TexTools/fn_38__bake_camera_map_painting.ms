function fn_38__bake_camera_map_painting doExport _sizeW _sizeH _padding=(
	clearListener();
	--undo on(
	
	local btmp = bitmap _sizeW _sizeH;
	local maskColor = (color 255 0 255 255);
	
	
	local cam = undefined;
	local url	=  GetDir #image + "\\_renderToClipboard.bmp";--where the image will be stored temporarily
	local url_mask	=  GetDir #image + "\\_renderToClipboardMask.bmp";--where the image will be stored temporarily

	
	if ((maxVersion())[1] >= 10000)then(
		if (selection.count == 1 )then(--only max2008+ 
			local obj = selection[1];
		
			if (superClassOf  obj == GeometryClass)then(
				
				
				if (doExport)then(--create a temp camera from the view
					--#view_iso_user
					local size = getViewSize();
					renderWidth = size[1];
					renderHeight = size[2];	
					
					cam = $textools_camera;
					if (viewport.getType() == #view_persp_user) then(
						if (cam == undefined)then(--create new camera if not already existing
							cam = Freecamera targetDistance:50 isSelected:off
						)
						cam.type = #free;
						cam.fov = getVIewFOV()
						cam.Transform = Inverse(viewport.getTM())
					)else if ( viewport.getType()== #view_camera)then(
						if (cam == undefined)then(
							maxOps.cloneNodes (viewport.getCamera()) cloneType:#copy newNodes:&temp;
							cam = temp[1];
						)else(
							cam.type = #free;
							cam.transform = (viewport.getCamera()).transform;
							cam.fov = (viewport.getCamera()).fov;
						)
					)
					
					if (classof cam == Freecamera)then(
						local dir = cam.dir;
						local pos = cam.position;
						--cam.type = #target;
						--cam.target.pos = (-dir * gw.GetFocalDist() as float) + pos/2;
					)
					
					
					
					--viewport.setCamera vc
					if (cam != undefined)then(
						print("cam: "+cam as string+" \ntype: "+classof cam as string);
						--convert to target camera type (for matching the target spot light to mask out invisible stuff in the RTT process)
					

						if (superclassof cam == camera)then(
							cam.showCone = true;							
							cam.name = "textools_camera";
							clearSelection();--select none
							

							

							
							
							viewport.setCamera cam;
							local formerViewSettings = #();
							append formerViewSettings (viewport.getGridVisibility (viewport.activeViewport));
							append formerViewSettings (viewport.getRenderLevel());
							append formerViewSettings (viewport.GetShowEdgeFaces());
							append formerViewSettings (viewport.GetTransparencyLevel());
							
							viewport.setGridVisibility #all false;
							viewport.SetRenderLevel #flat;
							viewport.SetShowEdgeFaces false;
							viewport.SetTransparencyLevel 0;

							completeRedraw()
							
							local size = getViewSize();
							print("continue with script"+size as string);
							
							local grab = gw.getViewportDib();
							--display grab; 
							/*
							grab.filename = url;
							save grab quiet:true;
							close grab;*/
							/*local clipboardClass = dotNetClass "System.Windows.Forms.Clipboard" --create a Clipboard dotNetClass
							local theImage = dotNetClass "System.Drawing.Image" --create an Image dotNetClass
							local theBitmap = theImage.FromFile  url--get the saved image from file as bitmap
							clipboardClass.setImage theBitmap; --copy the image to clipboard
							theBitmap.Dispose() --release the bitmap 
							*/
							select obj;
							actionMan.executeAction 0 "40182"  -- Views: Perspective User View

							--restore previous view settings
							viewport.setGridVisibility #all formerViewSettings[1];
							viewport.SetRenderLevel formerViewSettings[2];
							viewport.SetShowEdgeFaces formerViewSettings[3];
							viewport.SetTransparencyLevel formerViewSettings[4];
							/*
							btmp = openBitMap url;
							deleteFile url_save_path; --delete the temp. file
			*/
							return grab;--return this as output bitmap
							
							
							
							
							
							
							--return true;--success, return this so that radiobutton swap can be updated outside this function
						)
					)
				)else(

					local grab = getclipboardBitmap();
					if (grab != undefined)then(
						
						--display grab;
						cam = $textools_camera;
						if (classof cam == Freecamera)then(--cam was not deleted and broke
							
							disableSceneRedraw();
							

							
							grab.filename = url;
							save grab quiet:true;		
							close grab;
							renderWidth = grab.width;--needed for the camera map material	
							renderHeight = grab.height;	
							
							objCopy = snapshot obj;
							convertToMesh objCopy;
							objCopy.material = standard diffuseMap:(Camera_Map_Per_Pixel ()) showInViewport:false;
							objCopy.material.diffusemap.camera = cam;
							objCopy.material.diffuse = maskColor;
							objCopy.material.selfIllumAmount = 100
							objCopy.material.diffusemap.texture  = Bitmaptexture fileName:url;
							
							--delete not visible faces
							local visibleFaces=#{}
							local a, b, c, d;
							d = cos(100);
							a = cam.dir;
							for i = 1 to objCopy.numFaces do(
								b=getFaceNormal objCopy i	-- Normalized vector.
								c=dot a b;
								if c>=d then(
									append visibleFaces i;
								)
							)
							--delete not visible faces
							meshop.deleteFaces objCopy (#{1..objCopy.numFaces} - visibleFaces);--delete inverse visible faces

							
							local dif_bake = DiffuseMap();
							dif_bake.outputSzX    =    _sizeW;
							dif_bake.outputSzY    =    _sizeH;
							dif_bake.autoSzOn     = false;
							dif_bake.filterOn	 = true;
							dif_bake.elementName  = "DiffuseMap";
							dif_bake.enabled      = true;
							dif_bake.backgroundColor = maskColor;
							dif_bake.fileName = url;--just in case for 64-bit machines
							dif_bake.fileType = url;--appearently the target filename/path
							
							-- Baking job
							bake = objCopy.INodeBakeProperties;
							bake.bakeChannel = 1;--_channelNr;
							bake.nDilations  = _padding;-- (Pading)
							bake.flags       = 1; --bit 1 of flag will be set to signify map channel conflict
							bake.bakeEnabled  = true;
							bake.addBakeElement dif_bake;
							--bake.addBakeElement lght_bake;
							
							select #(objCopy);
							render rendertype:#bakeSelected outputwidth:_sizeW outputheight:_sizeH antiAliasing:true vfb:false progressBar:true;		
							
							--remove traces
							bake.removeBakeElement dif_bake;--remove the dif bake entry
							
							--copy to clipoard and delete temp image
							/*
							print("saved :\n"+url);
							
							grab = openBitMap url;
							local fail = setclipboardBitmap grab;
							if (fail != true)then(
								messageBox "The bitmap could not be stored to your clipboard (memory could not be allocated). Instead the file will now be opened for you!" title:"Clipboard error" beep:false;
								shellLaunch "explorer.exe" url;
							)
							*/
							
							delete objCopy;
							--delete lght;
							select obj;
							gc();
							enableSceneRedraw();
							actionMan.executeAction 0 "40182"  -- Views: Perspective User View
							completeRedraw();
							--return true;--success, return this so that radiobutton swap can be updated outside this function
							
							btmp = openBitMap url;
							deleteFile url; --delete the temp. file
							return btmp;
						)
					)else(
						messageBox "There is no Bitmap in your clipboard!" title:"Empty Clipboard" beep:false;
					)
				)
			)
		)else(
			messageBox "Select 1 geometry Object" title:"Wrong Selection" beep:false;
		)
	)else(
		messageBox "Only 3dsmax 2008+ supported for now" title:"Only 3dsMax 2008" beep:false;
	)
	
	return false;--something went wrong
)

--fn_38__camera_map_painting false;
--fn_38__bake_camera_map_painting false 256 256 8;