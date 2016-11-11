global exportFolder = "F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\export\\blender\\"
global exportFileName = "3dsNodes.fbx";
global binaryFacesObjects = "3dsFaces.bin"
global binarySelectedFace = "3dsSelectedFace.bin"

fn exportObjectToFBX obj filename = 
(
	local filePath = exportFolder + filename
	print filePath
	exportFile filePath #noPrompt selectedOnly:true 
)