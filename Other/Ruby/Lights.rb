$suLightAttr = {
    'VRayLight' => {},
    'VRaySun' => {},
    'VRayIES' => {},
}

def ll (suData,maxKey,suKey)
  if (suData[suKey] == nil) or (maxKey == "MAX")
    return ""
  end
  suVal = suData[suKey].elementsByTagName( "value" )
  if suVal.length() == 0
    suVal = ""
  end
  maxValue = get_ms_val(suVal[0],suData[suKey].attribute('type').to_s)
  if maxValue != nil
    return "\nll.#{maxKey}=#{maxValue}"
  else
    return ""
  end  
end

def puts_maxLight(lName)
    puts "\n--------------- Thuoc tinh cho MaxLight: #{lName}--------------"
    h = Hash[$suLightAttr[lName].sort]
    h.each do |key, val|
      puts val
    end      
    h.each do |key, val|
      puts "s << ll(suData,'MAX','#{key}')"
    end      
end

def get_all_params_node( parentNode, data = nil )
    if data == nil
        data = {}
    end
    nodes = parentNode.elementsByTagName("parameter")
    return nil if nodes.length() == 0
    for currentNode in nodes
        name = currentNode.attribute('name')
        data[name] = currentNode
        # puts "s << ll(suData,'MAX','#{name}')"
    end
    return data
end

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
  when "filename"
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

def creatVRaySun(suData)
  return nil if suData==nil
  suData.each do |key, val|
    $suLightAttr['VRaySun'][key] = "#{key} - (type:#{val.attribute('type').to_s})"
  end  
  s = "ll = VRaySun()"
end

def creatVRayLight(suData)
  return nil if suData==nil
  suData.each do |key, val|
    $suLightAttr['VRayLight'][key] = "#{key} - (type:#{val.attribute('type').to_s})"
  end  
  s = "ll = VRayLight()"
  s << ll(suData,'affect_diffuse','affectDiffuse')
  s << ll(suData,'affect_reflections','affectReflections')
  s << ll(suData,'affect_specualr','affectSpecular')
  s << ll(suData,'MAX','areaSpeculars')
  s << ll(suData,'MAX','area_speculars')
  s << ll(suData,'MAX','barnDoor')
  s << ll(suData,'MAX','barnDoorBottom')
  s << ll(suData,'MAX','barnDoorLeft')
  s << ll(suData,'MAX','barnDoorRight')
  s << ll(suData,'MAX','barnDoorTop')
  s << ll(suData,'MAX','bumped_below_surface_check')
  s << ll(suData,'MAX','causticMult')
  s << ll(suData,'MAX','causticSubdivs')
  s << ll(suData,'MAX','channels')
  s << ll(suData,'MAX','channels_diffuse')
  s << ll(suData,'MAX','channels_raw')
  s << ll(suData,'MAX','channels_specular')
  s << ll(suData,'color','color')
  s << ll(suData,'MAX','color_tex')
  s << ll(suData,'MAX','coneAngle')
  s << ll(suData,'MAX','cutoffThreshold')
  s << ll(suData,'MAX','decay')
  s << ll(suData,'MAX','diffuseMult')
  s << ll(suData,'MAX','diffuse_contribution')
  s << ll(suData,'dome_emitRadius','dome_emitRadius')
  s << ll(suData,'dome_rayDistance','dome_rayDistance')
  s << ll(suData,'dome_rayDistanceMode','dome_rayDistanceMode')
  s << ll(suData,'dome_spherical','dome_spherical')
  s << ll(suData,'dome_targetRadius','dome_targetRadius')
  s << ll(suData,'MAX','dome_tex')
  s << ll(suData,'doubleSided','doubleSided')
  s << ll(suData,'MAX','dropOff')
  s << ll(suData,'on','enabled')
  s << ll(suData,'MAX','endDistance1')
  s << ll(suData,'MAX','endDistance2')
  s << ll(suData,'MAX','endDistance3')
  s << ll(suData,'MAX','falloffType')
  s << ll(suData,'MAX','filter_color')
  s << ll(suData,'MAX','ignoreLightNormals')
  s << ll(suData,'MAX','intensity')
  s << ll(suData,'MAX','intensity_tex')
  s << ll(suData,'invisible','invisible')
  s << ll(suData,'MAX','legacyMode')
  s << ll(suData,'MAX','lightPortal')
  s << ll(suData,'noDecay','noDecay')
  s << ll(suData,'MAX','nsamples')
  s << ll(suData,'MAX','objectID')
  s << ll(suData,'MAX','penumbraAngle')
  s << ll(suData,'MAX','photonSubdivs')
  s << ll(suData,'MAX','power')
  s << ll(suData,'MAX','radius')
  s << ll(suData,'MAX','rect_tex')
  s << ll(suData,'shadowBias','shadowBias')
  s << ll(suData,'MAX','shadowColor')
  s << ll(suData,'MAX','shadowColor_tex')
  s << ll(suData,'MAX','shadowRadius')
  s << ll(suData,'MAX','shadowSubdivs')
  s << ll(suData,'MAX','shadows')
  s << ll(suData,'MAX','specular_contribution')
  s << ll(suData,'MAX','sphere_segments')
  s << ll(suData,'MAX','startDistance1')
  s << ll(suData,'MAX','startDistance2')
  s << ll(suData,'MAX','startDistance3')
  s << ll(suData,'storeWithIrradMap','storeWithIrradianceMap')
  s << ll(suData,'subdivs','subdivs')
  s << ll(suData,'MAX','tex_adaptive')
  s << ll(suData,'MAX','tex_resolution')
  s << ll(suData,'transform','transform')
  s << ll(suData,'MAX','u_size')
  s << ll(suData,'MAX','units')
  s << ll(suData,'MAX','useDecayRegions')
  s << ll(suData,'MAX','use_dome_tex')
  s << ll(suData,'MAX','v_size')
  puts s
end

def creatVRayIES(suData)
  return nil if suData==nil
  suData.each do |key, val|
    $suLightAttr['VRayIES'][key] = "#{key} - (type:#{val.attribute('type').to_s})"
  end  
  s = "ll = VRayIES()"
  s << ll(suData,'affect_diffuse','affectDiffuse')
  s << ll(suData,'affect_specular','affectSpecular')
  s << ll(suData,'area_speculars','area_speculars')
  s << ll(suData,'MAX','bumped_below_surface_check')
  s << ll(suData,'MAX','causticMult')
  s << ll(suData,'MAX','causticSubdivs')
  s << ll(suData,'MAX','channels')
  s << ll(suData,'MAX','channels_diffuse')
  s << ll(suData,'MAX','channels_raw')
  s << ll(suData,'MAX','channels_specular')
  s << ll(suData,'color','color')
  s << ll(suData,'MAX','color_tex')
  s << ll(suData,'MAX','cutoffThreshold')
  s << ll(suData,'MAX','diffuseMult')
  s << ll(suData,'MAX','diffuse_contribution')
  s << ll(suData,'MAX','enabled')
  s << ll(suData,'MAX','filter_color')
  s << ll(suData,'ies_file','ies_file')
  s << ll(suData,'MAX','nsamples')
  s << ll(suData,'MAX','photonSubdivs')
  s << ll(suData,'power','power')
  s << ll(suData,'shadow_bias','shadowBias')
  s << ll(suData,'MAX','shadowColor')
  s << ll(suData,'MAX','shadowColor_tex')
  s << ll(suData,'MAX','shadowSubdivs')
  s << ll(suData,'MAX','shadows')
  s << ll(suData,'MAX','soft_shadows')
  s << ll(suData,'MAX','specular_contribution')
  s << ll(suData,'transform','transform')
  s << ll(suData,'MAX','use_global_light_level')
  puts s
end


def find_similar_light(suLight)
  case suLight
    when "LightRectangle",  "LightSpot", "LightDome", "LightSphere"
      return "VRayLight"
    when "LightIES"
      return "VRayIES"
    # when "LightOmni"
    #   return "Omnilight"
    else return nil
  end
end

model = Sketchup.active_model
entities = model.entities
for e in entities
  if (VRayForSketchUp.is_light( e ))
    light_xml = VRayForSketchUp.get_updated_light_xml(e)
    light_xml_doc = VRayXML::QDomDocument.new light_xml

    lt = VRayForSketchUp.get_vrayplugin_name_from_asset_node(light_xml_doc)
    maxLight = find_similar_light(lt)
    if maxLight != nil
      puts "\n------ Light: #{lt} => #{maxLight} ------------------"
      suData = get_all_params_node(light_xml_doc)
      # suData.each do |key, val|
      #   puts "#{key}-(type:#{val.attribute('type').to_s})"
      # end

      if maxLight == "VRayLight"
        creatVRayLight(suData)
      elsif maxLight == "VRayIES"
        creatVRayIES(suData)
      end
    else
      puts "\n -------------------- Chua co du lieu den tuong ung voi loai den #{lt} =============================="
    end
    # if lt == "LightRectangle"
    #   c = VRayForSketchUp.get_integer_parameter_value_from_xml_node( light_xml_doc, "causticSubdivs")
    #   puts "c= #{c}"
    # end
    #puts light_xml
    puts "\n---------------------------------------"
  end
end
puts_maxLight('VRayLight')
puts_maxLight('VRayIES')

#VRayForSketchUp.find_assets_of_type_in_doc()