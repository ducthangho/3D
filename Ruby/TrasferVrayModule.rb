require 'sketchup.rb'
require 'VfSExport.rb'

def gen_ms_by_type(maxKey,suValue,suType,prefix="vr.")
  s = prefix
  case suType
    when "filename" 
      s << "#{maxKey}=undefined"
    when "bool"
      s << "#{maxKey}=" << (suValue ? "1" : "0")
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

def c (output_xml_node,input, output)
    if (output == "default") 
        return ""
    end
    # puts get_from_xml(output_xml_node,output)
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
    out = get_from_xml(output_xml_node,output,type)
	return gen_ms_by_type(input,out,type)
    # puts "#{input} : #{out}   #{type}"
    # return cnv(input, out,type)
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
    val = VRayForSketchUp.get_param_value_node( output_xml_node, name,"string")
    return "\"#{val.firstChild.to_s}\"" if val
    return "\"\""    
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
    # get_all_params_nodes output_xml_node

	 
 
	
    rgn_width = get_float( output_xml_node, "rgn_width")    
    r_height = get_int( output_xml_node, "r_height")    
    rgn_height = get_float( output_xml_node, "rgn_height")    
    rgn_left = get_float(output_xml_node, "rgn_left")    
    r_left = get_int( output_xml_node, "r_left")    
    rgn_top = get_float(output_xml_node,"rgn_top")    
    r_top = get_int( output_xml_node, "r_top")    
    r_width =  get_int( output_xml_node, "r_width")        
    img_rawFile = get_string( output_xml_node, "img_rawFile")        
    img_file = get_string( output_xml_node, "img_file")        

   
    s << c(output_xml_node,"output_aspectlock","img_pixelAspectLocked")
    s << c(output_xml_node,"output_imageaspect","img_imageAspect")
    s << c(output_xml_node,"output_width","img_width")
    s << c(output_xml_node,"output_height","img_height")
    s << c(output_xml_node,"output_aspect","img_pixelAspect")

    s << cnv("output_fileOnly",(not img_file == "" and img_rawFile),"bool")
    
    
    s << c(output_xml_node,"output_fileName","img_file")
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

    s << cnv("output_regxmin",rgn_left,"float")
    s << cnv("output_regxmax",rgn_left+rgn_width,"float")
    s << cnv("output_regymin",rgn_top,"float")
    s << cnv("output_regymax",rgn_top+rgn_height,"float")  
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
    # puts output_xml_node

    s << c(output_xml_node,"photonMap_searchDist","search_distance" )
    s << c(output_xml_node,"photonMap_autoDist","auto_search_distance" )
    s << c(output_xml_node,"photonMap_autoSave","auto_save" )
    s << c(output_xml_node,"photonMap_autoSaveFileName","auto_save_file" )
    s << c(output_xml_node,"photonMap_bounces","bounces") 
    s << c(output_xml_node,"photonMap_maxPhotons","max_photons")

    s << c(output_xml_node,"photonMap_convexHullEstimate","convex_hull_estimate") 
    s << c(output_xml_node,"photonMap_dontDelete","dont_delete") 
    s << c(output_xml_node,"photonMap_loadFileName","file")
    s << c(output_xml_node,"photonMap_maxDensity","max_density")
    s << c(output_xml_node,"photonMap_mode","mode")
    s << c(output_xml_node,"photonMap_multiplier","multiplier")
    s << c(output_xml_node,"photonMap_retraceBounces", "retrace_bounces") 
    s << c(output_xml_node,"photonMap_retraceCorners","retrace_corners")
    s << c(output_xml_node,"photonMap_saveFileName","file")
    s << c(output_xml_node,"photonMap_showCalcPhase","show_calc_phase")
    s << c(output_xml_node,"photonMap_storeDirectLight","store_direct_light")
    # s << cnv("photonMap_switchToSavedMap", get_bool(output_xml_node,"store_direct_light") )
    # s << cnv("photonMap_convert",get_bool(output_xml_node,"max_photons"))
    s << c(output_xml_node,"photonMap_interpSamples","prefilter_samples")
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
    # puts output_xml_node

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
    s << c(output_xml_node,"lightcache_saveFileName","file")
    s << c(output_xml_node,"lightcache_scale","world_scale")
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
    s << c(output_xml_node,"gi_on","on")
    s << c(output_xml_node,"gi_primary_multiplier","primary_multiplier")
    s << c(output_xml_node,"gi_primary_type","primary_engine")
    s << c(output_xml_node,"gi_rayDistance","ray_distance")
    s << c(output_xml_node,"gi_rayDistanceOn","ray_distance_on")
   
    s << c(output_xml_node,"gi_saturation","saturation")
    s << c(output_xml_node,"gi_saveMapsPerFrame","save_maps_per_frame")
    s << c(output_xml_node,"gi_secondary_multiplier","secondary_multiplier")
    s << c(output_xml_node,"gi_secondary_type","secondary_engine")
    s << c(output_xml_node,"gi_ui_view","default")

    s << c(output_xml_node,"dmcgi_depth","gi_depth")
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
    # get_all_params_nodes output_xml_node
   
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

def export_camera_dof
    s = ""
    output_xml_node = get_xml_node("SettingsCameraDof");    
    # get_all_params_nodes output_xml_node
   
    s << c(output_xml_node,"dof_anisotropy","anisotropy")
    s << c(output_xml_node,"dof_bias","center_bias")
    s << c(output_xml_node,"dof_distance","focal_dist")
    s << c(output_xml_node,"dof_getFromCamera","default")
    s << c(output_xml_node,"dof_on","on")
    s << c(output_xml_node,"dof_shutter","aperture")
    s << c(output_xml_node,"dof_sides_num","sides_num")
    s << c(output_xml_node,"dof_sides_on","sides_on")
    s << c(output_xml_node,"dof_sides_rotation","rotation")
    s << c(output_xml_node,"dof_subdivs","subdivs")
   
    puts s 
end

def export_dmc_sampler
    s = ""
    output_xml_node = get_xml_node("SettingsDMCSampler");    
    # get_all_params_nodes output_xml_node
   
    s << c(output_xml_node,"dmc_balance_subdivs","default")
    s << c(output_xml_node,"dmc_earlyTermination_amount","adaptive_amount")
    s << c(output_xml_node,"dmc_earlyTermination_minSamples","adaptive_min_samples")
    s << c(output_xml_node,"dmc_earlyTermination_threshold","adaptive_threshold")
    s << c(output_xml_node,"dmc_importanceSampling","default")
    s << c(output_xml_node,"dmc_pathSampler_type","path_sampler_type")
    s << c(output_xml_node,"dmc_subdivs_mult","subdivs_mult")
    s << c(output_xml_node,"dmc_timeDependent","time_dependent")
    
    s << c(output_xml_node,"dmcgi_subdivs","default")
   
    puts s 
end

def export_rt_engine
    s = ""
    output_xml_node = get_xml_node("RTEngine");    
    # get_all_params_nodes output_xml_node

    s << c(output_xml_node,"dmcgi_depth","gi_depth")         
    # s << c(output_xml_node,"dmcgi_subdivs","default")

    s << c(output_xml_node,"gi_reflectCaustics","gi_reflective_caustics")
    s << c(output_xml_node,"gi_refractCaustics","gi_refractive_caustics")
   
    puts s 
end



file_loaded("VfSExport.rb")
file_loaded("TrasferVrayModule.rb")
export_settings_output
# export_photonMap
# export_irrad_map
# export_lightcache
# export_raycaster
# export_motionblur
# export_image_sampler
# export_gi
# export_irradmap
# export_environment
# export_camera_dof
# export_dmc_sampler
# export_rt_engine