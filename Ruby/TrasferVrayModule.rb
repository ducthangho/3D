require 'sketchup.rb'
require 'VfSExport.rb'




puts "hello"


def cnv (input, output)
	return "vr."<< input <<" = "<< output.to_s << "\n"
end

def export_maxscript 
	s = ""
	options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
            # if options_hash_as_array != nil
    options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )

	output_xml_string = options_hash["/SettingsOutput"]
	output_xml_doc = VRayXML::QDomDocument.new output_xml_string

	output_xml_node = VRayForSketchUp.find_asset_in_doc(output_xml_doc, "/SettingsOutput" );

	animStart = 0.0
    animEnd = 0.0
    framesPerSec = 30.0
    
    # if self.do_animation
    #     animEnd = self.end_time
    #     framesPerSec = self.fps
        
    # end
	


	@img_width = VRayForSketchUp.get_integer_parameter_value_from_xml_node( output_xml_node, "img_width")
    @img_height = VRayForSketchUp.get_integer_parameter_value_from_xml_node( output_xml_node, "img_height")
    override_viewport = VRayForSketchUp.get_bool_parameter_value_from_xml_node( output_xml_node, "override_viewport" )
    if override_viewport
        img_width = @img_width
        img_height = @img_height
    else
        img_width = camera.img_width
        img_height = camera.img_height
    end

    save_image = VRayForSketchUp.get_bool_parameter_value_from_xml_node( output_xml_node, "save_render" )
    output_path_xml = VRayForSketchUp.make_string_parameter_xml( "img_file", "" )
    
    img_width_xml = VRayForSketchUp.make_int_parameter_xml( "img_width", img_width )
    img_height_xml = VRayForSketchUp.make_int_parameter_xml("img_height", img_height )
    bmp_width_xml = VRayForSketchUp.make_int_parameter_xml("bmp_width", img_width )
    bmp_height_xml = VRayForSketchUp.make_int_parameter_xml( "bmp_height", img_height )

    region = PythonBridge.get_vfb_region()
    region_width = img_width
    region_height = img_height
    region_left = 0
    region_top = 0
    if not region.nil?
        region_left = region[0]
        region_top = region[1]
        region_width = region[2] - region[0]
        region_height = region[3] - region[1]                    
    end
    # r_left_xml = VRayForSketchUp.make_int_parameter_xml( "r_left", region_left )
    # r_top_xml = VRayForSketchUp.make_int_parameter_xml( "r_top", region_top )
    # r_width_xml = VRayForSketchUp.make_int_parameter_xml( "r_width", region_width )
    # r_height_xml = VRayForSketchUp.make_int_parameter_xml( "r_height", region_height )
    # rgn_width_xml =VRayForSketchUp.make_float_parameter_xml("rgn_width", img_width )
    # rgn_height_xml = VRayForSketchUp.make_float_parameter_xml( "rgn_height", img_height )
    
    # anim_fps_xml = VRayForSketchUp.make_float_parameter_xml("frames_per_second", framesPerSec)
    # render_frame_range=VRayForSketchUp.get_bool_parameter_value_from_xml_node( output_xml_node, "render_frame_range")
    # do_animation = VRayForSketchUp.get_bool_parameter_value_from_xml_node( output_xml_node, "do_animation")
    # frame_start_xml = VRayForSketchUp.make_float_parameter_xml( "frame_start", 0 )
    # If the user is not specifying their own frame range, we will set it up automatically.
    # if not render_frame_range
    #    anim_frames_xml = VRayForSketchUp.make_int_list_parameter_xml_from_range( "frames", 0, self.frame_count)
    #    VRayForSketchUp.replace_parameter_in_xml_node( output_xml_node, anim_frames_xml, 1 )
    # end

    # anim_start_xml = VRayForSketchUp.make_float_parameter_xml("anim_start", animStart)
    # anim_end_xml = VRayForSketchUp.make_float_parameter_xml("anim_end", animEnd)
    
    output_image_path = VRayForSketchUp.get_param_value_as_text( output_xml_doc, "img_file", "string")
    if output_image_path.size > 0
        output_image_dir_path = File.split(output_image_path)[0]
        if output_image_dir_path.length <= 1 or not PythonBridge.DoesFileExist(output_image_dir_path)
            puts "an invalid path was specified for the image output path in the V-Ray Render Options, please correct it if you wish to automatically save your render to disk."
            output_image_path_param = VRayForSketchUp.make_string_parameter_xml("img_file", "")
            VRayForSketchUp.replace_parameter_in_xml_node(output_xml_doc, output_image_path_param)   
        end
    end
    # if do_animation
    #     if not( save_image and ( output_image_path.size > 0) )
    #         msg= "You are not setup to save animation render output!"
    #         UI.messagebox msg
    #         raise msg
    #     end
    #     # There is really no reason for this to be off if animation is going
    #     VRayForSketchUp.replace_parameter_in_xml_node(output_xml_doc, VRayForSketchUp.make_bool_parameter_xml("img_file_needFrameNumber", true ))
    # end
    # if !save_image
    #     VRayForSketchUp.replace_parameter_in_xml_node( output_xml_node, output_path_xml, 1 )
    # end
    s << cnv("output_width",img_width)
    s << cnv("output_height",img_height)
    s << cnv("output_fileName",output_image_path)
    s << cnv("output_regxmin",region_left)
    s << cnv("output_regxmax",region_left+region_width)

    puts s
end

file_loaded("a.rb")
export_maxscript