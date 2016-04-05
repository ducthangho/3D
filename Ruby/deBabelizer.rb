=begin
TIG (c) 2009-2013

deBabelizer(string,path)

This adds 'translation' capabilities to Tools - 
e.g. the 2D Tools' text, menus etc
It looks for a 'lingvo' file in the format:-
  xxxEN-US.lingvo for US English [typically the base set]
  xxxES.lingvo for Español
  xxxFR.lingvo for FRançaise
  etc etc
If the file is found, it tries to translate the string and returns it.
If the string is not found in the file then it is returned unchanged.
If the file is not found then the string is returned unchanged.
The lingvo file naming can be adjusted and tailored to suit any Tool, 
where 'xxx' might be the Tool name - it must end with EE where EE is 
the locale code for your langauge.

You then can shortcut deBabelizer() inside your tool using db(string) 
as explained nearer the end of this file.

To make a 'lingvo' file for your Tool 'xxx' call it xxxEN-US.lingvo - 
assuming that the tool is in US-English, otherwise name it to suit 
its language.  Now put the lingvo file into a location that's readily 
pathed to - e.g. a 'xxx' sub-folder in the 'Plugins' folder - 
we'll need to find this later for the db() shortcut.
We will assume it's called 'xxxEN-US.lingvo' from now on...
Edit it with a plain-text editor like NotePad and copy any text strings 
from within the Tool's code one line at a time.  These text strings 
will be found within "" or '', NOTE the code CANNOT have strings like 
"text #{var}." it must be in the form "text "+var.to_s+".", so that any 
variables are outside the "" string parts.
Also keep '\n' line-breaks aouside of deBabelized strings - so do not 
use "cat\ndog" - it MUST be (db"cat")+"\n"+(db"dog"), to get translated 
properly, with separate lines in the lingvo file for cat and dog.
Tabs ['\t'] are similarly better avoided in lingvo strings.
You now need to add the translations for each line of text.  
In the base file add <==> and copy the text to the right of it; 
for an EN-US file its format would be:-
The Cat<==>The Cat
A Dog.<==>A Dog.
You now have your base lingvo file.  
To make a new 'lingvo' file for your language see the list below.
To find your own locale type Sketchup.get_locale in the Ruby Console*.
First copy the US English file 'xxxEN-US.lingvo', rename that copy 
with your language suffix [e.g. 'xxxES.lingvo'], then edit it.
Translate each line by replacing the second 'field' of each line with 
your translation.
The text/translation is always separated with <==> - therefore <==> is 
not allowed in the original text or in the translated text (unlikely).
Any lines not containing a <==> are ignored.
For example:-
EN-US<==>ES
The Cat<==>El Gato
A Dog.<==>Un Pero.
Remember to keep any punctuation and all leading/trailing spaces.
*    It is case-sensitive so never change the first [original] field.
**   Do not type manual newlines in the text.
***  Always leave any \t [tab] etc in the translation.
**** Avoid accents in translations of Yes|No type dialogs [Si not Sí].


* Acceptable language codes are:-

Arabic - AR
Bulgarian - BG
Catalan - CA
Chinese (simplified) - ZH-CN
Chinese (traditional) - ZH-TW
Croatian - HR
Czech - CS
Danish - DA
Dutch - NL
English (UK) - EN-GB
English (US) - EN-US *** OFTEN THE BASE SET ***
Finnish - FI
French - FR
German - DE
Greek - EL
Hebrew - IW
Hindi - HI
Hungarian -HU
Indonesian - ID
Italian - IT
Japanese - JA
Korean - KO
Latvian - LV
Lithuanian - LT
Norwegian - NO
Polish - PL
Portuguese (Brazil) - PT-BR
Portuguese (Portugal) - PT-PT
Romanian - RO
Russian - RU
Serbian - SR
Slovak - SK
Slovenian - SL
Spanish - ES
Swedish - SV
Tagalog - TL
Thai - TH
Turkish - TR
Ukrainian - UK
Vietnamese - VI
... these locale codes might be added to later...

If you want to translate strings BUT keep your locale unchanged then 
you can make a lingvo file suffixed with your locale code BUT include 
translated strings in it - for example that way you could have strings 
translated into Esperanto [EO] which is not currently a recognised 
locale code !

### v1.0 first issue of deBabelizer - used in 2D Tools. 20091120
### v1.1 example disabled so db(txt) usable elsewhere.  20100110

=end
###
require('sketchup.rb')
###
def deBabelizer(string="",file=nil)
  string=string.to_s
  unless file && FileTest.exist?(file)
    return string
  else ### it's perhaps translated
    IO.readlines(file).each{|line|
	  next if line =~ /^[#]/
      line.chomp! ### loose \n off end
      if line =~ /[<][=][=][>]/
        set = line.split("<==>")
        if set[0] == string
			string = set[1]
			break
		end
      end#if
    }
    return string
  end#if
end#def
