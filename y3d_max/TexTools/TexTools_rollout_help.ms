rl_TexTools_help;
fn_00__helppage;
try (--close if possible
	DestroyDialog rl_TexTools_help;
) catch();

function fn_00__helppage nr =(
	local url = ((pathConfig.GetDir #userScripts)+"\\TexTools\\");
	if (nr == 1)then(
		rl_TexTools_help.btn_getting_started.visible = true;
		rl_TexTools_help.btn_overview.visible = false;
		rl_TexTools_help.bmp1.bitmap = openBitmap (url+"TexTools_help_page1.bmp");
	)else(
		rl_TexTools_help.btn_getting_started.visible = false;
		rl_TexTools_help.btn_overview.visible = true;
		rl_TexTools_help.bmp1.bitmap = openBitmap (url+"TexTools_help_page2.bmp");
	)

)

rollout rl_TexTools_help "TexTools Help" width:256 height:616
(
	ImgTag bmp1 "Bitmap" pos:[0,26] width:256 height:444 opacity:1 transparent:(color 0 255 255)
	label lbl4 "TexTools is developed and maintained by Hendrik-Jan Schoenmaker with support and help of many others. Web links:" pos:[2,475] width:253 height:50
	
	button btn_credits "view credits" pos:[0,591] width:256 height:24
	button btn_getting_started "Getting started..." pos:[128,1] width:128 height:24
	button btn_overview "Overview..." pos:[0,1] width:128 height:24
	
	hyperLink lnk1 "www.renderhjs.net/textools" color:(color 36 52 91) hoverColor:(color 69 110 213) pos:[4,522] address:"http://www.renderhjs.net/textools/"
	hyperLink lnk2 "polycount.net Thread" color:(color 36 52 91) hoverColor:(color 69 110 213) pos:[4,538] address:"http://boards.polycount.net/showthread.php?t=60553"
	hyperLink lnk3 "Youtube Videos" color:(color 36 52 91) hoverColor:(color 69 110 213) pos:[4,555] address:"http://www.youtube.com/renderhjs#p/c/22510F74F5283AE3"
	hyperLink lnk4 "Scriptspot.com Entry" color:(color 36 52 91) hoverColor:(color 69 110 213) pos:[4,571] address:"http://www.scriptspot.com/3ds-max/scripts/textools-toolbox-for-the-texture-artist"


	-----------------------------------
	on btn_getting_started pressed do
	(
		fn_00__helppage(2);
	)
	on btn_overview pressed do
	(
		fn_00__helppage(1);
	)
	on btn_credits pressed do
	(
		local url = ((pathConfig.GetDir #userScripts)+"\\TexTools\\credits.txt");
		shellLaunch "explorer.exe" url;
	)
	
)

--

CreateDialog rl_TexTools_help 256 616;
(
fn_00__helppage(1);
)