# First we pull in the standard API hooks.
require 'sketchup.rb'

# Show the Ruby Console at startup so we can
# see any programming errors we may make.
# SKETCHUP_CONSOLE.show

# Add a menu item to launch our plugin.
UI.menu("Plugins").add_item("Change Materials Name Module") {
  UI.messagebox("I'm about to draw stairs!") 

  # Call our new method.
  changeMaterialsName
}

# def draw_stairs
#   # Get "handles" to our model and the Entities collection it contains.
#   model = Sketchup.active_model
#   entities = model.entities

#   # Create a series of "points", each a 3-item array containing x, y, and z.
#   pt1 = [0, 0, 0]
#   pt2 = [9, 0, 0]
#   pt3 = [9, 9, 0]
#   pt4 = [0, 9, 0]
  
#   # Call methods on the Entities collection to draw stuff.
#   new_face = entities.add_face pt1, pt2, pt3, pt4
# end

def changeMaterialsName
  # count = 0
  regex = "/[^a-zA-Z0-9]/"
  materials = Sketchup.active_model.materials;

  # materials.each { |x| puts x.name }
  materials.each do |x|
    # count = count+1 
    # x.name = count.to_s
    x.name = x.name.gsub(/[^a-zA-Z0-9]/,"_");

  end
  # # Create some variables.
  # stairs = 10
  # rise = 8
  # run = 12
  # width = 100
  # thickness = 3

  # # Get handles to our model and the Entities collection it contains.
  # model = Sketchup.active_model
  # entities = model.entities

  # # Loop across the same code several times
  # for step in 1..stairs
    
  #   # Calculate our stair corners.
  #   x1 = 0
  #   x2 = width
  #   y1 = run * step
  #   y2 = run * (step + 1)
  #   z = rise * step
    
  #   # Create a series of "points", each a 3-item array containing x, y, and z.
  #   pt1 = [x1, y1, z]
  #   pt2 = [x2, y1, z]
  #   pt3 = [x2, y2, z]
  #   pt4 = [x1, y2, z]

  #   # Call methods on the Entities collection to draw stuff.
  #   new_face = entities.add_face pt1, pt2, pt3, pt4
  #   new_face.pushpull thickness
  # end

end

