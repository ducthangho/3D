fn fn_21__normalize_uv_shells = (
	undo on(
	--original script PAUL GREVESON
	--http://www.greveson.co.uk/scripts/normalizeUvScale.ms
		if selection.count == 1 then (
			local obj = selection[ 1 ];
			local hadUnwrap = true
			local success = true
			local normalizeAll = true
			if classOf obj.baseObject == Editable_Poly then (
				max modify mode
				if obj.modifiers[ #unwrap_uvw ] == undefined then (
					hadUnwrap = false
					addModifier obj ( unwrap_uvw() )
				)
				
				local uv = obj.modifiers[ #unwrap_uvw ]
				-- Store current selection, and check if the edit window is open or not.
				local origLevel = 0
				for i = 0 to 3 do (
					if subObjectLevel == i then origLevel = i
				)
				local windowOpen = false
				local uvWndH = uv.getWindowH()
				if uvWndH > 0 and uvWndH < 5000 then windowOpen = true
				local origVerts = uv.getSelectedVertices()
				local origEdges = uv.getSelectedEdges()
				local origFaces = uv.getSelectedFaces()
				local alwaysEdit = uv.getAlwaysEdit()
				
				uv.setAlwaysEdit false
				
				-- Check what the original selection was for each mode.
				subObjectLevel = 1
				local sel = #{}
				if origLevel == 0 then (
					max select all
					sel = uv.getSelectedVertices()
				)
				else if origLevel == 1 then (
					if ( origVerts as array ).count == 0 then (
						max select all
						sel = uv.getSelectedVertices()
					)
					else (
						sel = origVerts
						normalizeAll = false
					)
				)
				else if origLevel == 2 then (
					if ( origEdges as array ).count == 0 then (
						max select all
						sel = uv.getSelectedVertices()
					)
					else (
						uv.edgeToVertSelect()
						sel = uv.getSelectedVertices()
						normalizeAll = false
					)
				)
				else (
					if ( origFaces as array ).count == 0 then (
						max select all
						sel = uv.getSelectedVertices()
					)
					else (
						uv.faceToVertSelect()
						sel = uv.getSelectedVertices()
						normalizeAll = false
					)
				)
				
				-- Determine the amount of elements and store the vertex index of all vertices in each element.
				local numVerts = uv.numberVertices()
				local vertElemArray = #()
				for i = 1 to numVerts do ( vertElemArray[ i ] = 0 )
				local elements = 0
				local elemVerts = #{}
				
				with redraw off
				for v = 1 to numVerts do (
					-- If vertex has no element assigned yet, create a new element.
					if vertElemArray[ v ] == 0 then (
						uv.selectVertices #{ v }
						uv.selectElement()
						elemVerts = uv.getSelectedVertices()
						-- Ignore elements with less than 3 UV vertices.
						if ( elemVerts as array ).count > 2 then (
							elements += 1
							for i in elemVerts do (
								vertElemArray[ i ] = elements -- Mark these vertices with their element number in vertElemArray.
							)
						)
					)
				)
			
				local elementArray = #()
				-- Get the first vertex index for each element.
				for e = 1 to elements do (
					for v = 1 to vertElemArray.count do (
						if vertElemArray[ v ] == e then (
							elementArray[ e ] = v
							exit
						)
					)
					e += 1
				)
				
				local workObj = obj
				-- Check if we need to snapshot or not.
				if obj.modifiers.count > 1 then ( 	
					-- Snapshot to work with.
					objSnap = copy obj
					collapseStack objSnap

					workObj = objSnap
				)
				
				if success and elements > 0 then (
					-- Check how many polys in the object, only show progress bar for dense meshes.
					local showProgress = false
					if polyop.getNumFaces workObj > 2000 then showProgress = true
					
					local uvAreas = #()
					local geoAreas = #()
					local uvRatios = #()
					uvRatios[ elements ] = 0;
					
					with redraw off
					for i = 1 to elements do (
						
						local uvArea = 0.0
						local geoArea = 0.0
						
						subObjectLevel = 1
						uv.selectVertices #{ elementArray[ i ] }
						uv.selectElement()
						uv.vertToFaceSelect()
						subObjectLevel = 3
						local polySel = uv.getSelectedPolygons() as array
						
						-- Start a progress bar.
						local progString = "" as stringStream
						format "Normalizing element %/%..." i elements to:progString
						if showProgress then progressStart ( progString as string )
						
						-- Work through all the faces in the element to get its UV verts and face area.
						for p = 1 to polySel.count do (
							local vCount = ( uv.numberPointsInFace polySel[ p ] )
							local faceVerts = #()
							local uvPos = #()
							local thisUvArea = 0.0
							faceVerts[ vCount ] = 0
							uvPos[ vCount ] = 0
							
							for v = 1 to vCount do (
								faceVerts[ v ] = ( uv.getVertexIndexFromFace polySel[ p ] v )
							)
							
							local temp = 1
							for f in faceVerts do (
								uvPos[ temp ] = ( uv.getVertexPosition 0 f )
								temp += 1
							)
							
							-- Cheap hack to avoid more complicated maths later! :)
							append uvPos uvPos[ 1 ]
							
							-- Add up each UV face area for this element.
							for u = 1 to vCount do (
								local self = uvPos[ u ]
								local next = uvPos[ u+1 ]
								thisUvArea += ( ( self.x * next.y ) - ( next.x * self.y ) )
							)
							-- Make sure our UV area amount is +ive and do the final calculation.
							thisUvArea = abs thisUvArea
							thisUvArea *= 0.5
							
							-- Add up all the UV and geometric face areas of this element.
							geoArea += ( polyOp.getFaceArea workObj polySel[ p ] )
							uvArea += thisUvArea
		
							-- Update the progress bar for this element.
							if showProgress then progressUpdate ( 100.0 * p / polySel.count )
						)
						if showProgress then progressEnd()
		
						-- Find the surface ratio for this element.
						uvRatios[ i ] = ( sqrt ( uvArea / geoArea ) )
					)
					
					-- Find which elements correspond to original selections.
					local selElements = #()
					selElements[ elements ] = 2
					if normalizeAll then (
						for i = 1 to elements do (
							selElements[ i ] = 1
						)
					)
					else (
						for i = 1 to elements do (
							subObjectLevel = 1
							uv.selectVertices #{ elementArray[ i ] }
							uv.selectElement()
							local tempSel = uv.getSelectedVertices()
							for e in sel do (
								for t in tempSel do (
									if e == t then (
										selElements[ i ] = 1
										exit
									)
								)
							)
							if selElements[ i ] != 1 then (
								selElements[ i ] = 0
							)
						)
					)
				
					-- Scale each element relative to the first.
					local scaleFactor = 1.0
					local avgFactor = 0.0
					with redraw off
					for i = 1 to elements do (
						if selElements[ i ] == 1 then (
							scaleFactor = ( uvRatios[ 1 ] / uvRatios[ i ] )
							subObjectLevel = 1
							uv.selectVertices #{ elementArray[ i ] }
							uv.selectElement()
							uv.scaleSelectedVerticesCenter scaleFactor 0
						)
						avgFactor += ( 1 / scaleFactor )
					)
					
					avgFactor = avgFactor / elements
					
					-- Average 'em all.
					with redraw off
					for i = 1 to elements do (
						if selElements[ i ] == 1 then (
							subObjectLevel = 1
							uv.selectVertices #{ elementArray[ i ] }
							uv.selectElement()
							uv.scaleSelectedVerticesCenter avgFactor 0
						)
					)
				)
								
				-- Delete the temporary object if necessary.
				if obj.modifiers.count > 1 then (
					delete objSnap
				)
				
				-- Reset the selections to the originals.
				uv.selectVertices origVerts
				uv.selectEdges origEdges
				uv.selectFaces origFaces
				uv.setAlwaysEdit alwaysEdit
				-- Also open the Edit UVWs window if it was already open.
				if windowOpen then uv.edit()
				-- Go back to original subObjectLevel.
				subObjectLevel = origLevel
				-- Remove the Unwrap UVW modifier if necessary after the script failed.
				if not hadUnwrap and not success then deleteModifier obj uv
			)
		)
	)
)