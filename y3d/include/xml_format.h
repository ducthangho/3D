//
// Based on the original text_format.h file in the google.protobuf library
//
// Utilities for printing and parsing protocol messages in a simple xml format.
//
#ifndef GOOGLE_PROTOBUF_XML_FORMAT_H__
#define GOOGLE_PROTOBUF_XML_FORMAT_H__

#include <string>
#include "pugixml.hpp"
#include <map>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

typedef std::map<pugi::string_t, pugi::string_t> ReplacementMap;

namespace google {
namespace protobuf {

// This class implements protocol buffer xml format.
//
// This class is really a namespace that contains only static methods.
class LIBPROTOBUF_EXPORT XmlFormat {
 public:

  // Like MessageToDOM(), but outputs xml directly to a string.
  static void PrintToXmlString(const Message& message, pugi::string_t* output,ReplacementMap* map = nullptr);

  // Like ...
  static void MessageToDOM(const Message& message, pugi::xml_document* doc, ReplacementMap* map = nullptr);
  
  // Class for those users which require more fine-grained control over how
  // a protobuffer message is printed out.
  class LIBPROTOBUF_EXPORT Printer {
   public:
    Printer();
    ~Printer();

    // Like XmlFormat::PrintToXmlString
    void PrintToXmlString(const Message& message, pugi::string_t* output, ReplacementMap* map = nullptr);
    
	void MessageToDOM(const Message& message, pugi::xml_document* doc, ReplacementMap* map = nullptr);


   private:

    // Utility function to return the right name function based
    // on field type
    string GetXmlFieldName(const Message& message,
                        const Reflection* reflection,
                        const FieldDescriptor* field);

	// Internal Print method
	void PrintXml(const Message& message,
		pugi::xml_document* doc,
		pugi::xml_node& node, ReplacementMap* map = nullptr);

	// Print a single field.
	void PrintXmlField(const Message& message,
		const Reflection* reflection,
		const FieldDescriptor* field,
		pugi::xml_document* doc,
		pugi::xml_node& node, ReplacementMap* map = nullptr);

	// Utility function to return the right name function based
	// on field type
	//string GetXmlFieldName(const Message& message,
	//	const Reflection* reflection,
	//	const FieldDescriptor* field);

	// Outputs a textual representation of the value of the field supplied on
	// the message supplied or the default value if not set.
	void PrintXmlFieldValue(const Message& message,
		const Reflection* reflection,
		const FieldDescriptor* field,
		int field_index,
		pugi::xml_document* doc,
		pugi::xml_node& node, ReplacementMap* map = nullptr);

  };


 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(XmlFormat);
};

}  // namespace protobuf

}  // namespace google
#endif  // GOOGLE_PROTOBUF_XML_FORMAT_H__

