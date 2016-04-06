require 'sketchup.rb'
require 'VfSExport.rb'



def cnv (input, output)
    if(output)
        return "vr."<< input <<" = "<< output.to_s << "\n"

    end

    return ""
end






def get_int(output_xml_node,name)
    return VRayForSketchUp.get_integer_parameter_value_from_xml_node( output_xml_node, name)
end

def get_bool(output_xml_node,name)
    return VRayForSketchUp.get_bool_parameter_value_from_xml_node( output_xml_node, name)
end

def get_float(output_xml_node,name)
    return VRayForSketchUp.get_float_parameter_value_from_xml_node( output_xml_node, name)
end

def get_double(output_xml_node,name)
    return VRayForSketchUp.get_param_value_node( output_xml_node, name,"double")
end

def get_string(output_xml_node,name)
    return VRayForSketchUp.get_param_value_node( output_xml_node, name,"string").firstChild.to_s
end

def get_list_int(output_xml_node,name)
    framesList = VRayForSketchUp.get_param_value_node(output_xml_node,name,"list").elementsByTagName("entry")
    frames = [];

    framesList.each{ |frame|
        frames << frame.firstChild.to_s.to_i
    }
    return frames
end


def get_list(output_xml_node,name)
    framesList = VRayForSketchUp.get_param_value_node(output_xml_node,name,"list").elementsByTagName("entry")
    frames = [];

    framesList.each{ |frame|
        frames << frame.firstChild.to_s
    }
    return frames
end


def get_from_xml(output_xml_node,name)
  type = "string"
  params = output_xml_node.elementsByTagName( "parameter" )
  for currentParam in params 
    paramName = currentParam.attribute("name")
    if (paramName == name)
      type = currentParam.attribute("type")
      break
    end
  end 
  if type=="integer"
    return VRayForSketchUp.get_integer_parameter_value_from_xml_node( output_xml_node, name)
  elsif type=="bool"
    return VRayForSketchUp.get_bool_parameter_value_from_xml_node( output_xml_node, name)
  elsif type=="float"
    return VRayForSketchUp.get_float_parameter_value_from_xml_node( output_xml_node, name)
  elsif type=="double"
    return VRayForSketchUp.get_param_value_node( output_xml_node, name,"double")
  elsif type=="string"
    return VRayForSketchUp.get_param_value_node( output_xml_node, name,"string").firstChild.to_s
  elsif type=="list"
    framesList = VRayForSketchUp.get_param_value_node(output_xml_node,name,"list").elementsByTagName("entry")
    frames = [];

    framesList.each{ |frame|
        frames << frame.firstChild.to_s.to_i
    }
    return frames
  end
  return nil
end

def c (output_xml_node,input, output)
    puts get_from_xml(output_xml_node,output)
   return cnv(input,get_from_xml(output_xml_node,output) )
end

def get_all_params_node( parentNode, nodeNames = nil )
    if nodeNames == nil
        nodeNames = []
    end
    nodes = parentNode.elementsByTagName("parameter")
    return nil if nodes.length() == 0
    for currentNode in nodes
        nodeNames.push currentNode.attribute('name') 
        # puts currentNode.attribute('name')
    end
    return nodeNames
end



def get_all_params_nodes( parentNode )    
    nodes = parentNode.elementsByTagName("parameter")
    return nil if nodes.length() == 0
    for currentNode in nodes
        key = currentNode.attribute('name') 
        type = currentNode.attribute('type')
        s = "get_"
        if (type == "integer")
            s << "int"
        else
            s << type.to_s
        end

s << c(output_xml_node,        puts "#{key},"default")
        # if su dung thuoc
        #     s = vr.(fdf) = cnafat .valuej
s << c(output_xml_node,        # # puts (key.to_s) << ","default")
        # # puts (key.to_s) << "\n"
        
    end
    
end


def export_settings_output 
    s = ""
    # s = VRayForSketchUp::StringIO.new
    options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
 #            # if options_hash_as_array != nil
    options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )

    # VRayForSketchUp.initScene()
    # VfSExport.scene.cache_scene_options()
    # options_hash = VfSExport.scene.modified_options_lookup

    output_xml_string = options_hash["/SettingsOutput"]
    output_xml_doc = VRayXML::QDomDocument.new output_xml_string

    output_xml_node = VRayForSketchUp.find_asset_in_doc(output_xml_doc, "/SettingsOutput" );
     
    
    # if scene.do_animation
    #     animEnd = scene.end_time
    #     framesPerSec = self.fps
        
    # end
    
    # puts output_xml_node


    @img_width = get_int( output_xml_node, "img_width")
    @img_height = get_int( output_xml_node, "img_height")
    override_viewport = get_bool( output_xml_node, "override_viewport" )
    if override_viewport
        img_width = @img_width
        img_height = @img_height
    else
        img_width = camera.img_width
        img_height = camera.img_height
    end

    img_rawFileVFB = get_int( output_xml_node, "img_rawFileVFB")
    rgn_width = get_float( output_xml_node, "rgn_width")
    img_rawFile = get_bool( output_xml_node, "img_rawFile")
    r_height = get_int( output_xml_node, "r_height")
    frame_start = get_int( output_xml_node, "frame_start")
    # framesList = VRayForSketchUp.get_param_value_node(output_xml_node,"frames","list").elementsByTagName("entry")
    frames = get_list_int(output_xml_node,"frames")    
    bmp_width = get_int( output_xml_node, "bmp_width")
    anim_start = get_double( output_xml_node, "anim_start")
    rgn_height = get_float( output_xml_node, "rgn_height")
    save_render = get_bool(output_xml_node, "save_render")
    frames_per_second = get_float(output_xml_node, "frames_per_second")
    frame_stamp_enabled = get_bool(output_xml_node, "frame_stamp_enabled")
    rgn_left = get_float(output_xml_node, "rgn_left")
    do_animation = get_bool(output_xml_node, "do_animation")
    render_frame_range = get_bool(output_xml_node, "render_frame_range")
    r_left = get_int( output_xml_node, "r_left")
    img_file = get_string(output_xml_node, "img_file")
    rgn_top = get_float(output_xml_node,"rgn_top")
    img_pixelAspect = get_float(output_xml_node,"img_pixelAspect")
    img_imageAspectLocked = get_bool(output_xml_node, "img_imageAspectLocked")
    frame_stamp_text = get_string(output_xml_node, "frame_stamp_text")
    img_pixelAspectLocked = get_bool(output_xml_node, "img_pixelAspectLocked")
    frame_rate = get_int( output_xml_node, "frame_rate")
    img_dir = get_string(output_xml_node, "img_dir") 
    img_imageAspect = get_float(output_xml_node, "img_imageAspect")
    img_separateAlpha = get_bool(output_xml_node, "img_separateAlpha")
    anim_end = get_double(output_xml_node, "anim_end")
    r_top = get_int( output_xml_node, "r_top")
    bmp_height = get_int( output_xml_node, "bmp_height")
    r_width =  get_int( output_xml_node, "r_width")
    img_file_needFrameNumber = get_bool( output_xml_node, "img_file_needFrameNumber")
     



    vroptionSketchup = get_bool( output_xml_node, dsadsakjhdsk)
    vroptionSketchup = get_bool( output_xml_node, dsadsakjhdsk)
    vroptionSketchup = get_bool( output_xml_node, dsadsakjhdsk)
    vroptionSketchup = get_bool( output_xml_node, dsadsakjhdsk)
   
    s << cnv("output_aspectlock",img_pixelAspectLocked)
    s << cnv("output_imageaspect ",img_imageAspect)
    s << cnv("output_width",img_width)
    s << cnv("output_height",img_height)
    s << cnv("output_aspect",img_pixelAspect)

    s << cnv("output_fileOnly",(not img_rawFile and not img_file == ""))
    s << cnv("output_saveFile",(not img_file == ""))
    
    s << cnv("output_fileName",img_file)
    s << cnv("output_saveRawFile",img_rawFile)
    # s << cnv("output_rawFileName",img_rawFile)

    # s << cnv("output_useram",true)
  #   s << cnv("output_genpreview",true)
s << c(output_xml_node,  #   .output_splitgbuffer,"default")
s << c(output_xml_node,  # .output_splitfilename,"default")
s << c(output_xml_node,  # .output_splitbitmap,"default")
s << c(output_xml_node,  # .output_getsetsfrommax,"default")
s << c(output_xml_node,  # .output_splitRGB,"default")
s << c(output_xml_node,  # .output_splitAlpha,"default")
s << c(output_xml_node,  # .output_renderType,"default")

    s << cnv("output_regxmin",rgn_left)
    s << cnv("output_regxmax",rgn_left+rgn_width)
    s << cnv("output_regymin",rgn_top)
    s << cnv("output_regymax",rgn_top+rgn_height)      
    

    puts s.to_s
end

def export_photonMap
    s = ""
    # s = VRayForSketchUp::StringIO.new
    options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
 #            # if options_hash_as_array != nil
    options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )

    # VRayForSketchUp.initScene()
    # VfSExport.scene.cache_scene_options()
    # options_hash = VfSExport.scene.modified_options_lookup

    output_xml_string = options_hash["/SettingsPhotonMap"]
    output_xml_doc = VRayXML::QDomDocument.new output_xml_string

    output_xml_node = VRayForSketchUp.find_asset_in_doc(output_xml_doc, "/SettingsPhotonMap" );
    # puts output_xml_node
    get_all_params_nodes output_xml_node
    # puts nodeNames

    s << cnv("photonMap_bounces",get_int(output_xml_node,"bounces") )
    s << cnv("photonMap_searchDist",get_float(output_xml_node,"search_distance" ) )
    s << cnv("photonMap_autoDist",get_bool(output_xml_node,auto_search_distance) )
    s << cnv("photonMap_autoSave",get_bool(output_xml_node,auto_save) )
    s << cnv("photonMap_autoSaveFileName",get_string(output_xml_node,auto_save_file) )
    s << cnv("photonMap_maxPhotons",get_int(output_xml_node,"max_photons"))

    # s << cnv("photonMap_convert",get_bool(output_xml_node,"max_photons"))
    s << cnv("photonMap_interpSamples",get_int(output_xml_node,"max_photons"))
end

# def export_irrad_map
#     s = ""
#     # s = VRayForSketchUp::StringIO.new
#     options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
#  #            # if options_hash_as_array != nil
#     options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )

#     # VRayForSketchUp.initScene()
#     # VfSExport.scene.cache_scene_options()
#     # options_hash = VfSExport.scene.modified_options_lookup

#     output_xml_string = options_hash["/SettingsIrradianceMap"]
#     output_xml_doc = VRayXML::QDomDocument.new output_xml_string

#     output_xml_node = VRayForSketchUp.find_asset_in_doc(output_xml_doc, "/SettingsIrradianceMap" );
#     puts output_xml_node

#     s << cnv()
# end

def export_photonMap
    s = ""
    # s = VRayForSketchUp::StringIO.new
    options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
 #            # if options_hash_as_array != nil
    options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )

    # VRayForSketchUp.initScene()
    # VfSExport.scene.cache_scene_options()
    # options_hash = VfSExport.scene.modified_options_lookup

    output_xml_string = options_hash["/SettingsPhotonMap"]
    output_xml_doc = VRayXML::QDomDocument.new output_xml_string

    output_xml_node = VRayForSketchUp.find_asset_in_doc(output_xml_doc, "/SettingsPhotonMap" );
    # puts output_xml_node
    puts (get_all_params_node output_xml_node)
    # puts nodeNames

    s << cnv("photonMap_bounces",get_int(output_xml_node,"bounces") )
    s << cnv("photonMap_searchDist",get_float(output_xml_node,"search_distance" ) )
    s << cnv("photonMap_autoDist",get_bool(output_xml_node,auto_search_distance) )
    s << cnv("photonMap_autoSave",get_bool(output_xml_node,auto_save) )
    s << cnv("photonMap_autoSaveFileName",get_string(output_xml_node,auto_save_file) )
    s << cnv("photonMap_maxPhotons",get_int(output_xml_node,"max_photons"))

    # s << cnv("photonMap_convert",get_bool(output_xml_node,"max_photons"))
    s << cnv("photonMap_interpSamples",get_int(output_xml_node,"max_photons"))
end

def export_option
    s = ""
    # s = VRayForSketchUp::StringIO.new
    options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
 #            # if options_hash_as_array != nil
    options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )

    # VRayForSketchUp.initScene()
    # VfSExport.scene.cache_scene_options()
    # options_hash = VfSExport.scene.modified_options_lookup

    output_xml_string = options_hash["/SettingsOptions"]
    output_xml_doc = VRayXML::QDomDocument.new output_xml_string

    output_xml_node = VRayForSketchUp.find_asset_in_doc(output_xml_doc, "/SettingsOptions" );
    # puts output_xml_node
    # get_all_params_nodes output_xml_node

    # s << c(".options_defaultLights",)
    # s << c(output_xml_doc,"options_defaultLights")
    s << c(output_xml_node,"options_displacement","geom_displacement")
    # s << cnv(".")
    s << c(output_xml_node,"options_dontRenderImage","gi_dontRenderImage")
    s << c(output_xml_node,"options_displacement","geom_displacement")
    s << c(output_xml_node,"options_filterMaps","mtl_filterMaps")
    s << c(output_xml_node,"options_geom_backfaceCull","geom_backfaceCull")
    s << c(output_xml_node,"options_glossyEffects","mtl_glossy")
    s << c(output_xml_node,"options_glossyEffects","mtl_glossy")







#      s << c(output_xml_node,"options_defaultLights","default")
#         s << c(output_xml_node,"options_displacement","default")
# s << c(output_xml_node,"options_dontRenderImage","default")
# s << c(output_xml_node,"options_filterMaps","default")
# s << c(output_xml_node,"options_geom_backfaceCull","default")
# s << c(output_xml_node,"options_glossyEffects","default")
# s << c(output_xml_node,"options_hiddenLights","default")
# s << c(output_xml_node,"options_lights","default")
#   .options_limitDepth (alias for options_overrideDepth_on)
# s << c(output_xml_node,"options_maps","default")
#   .options_maxDepth (alias for options_overrideDepth)
# s << c(output_xml_node,"options_maxRayIntensity","default")
# s << c(output_xml_node,"options_maxRayIntensity_on (options_clampRayIntensity)","default")
# s << c(output_xml_node,"options_mtl_filterMaps_indirect","default")
# s << c(output_xml_node,"options_overrideDepth","default")
# s << c(output_xml_node,"options_overrideDepth_on","default")
# s << c(output_xml_node,"options_overrideMtl_mtl","default")
# s << c(output_xml_node,"options_overrideMtl_on","default")
# s << c(output_xml_node,"options_probabilisticLights","default")
# s << c(output_xml_node,"options_probabilisticLightsCount","default")
# s << c(output_xml_node,"options_ray_bias","default")
# s << c(output_xml_node,"options_reflectionRefraction","default")
# s << c(output_xml_node,"options_shadows","default")
# s << c(output_xml_node,"options_showGIOnly","default")
# s << c(output_xml_node,"options_transpCutoff","default")
# s << c(output_xml_node,"options_transpMaxLevels","default")
# s << c(output_xml_node,"options_use3dsMaxPhotometricUnits","default")
# s << c(output_xml_node,"options_useLegacyModels","default")
# s << c(output_xml_node,"options_ui_view","default")

    puts s

end



file_loaded("VfSExport.rb")
file_loaded("TrasferVrayModule.rb")
# export_settings_output
# export_photonMap
# export_irrad_map
export_option