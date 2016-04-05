=begin
(c) TIG 2010-2012
[incorporates some of ThomThom's UV ideas too]
    FixReversedFaceMaterials.rb
Usage:
    Plugins > TIG: Reversed Face Tools... > Fix Reversed Face Materials
Select entities including any suspect faces [edges etc are ignored].
If any face has a back-material BUT it has no front-material it is assumed 
to have been made incorrectly and to be oriented the wrong way round, 
with the desired material applied to the back instead of the front.
So then the face is reversed and its front-material is made to match the 
back-material - including any UV-mapping appropriately corrected - 
the back-material is then removed.
If there is no selection you are asked if you want to process the whole 
model, if you reply "Yes" then absolutely everything is selected !
If the selection includes groups or components you are asked if you want to 
process these too - if you reply "Yes" then any faces inside them are also 
processed.  If there are 'nested' groups or components with these they are 
also processed through the nesting.
A closing dialog reports how many faces have been fixed.
To see the affect switch from Texture mode and view in MonoChrome mode, run 
the tool and you should see the changed faces flip - switch back to Texture 
mode and you should see no difference in appearance since all the materials 
should still be the same, but incorrectly oriented faces have been fixed.

IF there are still backfaces oriented the wrong way then they will have 
materials applied to both sides and so will have been excluded from the fix.
There is another option to force selected faces to flip, swap the front and 
back materials and then delete the unneeded material that is by then 
swapped onto the back face.
  Plugins > TIG: Reversed Face Tools... > Fix Reversed Face Materials [Forced]
Use this option with care!  It will flip any selected faces that have 
materials applied to both faces, swap the materials over and delete the 
material that is by then swapped onto the back face.  Pick only the faces 
you know to be wrong [in MonoChrome], otherwise you might mess up already 
correct faces.  As a safety precaution if any of the selected faces have 
their front face towards the camera a dialog asks if you want to continue - 
if you answer 'Yes' then they are all processed, if you answer 'No' then 
the operation is canceled and nothing is changed.

If you have an object [say an imported OBJ] where faces/materials are inverted 
you can force the faces to reverse keeping the correct material texture 
UV-mapping on the flipped materials...
  Plugins > TIG: Reversed Face Tools... > Reverse Faces and Materials
Use this option with care BUT you can undo or redo to re-flip back.
It will reverse any selected faces, and swap the front/back materials over 
so the inside material becomes the outside material, whilst keeping the 
UV-mapping [smething that is lost with simple 'reverse faces' in surfacss].

Once you are happy that you model has its faces oriented as desired then 
there is another tool to tidy up face-materials...
  Plugins > TIG: Reversed Face Tools... > Delete Back-Materials
Select entities including any faces with back-materials you want processing 
[edges etc are ignored].
If any selected face has a back-material its material will be deleted.
If there is no selection you are asked if you want to process the whole 
model, if you reply "Yes" then absolutely everything is selected !
If the selection includes groups or components you are asked if you want to 
process these too - if you reply "Yes" then any faces inside them are also 
processed.  If there are 'nested' groups or components with these they are 
also processed through the nesting.
A closing dialog reports how many faces had their back-materials 
deleted.
Note: Groups and Component-Instances that can have materials applied 
directly onto them - then any faces within them that have the default-
material [front and/or back] will then appear to have that material applied 
to them.  In fact these faces within the Group/Definition will still have 
the default-materials [front and/or back].  If you want to quickly remove 
the appearance of this material on the back-faces within the group then 
explode the group and immediately regroup the geometry - the group's 
previous material will then be individually transfered to each face that 
had the default-material.  Now use this 'Delete Back-Materials' to delete 
the unneeded back-material.  With component-instances you can similarly 
explode the instance and immediately make its geometry into a Component - 
using a new name if only that instance is to change or using the same name, 
overwriting the original definition to update all instances of it - 
however, remember that you can have several instances of a definition with 
different materials applied to them and displayed in lieu of the default-
material, so doing this might have unexpected consequences, so think 
beforehand!
Use 'Delete Back-Materials' this with care and only AFTER using other tools 
to correct flip and/or swap front/back material etc...

If you have faces with different materials front and back and you would like the 
back to match the front use this tool...
  Plugins > TIG: Reversed Face Tools... > Match Back-Materials
It will match all selected faces' back-materials with the front-materials, 
maintainng UVmapping if the material is tetxured etc...  
There is no reversing or flipping of faces - the back-material is simply matched 
to the front-material.
Do not use this as an excuse for sloppy modeling!

Donations:
by PayPal to info @ revitrev.org

Version:
1.0 20100806 First Issue.
1.1 20100811 Text deBabelized.
1.2 20100816 'Forced' option added and lingvos updated.
1.3 20100817 New 'Delete Back-Materials' tool added.
             Glitch in translation of strings fixed.
             Updated all lingvos.
1.4 20100826 New 'Reverse Faces and Materials' added.
1.5 20120216 Four tools now under "Plugins > TIG: Reversed Face Tools..."
1.6 20120716 New 'Match Back-Materials' added. Lingvos updated to suit.
=end
###
require 'sketchup.rb'
require 'deBabelizer.rb'
###
class FixReversedFaceMaterials

 ### TRANSLATOR ###
 def db(string)
  dir=File.join(File.dirname(__FILE__), "FixReversedFaceMaterials")
  locale=Sketchup.get_locale.upcase
  path=File.join(dir, ("FixReversedFaceMaterials"+locale+".lingvo"))
  if not File.exist?(path)
    return string
  else
    deBabelizer(string, path)
  end
 end#def

 def FixReversedFaceMaterials::db(string)
  dir=File.join(File.dirname(__FILE__), "FixReversedFaceMaterials")
  locale=Sketchup.get_locale.upcase
  path=File.join(dir, ("FixReversedFaceMaterials"+locale+".lingvo"))
  if not File.exist?(path)
    return string
  else
    deBabelizer(string, path)
  end
 end#if
 ############
 
  def activate
    @model=Sketchup.active_model
    sel=@model.selection
    sela=sel.to_a
    sel.clear
    if not sela[0] ### 6=YES 7=NO
      UI.beep
      yn=UI.messagebox( ( (db("There is NO selection!"))+"\n"+(db("Do you want to process ALL faces in the model?") ) ),MB_YESNO,"")
      self.deactivate(nil) if yn==7
      return nil if yn==7
      sela=@model.entities.to_a
    end#if
    @faces=[]; groups=[]; defs=[]; insts=0
    sela.each{|e|
      @faces<< e if e.class==Sketchup::Face
      groups<< e if e.class==Sketchup::Group
      defs<< e.definition if e.class==Sketchup::ComponentInstance and not defs.include?(e.definition)
      insts+=1 if e.class==Sketchup::ComponentInstance
    }
    if groups[1] ### 6=YES 7=NO
      yng=UI.messagebox( ( (db("There are"))+" "+groups.length.to_s+" "+(db("Groups in the selection!"))+"\n"+(db("Do you want to process any faces in them too?")) ),MB_YESNO,"")
    elsif groups[0]
      yng=UI.messagebox( ( (db("There is a Group in the selection!"))+"\n"+(db("Do you want to process any faces in it too?")) ),MB_YESNO,"")
    end#if
    ###
    if defs[1] ### 6=YES 7=NO
        ynd=UI.messagebox( ( (db("There are Instances of"))+" "+defs.length.to_s+" "+(db("Components in the selection!"))+"\n"+(db("Do you want to process any faces in them?")) ),MB_YESNO,"")
    elsif defs[0]
      if insts>1
        ynd=UI.messagebox( ( (db("There are Instances of a Component in the selection!"))+"\n"+(db("Do you want to process any faces in it?")) ),MB_YESNO,"")
      else
        ynd=UI.messagebox( ( (db("There is an Instance of a Component in the selection!"))+"\n"+(db("Do you want to process any faces in it?")) ),MB_YESNO,"")
      end#if
    end#if
    def do_group(g)
      g.entities.each{|e|
        @faces<< e if e.class==Sketchup::Face
        do_group(e) if e.class==Sketchup::Group
        do_def(e.definition) if e.class==Sketchup::ComponentInstance
      }
    end#def
    def do_def(d)
      d.entities.each{|e|
        @faces<< e if e.class==Sketchup::Face
        do_group(e) if e.class==Sketchup::Group
        do_def(e.definition) if e.class==Sketchup::ComponentInstance
      }
    end#def
    groups.each{|g|do_group(g)} if yng==6
    defs.each{|d| d.entities.each{|e|do_def(d) } } if ynd==6
    ### process faces
    count=self.process_faces(@faces)
    if count==1
      UI.messagebox(db("1 face fixed."))
    else
      UI.messagebox(count.to_s+" "+(db("faces fixed.")))
    end#if
    ###
    self.deactivate(nil)
    ###
  end#def
  
  def deactivate(view=nil)
    Sketchup.send_action("selectSelectionTool:")
    return nil
  end#if
  
  def process_faces(faces)
    @model.start_operation(db("Fix Reversed Face Materials"))
    texture_writer=Sketchup.create_texture_writer
    count=0
    faces.each{|face|
      if face.material==nil and face.back_material!=nil
        if face.back_material.texture==nil
          face.material=face.back_material
          face.back_material=nil ### remove this to leave back unchanged
          face.reverse!
        else
          samples = []
          samples << face.vertices[0].position			   ### 0,0 | Origin
		  samples << samples[0].offset(face.normal.axes.x) ### 1,0 | Offset Origin in X
	  	  samples << samples[0].offset(face.normal.axes.y) ### 0,1 | Offset Origin in Y
		  samples << samples[1].offset(face.normal.axes.y) ### 1,1 | Offset X in Y
		  xyz = [];uv = []### Arrays containing 3D and UV points.
		  uvh = face.get_UVHelper(true, true, texture_writer)
		  samples.each { |position|
			xyz << position ### XYZ 3D coordinates
			uvq = uvh.get_back_UVQ(position) ### UV 2D coordinates
			uv << self.flattenUVQ(uvq)
		  }
		  pts = [] ### Position texture.
		  (0..3).each { |i|
			 pts << xyz[i]
			 pts << uv[i]
		  }
          mat=face.back_material
          face.reverse!
		  face.position_material(mat, pts, true)
          # face.back_material=nil ### remove this to leave back unchanged
        end#if
        count+=1
      end#if
    }
    @model.commit_operation
    return count
    ###
  end#def
  
  ### Get UV coordinates from UVQ matrix.
  def flattenUVQ(uvq)
	return Geom::Point3d.new(uvq.x / uvq.z, uvq.y / uvq.z, 1.0)
  end

end#class

################
class FixReversedFaceMaterialsForced

 ### TRANSLATOR ###
 def db(string)
  dir=File.join(File.dirname(__FILE__), "FixReversedFaceMaterials")
  locale=Sketchup.get_locale.upcase
  path=File.join(dir, ("FixReversedFaceMaterials"+locale+".lingvo"))
  if not File.exist?(path)
    return string
  else
    deBabelizer(string, path)
  end
 end#def

 def FixReversedFaceMaterialsForced::db(string)
  dir=File.join(File.dirname(__FILE__), "FixReversedFaceMaterials")
  locale=Sketchup.get_locale.upcase
  path=File.join(dir, ("FixReversedFaceMaterials"+locale+".lingvo"))
  if not File.exist?(path)
    return string
  else
    deBabelizer(string, path)
  end
 end#if
 ############
 
  def activate
    @model=Sketchup.active_model
    sel=@model.selection
    sela=sel.to_a
    sel.clear
    if not sela[0]
      UI.beep
      UI.messagebox( ( (db("There is NO selection!"))+"\n"+(db("Select only the faces you want to force to flip.") ) ))
      self.deactivate(nil)
      return nil
    end#if
    @faces=[]
    sela.each{|e|@faces<< e if e.class==Sketchup::Face}
    ### double check orientation
    cam=@model.active_view.camera
    eye=cam.eye
    tar=cam.target
    vec=eye.vector_to(tar)
    ofaces=[]
    @faces.each{|face|ofaces<< face if vec.angle_between(face.normal)>90.degrees}
    if ofaces[0]
      UI.beep
      sel.add(ofaces)
      if ofaces[1]
        yn=UI.messagebox( ( (db("There are"))+" "+ofaces.length.to_s+" "+(db("selected faces oriented towards the camera!"))+"\n"+(db("Do you still want to flip the whole selection?") ) ),MB_YESNO,"")### 6=YES 7=NO
      else
        yn=UI.messagebox( ( (db("There is 1 selected face oriented towards the camera!"))+"\n"+(db("Do you still want to flip the whole selection?") ) ),MB_YESNO,"")### 6=YES 7=NO
      end#if
      sel.clear
      self.deactivate(nil) if yn==7
      return nil if yn==7
    end#if
    ### process faces
    count=self.process_faces(@faces)
    if count==1
      UI.messagebox(db("1 face fixed."))
    else
      UI.messagebox(count.to_s+" "+(db("faces fixed.")))
    end#if
    ###
    self.deactivate(nil)
    ###
  end#def
  
  def deactivate(view=nil)
    Sketchup.send_action("selectSelectionTool:")
    return nil
  end#if
  
  def process_faces(faces)
    @model.start_operation(db("Fix Reversed Face Materials [Forced]"))
    texture_writer=Sketchup.create_texture_writer
    count=0
    faces.each{|face|
      if face.back_material!=nil
        if face.back_material.texture==nil
          face.material=face.back_material
          face.back_material=nil ### remove this to leave back unchanged
          face.reverse!
        else
          samples = []
          samples << face.vertices[0].position			   ### 0,0 | Origin
		  samples << samples[0].offset(face.normal.axes.x) ### 1,0 | Offset Origin in X
	  	  samples << samples[0].offset(face.normal.axes.y) ### 0,1 | Offset Origin in Y
		  samples << samples[1].offset(face.normal.axes.y) ### 1,1 | Offset X in Y
		  xyz = [];uv = []### Arrays containing 3D and UV points.
		  uvh = face.get_UVHelper(true, true, texture_writer)
		  samples.each { |position|
			xyz << position ### XYZ 3D coordinates
			uvq = uvh.get_back_UVQ(position) ### UV 2D coordinates
			uv << self.flattenUVQ(uvq)
		  }
		  pts = [] ### Position texture.
		  (0..3).each { |i|
			 pts << xyz[i]
			 pts << uv[i]
		  }
          mat=face.back_material
          face.reverse!
		  face.position_material(mat, pts, true)
          # face.back_material=nil ### remove this to leave back unchanged
        end#if
        count+=1
      ### else ### NO BACK MATERIAL
        ###
      end#if
    }
    @model.commit_operation
    return count
    ###
  end#def
  
  ### Get UV coordinates from UVQ matrix.
  def flattenUVQ(uvq)
	return Geom::Point3d.new(uvq.x / uvq.z, uvq.y / uvq.z, 1.0)
  end

end#class

################
class FixReversedFaceMaterialsFlip

 ### TRANSLATOR ###
 def db(string)
  dir=File.join(File.dirname(__FILE__), "FixReversedFaceMaterials")
  locale=Sketchup.get_locale.upcase
  path=File.join(dir, ("FixReversedFaceMaterials"+locale+".lingvo"))
  if not File.exist?(path)
    return string
  else
    deBabelizer(string, path)
  end
 end#def

 def FixReversedFaceMaterialsFlip::db(string)
  dir=File.join(File.dirname(__FILE__), "FixReversedFaceMaterials")
  locale=Sketchup.get_locale.upcase
  path=File.join(dir, ("FixReversedFaceMaterials"+locale+".lingvo"))
  if not File.exist?(path)
    return string
  else
    deBabelizer(string, path)
  end
 end#if
 ############
 
  def activate
    @model=Sketchup.active_model
    sel=@model.selection
    sela=sel.to_a
    sel.clear
    if not sela[0]
      UI.beep
      UI.messagebox( ( (db("There is NO selection!"))+"\n"+(db("Select only the faces you want to force to flip.") ) ))
      self.deactivate(nil)
      return nil
    end#if
    @faces=[]
    sela.each{|e|@faces<< e if e.class==Sketchup::Face}
    ### double check orientation
    cam=@model.active_view.camera
    eye=cam.eye
    tar=cam.target
    vec=eye.vector_to(tar)
    ### process faces
    count=self.process_faces(@faces)
    if count==1
      UI.messagebox(db("1 face fixed."))
    else
      UI.messagebox(count.to_s+" "+(db("faces fixed.")))
    end#if
    ###
    self.deactivate(nil)
    ###
  end#def
  
  def deactivate(view=nil)
    Sketchup.send_action("selectSelectionTool:")
    return nil
  end#if
  
  def process_faces(faces)
    @model.start_operation(db("Reverse Faces and Materials"))
    texture_writer=Sketchup.create_texture_writer
    count=0
    faces.each{|face|
        fpts=[]
        bpts=[]
        ###
        if face.material and face.material.texture
          samples = []
          samples << face.vertices[0].position			   ### 0,0 | Origin
		  samples << samples[0].offset(face.normal.axes.x) ### 1,0 | Offset Origin in X
	  	  samples << samples[0].offset(face.normal.axes.y) ### 0,1 | Offset Origin in Y
		  samples << samples[1].offset(face.normal.axes.y) ### 1,1 | Offset X in Y
		  xyz = [];uv = []### Arrays containing 3D and UV points.
		  uvh = face.get_UVHelper(true, true, texture_writer)
		  samples.each { |position|
			xyz << position ### XYZ 3D coordinates
			uvq = uvh.get_front_UVQ(position) ### UV 2D coordinates
			uv << self.flattenUVQ(uvq)
		  }
		  ### Position texture.
		  (0..3).each { |i|
			 fpts << xyz[i]
			 fpts << uv[i]
		  }
          fmat=face.material
        end#if
        ###
        if face.back_material and face.back_material.texture
          samples = []
          samples << face.vertices[0].position			   ### 0,0 | Origin
		  samples << samples[0].offset(face.normal.axes.x) ### 1,0 | Offset Origin in X
	  	  samples << samples[0].offset(face.normal.axes.y) ### 0,1 | Offset Origin in Y
		  samples << samples[1].offset(face.normal.axes.y) ### 1,1 | Offset X in Y
		  xyz = [];uv = []### Arrays containing 3D and UV points.
		  uvh = face.get_UVHelper(true, true, texture_writer)
		  samples.each { |position|
			xyz << position ### XYZ 3D coordinates
			uvq = uvh.get_back_UVQ(position) ### UV 2D coordinates
			uv << self.flattenUVQ(uvq)
		  }
		  ### Position texture.
		  (0..3).each { |i|
			 bpts << xyz[i]
			 bpts << uv[i]
		  }
         
        end#if
        ###
        fmat=face.material
        bmat=face.back_material
        ###
        face.reverse! ##################################################
        ###
        if bpts[0]
		  face.position_material(bmat, bpts, false)
        else
          face.back_material=bmat
        end#if
        ###
        if fpts[0]
          face.position_material(fmat, fpts, true)
        else
          face.material=fmat
        end#if
        ###
        count+=1
    }
    @model.commit_operation
    return count
    ###
  end#def
  
  ### Get UV coordinates from UVQ matrix.
  def flattenUVQ(uvq)
	return Geom::Point3d.new(uvq.x / uvq.z, uvq.y / uvq.z, 1.0)
  end

end#class


################
class DeleteFacesBackMaterials

 ### TRANSLATOR ###
 def db(string)
  dir=File.join(File.dirname(__FILE__), "FixReversedFaceMaterials")
  locale=Sketchup.get_locale.upcase
  path=File.join(dir, ("FixReversedFaceMaterials"+locale+".lingvo"))
  if not File.exist?(path)
    return string
  else
    deBabelizer(string, path)
  end
 end#def

 def DeleteFacesBackMaterials::db(string)
  dir=File.join(File.dirname(__FILE__), "FixReversedFaceMaterials")
  locale=Sketchup.get_locale.upcase
  path=File.join(dir, ("FixReversedFaceMaterials"+locale+".lingvo"))
  if not File.exist?(path)
    return string
  else
    deBabelizer(string, path)
  end
 end#if
 ############
 
  def activate
    @model=Sketchup.active_model
    sel=@model.selection
    sela=sel.to_a
    sel.clear
    if not sela[0] ### 6=YES 7=NO
      UI.beep
      yn=UI.messagebox( ( (db("Delete Back-Materials"))+":\n"+(db("There is NO selection!"))+"\n"+(db("Do you want to process ALL faces in the model?") ) ),MB_YESNO,"")
      self.deactivate(nil) if yn==7
      return nil if yn==7
      sela=@model.entities.to_a
    end#if
    @faces=[]; groups=[]; defs=[]; insts=0
    sela.each{|e|
      @faces<< e if e.class==Sketchup::Face
      groups<< e if e.class==Sketchup::Group
      defs<< e.definition if e.class==Sketchup::ComponentInstance and not defs.include?(e.definition)
      insts+=1 if e.class==Sketchup::ComponentInstance
    }
    if groups[1] ### 6=YES 7=NO
      yng=UI.messagebox( ( (db("Delete Back-Materials"))+":\n"+(db("There are"))+" "+groups.length.to_s+" "+(db("Groups in the selection!"))+"\n"+(db("Do you want to process any faces in them too?")) ),MB_YESNO,"")
    elsif groups[0]
      yng=UI.messagebox( ( (db("Delete Back-Materials"))+":\n"+(db("There is a Group in the selection!"))+"\n"+(db("Do you want to process any faces in it too?")) ),MB_YESNO,"")
    end#if
    ###
    if defs[1] ### 6=YES 7=NO
        ynd=UI.messagebox( ( (db("Delete Back-Materials"))+":\n"+(db("There are Instances of"))+" "+defs.length.to_s+" "+(db("Components in the selection!"))+"\n"+(db("Do you want to process any faces in them?")) ),MB_YESNO,"")
    elsif defs[0]
      if insts>1
        ynd=UI.messagebox( ( (db("Delete Back-Materials"))+":\n"+(db("There are Instances of a Component in the selection!"))+"\n"+(db("Do you want to process any faces in it?")) ),MB_YESNO,"")
      else
        ynd=UI.messagebox( ( (db("Delete Back-Materials"))+":\n"+(db("There is an Instance of a Component in the selection!"))+"\n"+(db("Do you want to process any faces in it?")) ),MB_YESNO,"")
      end#if
    end#if
    def do_group(g)
      g.entities.each{|e|
        @faces<< e if e.class==Sketchup::Face
        do_group(e) if e.class==Sketchup::Group
        do_def(e.definition) if e.class==Sketchup::ComponentInstance
      }
    end#def
    def do_def(d)
      d.entities.each{|e|
        @faces<< e if e.class==Sketchup::Face
        do_group(e) if e.class==Sketchup::Group
        do_def(e.definition) if e.class==Sketchup::ComponentInstance
      }
    end#def
    groups.each{|g|do_group(g)} if yng==6
    defs.each{|d| d.entities.each{|e|do_def(d) } } if ynd==6
    ### process faces
    count=self.process_faces(@faces)
    if count==1
      UI.messagebox((db("Delete Back-Materials"))+":\n"+(db("1 face fixed.")))
    else
      UI.messagebox((db("Delete Back-Materials"))+":\n"+count.to_s+" "+(db("faces fixed.")))
    end#if
    ###
    self.deactivate(nil)
    ###
  end#def
  
  def deactivate(view=nil)
    Sketchup.send_action("selectSelectionTool:")
    return nil
  end#if
  
  def process_faces(faces)
    @model.start_operation(db("Delete Back-Materials"))
    count=0
    faces.each{|face|
      if face.back_material!=nil
        face.back_material=nil
        count+=1
      end#if
    }
    @model.commit_operation
    return count
    ###
  end#def
  
end#class

#################

class MatchFacesBackMaterials

 ### TRANSLATOR ###
 def db(string)
  dir=File.join(File.dirname(__FILE__), "MatchFacesBackMaterials")
  locale=Sketchup.get_locale.upcase
  path=File.join(dir, ("MatchFacesBackMaterials"+locale+".lingvo"))
  if not File.exist?(path)
    return string
  else
    deBabelizer(string, path)
  end
 end#def

 def MatchFacesBackMaterials::db(string)
  dir=File.join(File.dirname(__FILE__), "MatchFacesBackMaterials")
  locale=Sketchup.get_locale.upcase
  path=File.join(dir, ("MatchFacesBackMaterials"+locale+".lingvo"))
  if not File.exist?(path)
    return string
  else
    deBabelizer(string, path)
  end
 end#if
 ############
 
  def activate
    @model=Sketchup.active_model
    sel=@model.selection
    sela=sel.to_a
    sel.clear
    if not sela[0] ### 6=YES 7=NO
      UI.beep
      yn=UI.messagebox( ( (db("There is NO selection!"))+"\n"+(db("Do you want to process ALL faces in the model?") ) ),MB_YESNO,"")
      self.deactivate(nil) if yn==7
      return nil if yn==7
      sela=@model.entities.to_a
    end#if
    @faces=[]; groups=[]; defs=[]; insts=0
    sela.each{|e|
      @faces<< e if e.class==Sketchup::Face
      groups<< e if e.class==Sketchup::Group
      defs<< e.definition if e.class==Sketchup::ComponentInstance and not defs.include?(e.definition)
      insts+=1 if e.class==Sketchup::ComponentInstance
    }
    if groups[1] ### 6=YES 7=NO
      yng=UI.messagebox( ( (db("There are"))+" "+groups.length.to_s+" "+(db("Groups in the selection!"))+"\n"+(db("Do you want to process any faces in them too?")) ),MB_YESNO,"")
    elsif groups[0]
      yng=UI.messagebox( ( (db("There is a Group in the selection!"))+"\n"+(db("Do you want to process any faces in it too?")) ),MB_YESNO,"")
    end#if
    ###
    if defs[1] ### 6=YES 7=NO
        ynd=UI.messagebox( ( (db("There are Instances of"))+" "+defs.length.to_s+" "+(db("Components in the selection!"))+"\n"+(db("Do you want to process any faces in them?")) ),MB_YESNO,"")
    elsif defs[0]
      if insts>1
        ynd=UI.messagebox( ( (db("There are Instances of a Component in the selection!"))+"\n"+(db("Do you want to process any faces in it?")) ),MB_YESNO,"")
      else
        ynd=UI.messagebox( ( (db("There is an Instance of a Component in the selection!"))+"\n"+(db("Do you want to process any faces in it?")) ),MB_YESNO,"")
      end#if
    end#if
    def do_group(g)
      g.entities.each{|e|
        @faces<< e if e.class==Sketchup::Face
        do_group(e) if e.class==Sketchup::Group
        do_def(e.definition) if e.class==Sketchup::ComponentInstance
      }
    end#def
    def do_def(d)
      d.entities.each{|e|
        @faces<< e if e.class==Sketchup::Face
        do_group(e) if e.class==Sketchup::Group
        do_def(e.definition) if e.class==Sketchup::ComponentInstance
      }
    end#def
    groups.each{|g|do_group(g)} if yng==6
    defs.each{|d| d.entities.each{|e|do_def(d) } } if ynd==6
    ### process faces
    count=self.process_faces(@faces)
    if count==1
      UI.messagebox(db("1 face fixed."))
    else
      UI.messagebox(count.to_s+" "+(db("faces fixed.")))
    end#if
    ###
    self.deactivate(nil)
    ###
  end#def
  
  def deactivate(view=nil)
    Sketchup.send_action("selectSelectionTool:")
    return nil
  end#if
  
  def process_faces(faces)
    @model.start_operation(db("Match Reversed Face Materials"))
    texture_writer=Sketchup.create_texture_writer
    count=0
    faces.each{|face|
        if face.material==nil
          face.back_material=nil
        elsif face.material.texture==nil
          face.back_material=face.material
        else ### textured
          samples = []
          samples << face.vertices[0].position			   ### 0,0 | Origin
		  samples << samples[0].offset(face.normal.axes.x) ### 1,0 | Offset Origin in X
	  	  samples << samples[0].offset(face.normal.axes.y) ### 0,1 | Offset Origin in Y
		  samples << samples[1].offset(face.normal.axes.y) ### 1,1 | Offset X in Y
		  xyz = [];uv = []### Arrays containing 3D and UV points.
		  uvh = face.get_UVHelper(true, true, texture_writer)
		  samples.each { |position|
			xyz << position ### XYZ 3D coordinates
			uvq = uvh.get_front_UVQ(position) ### UV 2D coordinates
			uv << self.flattenUVQ(uvq)
		  }
		  pts = [] ### Position texture.
		  (0..3).each { |i|
			 pts << xyz[i]
			 pts << uv[i]
		  }
          mat=face.material
		  face.position_material(mat, pts, false)
        end#if
        count+=1
    }
    @model.commit_operation
    return count
    ###
  end#def
  
  ### Get UV coordinates from UVQ matrix.
  def flattenUVQ(uvq)
	return Geom::Point3d.new(uvq.x / uvq.z, uvq.y / uvq.z, 1.0)
  end

end#class

################

def fixreversedfacematerials()
  Sketchup.active_model.select_tool(FixReversedFaceMaterials.new)
end#def
def fixreversedfacematerialsforced()
  Sketchup.active_model.select_tool(FixReversedFaceMaterialsForced.new)
end#def
def fixreversedfacematerialsflip()
  Sketchup.active_model.select_tool(FixReversedFaceMaterialsFlip.new)
end#def
def deletefacesbackmaterials()
  Sketchup.active_model.select_tool(DeleteFacesBackMaterials.new)
end#def
def matchfacesbackmaterials()
  Sketchup.active_model.select_tool(MatchFacesBackMaterials.new)
end#def

### add menu item etc #####################################################
unless file_loaded?(File.basename(__FILE__))
  sub=UI.menu("Plugins").add_submenu(FixReversedFaceMaterials::db("TIG: Reversed Face Tools..."))
  ### make command1
  text10 = (FixReversedFaceMaterials::db("Fix Reversed Face Materials"))
  text11 = (FixReversedFaceMaterials::db("Fix Reversed Face Materials"))
  text12 = (FixReversedFaceMaterials::db("Fix Reversed Face Materials"))+"..."
  cmd1=UI::Command.new(text10){fixreversedfacematerials()}
  cmd1.tooltip=text11
  cmd1.status_bar_text=text12
  sub.add_item(cmd1)
  ### make command2
  text20 = (FixReversedFaceMaterialsForced::db("Fix Reversed Face Materials [Forced]"))
  text21 = (FixReversedFaceMaterialsForced::db("Fix Reversed Face Materials [Forced]"))
  text22 = (FixReversedFaceMaterialsForced::db("Fix Reversed Face Materials [Forced]"))+"..."
  cmd2=UI::Command.new(text20){fixreversedfacematerialsforced()}
  cmd2.tooltip=text21
  cmd2.status_bar_text=text22
  sub.add_item(cmd2)
  ### make command3
  text30 = (FixReversedFaceMaterialsFlip::db("Reverse Faces and Materials"))
  text31 = (FixReversedFaceMaterialsFlip::db("Reverse Faces and Materials"))
  text32 = (FixReversedFaceMaterialsFlip::db("Reverse Faces and Materials"))+"..."
  cmd3=UI::Command.new(text30){fixreversedfacematerialsflip()}
  cmd3.tooltip=text31
  cmd3.status_bar_text=text32
  sub.add_item(cmd3)
  ### make command4
  text40 = (DeleteFacesBackMaterials::db("Delete Back-Materials"))
  text41 = (DeleteFacesBackMaterials::db("Delete Back-Materials"))
  text42 = (DeleteFacesBackMaterials::db("Delete Back-Materials"))+"..."
  cmd4=UI::Command.new(text40){deletefacesbackmaterials()}
  cmd4.tooltip=text41
  cmd4.status_bar_text=text42
  sub.add_item(cmd4)
  ### make command5
  text50 = (MatchFacesBackMaterials::db("Match Back-Materials"))
  text51 = (MatchFacesBackMaterials::db("Match Back-Materials"))
  text52 = (MatchFacesBackMaterials::db("Match Back-Materials"))+"..."
  cmd5=UI::Command.new(text50){matchfacesbackmaterials()}
  cmd5.tooltip=text51
  cmd5.status_bar_text=text52
  sub.add_item(cmd5)
  ###
end#if
file_loaded(File.basename(__FILE__))
###
