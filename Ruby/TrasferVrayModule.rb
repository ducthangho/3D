require 'sketchup.rb'
require 'VfSExport.rb'

def gen_ms_by_type(maxKey,suValue,suType,prefix="vr.")
  s = prefix
  case suType
    when "filename" 
      s << "#{maxKey}=undefined"
    when "string","float","worldunit","integer","double" 
      s << "#{maxKey}=#{suValue.to_s}"
    when "color"  #RGB Color
      s << "#{maxKey}=(#{suValue.r},#{suValue.g},#{suValue.b})"
    when "transform" 
      v0 = suValue[0]
      v1 = suValue[1]
      v2 = suValue[2]
      v3 = suValue[3]
      transform = "(matrix3 [#{v0.x},#{v0.y},#{v0.z}] [#{v1.x},#{v1.y},#{v1.z}] [#{v2.x},#{v2.y},#{v2.z}] [#{v3.x},#{v3.y},#{v3.z}])"
      s << "#{maxKey}=#{transform}"
    else return ""
  end
  s << "\n"
  return s
end

<<<<<<< HEAD

def cnv (input, output)
    if(output)
        if(output=="default") return ""
        end
        return "vr."<< input <<" = "<< output.to_s << "\n"
    end
    return ""
end

=======
def c (output_xml_node,input, output)
    if (output == "default") 
        return ""
    end
    type = nil
    params = output_xml_node.elementsByTagName( "parameter" )
    for currentParam in params 
        paramName = currentParam.attribute("name")
        if (paramName == output)
          type = currentParam.attribute("type").to_s
          break
        end
    end 
    if type == nil 
        return ""
    end
    return gen_ms_by_type(input,get_from_xml(output_xml_node,output,type),type)
    # puts get_from_xml(output_xml_node,output)
    # return cnv(input,get_from_xml(output_xml_node,output) )
end

def cnv (input, output)
    if(output == nil)
        return ""
    end

    if (output == "default") 
        return ""
    end
    return "vr."<< input <<" = "<< output.to_s << "\n"


    
end

>>>>>>> 2fbacd75c5d23e0f200dcea14a30a34508d1c230
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


def get_from_xml(output_xml_node,name,type=nil)
  if (type == nil) 
    type = "string"
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
    val = VRayForSketchUp.get_param_value_node( output_xml_node, name,"string")
    return "\"#{val.firstChild.to_s}\"" if val
    return "\"\""
  elsif type=="filename"
    val = VRayForSketchUp.get_param_value_node( output_xml_node, name,"string")
    return "\"#{val.firstChild.to_s}\"" if val
    return "\"\""
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
        # s = "get_"
        # if (type == "integer")
        #     s << "int"
        # else
        #     s << type.to_s
        # end
        puts "#{key} : #{type}"
        
    end
    
end


<<<<<<< HEAD
# def export_settings_output 
#     s = ""
#     # s = VRayForSketchUp::StringIO.new
#     options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
#  #            # if options_hash_as_array != nil
#     options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )
=======
def export_settings_output 
	s = ""
    # s = VRayForSketchUp::StringIO.new
    options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
 #            # if options_hash_as_array != nil
    options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )
>>>>>>> 2fbacd75c5d23e0f200dcea14a30a34508d1c230

#     # VRayForSketchUp.initScene()
#     # VfSExport.scene.cache_scene_options()
#     # options_hash = VfSExport.scene.modified_options_lookup

<<<<<<< HEAD
#     output_xml_string = options_hash["/SettingsOutput"]
#     output_xml_doc = VRayXML::QDomDocument.new output_xml_string

#     output_xml_node = VRayForSketchUp.find_asset_in_doc(output_xml_doc, "/SettingsOutput" );
     
=======
	output_xml_string = options_hash["/SettingsOutput"]
	output_xml_doc = VRayXML::QDomDocument.new output_xml_string

	output_xml_node = VRayForSketchUp.find_asset_in_doc(output_xml_doc, "/SettingsOutput" );
	 
>>>>>>> 2fbacd75c5d23e0f200dcea14a30a34508d1c230
    
#     # if scene.do_animation
#     #     animEnd = scene.end_time
#     #     framesPerSec = self.fps
        
<<<<<<< HEAD
#     # end
    
#     # puts output_xml_node


#     @img_width = get_int( output_xml_node, "img_width")
#     @img_height = get_int( output_xml_node, "img_height")
#     override_viewport = get_bool( output_xml_node, "override_viewport" )
#     if override_viewport
#         img_width = @img_width
#         img_height = @img_height
#     else
#         img_width = camera.img_width
#         img_height = camera.img_height
#     end

#     img_rawFileVFB = get_int( output_xml_node, "img_rawFileVFB")
#     rgn_width = get_float( output_xml_node, "rgn_width")
#     img_rawFile = get_bool( output_xml_node, "img_rawFile")
#     r_height = get_int( output_xml_node, "r_height")
#     frame_start = get_int( output_xml_node, "frame_start")
#     # framesList = VRayForSketchUp.get_param_value_node(output_xml_node,"frames","list").elementsByTagName("entry")
#     frames = get_list_int(output_xml_node,"frames")    
#     bmp_width = get_int( output_xml_node, "bmp_width")
#     anim_start = get_double( output_xml_node, "anim_start")
#     rgn_height = get_float( output_xml_node, "rgn_height")
#     save_render = get_bool(output_xml_node, "save_render")
#     frames_per_second = get_float(output_xml_node, "frames_per_second")
#     frame_stamp_enabled = get_bool(output_xml_node, "frame_stamp_enabled")
#     rgn_left = get_float(output_xml_node, "rgn_left")
#     do_animation = get_bool(output_xml_node, "do_animation")
#     render_frame_range = get_bool(output_xml_node, "render_frame_range")
#     r_left = get_int( output_xml_node, "r_left")
#     img_file = get_string(output_xml_node, "img_file")
#     rgn_top = get_float(output_xml_node,"rgn_top")
#     img_pixelAspect = get_float(output_xml_node,"img_pixelAspect")
#     img_imageAspectLocked = get_bool(output_xml_node, "img_imageAspectLocked")
#     frame_stamp_text = get_string(output_xml_node, "frame_stamp_text")
#     img_pixelAspectLocked = get_bool(output_xml_node, "img_pixelAspectLocked")
#     frame_rate = get_int( output_xml_node, "frame_rate")
#     img_dir = get_string(output_xml_node, "img_dir") 
#     img_imageAspect = get_float(output_xml_node, "img_imageAspect")
#     img_separateAlpha = get_bool(output_xml_node, "img_separateAlpha")
#     anim_end = get_double(output_xml_node, "anim_end")
#     r_top = get_int( output_xml_node, "r_top")
#     bmp_height = get_int( output_xml_node, "bmp_height")
#     r_width =  get_int( output_xml_node, "r_width")
#     img_file_needFrameNumber = get_bool( output_xml_node, "img_file_needFrameNumber")
=======
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
>>>>>>> 2fbacd75c5d23e0f200dcea14a30a34508d1c230
    
   
#     s << cnv("output_aspectlock",img_pixelAspectLocked)
#     s << cnv("output_imageaspect ",img_imageAspect)
#     s << cnv("output_width",img_width)
#     s << cnv("output_height",img_height)
#     s << cnv("output_aspect",img_pixelAspect)

#     s << cnv("output_fileOnly",(not img_rawFile and not img_file == ""))
#     s << cnv("output_saveFile",(not img_file == ""))
    
<<<<<<< HEAD
#     s << cnv("output_fileName",img_file)
#     s << cnv("output_saveRawFile",img_rawFile)
#     # s << cnv("output_rawFileName",img_rawFile)

#     # s << cnv("output_useram",true)
#   #   s << cnv("output_genpreview",true)
#   #   .output_splitgbuffer : boolean
#   # .output_splitfilename : filename
#   # .output_splitbitmap : bitmap
#   # .output_getsetsfrommax : boolean
#   # .output_splitRGB : boolean
#   # .output_splitAlpha : boolean
#   # .output_renderType : integer    
=======
    s << cnv("output_fileName",img_file)
    s << cnv("output_saveRawFile",img_rawFile)
    # s << cnv("output_rawFileName",img_rawFile)

    # s << cnv("output_useram",true)
  #   s << cnv("output_genpreview",true)
  #   .output_splitgbuffer : boolean
  # .output_splitfilename : filename
  # .output_splitbitmap : bitmap
  # .output_getsetsfrommax : boolean
  # .output_splitRGB : boolean
  # .output_splitAlpha : boolean
  # .output_renderType : integer

    s << cnv("output_regxmin",rgn_left)
    s << cnv("output_regxmax",rgn_left+rgn_width)
    s << cnv("output_regymin",rgn_top)
    s << cnv("output_regymax",rgn_top+rgn_height)      
>>>>>>> 2fbacd75c5d23e0f200dcea14a30a34508d1c230
    

#     puts s.to_s
# end

# def export_photonMap
#     s = ""
#     # s = VRayForSketchUp::StringIO.new
#     options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
#  #            # if options_hash_as_array != nil
#     options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )

#     # VRayForSketchUp.initScene()
#     # VfSExport.scene.cache_scene_options()
#     # options_hash = VfSExport.scene.modified_options_lookup

#     output_xml_string = options_hash["/SettingsPhotonMap"]
#     output_xml_doc = VRayXML::QDomDocument.new output_xml_string

<<<<<<< HEAD
#     output_xml_node = VRayForSketchUp.find_asset_in_doc(output_xml_doc, "/SettingsPhotonMap" );
#     # puts output_xml_node
#     get_all_params_nodes output_xml_node
#     # puts nodeNames

    
#     s << cnv("photonMap_searchDist",get_float(output_xml_node,"search_distance" ) )
#     s << cnv("photonMap_autoDist",get_bool(output_xml_node,"auto_search_distance") )
#     s << cnv("photonMap_autoSave",get_bool(output_xml_node,"auto_save") )
#     s << cnv("photonMap_autoSaveFileName",get_string(output_xml_node,"auto_save_file") )
#     s << cnv("photonMap_bounces",get_int(output_xml_node,"bounces") )
#     s << cnv("photonMap_maxPhotons",get_int(output_xml_node,"max_photons"))

#     s << cnv("photonMap_convexHullEstimate",get_bool(output_xml_node,"convex_hull_estimate") )
#     s << cnv("photonMap_dontDelete",get_bool(output_xml_node,"dont_delete") )
#     s << cnv("photonMap_loadFileName",get_string(output_xml_node,"file") )
#     s << cnv("photonMap_maxDensity",get_float(output_xml_node,"max_density") )

#     s << cnv("photonMap_mode",get_int(output_xml_node,"mode") )
#     s << cnv("photonMap_multiplier",get_float(output_xml_node,"multiplier") )
#     s << cnv("photonMap_retraceBounces", get_int(output_xml_node,"retrace_bounces") )
#     s << cnv("photonMap_retraceCorners", get_float(output_xml_node,"retrace_corners") )
#     s << cnv("photonMap_saveFileName", get_string(output_xml_node,"file") )
#     s << cnv("photonMap_showCalcPhase", get_bool(output_xml_node,"show_calc_phase") )
#     s << cnv("photonMap_storeDirectLight", get_bool(output_xml_node,"store_direct_light") )
#     # s << cnv("photonMap_switchToSavedMap", get_bool(output_xml_node,"store_direct_light") )
#     # s << cnv("photonMap_convert",get_bool(output_xml_node,"max_photons"))
#     s << cnv("photonMap_interpSamples",get_int(output_xml_node,"prefilter_samples"))
# end
=======
    output_xml_node = VRayForSketchUp.find_asset_in_doc(output_xml_doc, "/SettingsPhotonMap" );
    # puts output_xml_node
    # get_all_params_nodes output_xml_node
    # puts nodeNames

    
    s << cnv("photonMap_searchDist",get_float(output_xml_node,"search_distance" ) )
    s << cnv("photonMap_autoDist",get_bool(output_xml_node,"auto_search_distance") )
    s << cnv("photonMap_autoSave",get_bool(output_xml_node,"auto_save") )
    s << cnv("photonMap_autoSaveFileName",get_string(output_xml_node,"auto_save_file") )
    s << cnv("photonMap_bounces",get_int(output_xml_node,"bounces") )
    s << cnv("photonMap_maxPhotons",get_int(output_xml_node,"max_photons"))

    s << cnv("photonMap_convexHullEstimate",get_bool(output_xml_node,"convex_hull_estimate") )
    s << cnv("photonMap_dontDelete",get_bool(output_xml_node,"dont_delete") )
    s << cnv("photonMap_loadFileName",get_string(output_xml_node,"file") )
    s << cnv("photonMap_maxDensity",get_float(output_xml_node,"max_density") )

    s << cnv("photonMap_mode",get_int(output_xml_node,"mode") )
    s << cnv("photonMap_multiplier",get_float(output_xml_node,"multiplier") )
    s << cnv("photonMap_retraceBounces", get_int(output_xml_node,"retrace_bounces") )
    s << cnv("photonMap_retraceCorners", get_float(output_xml_node,"retrace_corners") )
    s << cnv("photonMap_saveFileName", get_string(output_xml_node,"file") )
    s << cnv("photonMap_showCalcPhase", get_bool(output_xml_node,"show_calc_phase") )
    s << cnv("photonMap_storeDirectLight", get_bool(output_xml_node,"store_direct_light") )
    # s << cnv("photonMap_switchToSavedMap", get_bool(output_xml_node,"store_direct_light") )
    # s << cnv("photonMap_convert",get_bool(output_xml_node,"max_photons"))
    s << cnv("photonMap_interpSamples",get_int(output_xml_node,"prefilter_samples"))
    puts s
end
>>>>>>> 2fbacd75c5d23e0f200dcea14a30a34508d1c230

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
    
    # s << cnv(".")
    s << c(output_xml_node,"options_dontRenderImage","gi_dontRenderImage")
    s << c(output_xml_node,"options_displacement","geom_displacement")
    s << c(output_xml_node,"options_filterMaps","mtl_filterMaps")
    s << c(output_xml_node,"options_geom_backfaceCull","geom_backfaceCull")
    s << c(output_xml_node,"options_glossyEffects","mtl_glossy")
    # s << c(output_xml_node,"options_glossyEffects","mtl_glossy")

<<<<<<< HEAD
    # s << c(output_xml_node,"options_defaultLights")
    s << c(output_xml_node,"options_displacement","default")
    s << c(output_xml_node,"options_displacement","geom_displacement")
    s << c(output_xml_node,"options_filterMaps","mtl_filterMaps")
    s << c(output_xml_node,"options_geom_backfaceCull","geom_backfaceCull")
    s << c(output_xml_node,"options_glossyEffects","mtl_glossy")
    s << c(output_xml_node,"options_hiddenLights","default")
    s << c(output_xml_node,"options_lights","default")
      .options_limitDepth (alias for options_overrideDepth_on)
    s << c(output_xml_node,"options_maps","default")
      .options_maxDepth (alias for options_overrideDepth)
    s << c(output_xml_node,"options_maxRayIntensity","default")
    s << c(output_xml_node,"options_maxRayIntensity_on (options_clampRayIntensity)","default")
    s << c(output_xml_node,"options_mtl_filterMaps_indirect","default")
    s << c(output_xml_node,"options_overrideDepth","default")
    s << c(output_xml_node,"options_overrideDepth_on","default")
    s << c(output_xml_node,"options_overrideMtl_mtl","default")
    s << c(output_xml_node,"options_overrideMtl_on","default")
    s << c(output_xml_node,"options_probabilisticLights","default")
    s << c(output_xml_node,"options_probabilisticLightsCount","default")
    s << c(output_xml_node,"options_ray_bias","default")
    s << c(output_xml_node,"options_reflectionRefraction","default")
    s << c(output_xml_node,"options_shadows","default")
    s << c(output_xml_node,"options_showGIOnly","default")
    s << c(output_xml_node,"options_transpCutoff","default")
    s << c(output_xml_node,"options_transpMaxLevels","default")
    s << c(output_xml_node,"options_use3dsMaxPhotometricUnits","default")
    s << c(output_xml_node,"options_useLegacyModels","default")
    s << c(output_xml_node,"options_ui_view","default")
=======
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
>>>>>>> 2fbacd75c5d23e0f200dcea14a30a34508d1c230

    puts s
end

def export_lightcache
    s = ""
    # s = VRayForSketchUp::StringIO.new
    options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
 #            # if options_hash_as_array != nil
    options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )

    # VRayForSketchUp.initScene()
    # VfSExport.scene.cache_scene_options()
    # options_hash = VfSExport.scene.modified_options_lookup

    output_xml_string = options_hash["/SettingsLightCache"]
    output_xml_doc = VRayXML::QDomDocument.new output_xml_string

    output_xml_node = VRayForSketchUp.find_asset_in_doc(output_xml_doc, "/SettingsLightCache" );
    # get_all_params_nodes output_xml_node


    s << c(output_xml_node,"lightcache_adaptiveTracing","adaptive_sampling")
    s << c(output_xml_node,"lightcache_adaptiveTracing_dirsOnly","adaptive_dirs_only")
    s << c(output_xml_node,"lightcache_autoSave","auto_save")
    s << c(output_xml_node,"lightcache_autoSaveFileName","auto_save_file")
    s << c(output_xml_node,"lightcache_bounces","default")
    s << c(output_xml_node,"lightcache_dontDelete","dont_delete")
    s << c(output_xml_node,"lightcache_filter_size","filter_size")
    s << c(output_xml_node,"lightcache_filter_type","filter_type")
    s << c(output_xml_node,"lightcache_interpSamples","filter_samples")
    s << c(output_xml_node,"lightcache_loadFileName","default")
    s << c(output_xml_node,"lightcache_minPathsPerSample","min_paths_per_sample")
    s << c(output_xml_node,"lightcache_mode","mode")
    s << c(output_xml_node,"lightcache_multipleViews","multiple_views")
    s << c(output_xml_node,"lightcache_numPasses","num_passes")
    s << c(output_xml_node,"lightcache_prefilter_on","prefilter")
    s << c(output_xml_node,"lightcache_prefilter_samples","prefilter_samples")
    s << c(output_xml_node,"lightcache_retrace_on","retrace_enabled")
    s << c(output_xml_node,"lightcache_retrace_threshold","retrace_threshold")
    s << c(output_xml_node,"lightcache_sampleSize","sample_size")
    s << c(output_xml_node,"lightcache_saveFileName","auto_save_file")
    s << c(output_xml_node,"lightcache_scale","default")
    s << c(output_xml_node,"lightcache_showCalcPhase","show_calc_phase")
    s << c(output_xml_node,"lightcache_storeDirectLight","store_direct_light")
    s << c(output_xml_node,"lightcache_subdivs","subdivs")
    s << c(output_xml_node,"lightcache_switchToSavedMap","default")
    s << c(output_xml_node,"lightcache_ui_view","default")
    s << c(output_xml_node,"lightcache_useForGlossyRays","use_for_glossy_rays")
    puts s 
end

def 


file_loaded("VfSExport.rb")
file_loaded("TrasferVrayModule.rb")
# export_settings_output
# export_photonMap
# export_irrad_map
export_lightcache