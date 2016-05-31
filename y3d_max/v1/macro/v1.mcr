macroScript y3d_v1
category:"y3d"
toolTip:"Multi tile Bake v1"
ButtonText:"Multi tile Bake v1"
Icon:#("cws",3)
(
	Include "$userScripts\y3d\lib\ylib.ms"
	-- try (--close if possible
	-- 	symbolicPaths.addUserPath "$ypath" ypath
	-- ) catch();
	 yInclude "v1/lib/y3d_lib.ms"
	 yInclude "v1/lib/blender_unwrap.ms"
	 yInclude "v1/scripts/SimpleRectanglePackAlogorithm.ms"
	 yInclude "v1/scripts/y3d_bake.ms"
	 yInclude "v1/scripts/y3d_pack.ms"
	 yInclude "v1/scripts/y3d_unwrap.ms"
	 yInclude "v1/scripts/y3d_material.ms"
	 yInclude "v1/scripts/y3d_ui.ms"
)