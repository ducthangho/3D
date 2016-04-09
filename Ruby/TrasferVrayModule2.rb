require 'sketchup.rb'
require 'VfSExport.rb'

def get_ms_val(suValue,suType)
    # puts suValue
    if (suType == nil) or (suValue==nil)
        return nil
    end
    case suType
    when "color"
        re = suValue.elementsByTagName("r")
        ge = suValue.elementsByTagName("g")
        be = suValue.elementsByTagName("b")
        r = re[0].firstChild().nodeValue().to_f
        g = ge[0].firstChild().nodeValue().to_f
        b = be[0].firstChild().nodeValue().to_f
        r = r*255
        g = g*255
        b = b*255
        return "(color #{r.to_i} #{g.to_i} #{b.to_i})"
    when "acolor"
        re = suValue.elementsByTagName("r")
        ge = suValue.elementsByTagName("g")
        be = suValue.elementsByTagName("b")
        ae = suValue.elementsByTagName("a")
        r = re[0].firstChild().nodeValue().to_f
        g = ge[0].firstChild().nodeValue().to_f
        b = be[0].firstChild().nodeValue().to_f
        a = ae[0].firstChild().nodeValue().to_f
        r = r*255
        g = g*255
        b = b*255
        return "(color #{r} #{g} #{b}) #{a}"
    when "transform"
        vv = suValue.elementsByTagName( "vector" )
        tf = "(matrix3"
        for v in vv
            xn = v.elementsByTagName( "x" )
            yn = v.elementsByTagName( "y" )
            zn = v.elementsByTagName( "z" )
            x = xn[0].firstChild().nodeValue()
            y = yn[0].firstChild().nodeValue()
            z = zn[0].firstChild().nodeValue()
            tf << " [#{x},#{y},#{z}]"
        end
        tf << ")"
        return tf
    when "bool"
        suValue = suValue.firstChild().nodeValue()
        if (suValue)
            return "true"
        else
            return "false"
        end
    when "string"
        suValue = suValue.firstChild().nodeValue()
        return "\"#{suValue.to_s}\""
    when "filename","plugin"
        suValue = suValue.firstChild().nodeValue()
        if suValue.length()==0
            return "undefined"
        else
            return "\"#{suValue.to_s}\""
        end
    else
        suValue = suValue.firstChild().nodeValue()
        return suValue
    end
end

def c(suData,maxKey,suKey)
    if (suData[suKey] == nil) or (suKey == "default")
        return ""
    end
    suVal = suData[suKey].elementsByTagName( "value" )
    if suVal.length() == 0
        suVal = ""
    end
    maxValue = get_ms_val(suVal[0],suData[suKey].attribute('type').to_s)
    if maxValue != nil
        return "\nvr.#{maxKey}=#{maxValue}"
    else
        return ""
    end
end

# def get_all_params_node2( parentNode, data = nil )
#     if data == nil
#         data = {}
#     end
#     nodes = parentNode.elementsByTagName("parameter")
#     return nil if nodes.length() == 0
#     for currentNode in nodes
#         name = currentNode.attribute('name')
#         data[name] = currentNode
#         # puts "s << ll(suData,'MAX','#{name}')"
#     end
#     return data
# end

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

def isPrimitiveType(type)
    return (type == "integer" || type == "bool" || type == "float" || type == "double" || type == "string" || type == "filename" || type == "color" || type == "acolor" || type == "transform" || type == "list" || type == "vector")
end

def get_all_params_node2( parentNode, data = nil )
    if data == nil
        data = {}
    end
    nodes = parentNode.elementsByTagName("parameter")
    return nil if nodes.length() == 0
    for currentNode in nodes
        name = currentNode.attribute('name')
        type = currentNode.attribute("type")
        if isPrimitiveType type
            data[name] = currentNode
        else
            # puts currentNode
            vals = currentNode.elementsByTagName( "value" )
            if (vals.length() ==0)
                next
            end

            val = vals[0].firstChild().to_s

            asset = VRayForSketchUp.find_asset_in_doc(parentNode,val)
            # puts "----------------"
            # puts "#{parentNode}"
            # puts ""
            # puts "#{name}   : #{type}  =  #{asset}"
            if (val == nil || asset == nil)
                data[name] = ""
                next
            end


            pluginElements = asset.elementsByTagName("vrayplugin")

            if pluginElements.length() != 1
                puts "Incorrect usage of get_vrayplugin_type_from_asset_node - expected 1 <plugin> node found #{pluginElements.length()}"
                data[name] = ""
                next
            end
            pluginNode = pluginElements[0]
            # oType = VRayForSketchUp.get_vrayplugin_type_from_asset_node(asset)
            # oName = VRayForSketchUp.get_vrayplugin_name_from_asset_node(asset)
            data[name] = pluginNode

        end
        # puts "s << ll(suData,'MAX','#{name}')"
    end
    return data
end

def get_all_params_nodes( parentNode )
    # goi ham in ra, ham in ra nay lam nhieu vu
    #     - dauvao: parentnode, space
    #     - daura: in ra ketqua

    #     for parentNode.each element
    #         convertEachElemtn to xml
    #         lay ra tham so parameter
    #         kiem tra kieu cua tham so parameter
    #             - neu co kieu la primitive thi in ra theo kieu name:type
    #             - neu co kieu la reference thi in ra theo dinh dang saturation
    #                 + name bien co kieu refercene : kieu refercene
    #                 + lay ra node duoc tham chieu toi
    #                      - goi ham in voi dau vao la node duoc them



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
    output_xml_string = options_hash["/#{key}"]

    output_xml_doc = VRayXML::QDomDocument.new output_xml_string

    return get_all_params_node2(output_xml_doc)

end

def set_property(obj, prop_name, prop_value)
    obj.send("#{prop_name}=",prop_value)
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

# def get_valueNode(valueNodes)
#   string s = ""
#   valueNodes.elementsByTagName
# end

def get_ms_val_M(suValue,suType)
    # puts suValue
    if (suType == nil) or (suValue==nil)
        return nil
    end
    case suType
    when "color"
        re = suValue.elementsByTagName("r")
        ge = suValue.elementsByTagName("g")
        be = suValue.elementsByTagName("b")
        r = re[0].firstChild().nodeValue().to_f
        g = ge[0].firstChild().nodeValue().to_f
        b = be[0].firstChild().nodeValue().to_f
        r = r*255
        g = g*255
        b = b*255
        return "(color #{r.to_i} #{g.to_i} #{b.to_i})"
    when "acolor"
        re = suValue.elementsByTagName("r")
        ge = suValue.elementsByTagName("g")
        be = suValue.elementsByTagName("b")
        ae = suValue.elementsByTagName("a")
        r = re[0].firstChild().nodeValue().to_f
        g = ge[0].firstChild().nodeValue().to_f
        b = be[0].firstChild().nodeValue().to_f
        a = ae[0].firstChild().nodeValue().to_f
        r = r*255
        g = g*255
        b = b*255
        return "(color #{r} #{g} #{b}) #{a}"
    when "transform"
        vv = suValue.elementsByTagName( "vector" )
        tf = "(matrix3"
        for v in vv
            xn = v.elementsByTagName( "x" )
            yn = v.elementsByTagName( "y" )
            zn = v.elementsByTagName( "z" )
            x = xn[0].firstChild().nodeValue()
            y = yn[0].firstChild().nodeValue()
            z = zn[0].firstChild().nodeValue()
            tf << " [#{x},#{y},#{z}]"
        end
        tf << ")"
        return tf
    when "bool"
        suValue = suValue.firstChild().nodeValue()
        if (suValue)
            return "true"
        else
            return "false"
        end
    when "string"
        suValue = suValue.firstChild().nodeValue()
        return "\"#{suValue.to_s}\""
    when "filename","plugin"
        suValue = suValue.firstChild().nodeValue()
        if suValue.length()==0
            return "undefined"
        else
            return "\"#{suValue.to_s}\""
        end
    else
        suValue = suValue.firstChild().nodeValue()
        return suValue
    end
end

count = 0;
def print(all_asset,xml_node,prefix,count)
    # count = count + 1
    # if(count>3) 
    #   return 3
    # end
    params = xml_node.elementsByTagName("parameter")
    # puts params.length

    a = prefix.gsub("-"," ");
    indent = "#{a}  -"

    params.each do |node|
        # puts  "#{node}"
        name = node.attribute("name")
        type = node.attribute("type")
        valueNodes = node.elementsByTagName("value");
        values = get_ms_val_M(valueNodes[0],type)
        # valueNodes.each do |v|
        #   values << v.firstChild.to_s << " "
        # end

        puts "#{prefix}#{name} : #{type} : #{values}"
        if(!(isPrimitiveType type))
            # asset = lay xml node ma bi tham chieu toi, co gia tri luu trong bien type
            url = node.firstChild.firstChild.to_s
            # puts "url -> #{url}"
            # puts "url  -> #{url}"
            asset = VRayForSketchUp.find_asset_in_doc(all_asset,url)
            if(asset!=nil)                 
                # puts "#{indent}(#{count})url -> #{url} -> asset: #{asset}"
                # print(asset)
                # puts "asset ->\n#{asset}"
                print(all_asset,asset,indent,count)
            end
        end
    end
end

def print_keyType(key)
    options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
    #            # if options_hash_as_array != nil
    options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )
    output_xml_string = options_hash["/#{key}"]

    output_xml_doc = VRayXML::QDomDocument.new output_xml_string


    puts "******************************"
    # puts output_xml_doc
    puts "******************************"

    print(output_xml_doc,output_xml_doc,"",0)

    # duyet tat ca cac tham so parameter cua node
end

def export_environment
    s = ""
    output_xml_node = get_xml_node("SettingsEnvironment");
    puts "-------------------------------------------------------------*"
    print_keyType "SettingsEnvironment"

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
    output_xml_node = get_xml_node("RTEngine")
    # get_all_params_nodes output_xml_node

    s << c(output_xml_node,"dmcgi_depth","gi_depth")
    # s << c(output_xml_node,"dmcgi_subdivs","default")

    s << c(output_xml_node,"gi_reflectCaustics","gi_reflective_caustics")
    s << c(output_xml_node,"gi_refractCaustics","gi_refractive_caustics")

    puts s
end

def  export_adaptiveSubdivision
    s = ""

    output_xml_node = get_xml_node("SettingsImageSampler")

    # get_all_params_nodes(output_xml_node)


    s << c(output_xml_node,"adaptiveSubdivision_dofMoblurSubdivs (adaptiveSubdivision_dofMoblur_subdivs)","default")
    s << c(output_xml_node,"adaptiveSubdivision_maxRate","subdivision_maxRate")
    s << c(output_xml_node,"adaptiveSubdivision_minRate","subdivision_minRate")
    s << c(output_xml_node,"adaptiveSubdivision_rand","default")
    s << c(output_xml_node,"adaptiveSubdivision_showSamples","dmc_show_samples")
    s << c(output_xml_node,"adaptiveSubdivision_threshold","default")

    puts s
end

def export_adv_irradmap
    s = ""
    output_xml_node = get_xml_node("SettingsIrradianceMap")

    s << c(output_xml_node,"adv_irradmap_autoSave","auto_save")
    s << c(output_xml_node,"adv_irradmap_autoSaveFileName","auto_save_file")
    s << c(output_xml_node,"adv_irradmap_calcInterpSamples","calc_interp_samples")
    s << c(output_xml_node,"adv_irradmap_checkSampleVisibility","check_sample_visibility")
    s << c(output_xml_node,"adv_irradmap_dontDelete","dont_delete")
    s << c(output_xml_node,"adv_irradmap_interpolationType","interpolation_mode")
    # s << c(output_xml_node,"adv_irradmap_limitSearchRange","default")
    s << c(output_xml_node,"adv_irradmap_loadFileName","file")
    s << c(output_xml_node,"adv_irradmap_lookupType","lookup_mode")
    s << c(output_xml_node,"adv_irradmap_mode","mode")
    s << c(output_xml_node,"adv_irradmap_randomize","randomize_samples")
    s << c(output_xml_node,"adv_irradmap_saveFileName","default")
    s << c(output_xml_node,"adv_irradmap_switchToSavedMap","default")
    s << c(output_xml_node,"adv_irradmap_useCurrentPassSamples","default")

    puts s
    # s << c(output_xml_node,"options_defaultLights","default")d
end

def export_camera
    s = ""
    output_xml_node = get_xml_node("CameraPhysical")
    settingsCameraDof = get_xml_node("SettingsCameraDof")
    settingsCamera = get_xml_node("SettingsCamera")
    # get_all_params_nodes(settingsCamera)

    s << c(settingsCamera,"camera_fish_autoDistance","default")
    s << c(settingsCamera,"camera_fish_curve","curve")
    s << c(settingsCamera,"camera_fish_distance","dist")
    s << c(settingsCamera,"camera_fov","fov")
    s << c(output_xml_node,"camera_motion_blur","use_moblur")
    s << c(settingsCamera,"camera_overrideFOV","override_fov")
    s << c(settingsCamera,"camera_type","type")
    s << c(settingsCamera,"camera_cyl_height","height")

    puts s
end

def export_caustic
    s = ""
    output_xml_node = get_xml_node("SettingsCaustics")

    # get_all_params_nodes(output_xml_node)

    # .casutics_autoSave (alias for caustics_autoSave)
    s << c(output_xml_node,"caustics_autoSave","auto_save")
    s << c(output_xml_node,"caustics_autoSaveFileName","auto_save_file")
    s << c(output_xml_node,"caustics_dontDelete","dont_delete")
    s << c(output_xml_node,"caustics_loadFileName","file")
    s << c(output_xml_node,"caustics_maxDensity (caustics_maxDens)","max_density")
    s << c(output_xml_node,"caustics_maxPhotons","max_photons")
    s << c(output_xml_node,"caustics_mode","mode")
    s << c(output_xml_node,"caustics_multiplier","multiplier")
    s << c(output_xml_node,"caustics_on","on")
    s << c(output_xml_node,"caustics_saveFileName","auto_save_file")
    s << c(output_xml_node,"caustics_searchDist","search_distance")
    s << c(output_xml_node,"caustics_showCalcPhase (caustics_directVisualization)","show_calc_phase")
    s << c(output_xml_node,"caustics_switchToSavedMap","default")
    s << c(output_xml_node,"caustics_ui_view","default")

    puts s
end

def export_colorMapping
    s = ""
    output_xml_node = get_xml_node("SettingsColorMapping")

    # get_all_params_nodes(output_xml_node)


    s << c(output_xml_node,"colorMapping_adaptationOnly","adaptation_only")
    s << c(output_xml_node,"colorMapping_affectBackground","affect_background")
    s << c(output_xml_node,"colorMapping_brightMult","bright_mult")
    s << c(output_xml_node,"colorMapping_clampLevel","clamp_level")
    s << c(output_xml_node,"colorMapping_clampOutput","clamp_output")
    s << c(output_xml_node,"colorMapping_darkMult","dark_mult")
    s << c(output_xml_node,"colorMapping_gamma","gamma")
    s << c(output_xml_node,"colorMapping_linearWorkflow","linearWorkflow")
    s << c(output_xml_node,"colorMapping_subpixel","subpixel_mapping")
    s << c(output_xml_node,"colorMapping_type","type")
    # s << c(output_xml_node,"colormapping_ui_view","default")

    puts s
end

def export_Displacement
    s = ""

    options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)

    options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )

    output_xml_node = get_xml_node("SettingsDefaultDisplacement")

    # get_all_params_nodes(output_xml_node)

    s << c(output_xml_node,"displacement_amount","amount")
    s << c(output_xml_node,"displacement_edgeLength","edgeLength")
    s << c(output_xml_node,"displacement_maxSubdivs","maxSubdivs")
    s << c(output_xml_node,"displacement_overrideMax","override_on")
    s << c(output_xml_node,"displacement_relative","relative")
    s << c(output_xml_node,"displacement_tightBounds","tightBounds")
    s << c(output_xml_node,"displacement_viewDependent","viewDependent")

    puts s
end

def export_dmc

    s = ""
    output_xml_node = get_xml_node("SettingsDMCSampler")
    s << c(output_xml_node,"dmc_balance_subdivs (dmc_balanceSubdivs)","default")
    s << c(output_xml_node,"dmc_earlyTermination_amount (mc_earlyTermination_amount)","adaptive_amount")
    s << c(output_xml_node,"dmc_earlyTermination_minSamples (mc_earlyTermination_minSamples)","adaptive_min_samples")
    s << c(output_xml_node,"dmc_earlyTermination_threshold (mc_earlyTermination_threshold)","adaptive_threshold")
    s << c(output_xml_node,"dmc_importanceSampling (mc_importanceSampling)","default")
    s << c(output_xml_node,"dmc_pathSampler_type (mc_pathSampler_type)","path_sampler_type")
    s << c(output_xml_node,"dmc_subdivs_mult (mc_subdivs_mult)","subdivs_mult")
    s << c(output_xml_node,"dmc_timeDependent (mc_lockToPixels)","time_dependent")

    puts s
end

def export_dmcgi

    s = ""
    options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
    options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )
    output_xml_node = get_xml_node("SettingsDMCGI")
    s << c(output_xml_node,"dmcgi_depth (mcgi_depth)","depth")
    s << c(output_xml_node,"dmcgi_subdivs (mcgi_subdivs)","subdivs")

    puts s

end




file_loaded("VfSExport.rb")
file_loaded("TrasferVrayModule.rb")
#export_settings_output
# export_photonMap
# export_irradmap
# export_lightcache
# export_colorMapping
# export_dmc

# export_raycaster
# export_motionblur
# export_image_sampler
# export_gi
# export_irradmap
export_environment
# export_camera_dof
# export_dmc_sampler
# export_rt_engine
# get_xml_node("SettingsDMCGI")
