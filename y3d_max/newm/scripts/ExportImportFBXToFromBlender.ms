-- global exportFolder = "F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\export\\blender\\"
global exportFolder = "D:\\"
global exportFileName = "3dsNodes.fbx";
global exportHightPolyFileName = "3dsNodesHightPoly.fbx"
global binaryFacesObjects = "3dsFaces.bin"
global binarySelectedFace = "3dsSelectedFace.bin"
global blenderPath = "\"C:\\Program Files\\Blender Foundation\\Blender\\blender.exe\""

-- global scriptFolder  = "F:\\WorkSpace\\3D\\y3d_max\\newm\\scripts\\"
global scriptFolder  = ypath  + "\\newm\\scripts\\"


global pythonScript =  scriptFolder + "blender.py"
global blenderFile = exportFolder+"3dsMax.blend"

fn exportObjectToFBX obj filename = 
(
	local filePath = exportFolder + filename
	print filePath
	exportFile filePath #noPrompt selectedOnly:true 
)	