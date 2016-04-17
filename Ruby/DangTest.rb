SKETCHUP_CONSOLE.clear
$suLightAttr = {
    'VRayLight' => {},
    'VRaySun' => {},
    'VRayIES' => {},
}

$sunLight = {}

$prefix_cv = "vv"
$print_tree = false
$print_obj = false
$print_ms = true
$print_xml = false

$print_list_ms = false


def get_ms_val(suValue,suType)
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
        a = a*255
        return "(color #{r.to_i} #{g.to_i} #{b.to_i} #{a.to_i})"
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

    when "componentTransform"
        vv = suValue.elementsByTagName( "componentTransform" )
        tf = "(matrix3"
        v = vv[0]
        x = v.elementsByTagName( "x_rotation" )
        x = x[0].firstChild().nodeValue()
        y = v.elementsByTagName( "y_rotation" )
        y = y[0].firstChild().nodeValue()
        z = v.elementsByTagName( "z_rotation" )
        z = z[0].firstChild().nodeValue()
        tf << " [#{x},#{y},#{z}]"

        x = v.elementsByTagName( "x_scale" )
        x = x[0].firstChild().nodeValue()
        y = v.elementsByTagName( "y_scale" )
        y = y[0].firstChild().nodeValue()
        z = v.elementsByTagName( "z_scale" )
        z = z[0].firstChild().nodeValue()
        tf << " [#{x},#{y},#{z}]"

        x = v.elementsByTagName( "x_mirror" )
        x = x[0].firstChild().nodeValue()
        y = v.elementsByTagName( "y_mirror" )
        y = y[0].firstChild().nodeValue()
        z = v.elementsByTagName( "z_mirror" )
        z = z[0].firstChild().nodeValue()
        tf << " [#{x},#{y},#{z}]"

        x = v.elementsByTagName( "x_offset" )
        x = x[0].firstChild().nodeValue()
        y = v.elementsByTagName( "y_offset" )
        y = y[0].firstChild().nodeValue()
        z = v.elementsByTagName( "z_offset" )
        z = z[0].firstChild().nodeValue()
        tf << " [#{x},#{y},#{z}]"

        tf << ")"
        return tf
    when "bool"
        suValue = suValue.firstChild().nodeValue()
     
        if (suValue==1) or (suValue=="1")
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
            return ""
        else
            return "\"#{suValue.to_s}\""
        end
    else
        suValue = suValue.firstChild().nodeValue()
        return suValue
    end
end

def isPrimitiveType(type)
    return (type == "integer" || type == "bool" || type == "float" || type == "double" || type == "string" || type == "filename" || type == "color" || type == "acolor" || type == "transform" || type == "list" || type == "vector" || type=="componentTransform")
end

def xml2obj( parentNode)
    data = {}
    nodes = parentNode.elementsByTagName("parameter")
    return nil if nodes.length() == 0

    puts "\n =========================" if $print_obj
    for node in nodes
      name = node.attribute('name')
      type = node.attribute("type")
      valueNodes = node.elementsByTagName("value");
      if (isPrimitiveType type)
        data[name]=get_ms_val(valueNodes[0],type)
        if (!data[name]) 
          data[name] = ""
        end
        puts "#{name}=#{data[name]}  (t:#{type})"  if $print_obj
      else
        url = node.firstChild.firstChild.to_s
        pp = {
          "v"=>url,
          "t"=>type
        }
        data[name]= pp
        puts "---URL:#{name}(type=#{type}):#{url}\n" if $print_obj
      end
    end
    puts "\n =========================" if $print_obj
    return data
end

# def print_opt_by_url(url)
#     options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
#     options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )
#     output_xml_string = options_hash["/#{url}"]
#     output_xml_doc = VRayXML::QDomDocument.new output_xml_string
#     # suData = xml2obj(output_xml_doc)
# end

def puts_maxLight(lName)
    puts "\n--------------- Thuoc tinh cho MaxLight: #{lName}--------------"
    h = Hash[$suLightAttr[lName].sort]
    h.each do |key, val|
      puts val
    end      
    h.each do |key, val|
      puts "s << ll(suData,'MAX','#{key}')" if $print_list_ms
    end      
end

def find_similar_light(suLight)
  case suLight
    when "LightRectangle",  "LightSpot", "LightDome", "LightSphere", "LightOmni"
      return "VRayLight"
    when "LightIES"
      return "VRayIES"
    # when "LightOmni"
    #   return "Omnilight"
    else return nil
  end
end

def make_max_from_xml(xmlNode,assetType="setting")
  assets = xmlNode.elementsByTagName("Asset")  
  dd = {}
  rootUrl = nil
  for ass in assets
    url = ass.attribute('url')
    atype = ass.attribute('type')
    
    p = ass.elementsByTagName("vrayplugin")
    if (p.length()==1)
      ptype = p[0].attribute('type')
      if (ptype==assetType)
        rootUrl = url
      end
      pname = p[0].attribute('name')
      puts "\n -----------#{url}:#{atype}:#{pname}--------" if $print_obj
      aa = {}
      # aa["v"] = xml2obj(p[0])
      # aa["t"] = ptype
      # aa["n"] = pname
      dd[url] = xml2obj(p[0])
      dd[url]["y3d_type"] = ptype
      dd[url]["y3d_name"] = pname
    end
  end  
  puts "Root URL = #{rootUrl}" if $print_obj
  dd["rootUrl"] = rootUrl
  # puts dd
  return dd
end

class Hash
  def dig(dotted_path)
    parts = dotted_path.split '.', 2
    match = self[parts[0]]
    if !parts[1] or match.nil?
      return match
    else
      return match.dig(parts[1])
    end
  end
end

def ms_TexBitmap(sData)
  s = "\n#{$prefix_cv} = Bitmaptexture()"
  s << ms(sData,"coords.Noise_Amount","uv_noise_amount")
  s << ms(sData,"coords.Noise_On","uv_noise_on","bool")
  s << ms(sData,"coords.Noise_Levels","uv_noise_levels")
  s << ms(sData,"coords.Noise_Size","uv_noise_size")
  s << ms(sData,"coords.phase","un_noise_phase")
  s << ms(sData,"coords.U_Tiling","tile_u")
  s << ms(sData,"coords.V_Tiling","tile_v")
  s << ms(sData,"coords.mapChannel","uvwgen.uvw_channel")
  # s << ms(sData,"coords.UVTransform","uvwgen.uvw_transform")
  s << ms(sData,"clipu","u")
  s << ms(sData,"clipv","v")
  s << ms(sData,"clipw","w")
  s << ms(sData,"cliph","h")
  s << ms(sData,"cropPlace","placement_type")
  s << ms(sData,"fileName","bitmap.file")
  s << ms(sData,'jitter',"jitter")
  s << ms(sData,'filtering',"filter_type")
  return s
end

def ms_TexAColorOp(sData)
  return ""
end

def ms_TexSky(sData)
  url =  sData["sun"]["y3d_url"]
  if $sunLight[url]
    return "\n#{$prefix_cv}=vrk[vrk.count]"
  end
  $sunLight[url] = true

  s = ""
  s << ms(sData,"MAX","sun.shadow_color",nil,true)
  s << ms(sData,"MAX","sun.causticMult",nil,true)
  s << ms(sData,"MAX","sun.channels_specular",nil,true)
  s << ms(sData,"y3d_sun.size_multiplier","sun.size_multiplier",nil,true)
  s << ms(sData,"MAX","sun.causticSubdivs",nil,true)
  s << ms(sData,"y3d_sun.invisible","sun.invisible",nil,true)
  s << ms(sData,"y3d_sun.water_vapour","sun.water_vapour",nil,true)
  s << ms(sData,"y3d_sun.shadow_bias","sun.shadowBias",nil,true)
  s << ms(sData,"y3d_sun.turbidity","sun.turbidity",nil,true)
  s << ms(sData,"y3d_sun.intensity_multiplier","sun.intensity_multiplier",nil,true)
  s << ms(sData,"y3d_sun.sky_model","sun.sky_model",nil,true)
  s << ms(sData,"y3d_sun.transform","sun.transform",nil,true)
  sData["sun"]["color_mode"] = "1"
  # puts sData["sun"]
  s << ms(sData,"y3d_sun.color_mode","sun.color_mode",nil,true)
  ysun = s
  s = ""
  s << ms(sData,"y3d_sky.multiplier","size_multiplier",nil,true)
  ysky = s
  s = "\n-----Create Daylight system------
dl = DaylightSystemFactory2.Create sunClass:VRaySun skyClass:Skylight
vrk = getClassInstances VRaySky
y3d_sun = dl.getSun()
y3d_sky = dl.getSky()#{ysun}#{ysky}
#{$prefix_cv} = vrk[vrk.count]"
  s << ms(sData,"indirect_horiz_illum","horiz_illum")
  s << ms(sData,"MAX","up_vector")
  s << ms(sData,"MAX","sun")
  # s << ms(sData,"transform","transform")
  s << ms(sData,"sun_ozone","ozone")
  s << ms(sData,"sun_invisible","invisible","on_off")
  s << ms(sData,"sun_water","water_vapour")
  s << ms(sData,"sky_model","sky_model")
  # s << ms(sData,"sun_turbidity","turbidity")
  s << ms(sData,"sun_intensity_multiplier","intensity_multiplier")
  s << ms(sData,"MAX","target_transform")
  return s
end

def ms_UVWGenChannel(sData)

end

def ms_SunLight(sData)
  puts "SunLight"
end

def ms_plugins(sData,maxKey,suKey,print_out = false,level=0)
  return "" if sData[suKey]==nil
  out = ''
  old_cv = $prefix_cv
  $prefix_cv = $prefix_cv +"."+ maxKey if maxKey!="MAX"
  data = sData[suKey]
  y3d_name = data["y3d_name"]
  if (print_out)
    pre =  ("-----"*level)
    puts "#{pre}s << ms_plugins(sData,'#{maxKey}','#{suKey}')   t:#{y3d_name}" if $print_list_ms
  else
    if ['TexBitmap', 'TexSky', 'TexAColorOp','UVWGenChannel'].include? y3d_name
      out = eval("ms_#{y3d_name}(data)")
      # puts out
    end
  end
  # if (y3d_name=="TexBitmap")
  #   out = ms_TexBitmap(data)
  # elsif (y3d_name=="TexSky")
  #   out = ms_TexSky(data)
  # elsif (y3d_name=="TexAColorOp")
  #   out = ms_TexAColorOp(data)  
  # elsif (y3d_name=="UVWGenChannel")
  #   out = ms_TexAColorOp(data)
  # end

  data.each do |key, val|
    # puts "key:#{key}:::"
    if val["y3d_name"]!=nil
      oo = ms_plugins(data,"MAX",key,print_out,level+1) 
      if oo!=nil 
        out << oo
      end
      # puts "------key:#{key}------val:#{val["y3d_name"]}"
    end
  end

  $prefix_cv = old_cv
  return out
end

def ms (suData,maxKey,suKey,ovr_type=nil,ovr_key=false)
  suVal = suData.dig(suKey)
  return "" if (suVal == nil) or (maxKey == "MAX") or (suVal=="")
  if (ovr_type!=nil) 
    case ovr_type
    when "bool"
      if (suVal==1) or (suVal=="true") or (suVal=="1")
          suVal =  "true"
      else
          suVal = "false"
      end      
    when "on_off"
      if (suVal==1) or (suVal=="true") or (suVal=="1")
          suVal =  "on"
      else
          suVal = "off"
      end         
    when "0_1"
      if (suVal==1) or (suVal=="true") or (suVal=="1")
          suVal =  "1"
      else
          suVal = "0"
      end   
    when "not_b"
      if (suVal==1) or (suVal=="true") or (suVal=="1")
          suVal =  "false"
      else
          suVal = "true"
      end   
    end
  end
  return "\n#{maxKey}=#{suVal}" if ovr_key
  return "\n#{$prefix_cv}.#{maxKey}=#{suVal}"
end

def creatVRayLight(sData)
  puts "\n-------------MAX SCRIPTS:VRAYLIGHT-----#{sData["y3d_name"]}--------------"
  s = "vv = VRayLight()"

  case sData["y3d_name"]
    when "LightRectangle"
      s << "\nvv.type=0"
    when "LightDome"
      s << "\nvv.type=1"
      s << ms(sData,'dome_emitRadius','dome_emitRadius')
      s << ms(sData,'dome_rayDistance','dome_rayDistance')
      s << ms(sData,'dome_rayDistanceMode','dome_rayDistanceMode')
      s << ms(sData,'dome_spherical','dome_spherical')
      s << ms(sData,'dome_targetRadius','dome_targetRadius')
  end

  s << ms(sData,'MAX','causticMult')
  s << ms(sData,'MAX','bumped_below_surface_check')
  s << ms(sData,'MAX','objectID')
  s << ms(sData,'MAX','photonSubdivs')
  s << ms(sData,'color','color')
  s << ms(sData,'MAX','specular_contribution')
  s << ms(sData,'MAX','channels')
  s << ms(sData,'subdivs','subdivs')
  s << ms(sData,'multiplier','intensity')
  s << ms(sData,'MAX','tex_adaptive')
  s << ms(sData,'doubleSided','doubleSided')
  s << ms(sData,'affect_diffuse','affectDiffuse')
  s << ms(sData,'MAX','v_size')
  s << ms(sData,'noDecay','noDecay')
  s << ms(sData,'MAX','shadowColor_tex')
  s << ms(sData,'MAX','causticSubdivs')
  s << ms(sData,'MAX','transform')
  s << ms(sData,'affect_reflections','affectReflections')
  s << ms(sData,'MAX','use_global_light_level')
  s << ms(sData,'MAX','shadowColor')
  s << ms(sData,'MAX','tex_resolution')
  s << ms(sData,'MAX','channels_raw')
  s << ms(sData,'normalizeColor','units')
  s << ms_plugins(sData,'texmap','rect_tex')
  s << ms(sData,'MAX','diffuse_contribution')
  s << ms(sData,'MAX','use_rect_tex')
  s << ms(sData,'MAX','channels_specular')
  s << ms(sData,'storeWithIrradMap','storeWithIrradianceMap')
  s << ms(sData,'MAX','ignoreLightNormals')
  s << ms(sData,'cutoffThreshold','cutoffThreshold')
  s << ms(sData,'MAX','diffuseMult')
  s << ms(sData,'MAX','intensity_tex')
  s << ms(sData,'invisible','invisible')
  s << ms(sData,'shadowBias','shadowBias')
  s << ms(sData,'MAX','nsamples')
  s << ms(sData,'MAX','u_size')
  s << ms(sData,'castShadows','shadows',"on_off")
  s << ms(sData,'MAX','color_tex')
  s << ms(sData,'affect_specualr','affectSpecular')
  s << ms(sData,'on','enabled')
  s << ms(sData,'skylightPortal','lightPortal',"on_off")
  s << ms(sData,'MAX','channels_diffuse')
  s << ms(sData,'transform','transform')
  puts s
end

def creatVRayIES(sData)
  puts "\n-------------MAX SCRIPTS:VRayIES-----#{sData["y3d_name"]}--------------"
  s = "vv = VRayIES()"

  s << ms(sData,'MAX','causticMult')
  s << ms(sData,'affect_diffuse','affectDiffuse')
  s << ms(sData,'affect_specular','affectSpecular')
  s << ms(sData,'area_speculars','area_speculars')
  s << ms(sData,'MAX','bumped_below_surface_check')
  s << ms(sData,'MAX','causticMult')
  s << ms(sData,'MAX','causticSubdivs')
  s << ms(sData,'MAX','channels')
  s << ms(sData,'MAX','channels_diffuse')
  s << ms(sData,'MAX','channels_raw')
  s << ms(sData,'MAX','channels_specular')
  s << ms(sData,'color','color')
  s << ms(sData,'MAX','color_tex')
  s << ms(sData,'MAX','cutoffThreshold')
  s << ms(sData,'MAX','diffuseMult')
  s << ms(sData,'MAX','diffuse_contribution')
  s << ms(sData,'enabled','enabled')
  s << ms(sData,'MAX','filter_color')
  s << ms(sData,'ies_file','ies_file')
  s << ms(sData,'MAX','nsamples')
  s << ms(sData,'MAX','photonSubdivs')
  s << ms(sData,'power','power')
  s << ms(sData,'shadow_bias','shadowBias')
  s << ms(sData,'MAX','shadowColor')
  s << ms(sData,'MAX','shadowColor_tex')
  s << ms(sData,'MAX','shadowSubdivs')
  s << ms(sData,'castShadows','shadows',"on_off")
  s << ms(sData,'MAX','soft_shadows')
  s << ms(sData,'MAX','specular_contribution')
  s << ms(sData,'transform','transform')
  s << ms(sData,'MAX','use_global_light_level')
  puts s
end

def make_tree(suData, url=nil, level=0)
  if (url==nil)
    url = suData["rootUrl"]
  end
  if suData[url].length()==0
    return ""
  end
  # data = Hash[suData[url]].sort
  data = suData[url]
  # puts data
  data.each do |key, val|
    pre =  ("....."*level)
    puts "#{pre} #{key} ==> #{val}\n" if $print_tree
    if val["t"] != nil 
    # if val.has_key?("t")
      if suData[val["v"]] !=nil 
        dd = {}
        dd = make_tree(suData,val["v"],level+1)
        dd["y3d_url"] = val["v"]
        dd["y3d_type"] = suData[val["v"]]["y3d_type"]
        data[key] = dd
      end
    end
  end
  return data
  # puts data
end

#dung de in ra dong lenh s << ms mac dinh
def creat_list_ms(sData)
  puts "========================MS PRINT FUNC==============================" if $print_list_ms
  sData.each do |key, val|
    if val["y3d_type"]==nil
      puts "s << ms(sData,'MAX','#{key}')\n" 
    else
      ms_plugins(sData,'MAX',key,true)
    end
  end
  puts "========================MS PRINT FUNC END==========================" if $print_list_ms
end

def export_light
  for comp in VRayForSketchUp.active_model.definitions
      for inst in comp.instances
        if (VRayForSketchUp.is_light( inst ))

          light_xml = VRayForSketchUp.get_vfs_light_attribute( inst )
          light_xml_doc = VRayXML::QDomDocument.new light_xml
          # puts light_xml_doc
          z = make_max_from_xml(light_xml_doc,"light")
          zz = make_tree(z)
          maxLight = find_similar_light(zz["y3d_name"])
          if maxLight == "VRayLight"
            creatVRayLight(zz)
          elsif maxLight == "VRayIES"
            creatVRayIES(zz)
          end
       end
      end
  end
end

def get_xml_option_node(key)
    options_hash_as_array = VRayForSketchUp.get_vfs_scene_attribute(VRayForSketchUp::VFS_OPTIONS_DICTIONARY)
    options_hash = VRayForSketchUp.array_to_hash( options_hash_as_array )
    output_xml_string = options_hash["/#{key}"]
    puts output_xml_string if $print_xml
    o = VRayXML::QDomDocument.new output_xml_string
    oo = make_max_from_xml(o,"settings")
    puts "\n\n--//////////////////////// KEY:#{key} //////////////////////"
    puts "========================TREE==============================" if $print_tree
    sData = make_tree(oo)
    puts "========================TREE END==========================" if $print_tree
    creat_list_ms(sData) if $print_list_ms 
    puts "-------------------MAXSCRIPT:#{key}-----------------------"
    return sData
end

def export_environment
    sData = get_xml_option_node("SettingsEnvironment");
    # creat_list_ms(sData) if $print_list_ms 
    s = ""
    s << ms(sData,'MAX','reflect_tex_tex')
    s << ms(sData,'MAX','num_environment_objects')
    s << ms(sData,'environment_refract_on','use_refract')
    s << ms(sData,'environment_rr_color_multiplier','refract_tex_mult')
    s << ms(sData,'environment_gi_on','use_gi')
    s << ms(sData,'MAX','use_bg')
    s << ms(sData,'MAX','bg_tex_on')
    s << ms(sData,'MAX','gi_tex_mult')
    s << ms_plugins(sData,'environment_gi_map','gi_tex') 

    # ____ s << ms_plugins(sData,'MAX','color_a')   t:TexSky
    # __________s << ms_plugins(sData,'MAX','sun')   t:SunLight
    # s << ms_plugins(sData,'MAX','bg_tex_tex')   t:TexSky
    # _____s << ms_plugins(sData,'MAX','sun')   t:SunLight
    s << ms(sData,'environment_refract_color','refract_color')
    # s << ms_plugins(sData,'environment_gi_map','gi_tex_tex') 
    # _____s << ms_plugins(sData,'MAX','sun')   t:SunLight
    s << ms(sData,'MAX','texture_multiplier_mode')
    s << ms(sData,'environment_refract_map_on','refract_tex_on')
    s << ms(sData,'environment_gi_color','gi_color')
    s << ms(sData,'MAX','environment_volume')
    s << ms(sData,'environment_rr_color','reflect_color')
    s << ms(sData,'environment_rr_on','use_reflect')
    s << ms(sData,'MAX','bg_color')
    # s << ms_plugins(sData,'MAX','reflect_tex')
    s << ms(sData,'MAX','refract_tex_tex')
    s << ms(sData,'environment_rr_map_on','reflect_tex_on')
    s << ms_plugins(sData,'environment_refract_map','refract_tex')
    s << ms(sData,'MAX','reflect_tex_mult')
    s << ms(sData,'environment_gi_map_on','gi_tex_on')
    s << ms_plugins(sData,'environment_rr_map','bg_tex')
    # _____s << ms_plugins(sData,'MAX','color_a')   t:TexSky
    # __________s << ms_plugins(sData,'MAX','sun')   t:SunLight
    s << ms(sData,'MAX','bg_tex_mult')
    s << ms(sData,'MAX','y3d_type')
    s << ms(sData,'MAX','y3d_name')
    # s << c(output_xml_node,"environment_gi_color","gi_color")
    # s << c(output_xml_node,"environment_gi_color_multiplier","default")
    # s << c(output_xml_node,"environment_gi_map","gi_tex")
    # s << c(output_xml_node,"environment_gi_map_on","gi_tex_on")
    # s << c(output_xml_node,"environment_gi_on","use_gi")
    # s << c(output_xml_node,"environment_refract_color","refract_color")
    # s << c(output_xml_node,"environment_refract_color_multiplier","default")
    # s << c(output_xml_node,"environment_refract_map","refract_tex")
    # s << c(output_xml_node,"environment_refract_map_on","refract_tex_on")
    # s << c(output_xml_node,"environment_refract_on","use_refract")
    # s << c(output_xml_node,"environment_rr_color","reflect_color")
    # s << c(output_xml_node,"environment_rr_color_multiplier","reflect_tex_mult")
    # s << c(output_xml_node,"environment_rr_map","reflect_tex")
    # s << c(output_xml_node,"environment_rr_map_on","reflect_tex_on")
    # s << c(output_xml_node,"environment_rr_on","use_reflect")

    puts s
end

def export_gi
    s = ""
    sData = get_xml_option_node("SettingsGI");
    # creat_list_ms(sData) if $print_list_ms # dung de in ra dong lenh s << ms mac dinh

    s << ms(sData,'MAX','on')
    s << ms(sData,'gi_saturation','saturation')
    s << ms(sData,'gi_rayDistanceOn','ray_distance_on')
    s << ms(sData,'gi_rayDistance','ray_distance')
    s << ms(sData,'MAX','primary_engine')
    s << ms(sData,'gi_saveMapsPerFrame','save_maps_per_frame')
    s << ms(sData,'gi_contrast_base','contrast_base')
    s << ms(sData,'gi_secondary_multiplier','secondary_multiplier')
    s << ms(sData,'gi_ao_on','ao_on')
    s << ms(sData,'gi_secondary_type','secondary_engine')
    s << ms(sData,'MAX','reflect_caustics')
    s << ms(sData,'gi_ao_amount','ao_amount')
    s << ms(sData,'gi_primary_multiplier','primary_multiplier')
    s << ms(sData,'MAX','refract_caustics')
    s << ms(sData,'gi_ao_radius','ao_radius')
    s << ms(sData,'gi_contrast','contrast')
    s << ms(sData,'gi_ao_subdivs','ao_subdivs')
    # s << c(output_xml_node,"gi_ao_amount","ao_amount")
    # s << c(output_xml_node,"gi_ao_on","ao_on")
    # s << c(output_xml_node,"gi_ao_radius","ao_radius")
    # s << c(output_xml_node,"gi_ao_subdivs","ao_subdivs")
    # s << c(output_xml_node,"gi_contrast","contrast")
    # s << c(output_xml_node,"gi_contrast_base","contrast_base")
    # s << c(output_xml_node,"gi_on","on")
    # s << c(output_xml_node,"gi_primary_multiplier","primary_multiplier")
    # s << c(output_xml_node,"gi_primary_type","primary_engine")
    # s << c(output_xml_node,"gi_rayDistance","ray_distance")
    # s << c(output_xml_node,"gi_rayDistanceOn","ray_distance_on")

    # s << c(output_xml_node,"gi_saturation","saturation")
    # s << c(output_xml_node,"gi_saveMapsPerFrame","save_maps_per_frame")
    # s << c(output_xml_node,"gi_secondary_multiplier","secondary_multiplier")
    # s << c(output_xml_node,"gi_secondary_type","secondary_engine")
    # s << c(output_xml_node,"gi_ui_view","default")

    # s << c(output_xml_node,"dmcgi_depth","gi_depth")
    puts s
end

def export_image_sampler
    
    sData = get_xml_option_node("SettingsImageSampler");
    # creat_list_ms(sData) if $print_list_ms
    s = ""
    s << ms(sData,'MAX','dmc_minSubdivs')
    s << ms(sData,'MAX','subdivision_normals')
    s << ms(sData,'adaptiveSubdivision_maxRate','subdivision_maxRate')
    s << ms(sData,'MAX','subdivision_threshold')
    s << ms(sData,'MAX','dmc_threshold')
    s << ms(sData,'MAX','dmc_maxSubdivs')
    s << ms(sData,'adaptiveSubdivision_minRate','subdivision_minRate')
    s << ms(sData,'fixedRate_subdivs','fixed_subdivs')
    s << ms(sData,'MAX','subdivision_jitter')
    s << ms(sData,'MAX','dmc_use_dmc_sampler_thresh')
    s << ms(sData,'adaptiveSubdivision_showSamples','dmc_show_samples')
    s << ms(sData,'imageSamper_normal_threshold','subdivision_normals_threshold')
    s << ms(sData,'imageSampler_type','type')
    s << ms(sData,'imageSampler_outline','subdivision_edges')
    s << ms(sData,'MAX','subdivision_show_samples')
    # s << c(output_xml_node,"imageSamper_normal_threshold","subdivision_normals_threshold")
    # s << c(output_xml_node,"imageSamper_renderMask_layers","default")
    # s << c(output_xml_node,"imageSamper_renderMask_texmap","default")
    # s << c(output_xml_node,"imageSampler_mtlID","default")
    # s << c(output_xml_node,"imageSampler_normal","default")
    # s << c(output_xml_node,"imageSampler_outline","subdivision_edges")
    # s << c(output_xml_node,"imageSampler_renderMask_type","default")
    # s << c(output_xml_node,"imageSampler_shadingRate","default")
    # s << c(output_xml_node,"imageSampler_type","type")
    # s << c(output_xml_node,"imageSampler_zvalue","default")
    # s << c(output_xml_node,"imageSampler_zvalue_threshold","default")

    # s << c(output_xml_node,"fixedRate_subdivs","fixed_subdivs")
   
    puts s 
end

def export_photonMap
    sData = get_xml_option_node("SettingsPhotonMap");
    # creat_list_ms(sData) if $print_list_ms
    s = ""
    s << ms(sData,'MAX','prefilter_samples')
    s << ms(sData,'photonMap_loadFileName','file')
    s << ms(sData,'photonMap_showCalcPhase','show_calc_phase')
    s << ms(sData,'photonMap_retraceBounces','retrace_bounces')
    s << ms(sData,'photonMap_bounces','bounces')
    s << ms(sData,'photonMap_convexHullEstimate','convex_hull_estimate')
    s << ms(sData,'photonMap_dontDelete','dont_delete')
    s << ms(sData,'MAX','prefilter')
    s << ms(sData,'photonMap_maxPhotons','max_photons')
    s << ms(sData,'photonMap_storeDirectLight','store_direct_light')
    s << ms(sData,'MAX','search_distance_toggle')
    s << ms(sData,'photonMap_maxDensity','max_density')
    s << ms(sData,'photonMap_autoDist','auto_search_distance')
    s << ms(sData,'photonMap_autoSaveFileName','auto_save_file')
    s << ms(sData,'photonMap_multiplier','multiplier')
    s << ms(sData,'photonMap_retraceCorners','retrace_corners')
    s << ms(sData,'photonMap_searchDist','search_distance')
    s << ms(sData,'photonMap_autoSave','auto_save')
    s << ms(sData,'photonMap_mode','mode')    
    # s << c(output_xml_node,"photonMap_searchDist","search_distance" )
    # s << c(output_xml_node,"photonMap_autoDist","auto_search_distance" )
    # s << c(output_xml_node,"photonMap_autoSave","auto_save" )
    # s << c(output_xml_node,"photonMap_autoSaveFileName","auto_save_file" )
    # s << c(output_xml_node,"photonMap_bounces","bounces") 
    # s << c(output_xml_node,"photonMap_maxPhotons","max_photons")

    # s << c(output_xml_node,"photonMap_convexHullEstimate","convex_hull_estimate") 
    # s << c(output_xml_node,"photonMap_dontDelete","dont_delete") 
    # s << c(output_xml_node,"photonMap_loadFileName","file")
    # s << c(output_xml_node,"photonMap_maxDensity","max_density")
    # s << c(output_xml_node,"photonMap_mode","mode")
    # s << c(output_xml_node,"photonMap_multiplier","multiplier")
    # s << c(output_xml_node,"photonMap_retraceBounces", "retrace_bounces") 
    # s << c(output_xml_node,"photonMap_retraceCorners","retrace_corners")
    # s << c(output_xml_node,"photonMap_saveFileName","file")
    # s << c(output_xml_node,"photonMap_showCalcPhase","show_calc_phase")
    # s << c(output_xml_node,"photonMap_storeDirectLight","store_direct_light")
    # # s << cnv("photonMap_switchToSavedMap", get_bool(output_xml_node,"store_direct_light") )
    # # s << cnv("photonMap_convert",get_bool(output_xml_node,"max_photons"))
    # s << c(output_xml_node,"photonMap_interpSamples","prefilter_samples")
    puts s
end

def export_option
    sData = get_xml_option_node("SettingsOptions");
    # creat_list_ms(sData) if $print_list_ms
    s = ""
    s << ms(sData,'MAX','light_doDefaultLights')
    s << ms(sData,'MAX','batch_render')
    s << ms(sData,'MAX','light_doLights')
    s << ms(sData,'MAX','light_onlyGI')
    s << ms(sData,'MAX','mtl_maxDepth')
    s << ms(sData,'MAX','misc_transferAssets')
    s << ms(sData,'MAX','misc_lowThreadPriority')
    s << ms(sData,'MAX','misc_abortOnMissingAsset')
    s << ms(sData,'MAX','misc_useCachedAssets')
    s << ms(sData,'options_geom_backfaceCull','geom_backfaceCull')
    s << ms(sData,'MAX','mtl_transpMaxLevels')
    s << ms(sData,'MAX','ray_bias')
    s << ms(sData,'options_glossyEffects','mtl_glossy')
    s << ms(sData,'MAX','light_doHiddenLights')
    s << ms(sData,'MAX','geom_doHidden')
    s << ms(sData,'MAX','mtl_uninvertedNormalBump')
    s << ms(sData,'options_displacement','geom_displacement')
    s << ms(sData,'MAX','mtl_doMaps')
    s << ms(sData,'MAX','mtl_reflectionRefraction')
    s << ms(sData,'options_filterMaps','mtl_filterMaps')
    s << ms(sData,'MAX','mtl_override_on')
    s << ms(sData,'MAX','mtl_limitDepth')
    s << ms(sData,'MAX','mtl_filterMapsForSecondaryRays')
    s << ms(sData,'MAX','mtl_transpCutoff')
    s << ms(sData,'MAX','mtl_override_color')
    s << ms(sData,'MAX','gi_dontRenderImage')
    s << ms(sData,'MAX','light_doShadows')
    s << ms(sData,'MAX','show_progress_window')
    # s << c(output_xml_node,"options_displacement","geom_displacement")
    # # s << cnv(".")
    # s << c(output_xml_node,"options_dontRenderImage","gi_dontRenderImage")
    # s << c(output_xml_node,"options_displacement","geom_displacement")
    # s << c(output_xml_node,"options_filterMaps","mtl_filterMaps")
    # s << c(output_xml_node,"options_geom_backfaceCull","geom_backfaceCull")
    # s << c(output_xml_node,"options_glossyEffects","mtl_glossy")
    # s << c(output_xml_node,"options_glossyEffects","mtl_glossy")
    puts s
end

def export_lightcache
    sData = get_xml_option_node("SettingsLightCache");
    # creat_list_ms(sData) if $print_list_ms
    s = ""
    s << ms(sData,'lightcache_useForGlossyRays','use_for_glossy_rays')
    s << ms(sData,'lightcache_prefilter_on','prefilter')
    s << ms(sData,'lightcache_subdivs','subdivs')
    s << ms(sData,'lightcache_saveFileName','file')
    s << ms(sData,'lightcache_autoSave','auto_save')
    s << ms(sData,'lightcache_showCalcPhase','show_calc_phase',"0_1")
    s << ms(sData,'MAX','depth')
    s << ms(sData,'lightcache_multipleViews','multiple_views')
    s << ms(sData,'lightcache_autoSaveFileName','auto_save_file')
    s << ms(sData,'lightcache_adaptiveTracing','adaptive_sampling')
    s << ms(sData,'lightcache_retrace_threshold','retrace_threshold')
    s << ms(sData,'lightcache_prefilter_samples','prefilter_samples')
    s << ms(sData,'lightcache_minPathsPerSample','min_paths_per_sample')
    s << ms(sData,'lightcache_scale','world_scale',"0_1")
    s << ms(sData,'lightcache_filter_type','filter_type')
    s << ms(sData,'MAX','adaptive_dirs_only')
    s << ms(sData,'lightcache_interpSamples','filter_samples')
    s << ms(sData,'lightcache_filter_size','filter_size')
    s << ms(sData,'lightcache_dontDelete','dont_delete')
    s << ms(sData,'lightcache_sampleSize','sample_size')
    s << ms(sData,'lightcache_storeDirectLight','store_direct_light',"0_1")
    s << ms(sData,'lightcache_retrace_on','retrace_enabled')
    s << ms(sData,'lightcache_mode','mode')
    s << ms(sData,'lightcache_numPasses','num_passes')
    puts s
end

def export_raycaster
    sData = get_xml_option_node("SettingsRaycaster");
    s = ""
    s << ms(sData,'system_raycaster_maxLevels','maxLevels')
    s << ms(sData,'MAX','defaultGeometryType')
    s << ms(sData,'system_raycaster_memLimit','dynMemLimit')
    s << ms(sData,'system_raycaster_optLevel','optLevel')
    s << ms(sData,'system_raycaster_minLeafSize','minLeafSize')
    s << ms(sData,'system_raycaster_faceLevelCoeff','faceLevelCoef')
    
    puts s
end

def export_motionblur
    sData = get_xml_option_node("SettingsMotionBlur");
    s = ""
    s << ms(sData,'moblur_on','on')
    s << ms(sData,'moblur_geomSamples','geom_samples')
    s << ms(sData,'MAX','subdivs')
    s << ms(sData,'moblur_intervalCenter','interval_center')
    s << ms(sData,'moblur_bias','bias')
    s << ms(sData,'moblur_duration','duration')
    s << ms(sData,'moblur_dmc_minSamples','low_samples')
    
    puts s
end

def export_irradmap
    sData = get_xml_option_node("SettingsIrradianceMap");
    s = ""
    s << ms(sData,'gi_irradmap_subdivs','subdivs')
    s << ms(sData,'MAX','file')
    s << ms(sData,'MAX','auto_save')
    s << ms(sData,'MAX','lookup_mode')
    s << ms(sData,'gi_irradmap_showCalcPhase','show_calc_phase')
    s << ms(sData,'gi_irradmap_multipleViews','multiple_views')
    s << ms(sData,'gi_irradmap_detail_radius','detail_radius')
    s << ms(sData,'gi_irradmap_colorThreshold','color_threshold')
    s << ms(sData,'MAX','interpolation_mode')
    s << ms(sData,'MAX','randomize_samples')
    s << ms(sData,'gi_irradmap_maxRate','max_rate')
    s << ms(sData,'MAX','auto_save_file')
    s << ms(sData,'gi_irradmap_showSamples','show_samples')
    s << ms(sData,'gi_irradmap_showDirectLight','show_direct_light')
    s << ms(sData,'MAX','check_sample_visibility')
    s << ms(sData,'gi_irradmap_minRate','min_rate')
    s << ms(sData,'gi_irradmap_distThreshold','distance_threshold')
    s << ms(sData,'gi_irradmap_normalThreshold','normal_threshold')
    s << ms(sData,'gi_irradmap_detail_subdivsMult','detail_subdivs_mult')
    s << ms(sData,'gi_irradmap_interpSamples','interp_samples')
    s << ms(sData,'gi_irradmap_interpFrames','interp_frames')
    s << ms(sData,'MAX','multipass')
    s << ms(sData,'MAX','calc_interp_samples')
    s << ms(sData,'MAX','detail_scale')
    s << ms(sData,'MAX','dont_delete')
    s << ms(sData,'MAX','detail_enhancement')
    s << ms(sData,'MAX','mode')
    
    puts s
end
def export_camera_dof
    sData = get_xml_option_node("SettingsCameraDof");
    s = ""
    s << ms(sData,'dof_on','on')
    s << ms(sData,'dof_subdivs','subdivs')
    s << ms(sData,'dof_anisotropy','anisotropy')
    s << ms(sData,'dof_distance','focal_dist')
    s << ms(sData,'dof_sides_on','sides_on')
    s << ms(sData,'dof_bias','center_bias')
    s << ms(sData,'dof_sides_num','sides_num')
    s << ms(sData,'dof_sides_rotation','rotation')
    s << ms(sData,'MAX','override_focal_dist')
    s << ms(sData,'dof_shutter','aperture')
    
    puts s
end
def export_dmc_sampler
    sData = get_xml_option_node("SettingsDMCSampler");
    s = ""
    s << ms(sData,'dmc_earlyTermination_minSamples','adaptive_min_samples')
    s << ms(sData,'dmc_earlyTermination_amount','adaptive_amount')
    s << ms(sData,'dmc_pathSampler_type','path_sampler_type')
    s << ms(sData,'dmc_subdivs_mult','subdivs_mult')
    s << ms(sData,'dmc_earlyTermination_threshold','adaptive_threshold')
    s << ms(sData,'dmc_timeDependent','time_dependent')
    puts s
end
def export_rt_engine
    sData = get_xml_option_node("RTEngine");
    s = ""
    s << ms(sData,'MAX','separate_window')
    s << ms(sData,'MAX','trace_depth')
    s << ms(sData,'MAX','use_opencl')
    s << ms(sData,'gi_reflectCaustics','gi_reflective_caustics')
    s << ms(sData,'MAX','coherent_tracing')
    s << ms(sData,'MAX','show_mask')
    s << ms(sData,'MAX','enabled')
    s << ms(sData,'MAX','bundle_size')
    s << ms(sData,'MAX','max_sample_level')
    s << ms(sData,'MAX','stereo_eye_distance')
    s << ms(sData,'MAX','max_render_time')
    s << ms(sData,'MAX','samples_per_pixel')
    s << ms(sData,'MAX','use_gi')
    s << ms(sData,'MAX','stereo_mode')
    s << ms(sData,'MAX','stop_when_render_done')
    s << ms(sData,'MAX','noise_threshold')
    s << ms(sData,'dmcgi_depth','gi_depth')
    s << ms(sData,'MAX','rt_mode')
    s << ms(sData,'gi_refractCaustics','gi_refractive_caustics')
    s << ms(sData,'MAX','opencl_texsize')
    s << ms(sData,'MAX','stereo_focus')
    
    puts s
end
def export_adaptiveSubdivision
    sData = get_xml_option_node("SettingsImageSampler");
    # creat_list_ms(sData) if $print_list_ms
    s = ""
    puts s
end
def export_adv_irradmap
    sData = get_xml_option_node("SettingsIrradianceMap");
    # creat_list_ms(sData) if $print_list_ms
    s = ""
    puts s
end
def export_camera
    # sData = get_xml_option_node("CameraPhysical");
    sData = get_xml_option_node("SettingsCamera");
    s = ""

    s << ms(sData,'camera_fov','fov')
    s << ms(sData,'camera_fish_distance','dist')
    s << ms(sData,'camera_fish_curve','curve')
    s << ms(sData,'camera_cyl_height','height')
    s << ms(sData,'camera_overrideFOV','override_fov')
    s << ms(sData,'MAX','dont_affect_settings')
    s << ms(sData,'MAX','auto_fit')
    s << ms(sData,'camera_type','type')
    # s << ms(sData,'MAX','anisotropy')
    # s << ms(sData,'MAX','blades_enable')
    # s << ms(sData,'MAX','blades_num')
    # s << ms(sData,'MAX','blades_rotation')
    # s << ms(sData,'MAX','center_bias')
    # s << ms(sData,'MAX','distortion')
    # s << ms(sData,'MAX','dof_display_threshold')
    # s << ms(sData,'MAX','dont_affect_settings')
    # s << ms(sData,'MAX','exposure')
    # s << ms(sData,'MAX','f_number')
    # s << ms(sData,'MAX','film_width')
    # s << ms(sData,'MAX','focal_length')
    # s << ms(sData,'MAX','focus_distance')
    # s << ms(sData,'MAX','fov')
    # s << ms(sData,'MAX','horizontal_offset')
    # s << ms(sData,'MAX','horizontal_shift')
    # s << ms(sData,'MAX','ISO')
    # s << ms(sData,'MAX','latency')
    # s << ms(sData,'MAX','lens_shift')
    # s << ms(sData,'MAX','override_focal_length')
    # s << ms(sData,'MAX','shutter_angle')
    # s << ms(sData,'MAX','shutter_offset')
    # s << ms(sData,'MAX','shutter_speed')
    # s << ms(sData,'MAX','specify_film_width')
    # s << ms(sData,'MAX','specify_focus')
    # s << ms(sData,'MAX','specify_fov')
    # s << ms(sData,'MAX','subdivs')
    # s << ms(sData,'MAX','target_distance')
    # s << ms(sData,'MAX','targeted')
    # s << ms(sData,'MAX','type')
    # s << ms(sData,'MAX','use_dof')
    # s << ms(sData,'MAX','use_moblur')
    # s << ms(sData,'MAX','vertical_offset')
    # s << ms(sData,'MAX','vignetting')
    # s << ms(sData,'MAX','white_balance')
    # s << ms(sData,'MAX','zoom_factor')
    
    puts s
end
def export_caustic
    sData = get_xml_option_node("SettingsCaustics")
    s = ""
    s << ms(sData,'caustics_on','on')
    s << ms(sData,'caustics_loadFileName','file')
    s << ms(sData,'caustics_showCalcPhase','show_calc_phase')
    s << ms(sData,'caustics_dontDelete','dont_delete')
    s << ms(sData,'caustics_maxPhotons','max_photons')
    s << ms(sData,'caustics_maxDensity','max_density')
    s << ms(sData,'caustics_mode','mode')
    s << ms(sData,'caustics_autoSaveFileName','auto_save_file')
    s << ms(sData,'caustics_multiplier','multiplier')
    s << ms(sData,'caustics_searchDist','search_distance')
    s << ms(sData,'caustics_autoSave','auto_save')
    s = ""
    puts s
end
def export_colorMapping
    sData = get_xml_option_node("SettingsColorMapping")
    # creat_list_ms(sData) if $print_list_ms
    s = ""
    s << ms(sData,'colorMapping_clampOutput','clamp_output')
    s << ms(sData,'colorMapping_darkMult','dark_mult')
    s << ms(sData,'colorMapping_brightMult','bright_mult')
    # s << ms(sData,'colorMapping_adaptationOnly','adaptation_only')
    s << ms(sData,'MAX','gamma_correct_ldr_textures')
    s << ms(sData,'colorMapping_subpixel','subpixel_mapping')
    s << ms(sData,'colorMapping_clampLevel','clamp_level')
    s << ms(sData,'MAX','input_gamma')
    s << ms(sData,'colorMapping_affectBackground','affect_background')
    s << ms(sData,'colorMapping_type','type')
    s << ms(sData,'colorMapping_linearWorkflow','linearWorkflow')
    s << ms(sData,'colorMapping_gamma','gamma')
    s << ms(sData,'MAX','gamma_correct_rgb')
    puts s
end
def export_Displacement
    sData = get_xml_option_node("SettingsDefaultDisplacement");
    s = ""
    s << ms(sData,'displacement_overrideMax','override_on')
    s << ms(sData,'displacement_tightBounds','tightBounds')
    s << ms(sData,'displacement_maxSubdivs','maxSubdivs')
    s << ms(sData,'displacement_edgeLength','edgeLength')
    s << ms(sData,'displacement_relative','relative')
    s << ms(sData,'displacement_viewDependent','viewDependent')
    s << ms(sData,'displacement_amount','amount')
    puts s
end

def export_dmcgi
    sData = get_xml_option_node("SettingsDMCGI");
    # creat_list_ms(sData) if $print_list_ms
    s = ""
    s << ms(sData,'dmcgi_subdivs','subdivs')
    s << ms(sData,'dmcgi_depth','depth')
    puts s
end

def export_settings_output 
    sData = get_xml_option_node("SettingsOutput");
    s = ""
    rgn_width = sData["rgn_width"].to_f 
    r_height = sData["r_height"].to_i
    rgn_height = sData["rgn_height"].to_f
    rgn_left = sData["rgn_left"].to_f
    r_left = sData["r_left"].to_i
    rgn_top = sData["rgn_top"].to_f
    r_top = sData["r_top"].to_i
    r_width = sData["r_width"].to_i
    img_rawFile = sData["img_rawFile"].to_s
    img_file = sData["img_file"].to_s

    sData["output_regxmax"] = rgn_left+rgn_width
    sData["output_regymax"] = rgn_top+rgn_height

    s << ms(sData,'output_regxmin','rgn_left')
    s << ms(sData,'output_regxmax','output_regxmax')
    s << ms(sData,'output_regymin','rgn_top')
    s << ms(sData,'output_regymax','output_regymax')

    s << ms(sData,'MAX','anim_end')
    s << ms(sData,'MAX','anim_start')
    s << ms(sData,'MAX','bmp_height')
    s << ms(sData,'MAX','bmp_width')
    s << ms(sData,'MAX','do_animation')
    s << ms(sData,'MAX','frame_rate')
    s << ms(sData,'system_frameStamp_on','frame_stamp_enabled')
    s << ms(sData,'system_frameStamp_string','frame_stamp_text')
    s << ms(sData,'MAX','frame_start')
    s << ms(sData,'MAX','frames')
    s << ms(sData,'MAX','frames_per_second')
    s << ms(sData,'MAX','img_dir')
    s << ms(sData,'MAX','img_file_needFrameNumber')    
    s << ms(sData,'MAX','img_pixelAspectLocked')
    s << ms(sData,'output_saveRawFile','img_rawFile')
    s << ms(sData,'MAX','img_rawFileVFB')
    s << ms(sData,'output_splitAlpha','img_separateAlpha')
    
    # s << ms(sData,'output_getsetsfrommax','override_viewport')
    s << ms(sData,'output_getsetsfrommax','override_viewport',"not_b")


    s << ms(sData,'MAX','r_height')
    s << ms(sData,'MAX','r_left')
    s << ms(sData,'MAX','r_top')
    s << ms(sData,'MAX','r_width')
    s << ms(sData,'MAX','render_frame_range')
    s << ms(sData,'MAX','rgn_height')
    s << ms(sData,'MAX','rgn_left')
    s << ms(sData,'MAX','rgn_top')
    s << ms(sData,'MAX','rgn_width')
    s << ms(sData,'output_saveFile','save_render')
    s << ms(sData,'output_aspect','img_pixelAspect')
    s << ms(sData,'output_aspectlock','img_imageAspectLocked')
    s << ms(sData,'output_fileName','img_file')
    s << ms(sData,'output_imageaspect','img_imageAspect')
    s << ms(sData,'output_height','img_height')
    s << ms(sData,'output_width','img_width')
    # s << c(output_xml_node,"output_aspectlock","img_pixelAspectLocked")
    # s << c(output_xml_node,"output_imageaspect","img_imageAspect")
    # s << c(output_xml_node,"output_width","img_width")
    # s << c(output_xml_node,"output_height","img_height")
    # s << c(output_xml_node,"output_aspect","img_pixelAspect")

    # s << cnv("output_fileOnly",(not img_file == "" and img_rawFile),"bool")
     
    # s << c(output_xml_node,"output_fileName","img_file")
    # s << cnv("output_regxmin",rgn_left,"float")
    # s << cnv("output_regxmax",rgn_left+rgn_width,"float")
    # s << cnv("output_regymin",rgn_top,"float")
    # s << cnv("output_regymax",rgn_top+rgn_height,"float")  
    # s << c(output_xml_node,"output_splitAlpha","img_separateAlpha")    
    # s << c(output_xml_node,"output_saveFile","save_render")    
    # s << c(output_xml_node,"output_saveRawFile","img_rawFile")
    # s << c(output_xml_node,"output_renderType","default")
    # s << c(output_xml_node,"output_splitbitmap","default")

    # s << c(output_xml_node,"system_frameStamp_on","frame_stamp_enabled")
    # s << c(output_xml_node,"system_frameStamp_string","frame_stamp_text")
    # s << c(output_xml_node,"system_frameStamp_justify","default")
    # s << c(output_xml_node,"system_frameStamp_fullWidth","default")
    

    puts s.to_s
end


puts "#{$prefix_cv} = renderers.current\n"
export_settings_output
export_photonMap
export_image_sampler
export_gi
export_environment

export_lightcache
export_colorMapping
export_dmcgi
export_caustic
export_camera
export_adv_irradmap
export_adaptiveSubdivision
export_raycaster
export_motionblur
export_irradmap
export_Displacement
export_camera_dof
export_dmc_sampler
export_rt_engine
export_option


export_light

# puts_maxLight('VRayLight')
# puts_maxLight('VRayIES')

#VRayForSketchUp.find_assets_of_type_in_doc()