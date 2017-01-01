
global tempPath = pathConfig.GetDir #temp 
global exportFolder = tempPath 
global exportFileName = "3dsNodes.fbx";
global importFileName = "blenderNodes.fbx"
global exportHightPolyFileName = "3dsNodesHightPoly.fbx"
global importLowPolyFileName = "blenderNodesLowPoly.fbx"
global binaryFacesObjects = "3dsFaces.bin"
global binarySelectedFace = "3dsSelectedFace.bin"
global blenderPath = "\"C:\\Program Files\\Blender Foundation\\Blender\\blender.exe\""
global scriptFolder  = ypath  + "\\newm\\scripts"
global pythonScript =  scriptFolder + "\\blender.py"
global blenderFile = exportFolder+"\\3dsMax.blend"
global DoscommandOutput = ypath + "\\hiddencmdout.tmp"


fn exportObjectToFBX path = 
(
	FBXExporterSetParam  "FileVersion" "FBX201400" 
	exportFile path #noPrompt selectedOnly:true 
)	

fn importObjectFromFBXFile filePath = 
(
	FBXImporterSetParam "Mode" #create
	importFile filePath #noPrompt
)