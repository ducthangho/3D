macroScript y3d_v1
category:"y3d"
toolTip:"Y3D v1"
ButtonText:"Y3D v1"
Icon:#("cws",3)
(
	Include "$scripts\y3d\lib\ylib.ms"
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