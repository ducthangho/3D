fn printImporterParamValue ParamName = 
(
	local val = (FBXImporterGetParam ParamName as string)
	
	print ("\""+ParamName+"\""+ " " + val as string)
)

fn printExporterParamValue ParamName = 
(
	local val = (FBXExporterGetParam  ParamName as string)
	print ("\""+ParamName+"\""+ " " + val as string)
)

fn printAllImporterParamValue = 
(
	print "Import param Setting"
	printImporterParamValue "Animation"
	printImporterParamValue "AxisConversion"
	printImporterParamValue "BakeAnimationLayers"
	printImporterParamValue "Cameras"
	printImporterParamValue "ConvertUnit"
	printImporterParamValue "FillTimeline"
	printImporterParamValue "FilterKeyReducer"
	printImporterParamValue "FilterKeySync"
	printImporterParamValue "GenerateLog"
	printImporterParamValue "ImportBoneAsDummy"
	printImporterParamValue "KeepFrameRate"
	printImporterParamValue "Lights"
	printImporterParamValue "LoadImportPresetFile"
	printImporterParamValue "Markers"
	printImporterParamValue "Mode"
	printImporterParamValue "PopSettings"
	printImporterParamValue "PushSettings"
	printImporterParamValue "PointCache"
	printImporterParamValue "Resampling"
	printImporterParamValue "ResetImport"
	printImporterParamValue "ScaleConversion"
	printImporterParamValue "ScaleFactor"
	printImporterParamValue "Shape"
	printImporterParamValue "Skin"
	printImporterParamValue "SmoothingGroups"
	printImporterParamValue "TakeCount"
	printImporterParamValue "TakeIndex"
	printImporterParamValue "UpAxis"
	print "End import param setting fn\n"
)
 
fn printAllExporterParamValue = 
(
	print "Export param Setting"
	printExporterParamValue "Animation"
	printExporterParamValue "ASCII"
	printExporterParamValue "AxisConversionMethod"
	printExporterParamValue "BakeAnimation"
	printExporterParamValue "BakeFrameStart"
	printExporterParamValue "BakeFrameEnd"
	printExporterParamValue "BakeFrameStep"
	printExporterParamValue "BakeResampleAnimation"
	printExporterParamValue "Cameras"
	printExporterParamValue "CAT2HIK"
	printExporterParamValue "ColladaTriangulate"
	printExporterParamValue "ColladaSingleMatrix"
	printExporterParamValue "ColladaFrameRate"
	printExporterParamValue "Convert2Tiff"
	printExporterParamValue "ConvertUnit"
	printExporterParamValue "EmbedTextures"
	printExporterParamValue "FileVersion"
	printExporterParamValue "FilterKeyReducer"
	printExporterParamValue "GeomAsBone"
	printExporterParamValue "GenerateLog"
	printExporterParamValue "Lights"
	printExporterParamValue "LoadExportPresetFile"
	printExporterParamValue "NormalsPerPoly"
	printExporterParamValue "PointCache"
	printExporterParamValue "PopSettings"
	printExporterParamValue "Preserveinstances"
	printExporterParamValue "PushSettings"
	printExporterParamValue "Removesinglekeys"
	printExporterParamValue "Resampling"
	printExporterParamValue "ResetExport"
	printExporterParamValue "ScaleFactor"
	printExporterParamValue "SelectionSet"
	printExporterParamValue "SelectionSetExport"
	printExporterParamValue "Shape"
	printExporterParamValue "Skin"
	printExporterParamValue "ShowWarnings"
	printExporterParamValue "SmoothingGroups"
	printExporterParamValue "SmoothMeshExport"
	printExporterParamValue "SplitAnimationIntoTakes"
	printExporterParamValue "TangentSpaceExport"
	printExporterParamValue "Triangulate"
	printExporterParamValue "UpAxis"
	printExporterParamValue "UseSceneName"
	print "End export param setting fn\n"
)

 printAllImporterParamValue()
 printAllExporterParamValue()