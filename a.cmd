@set dir="%USERPROFILE%\AppData\Roaming\SketchUp\SketchUp 2016\SketchUp\Plugins"
@set dir2="%USERPROFILE%\AppData\Roaming\SketchUp\SketchUp 2015\SketchUp\Plugins"
@if exist %dir% (
	copy Ruby\* %dir%
	echo "Plugins have been installed for Sketchup 2016"
)

@if exist %dir2% (
	copy Ruby\* %dir2%
	echo "Plugins have been installed for Sketchup 2015"
)