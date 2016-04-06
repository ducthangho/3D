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

def ll (suData,maxKey,suKey)
  return gen_ms_by_type(maxKey,suData[suKey].attribute('value'),suData[suKey].attribute('type'),"ll.")
end

def creatVRayIES(suData)
  s = "ll = VRayIES()"
  s << ll(suData,"affect_diffuse","affectDiffuse")
end

def creatVRayLight(suData)
  s = "ll = VRayLight()"
  s << ll(suData,"affect_diffuse","affectDiffuse")
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
        puts "s << ll(suData,'MAX','#{name}')"
        #puts "#{currentNode.attribute('name')}-(type:#{currentNode.attribute('type').to_s})" 
    end
    return data
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
  if type=="interger"
    return VRayForSketchUp.get_integer_parameter_value_from_xml_node( output_xml_node, name)
  elsif type=="boolean"
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

def find_similar_light(suLight)
  case suLight
    when "LightIES", "LightRectangle", "LightOmni", "LightSpot"
      return "VRayLight"
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
    puts "\n------Light: #{lt}------------------"
    suData = get_all_params_node(light_xml_doc)
    if maxLight == "VRayLight"
      creatVRayLight(suData)
    elsif maxLight == "VRayIES"
      creatVRayIES(suData)
    end
    # if lt == "LightRectangle"
    #   c = VRayForSketchUp.get_integer_parameter_value_from_xml_node( light_xml_doc, "causticSubdivs")
    #   puts "c= #{c}"
    # end
    #puts light_xml
    puts "\n---------------------------------------"
  end
end

#VRayForSketchUp.find_assets_of_type_in_doc()