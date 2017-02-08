#include <sstream>
#include <iostream>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

#include "xml_format.h"
#include "xnormal.pb.h"
#include <google/protobuf/text_format.h>
#define XML_TRUE_STRING "true"
#define XML_FALSE_STRING "false"

using namespace std;
using namespace pugi;

namespace google {
namespace protobuf {

#ifdef PUGIXML_WCHAR_MODE
typedef  std::wstringstream StringStream;
#define toPugiString(X) pugi::as_wide(X)
#define toString(x) std::to_wstring(X)
	}
#else
typedef  std::stringstream StringStream;
#define toPugiString(X) X
#define toString(x) std::to_string(x)
#endif



XmlFormat::Printer::Printer() {}
XmlFormat::Printer::~Printer() {}

void XmlFormat::Printer::PrintToXmlString(const Message& message,
	pugi::string_t* output, ReplacementMap* map) {
  GOOGLE_DCHECK(output) << "output specified is NULL";

  output->clear();

  // create the xml dom
  pugi::xml_document doc;

  MessageToDOM(message,&doc,map);

  StringStream ss;
  ss << doc;
  *output = ss.str();

}


void XmlFormat::Printer::MessageToDOM(const Message& message, pugi::xml_document* doc, ReplacementMap* map) {
	// xml version and encoding
	auto xml_decl = doc->append_child(node_declaration);
	xml_decl.append_attribute("version") = "1.0";
	xml_decl.append_attribute("encoding") = "utf-8";
	

	// the root node of the protobuf xml is the name of the protobuf container
	auto root_node = doc->append_child(message.GetDescriptor()->name().c_str());	

	PrintXml(message, doc, root_node,map);
}



void XmlFormat::Printer::PrintXml(const Message& message,
	pugi::xml_document* doc,
	pugi::xml_node& node, ReplacementMap* map) {

	const Reflection* reflection = message.GetReflection();
	vector<const FieldDescriptor*> fields;
	reflection->ListFields(message, &fields);
	for (unsigned int i = 0; i < fields.size(); i++) {
		PrintXmlField(message, reflection, fields[i], doc, node,map);
	}

}


void XmlFormat::Printer::PrintXmlField(const Message& message,
	const Reflection* reflection,
	const FieldDescriptor* field,
	pugi::xml_document* doc,
	pugi::xml_node& node, ReplacementMap* map) {
	int count = 0;
	if (field->is_repeated()) {
		count = reflection->FieldSize(message, field);
	}
	else if (reflection->HasField(message, field)) {
		count = 1;
	}

	for (int j = 0; j < count; ++j) {
		// Write the field value.
		int field_index = j;
		if (!field->is_repeated()) {
			field_index = -1;
		}

		PrintXmlFieldValue(message, reflection, field, field_index, doc, node, map);
	}
}


void XmlFormat::Printer::PrintXmlFieldValue(
	const Message& message,
	const Reflection* reflection,
	const FieldDescriptor* field,
	int field_index,
	pugi::xml_document* doc,
	pugi::xml_node& node, ReplacementMap* map) {

	GOOGLE_DCHECK(field->is_repeated() || (field_index == -1))
		<< "field_index must be -1 for non-repeated fields";

	switch (field->cpp_type()) {

		//tm use the preprocessor to generate the numerical value cases
		// replace the google used string methods with using a stringstream
#define OUTPUT_FIELD2(CPPTYPE, METHOD, NUM_TYPE)                         \
      case FieldDescriptor::CPPTYPE_##CPPTYPE: {                              \
        NUM_TYPE value = field->is_repeated() ?                      \
          reflection->GetRepeated##METHOD(message, field, field_index) :     \
          reflection->Get##METHOD(message, field);                          \
        StringStream number_stream; \
	    number_stream << value; \
		pugi::string_t number = std::to_string(value);	\
    	node.append_attribute( GetXmlFieldName(message, reflection, field).c_str()) = number_stream.str().c_str();         \
        break;                                                               \
      }

		OUTPUT_FIELD2(INT32, Int32, int32_t);
		OUTPUT_FIELD2(INT64, Int64, int64_t);
		OUTPUT_FIELD2(UINT32, UInt32, uint32_t);
		OUTPUT_FIELD2(UINT64, UInt64, uint64_t);
		OUTPUT_FIELD2(FLOAT, Float, float);
		OUTPUT_FIELD2(DOUBLE, Double, double);
#undef OUTPUT_FIELD2
		
	case FieldDescriptor::CPPTYPE_STRING: {
		string scratch;
		
		const string& value = field->is_repeated() ?
			reflection->GetRepeatedStringReference(
				message, field, field_index, &scratch) :
			reflection->GetStringReference(message, field, &scratch);

		node.append_attribute(GetXmlFieldName(message, reflection, field).c_str()) = toPugiString( value.c_str());		
		break;
	}

	case FieldDescriptor::CPPTYPE_BOOL: {
		if (field->is_repeated()) {
			if (reflection->GetRepeatedBool(message, field, field_index)) {
				node.append_attribute(GetXmlFieldName(message, reflection, field).c_str()) = true;
			}
			else {
				node.append_attribute(GetXmlFieldName(message, reflection, field).c_str()) = false;
			}
		}
		else {
			if (reflection->GetBool(message, field)) {
				node.append_attribute(GetXmlFieldName(message, reflection, field).c_str()) = true;
			}
			else {
				node.append_attribute(GetXmlFieldName(message, reflection, field).c_str()) = false;
			}
		}
		break;
	}

	case FieldDescriptor::CPPTYPE_ENUM: {
		//string value = field->is_repeated() ?
		//	reflection->GetRepeatedEnum(message, field, field_index)->name() :
		//	reflection->GetEnum(message, field)->name();
		//auto val = reflection->GetEnumValue(message, field);
		//auto options = reflection->GetEnum(message, field)->options();
		//value += "   ";
		//auto* pool = ::google::protobuf::DescriptorPool::generated_pool();
		///*auto* desc = pool->FindExtensionByName("my_eSwizzleComponent_option");

		//value += desc->is_extension();*/
		//value += "  ----  ";
		////if (options.GetDescriptor()->extension_count()>0 )		
		//value += options.SerializePartialAsString();
		//value += "  ***  ";
		//value += options.Utf8DebugString();

		pugi::string_t value;
		int enum_value = field->is_repeated()
			? reflection->GetRepeatedEnumValue(message, field, field_index)
			: reflection->GetEnumValue(message, field);
		const EnumValueDescriptor* enum_desc =
			field->enum_type()->FindValueByNumber(enum_value);

		if (enum_desc != NULL) {
			value =  toPugiString(enum_desc->name());
		}
		else {
			// Ordinarily, enum_desc should not be null, because proto2 has the
			// invariant that set enum field values must be in-range, but with the
			// new integer-based API for enums (or the RepeatedField<int> loophole),
			// it is possible for the user to force an unknown integer value.  So we
			// simply use the integer value itself as the enum value name in this
			// case.
			
			value = toString(enum_value);
		}

		
		
		//TextFormat::PrintFieldValueToString(message, field, -1, &value);		
			//GetExtension(my_eSwizzleComponent_option);
		//enum_val_desc->number();
		if (map && map->find(value) != map->end() ) {
			value = (*map)[value];
		}
		node.append_attribute(GetXmlFieldName(message, reflection, field).c_str()) = value.c_str();
		break;
	}
	case FieldDescriptor::CPPTYPE_MESSAGE: {
		// create the child node and recurse
		auto message_node = node.append_child(GetXmlFieldName(message, reflection, field).c_str());

		PrintXml(field->is_repeated() ?
			reflection->GetRepeatedMessage(message, field, field_index) :
			reflection->GetMessage(message, field),
			doc, message_node, map);
		break;
	}
	}
}



pugi::string_t XmlFormat::Printer::GetXmlFieldName(const Message& message,
                                         const Reflection* reflection,
                                         const FieldDescriptor* field) {
	if (field->is_extension()) {
		// We special-case MessageSet elements for compatibility with proto1.
		if (field->containing_type()->options().message_set_wire_format()
			&& field->type() == FieldDescriptor::TYPE_MESSAGE
			&& field->is_optional()
			&& field->extension_scope() == field->message_type()) {
			pugi::string_t out = "[";
			out += field->message_type()->full_name();
			out += "]";
			return out;
		}
		else {
			pugi::string_t out = "[";
			out += field->full_name();
			out += "]";
			return out;
		}
	}
	else if (field->type() == FieldDescriptor::TYPE_GROUP) {
		// Groups must be serialized with their original capitalization.
		return toPugiString(field->message_type()->name());
	}
	else {
		return toPugiString(field->name());
	}
}


/* static */ void XmlFormat::PrintToXmlString(
    const Message& message, pugi::string_t* output, ReplacementMap* map) {
  Printer().PrintToXmlString(message, output,map);
}

/* static */ void XmlFormat::MessageToDOM(
	const Message& message, pugi::xml_document* doc, ReplacementMap* map) {
	Printer().MessageToDOM(message, doc,map);
}



}  // namespace protobuf
}  // namespace google
