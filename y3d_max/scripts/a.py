import argparse
import bpy
 
def get_args():
  parser = argparse.ArgumentParser()
 
  # get all script args
  _, all_arguments = parser.parse_known_args()
  double_dash_index = all_arguments.index('--')
  script_args = all_arguments[double_dash_index + 1: ]
 
  # add parser rules
  parser.add_argument('-n', '--number', help="number of cubes")
  parser.add_argument('-m', '--save', help="output file")
  parsed_script_args, _ = parser.parse_known_args(script_args)
  return parsed_script_args
 
args = get_args()
number_of_cubes = int(args.number)
 
# create cubes
if number_of_cubes > 1:
  for x in range(0, number_of_cubes):
    bpy.ops.mesh.primitive_cube_add(location=(x, 0, 0))
 
# export scene
bpy.ops.export_scene.obj(filepath=args.save)