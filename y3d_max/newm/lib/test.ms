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
