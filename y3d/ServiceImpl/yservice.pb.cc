// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: yservice.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "yservice.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace y3d {

namespace {


}  // namespace


void protobuf_AssignDesc_yservice_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_yservice_2eproto() {
  protobuf_AddDesc_yservice_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "yservice.proto");
  GOOGLE_CHECK(file != NULL);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_yservice_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_yservice_2eproto() {
}

void protobuf_AddDesc_yservice_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AddDesc_yservice_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::y3d::protobuf_AddDesc_y3d_2eproto();
  ::google::protobuf::protobuf_AddDesc_google_2fprotobuf_2fany_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\016yservice.proto\022\003y3d\032\ty3d.proto\032\031google"
    "/protobuf/any.proto2\240\t\n\005Tools\0224\n\014RenameO"
    "bject\022\020.y3d.RenameParam\032\020.y3d.ResultRepl"
    "y\"\000\0225\n\013SaveProject\022\020.y3d.ProjectInfo\032\022.y"
    "3d.ResponseEvent\"\000\022;\n\nNewProject\022\024.y3d.N"
    "ewProjectParam\032\025.y3d.ResponseNProject\"\000\022"
    "8\n\013LoadProject\022\020.y3d.ProjectInfo\032\025.y3d.R"
    "esponseNProject\"\000\022:\n\rDeleteProject\022\020.y3d"
    ".ProjectInfo\032\025.y3d.ResponseNProject\"\000\022*\n"
    "\006Unwrap\022\014.y3d.EUnwrap\032\020.y3d.ResultReply\""
    "\000\022,\n\007Packing\022\r.y3d.EPacking\032\020.y3d.Result"
    "Reply\"\000\022,\n\007LowPoly\022\r.y3d.ELowpoly\032\020.y3d."
    "ResultReply\"\000\022-\n\nLoadSystem\022\017.y3d.EmptyP"
    "aram\032\014.y3d.YSystem\"\000\022.\n\nSaveSystem\022\014.y3d"
    ".YSystem\032\020.y3d.ResultReply\"\000\022.\n\nBakeNorm"
    "al\022\014.y3d.ENormal\032\020.y3d.ResultReply\"\000\0221\n\r"
    "BakeNormalMax\022\014.y3d.ENormal\032\020.y3d.Result"
    "Reply\"\000\0228\n\rMakeNode4Edit\022\023.y3d.Make4Test"
    "Param\032\020.y3d.ResultReply\"\000\022<\n\rBatchOptimi"
    "ze\022\027.y3d.BatchOptimizeParam\032\020.y3d.Result"
    "Reply\"\000\022.\n\010TestTest\022\016.y3d.TestParam\032\020.y3"
    "d.ResultReply\"\000\0225\n\020GetObjectFromMax\022\017.y3"
    "d.EmptyParam\032\016.y3d.YAreaList\"\000\0222\n\013CloneO"
    "bject\022\017.y3d.EmptyParam\032\020.y3d.ResultReply"
    "\"\000\0223\n\016DoStreamClient\022\013.y3d.YEvent\032\022.y3d."
    "ResponseEvent(\001\0220\n\016DoStreamServer\022\017.y3d."
    "EmptyParam\032\013.y3d.YEvent0\001\022(\n\010DoAction\022\013."
    "y3d.YEvent\032\013.y3d.YEvent(\0010\001\022*\n\007DoEvent\022\013"
    ".y3d.YEvent\032\022.y3d.ResponseEvent\022,\n\010DoUnw"
    "rap\022\014.y3d.EUnwrap\032\022.y3d.ResponseEvent\022/\n"
    "\010Shutdown\022\017.y3d.EmptyParam\032\020.y3d.ResultR"
    "eply\"\000B\010\252\002\005y3d.sb\006proto3", 1264);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "yservice.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_yservice_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_yservice_2eproto {
  StaticDescriptorInitializer_yservice_2eproto() {
    protobuf_AddDesc_yservice_2eproto();
  }
} static_descriptor_initializer_yservice_2eproto_;

// @@protoc_insertion_point(namespace_scope)

}  // namespace y3d

// @@protoc_insertion_point(global_scope)
