require 'sketchup.rb'
require 'VfSExport.rb'

def gen_ms_by_type(maxKey,suValue,suType=nil)
  s = "vr."
  case suType
  when "string","filename"
    s << "#{maxKey}='#{suValue.to_s}'"
  when "float","worldunit","integer","double",nil
    s << "#{maxKey}=#{suValue.to_s}"
  when "color" #RGB Color
    s << "#{maxKey}=(#{suValue.r},#{suValue.g},#{suValue.b})"
  when "transform" #RGB Color
    v0 = suValue[0]
    v1 = suValue[1]
    v2 = suValue[2]
    v3 = suValue[3]
    transform = "(matrix3 [#{v0.x},#{v0.y},#{v0.z}] [#{v1.x},#{v1.y},#{v1.z}] [#{v2.x},#{v2.y},#{v2.z}] [#{v3.x},#{v3.y},#{v3.z}])"
    s << "#{maxKey}=#{transform}"
  end
  s << "\n"
  return s
end

def cnv (input, output, type = nil)
    if(output == nil)
        return ""
    end

    if (output == "default") 
        return ""
    end
    # return "vr."<< input <<" = "<< output.to_s << "\n"
    return gen_ms_by_type(input,output,type)    
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


def get_from_xml(output_xml_node,name,type=nil)
  if (type == nil)     
    params = output_xml_node.elementsByTagName( "parameter" )
    for currentParam in params 
        paramName = currentParam.attribute("name")
        if (paramName == name)
          type = currentParam.attribute("type")
          break
        end
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

def c (output_xml_node,input, output)
    if (output == "default") 
        return ""
    end
    # puts get_from_xml(output_xml_node,output)
    params = output_xml_node.elementsByTagName( "parameter" )
    for currentParam in params 
        paramName = currentParam.attribute("name")
        if (paramName == input)
          type = currentParam.attribute("type")
          break
        end
    end 

    out = get_from_xml(output_xml_node,output,type)
    # puts "#{input} : #{out}   #{type}"
    return cnv(input, out,type)
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

def get_xml_node(key)
     options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
 #            # if options_hash_as_array != nil
    options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )

    # VRayForSketchUp.initScene()
    # VfSExport.scene.cache_scene_options()
    # options_hash = VfSExport.scene.modified_options_lookup

    output_xml_string = options_hash["/#{key}"]
    output_xml_doc = VRayXML::QDomDocument.new output_xml_string
    return VRayForSketchUp.find_asset_in_doc(output_xml_doc, "/#{key}" );
end


def export_settings_output 
	s = ""
	output_xml_node = get_xml_node("SettingsOutput");
	 
 
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
    
   
    s << cnv("output_aspectlock",img_pixelAspectLocked)
    s << cnv("output_imageaspect ",img_imageAspect)
    s << cnv("output_width",img_width)
    s << cnv("output_height",img_height)
    s << cnv("output_aspect",img_pixelAspect)

    s << cnv("output_fileOnly",(not img_rawFile and not img_file == ""))
    
    
    s << cnv("output_fileName","\"#{img_file}\"")
    # s << cnv("output_saveRawFile",img_rawFile)
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
    s << c(output_xml_node,"output_splitAlpha","img_separateAlpha")    
    s << c(output_xml_node,"output_saveFile","save_render")    
    s << c(output_xml_node,"output_saveRawFile","img_rawFile")
    s << c(output_xml_node,"output_renderType","default")
    s << c(output_xml_node,"output_splitbitmap","default")

    s << c(output_xml_node,"system_frameStamp_on","frame_stamp_enabled")
    s << c(output_xml_node,"system_frameStamp_string","frame_stamp_text")
    s << c(output_xml_node,"system_frameStamp_justify","default")
    s << c(output_xml_node,"system_frameStamp_fullWidth","default")
    

    puts s.to_s
end

def export_photonMap
    s = ""
    output_xml_node = get_xml_node("SettingsPhotonMap");

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
    output_xml_node = get_xml_node("SettingsOptions");

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

def export_lightcache
    s = ""
    output_xml_node = get_xml_node("SettingsLightCache");    

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
    s << c(output_xml_node,"filter_size","filter_size")    


    s << c(output_xml_node,"progressive_max_render_time","default")
    s << c(output_xml_node,"progressive_maxSamples","default")
    s << c(output_xml_node,"progressive_minSamples","default")
    s << c(output_xml_node,"progressive_noise_threshold","default")
    s << c(output_xml_node,"progressive_ray_bundle_size","default")
    s << c(output_xml_node,"progressive_show_mask","default")
    s << c(output_xml_node,"progressive_track_radius","default")

    puts s 
end

def export_raycaster
    s = ""
    output_xml_node = get_xml_node("SettingsRaycaster");    
    #get_all_params_nodes output_xml_node


    s << c(output_xml_node,"system_raycaster_faceLevelCoeff","faceLevelCoef")
    s << c(output_xml_node,"system_raycaster_maxLevels","maxLevels")
    s << c(output_xml_node,"system_raycaster_memLimit","dynMemLimit")
    s << c(output_xml_node,"system_raycaster_minLeafSize","minLeafSize")
    s << c(output_xml_node,"system_raycaster_optLevel","optLevel")
    puts s 
end

def export_motionblur
    s = ""
    output_xml_node = get_xml_node("SettingsMotionBlur");    
    get_all_params_nodes output_xml_node

    s << c(output_xml_node,"moblur_bias","bias")
    s << c(output_xml_node,"moblur_dmc_minSamples","low_samples")
    s << c(output_xml_node,"moblur_duration","duration")
    s << c(output_xml_node,"moblur_geometryParticles","default")
    s << c(output_xml_node,"moblur_geomSamples","geom_samples")
    s << c(output_xml_node,"moblur_intervalCenter","interval_center")
    s << c(output_xml_node,"moblur_on","on")
    s << c(output_xml_node,"moblur_prepassSamples","default")
    s << c(output_xml_node,"moblur_shutterEfficiency","default")
   
    puts s 
end

def export_image_sampler
    s = ""
    output_xml_node = get_xml_node("SettingsImageSampler");    
    # get_all_params_nodes output_xml_node

    s << c(output_xml_node,"imageSamper_normal_threshold","subdivision_normals_threshold")
    s << c(output_xml_node,"imageSamper_renderMask_layers","default")
    s << c(output_xml_node,"imageSamper_renderMask_texmap","default")
    s << c(output_xml_node,"imageSampler_mtlID","default")
    s << c(output_xml_node,"imageSampler_normal","default")
    s << c(output_xml_node,"imageSampler_outline","subdivision_edges")
    s << c(output_xml_node,"imageSampler_renderMask_type","default")
    s << c(output_xml_node,"imageSampler_shadingRate","default")
    s << c(output_xml_node,"imageSampler_type","type")
    s << c(output_xml_node,"imageSampler_zvalue","default")
    s << c(output_xml_node,"imageSampler_zvalue_threshold","default")

    s << c(output_xml_node,"fixedRate_subdivs","fixed_subdivs")
   
    puts s 
end

def export_gi
    s = ""
    output_xml_node = get_xml_node("SettingsGI");    
    # get_all_params_nodes output_xml_node

    s << c(output_xml_node,"gi_ao_amount","ao_amount")
    s << c(output_xml_node,"gi_ao_on","ao_on")
    s << c(output_xml_node,"gi_ao_radius","ao_radius")
    s << c(output_xml_node,"gi_ao_subdivs","ao_subdivs")
    s << c(output_xml_node,"gi_contrast","contrast")
    s << c(output_xml_node,"gi_contrast_base","contrast_base")
    s << c(output_xml_node,"gi_irradmap_blurGI","default")
    s << c(output_xml_node,"gi_irradmap_colorThreshold","default")
    s << c(output_xml_node,"gi_irradmap_detail_on (irradmap_detail_on)","default")
    s << c(output_xml_node,"gi_irradmap_detail_radius (irradmap_detail_radius)","default")
    s << c(output_xml_node,"gi_irradmap_detail_scale (irradmap_detail_scale)","default")
    s << c(output_xml_node,"gi_irradmap_detail_subdivsMult (irradmap_detail_subdivsMult)","default")
    s << c(output_xml_node,"gi_irradmap_distThreshold","default")
    s << c(output_xml_node,"gi_irradmap_interpFrames","default")
    s << c(output_xml_node,"gi_irradmap_interpSamples","default")
    s << c(output_xml_node,"gi_irradmap_maxRate","default")
    s << c(output_xml_node,"gi_irradmap_minRate","default")
    s << c(output_xml_node,"gi_irradmap_multipleViews","default")
    s << c(output_xml_node,"gi_irradmap_normalThreshold","default")
    s << c(output_xml_node,"gi_irradmap_preset","default")
    s << c(output_xml_node,"gi_irradmap_previewMode (irradmap_previewMode)","default")
    s << c(output_xml_node,"gi_irradmap_showCalcPhase","default")
    s << c(output_xml_node,"gi_irradmap_showDirectLight","default")
    s << c(output_xml_node,"gi_irradmap_showSamples","default")
    s << c(output_xml_node,"gi_irradmap_subdivs","default")
    s << c(output_xml_node,"gi_on","on")
    s << c(output_xml_node,"gi_primary_multiplier","primary_multiplier")
    s << c(output_xml_node,"gi_primary_type","primary_engine")
    s << c(output_xml_node,"gi_rayDistance","ray_distance")
    s << c(output_xml_node,"gi_rayDistanceOn","ray_distance_on")
    s << c(output_xml_node,"gi_reflectCaustics","reflect_caustics")
    s << c(output_xml_node,"gi_refractCaustics","refract_caustics")
    s << c(output_xml_node,"gi_saturation","saturation")
    s << c(output_xml_node,"gi_saveMapsPerFrame","save_maps_per_frame")
    s << c(output_xml_node,"gi_secondary_multiplier","secondary_multiplier")
    s << c(output_xml_node,"gi_secondary_type","secondary_engine")
    s << c(output_xml_node,"gi_ui_view","default")
    puts s 
end

def export_irradmap
    s = ""
    output_xml_node = get_xml_node("SettingsIrradianceMap");    
    # get_all_params_nodes output_xml_node
   
    s << c(output_xml_node,"gi_irradmap_blurGI","default")
    s << c(output_xml_node,"gi_irradmap_colorThreshold","color_threshold")
    s << c(output_xml_node,"gi_irradmap_detail_on","default")
    s << c(output_xml_node,"gi_irradmap_detail_radius","detail_radius")
    s << c(output_xml_node,"gi_irradmap_detail_scale","detail_scale")
    s << c(output_xml_node,"gi_irradmap_detail_subdivsMult","detail_subdivs_mult")
    s << c(output_xml_node,"gi_irradmap_distThreshold","distance_threshold")
    s << c(output_xml_node,"gi_irradmap_interpFrames","interp_frames")
    s << c(output_xml_node,"gi_irradmap_interpSamples","interp_samples")
    s << c(output_xml_node,"gi_irradmap_maxRate","max_rate")
    s << c(output_xml_node,"gi_irradmap_minRate","min_rate")
    s << c(output_xml_node,"gi_irradmap_multipleViews","multiple_views")
    s << c(output_xml_node,"gi_irradmap_normalThreshold","normal_threshold")
    s << c(output_xml_node,"gi_irradmap_preset","default")
    s << c(output_xml_node,"gi_irradmap_previewMode","default")
    s << c(output_xml_node,"gi_irradmap_showCalcPhase","show_calc_phase")
    s << c(output_xml_node,"gi_irradmap_showDirectLight","show_direct_light")
    s << c(output_xml_node,"gi_irradmap_showSamples","show_samples")
    s << c(output_xml_node,"gi_irradmap_subdivs","subdivs")
   
    puts s 
end

def export_environment
    s = ""
    output_xml_node = get_xml_node("SettingsEnvironment");    
    get_all_params_nodes output_xml_node
   
    s << c(output_xml_node,"environment_gi_color","gi_color")
    s << c(output_xml_node,"environment_gi_color_multiplier","default")
    s << c(output_xml_node,"environment_gi_map","gi_tex")
    s << c(output_xml_node,"environment_gi_map_on","gi_tex_on")
    s << c(output_xml_node,"environment_gi_on","use_gi")
    s << c(output_xml_node,"environment_refract_color","refract_color")
    s << c(output_xml_node,"environment_refract_color_multiplier","default")
    s << c(output_xml_node,"environment_refract_map","refract_tex")
    s << c(output_xml_node,"environment_refract_map_on","refract_tex_on")
    s << c(output_xml_node,"environment_refract_on","use_refract")
    s << c(output_xml_node,"environment_rr_color","reflect_color")
    s << c(output_xml_node,"environment_rr_color_multiplier","reflect_tex_mult")
    s << c(output_xml_node,"environment_rr_map","reflect_tex")
    s << c(output_xml_node,"environment_rr_map_on","reflect_tex_on")
    s << c(output_xml_node,"environment_rr_on","use_reflect")
   
    puts s 
end


file_loaded("VfSExport.rb")
file_loaded("TrasferVrayModule.rb")
# export_settings_output
# export_photonMap
# export_irrad_map
#export_lightcache
# export_raycaster
# export_motionblur
# export_image_sampler
# export_gi
# export_irradmap
export_environment