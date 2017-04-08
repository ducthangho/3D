def get_all_params_node( parentNode, nodeNames = nil )
    if nodeNames == nil
        nodeNames = []
    end
    nodes = parentNode.elementsByTagName("parameter")
    return nil if nodes.length() == 0
    for currentNode in nodes
        nodeNames.push currentNode.attribute('name') 
        puts currentNode.attribute('name')
    end
    return nodeNames
end

# model = Sketchup.active_model
# entities = model.entities
# for e in entities
#   if (VRayForSketchUp.is_light( e ))
#     light_xml = VRayForSketchUp.get_updated_light_xml(e)
#     light_xml_doc = VRayXML::QDomDocument.new light_xml
#     puts "\n #{lt}"
#     lt = VRayForSketchUp.get_vrayplugin_name_from_asset_node(light_xml_doc)
#     get_all_params_node(light_xml_doc)

#     # if lt == "LightRectangle"
#     #   c = VRayForSketchUp.get_integer_parameter_value_from_xml_node( light_xml_doc, "causticSubdivs")
#     #   puts "c= #{c}"
#     # end
#     #puts light_xml
#     puts "\n---------------------------------------"
#   end
# end

#VRayForSketchUp.find_assets_of_type_in_doc()