fn testProOptimizer = 
(
	local sourceFilePath = "F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\scenes\\TestProOptimizerScene\\001.max"
	BatchProOptimizer.SourceFileMode = 0
	BatchProOptimizer.SourceFileFiles = #(sourceFilePath)
	BatchProOptimizer.batch() 
)

fn getCoreInterface = 
(
	core_interfaces = getcoreinterfaces()
	print core_interfaces.count
	count = 0
	for i in core_interfaces do 
	(
		count+=1
		--if count==1 do
		(
			print ("Coreinterface numbrer: "+count as string)
			showinterface i; format"\n"
		)
	)
)


fn testLayer = 
(
	for i = 0 to layerManager.count-1 do
	(
		  ilayer = layerManager.getLayer i
		  layerName = ilayer.name
		  layer = ILayerManager.getLayerObject i
		  layerNodes = refs.dependents layer
		  format "Layer: %; nodes: %\n" layerName layerNodes
	)
)

fn pre_optimize oFileDir oFileName projectPath = 
(
	-- fname = substring maxFileName 1 (maxFileName.count-4)
	-- path = maxFilePath + fname + "_"
	
	-- MessageBox curFile;
	--oFileDir = "D:\\3D\\3dmax\\1";
	--oFileName = "phongkhach 1";
	BatchProOptimizer.SourceFileMode = 0;
	BatchProOptimizer.SourceFileFiles = oFileDir +"\\"+ oFileName + ".max";

	BatchProOptimizer.OptimizationLevel1 = 90;
	BatchProOptimizer.OptimizationLevel2 = 0;
	BatchProOptimizer.OptimizationLevel3 = 0;
	BatchProOptimizer.OptimizationLevel4 = 0;
	BatchProOptimizer.OptimizationLevel5 = 0;

	BatchProOptimizer.OptimizationLevelMode = 0;
	BatchProOptimizer.OptimizeBordersMode = 1; --Protect Borders 
	BatchProOptimizer.AutoRename = false;
	BatchProOptimizer.DestFileNameMode = 1;
	BatchProOptimizer.DestFolderMode = 2;
	--BatchProOptimizer.DestFolderName = ".\\"+oFileName+"_y3d";
	BatchProOptimizer.DestFolderName = projectPath;
	BatchProOptimizer.batch();
	loadMaxFile (projectPath+"\\" + oFileName + "90.max") useFileUnits:true quiet:true
	-- MessageBox "load xong"
)


fn test2 = 
(
	b = box()
	-- if filename exists, ask user if ok to overwrite
	-- function returns true if file was saved, false otherwise
	fn maybe_saveNodes nodeArray filename =
	(
		if getFilenamePath filename == "" do
			filename = getdir #scene + "\\" + filename
		if (not (doesfileexist filename)) or querybox ("filename: "+filename + "\nexists. Overwrite?") then
		(
			saveNodes nodeArray filename
			true
		)
		else
			false
	)
)


fn xref_low oFileDir oFileName= 
(
	print (oFileDir +"\\"+ oFileName);
	oNames = #()
	for ss in selection do 
		(
			append oNames ss.name
		)
	objXRefMgr.dupObjNameAction  = #deleteOld
	xref_b = objXRefMgr.AddXRefItemsFromFile (oFileDir +"\\"+ oFileName + "_low0.max") objNames:oNames
	-- items = #()
	-- xref_b.GetItems #XRefObjectType &items
	-- for item in items do 
	-- 	(
	-- 		objXRefMgr.SetProxyItemSrcFile item (oFileDir +"\\"+ oFileName + "_high.max")
	-- 		item.proxyItemName = item.objectName
	-- 	)		
	-- delete not HL
	-- loadMaxFile strStudioFile useFileUnits:true --load the latest lighting file
	-- mergeMaxFile strFile #select #noRedraw #mergeDups #useMergedMtlDups #neverReparent --merge in everything from the hold-file
)


fn CreateLayer nodes layersname isolate:true = 
(
	local layerProps = LayerManager.newLayerFromName layersname
	-- if layerProps == undefined do layerProps = LayerManager.newLayerFromName (uniquename layersname)
	for node in nodes do
	(
		layerProps.addnode node
	)

)

fn test = 
(
	nodes = selection
	CreateLayer nodes "aa001" 
)

fn LowPolyUseProOptimizer objName ratio:20= 
(
	local obj = getNodeByName objName exact:true all:false ignoreCase:false
	addModifier obj (ProOptimizer())

	optMod = obj.modifiers["ProOptimizer"]
	optMod.VertexPercent = ratio
	optMod.OptimizationMode = 2
	optMod.KeepNormals = true
	optMod.NormalMode = 2
	optMod.NormalThreshold = 10
	optMod.MergePoints = true
	optMod.MergePointsThreshold  = 0
	optMod.MergeFaces = true
	optMod.MergeFacesAngle = 0
	optMod.CompactFaces = true
	optMod.PreventFlip = true
	optMod.Calculate = true

)