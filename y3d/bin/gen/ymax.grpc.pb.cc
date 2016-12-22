// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: ymax.proto

#include "ymax.pb.h"
#include "ymax.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace yproto {

static const char* YPrepare_method_names[] = {
  "/yproto.YPrepare/MakeBox",
};

std::unique_ptr< YPrepare::Stub> YPrepare::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< YPrepare::Stub> stub(new YPrepare::Stub(channel));
  return stub;
}

YPrepare::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_MakeBox_(YPrepare_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status YPrepare::Stub::MakeBox(::grpc::ClientContext* context, const ::yproto::NumFaceRange& request, ::yproto::ObjList* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_MakeBox_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::yproto::ObjList>* YPrepare::Stub::AsyncMakeBoxRaw(::grpc::ClientContext* context, const ::yproto::NumFaceRange& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::yproto::ObjList>(channel_.get(), cq, rpcmethod_MakeBox_, context, request);
}

YPrepare::Service::Service() {
  (void)YPrepare_method_names;
  AddMethod(new ::grpc::RpcServiceMethod(
      YPrepare_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< YPrepare::Service, ::yproto::NumFaceRange, ::yproto::ObjList>(
          std::mem_fn(&YPrepare::Service::MakeBox), this)));
}

YPrepare::Service::~Service() {
}

::grpc::Status YPrepare::Service::MakeBox(::grpc::ServerContext* context, const ::yproto::NumFaceRange* request, ::yproto::ObjList* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace yproto

