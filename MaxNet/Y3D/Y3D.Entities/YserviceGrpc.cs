// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: yservice.proto
// Original file comments:
// [START declaration]
#region Designer generated code

using System;
using System.Threading;
using System.Threading.Tasks;
using Grpc.Core;

namespace y3d.s {
  public static partial class Tools
  {
    static readonly string __ServiceName = "y3d.Tools";

    static readonly Marshaller<global::y3d.e.RenameParam> __Marshaller_RenameParam = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.RenameParam.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.ResultReply> __Marshaller_ResultReply = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.ResultReply.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.ProjectInfo> __Marshaller_ProjectInfo = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.ProjectInfo.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.ResponseEvent> __Marshaller_ResponseEvent = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.ResponseEvent.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.StringParam> __Marshaller_StringParam = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.StringParam.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.EUnwrap> __Marshaller_EUnwrap = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.EUnwrap.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.EPacking> __Marshaller_EPacking = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.EPacking.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.ELowpoly> __Marshaller_ELowpoly = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.ELowpoly.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.EmptyParam> __Marshaller_EmptyParam = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.EmptyParam.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.YSystem> __Marshaller_YSystem = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.YSystem.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.ENormal> __Marshaller_ENormal = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.ENormal.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.Make4TestParam> __Marshaller_Make4TestParam = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.Make4TestParam.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.BatchOptimizeParam> __Marshaller_BatchOptimizeParam = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.BatchOptimizeParam.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.TestParam> __Marshaller_TestParam = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.TestParam.Parser.ParseFrom);

    static readonly Method<global::y3d.e.RenameParam, global::y3d.e.ResultReply> __Method_RenameObject = new Method<global::y3d.e.RenameParam, global::y3d.e.ResultReply>(
        MethodType.Unary,
        __ServiceName,
        "RenameObject",
        __Marshaller_RenameParam,
        __Marshaller_ResultReply);

    static readonly Method<global::y3d.e.ProjectInfo, global::y3d.e.ResponseEvent> __Method_SaveProject = new Method<global::y3d.e.ProjectInfo, global::y3d.e.ResponseEvent>(
        MethodType.Unary,
        __ServiceName,
        "SaveProject",
        __Marshaller_ProjectInfo,
        __Marshaller_ResponseEvent);

    static readonly Method<global::y3d.e.StringParam, global::y3d.e.ProjectInfo> __Method_NewProject = new Method<global::y3d.e.StringParam, global::y3d.e.ProjectInfo>(
        MethodType.Unary,
        __ServiceName,
        "NewProject",
        __Marshaller_StringParam,
        __Marshaller_ProjectInfo);

    static readonly Method<global::y3d.e.StringParam, global::y3d.e.ProjectInfo> __Method_LoadProject = new Method<global::y3d.e.StringParam, global::y3d.e.ProjectInfo>(
        MethodType.Unary,
        __ServiceName,
        "LoadProject",
        __Marshaller_StringParam,
        __Marshaller_ProjectInfo);

    static readonly Method<global::y3d.e.EUnwrap, global::y3d.e.ResultReply> __Method_Unwrap = new Method<global::y3d.e.EUnwrap, global::y3d.e.ResultReply>(
        MethodType.Unary,
        __ServiceName,
        "Unwrap",
        __Marshaller_EUnwrap,
        __Marshaller_ResultReply);

    static readonly Method<global::y3d.e.EPacking, global::y3d.e.ResultReply> __Method_Packing = new Method<global::y3d.e.EPacking, global::y3d.e.ResultReply>(
        MethodType.Unary,
        __ServiceName,
        "Packing",
        __Marshaller_EPacking,
        __Marshaller_ResultReply);

    static readonly Method<global::y3d.e.ELowpoly, global::y3d.e.ResultReply> __Method_LowPoly = new Method<global::y3d.e.ELowpoly, global::y3d.e.ResultReply>(
        MethodType.Unary,
        __ServiceName,
        "LowPoly",
        __Marshaller_ELowpoly,
        __Marshaller_ResultReply);

    static readonly Method<global::y3d.e.EmptyParam, global::y3d.e.YSystem> __Method_LoadSystem = new Method<global::y3d.e.EmptyParam, global::y3d.e.YSystem>(
        MethodType.Unary,
        __ServiceName,
        "LoadSystem",
        __Marshaller_EmptyParam,
        __Marshaller_YSystem);

    static readonly Method<global::y3d.e.YSystem, global::y3d.e.ResultReply> __Method_SaveSystem = new Method<global::y3d.e.YSystem, global::y3d.e.ResultReply>(
        MethodType.Unary,
        __ServiceName,
        "SaveSystem",
        __Marshaller_YSystem,
        __Marshaller_ResultReply);

    static readonly Method<global::y3d.e.ENormal, global::y3d.e.ResultReply> __Method_BakeNormal = new Method<global::y3d.e.ENormal, global::y3d.e.ResultReply>(
        MethodType.Unary,
        __ServiceName,
        "BakeNormal",
        __Marshaller_ENormal,
        __Marshaller_ResultReply);

    static readonly Method<global::y3d.e.Make4TestParam, global::y3d.e.ResultReply> __Method_MakeNode4Edit = new Method<global::y3d.e.Make4TestParam, global::y3d.e.ResultReply>(
        MethodType.Unary,
        __ServiceName,
        "MakeNode4Edit",
        __Marshaller_Make4TestParam,
        __Marshaller_ResultReply);

    static readonly Method<global::y3d.e.BatchOptimizeParam, global::y3d.e.ResultReply> __Method_BatchOptimize = new Method<global::y3d.e.BatchOptimizeParam, global::y3d.e.ResultReply>(
        MethodType.Unary,
        __ServiceName,
        "BatchOptimize",
        __Marshaller_BatchOptimizeParam,
        __Marshaller_ResultReply);

    static readonly Method<global::y3d.e.TestParam, global::y3d.e.ResultReply> __Method_TestTest = new Method<global::y3d.e.TestParam, global::y3d.e.ResultReply>(
        MethodType.Unary,
        __ServiceName,
        "TestTest",
        __Marshaller_TestParam,
        __Marshaller_ResultReply);

    /// <summary>Service descriptor</summary>
    public static global::Google.Protobuf.Reflection.ServiceDescriptor Descriptor
    {
      get { return global::y3d.s.YserviceReflection.Descriptor.Services[0]; }
    }

    /// <summary>Base class for server-side implementations of Tools</summary>
    public abstract partial class ToolsBase
    {
      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ResultReply> RenameObject(global::y3d.e.RenameParam request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ResponseEvent> SaveProject(global::y3d.e.ProjectInfo request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ProjectInfo> NewProject(global::y3d.e.StringParam request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ProjectInfo> LoadProject(global::y3d.e.StringParam request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ResultReply> Unwrap(global::y3d.e.EUnwrap request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ResultReply> Packing(global::y3d.e.EPacking request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ResultReply> LowPoly(global::y3d.e.ELowpoly request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.e.YSystem> LoadSystem(global::y3d.e.EmptyParam request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ResultReply> SaveSystem(global::y3d.e.YSystem request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ResultReply> BakeNormal(global::y3d.e.ENormal request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ResultReply> MakeNode4Edit(global::y3d.e.Make4TestParam request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ResultReply> BatchOptimize(global::y3d.e.BatchOptimizeParam request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ResultReply> TestTest(global::y3d.e.TestParam request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

    }

    /// <summary>Client for Tools</summary>
    public partial class ToolsClient : ClientBase<ToolsClient>
    {
      /// <summary>Creates a new client for Tools</summary>
      /// <param name="channel">The channel to use to make remote calls.</param>
      public ToolsClient(Channel channel) : base(channel)
      {
      }
      /// <summary>Creates a new client for Tools that uses a custom <c>CallInvoker</c>.</summary>
      /// <param name="callInvoker">The callInvoker to use to make remote calls.</param>
      public ToolsClient(CallInvoker callInvoker) : base(callInvoker)
      {
      }
      /// <summary>Protected parameterless constructor to allow creation of test doubles.</summary>
      protected ToolsClient() : base()
      {
      }
      /// <summary>Protected constructor to allow creation of configured clients.</summary>
      /// <param name="configuration">The client configuration.</param>
      protected ToolsClient(ClientBaseConfiguration configuration) : base(configuration)
      {
      }

      public virtual global::y3d.e.ResultReply RenameObject(global::y3d.e.RenameParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return RenameObject(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.ResultReply RenameObject(global::y3d.e.RenameParam request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_RenameObject, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> RenameObjectAsync(global::y3d.e.RenameParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return RenameObjectAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> RenameObjectAsync(global::y3d.e.RenameParam request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_RenameObject, null, options, request);
      }
      public virtual global::y3d.e.ResponseEvent SaveProject(global::y3d.e.ProjectInfo request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return SaveProject(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.ResponseEvent SaveProject(global::y3d.e.ProjectInfo request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_SaveProject, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResponseEvent> SaveProjectAsync(global::y3d.e.ProjectInfo request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return SaveProjectAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResponseEvent> SaveProjectAsync(global::y3d.e.ProjectInfo request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_SaveProject, null, options, request);
      }
      public virtual global::y3d.e.ProjectInfo NewProject(global::y3d.e.StringParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return NewProject(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.ProjectInfo NewProject(global::y3d.e.StringParam request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_NewProject, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.ProjectInfo> NewProjectAsync(global::y3d.e.StringParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return NewProjectAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.ProjectInfo> NewProjectAsync(global::y3d.e.StringParam request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_NewProject, null, options, request);
      }
      public virtual global::y3d.e.ProjectInfo LoadProject(global::y3d.e.StringParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return LoadProject(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.ProjectInfo LoadProject(global::y3d.e.StringParam request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_LoadProject, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.ProjectInfo> LoadProjectAsync(global::y3d.e.StringParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return LoadProjectAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.ProjectInfo> LoadProjectAsync(global::y3d.e.StringParam request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_LoadProject, null, options, request);
      }
      public virtual global::y3d.e.ResultReply Unwrap(global::y3d.e.EUnwrap request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return Unwrap(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.ResultReply Unwrap(global::y3d.e.EUnwrap request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_Unwrap, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> UnwrapAsync(global::y3d.e.EUnwrap request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return UnwrapAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> UnwrapAsync(global::y3d.e.EUnwrap request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_Unwrap, null, options, request);
      }
      public virtual global::y3d.e.ResultReply Packing(global::y3d.e.EPacking request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return Packing(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.ResultReply Packing(global::y3d.e.EPacking request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_Packing, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> PackingAsync(global::y3d.e.EPacking request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return PackingAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> PackingAsync(global::y3d.e.EPacking request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_Packing, null, options, request);
      }
      public virtual global::y3d.e.ResultReply LowPoly(global::y3d.e.ELowpoly request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return LowPoly(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.ResultReply LowPoly(global::y3d.e.ELowpoly request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_LowPoly, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> LowPolyAsync(global::y3d.e.ELowpoly request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return LowPolyAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> LowPolyAsync(global::y3d.e.ELowpoly request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_LowPoly, null, options, request);
      }
      public virtual global::y3d.e.YSystem LoadSystem(global::y3d.e.EmptyParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return LoadSystem(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.YSystem LoadSystem(global::y3d.e.EmptyParam request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_LoadSystem, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.YSystem> LoadSystemAsync(global::y3d.e.EmptyParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return LoadSystemAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.YSystem> LoadSystemAsync(global::y3d.e.EmptyParam request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_LoadSystem, null, options, request);
      }
      public virtual global::y3d.e.ResultReply SaveSystem(global::y3d.e.YSystem request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return SaveSystem(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.ResultReply SaveSystem(global::y3d.e.YSystem request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_SaveSystem, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> SaveSystemAsync(global::y3d.e.YSystem request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return SaveSystemAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> SaveSystemAsync(global::y3d.e.YSystem request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_SaveSystem, null, options, request);
      }
      public virtual global::y3d.e.ResultReply BakeNormal(global::y3d.e.ENormal request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return BakeNormal(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.ResultReply BakeNormal(global::y3d.e.ENormal request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_BakeNormal, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> BakeNormalAsync(global::y3d.e.ENormal request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return BakeNormalAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> BakeNormalAsync(global::y3d.e.ENormal request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_BakeNormal, null, options, request);
      }
      public virtual global::y3d.e.ResultReply MakeNode4Edit(global::y3d.e.Make4TestParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return MakeNode4Edit(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.ResultReply MakeNode4Edit(global::y3d.e.Make4TestParam request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_MakeNode4Edit, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> MakeNode4EditAsync(global::y3d.e.Make4TestParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return MakeNode4EditAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> MakeNode4EditAsync(global::y3d.e.Make4TestParam request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_MakeNode4Edit, null, options, request);
      }
      public virtual global::y3d.e.ResultReply BatchOptimize(global::y3d.e.BatchOptimizeParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return BatchOptimize(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.ResultReply BatchOptimize(global::y3d.e.BatchOptimizeParam request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_BatchOptimize, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> BatchOptimizeAsync(global::y3d.e.BatchOptimizeParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return BatchOptimizeAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> BatchOptimizeAsync(global::y3d.e.BatchOptimizeParam request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_BatchOptimize, null, options, request);
      }
      public virtual global::y3d.e.ResultReply TestTest(global::y3d.e.TestParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return TestTest(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.ResultReply TestTest(global::y3d.e.TestParam request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_TestTest, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> TestTestAsync(global::y3d.e.TestParam request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return TestTestAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResultReply> TestTestAsync(global::y3d.e.TestParam request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_TestTest, null, options, request);
      }
      /// <summary>Creates a new instance of client from given <c>ClientBaseConfiguration</c>.</summary>
      protected override ToolsClient NewInstance(ClientBaseConfiguration configuration)
      {
        return new ToolsClient(configuration);
      }
    }

    /// <summary>Creates service definition that can be registered with a server</summary>
    /// <param name="serviceImpl">An object implementing the server-side handling logic.</param>
    public static ServerServiceDefinition BindService(ToolsBase serviceImpl)
    {
      return ServerServiceDefinition.CreateBuilder()
          .AddMethod(__Method_RenameObject, serviceImpl.RenameObject)
          .AddMethod(__Method_SaveProject, serviceImpl.SaveProject)
          .AddMethod(__Method_NewProject, serviceImpl.NewProject)
          .AddMethod(__Method_LoadProject, serviceImpl.LoadProject)
          .AddMethod(__Method_Unwrap, serviceImpl.Unwrap)
          .AddMethod(__Method_Packing, serviceImpl.Packing)
          .AddMethod(__Method_LowPoly, serviceImpl.LowPoly)
          .AddMethod(__Method_LoadSystem, serviceImpl.LoadSystem)
          .AddMethod(__Method_SaveSystem, serviceImpl.SaveSystem)
          .AddMethod(__Method_BakeNormal, serviceImpl.BakeNormal)
          .AddMethod(__Method_MakeNode4Edit, serviceImpl.MakeNode4Edit)
          .AddMethod(__Method_BatchOptimize, serviceImpl.BatchOptimize)
          .AddMethod(__Method_TestTest, serviceImpl.TestTest).Build();
    }

  }
  public static partial class YAction
  {
    static readonly string __ServiceName = "y3d.YAction";

    static readonly Marshaller<global::y3d.e.Event> __Marshaller_Event = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.Event.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.ResponseEvent> __Marshaller_ResponseEvent = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.ResponseEvent.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.e.EUnwrap> __Marshaller_EUnwrap = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.e.EUnwrap.Parser.ParseFrom);

    static readonly Method<global::y3d.e.Event, global::y3d.e.ResponseEvent> __Method_DoAction = new Method<global::y3d.e.Event, global::y3d.e.ResponseEvent>(
        MethodType.ClientStreaming,
        __ServiceName,
        "DoAction",
        __Marshaller_Event,
        __Marshaller_ResponseEvent);

    static readonly Method<global::y3d.e.EUnwrap, global::y3d.e.ResponseEvent> __Method_DoUnwrap = new Method<global::y3d.e.EUnwrap, global::y3d.e.ResponseEvent>(
        MethodType.Unary,
        __ServiceName,
        "DoUnwrap",
        __Marshaller_EUnwrap,
        __Marshaller_ResponseEvent);

    /// <summary>Service descriptor</summary>
    public static global::Google.Protobuf.Reflection.ServiceDescriptor Descriptor
    {
      get { return global::y3d.s.YserviceReflection.Descriptor.Services[1]; }
    }

    /// <summary>Base class for server-side implementations of YAction</summary>
    public abstract partial class YActionBase
    {
      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ResponseEvent> DoAction(IAsyncStreamReader<global::y3d.e.Event> requestStream, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.e.ResponseEvent> DoUnwrap(global::y3d.e.EUnwrap request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

    }

    /// <summary>Client for YAction</summary>
    public partial class YActionClient : ClientBase<YActionClient>
    {
      /// <summary>Creates a new client for YAction</summary>
      /// <param name="channel">The channel to use to make remote calls.</param>
      public YActionClient(Channel channel) : base(channel)
      {
      }
      /// <summary>Creates a new client for YAction that uses a custom <c>CallInvoker</c>.</summary>
      /// <param name="callInvoker">The callInvoker to use to make remote calls.</param>
      public YActionClient(CallInvoker callInvoker) : base(callInvoker)
      {
      }
      /// <summary>Protected parameterless constructor to allow creation of test doubles.</summary>
      protected YActionClient() : base()
      {
      }
      /// <summary>Protected constructor to allow creation of configured clients.</summary>
      /// <param name="configuration">The client configuration.</param>
      protected YActionClient(ClientBaseConfiguration configuration) : base(configuration)
      {
      }

      public virtual AsyncClientStreamingCall<global::y3d.e.Event, global::y3d.e.ResponseEvent> DoAction(Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return DoAction(new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncClientStreamingCall<global::y3d.e.Event, global::y3d.e.ResponseEvent> DoAction(CallOptions options)
      {
        return CallInvoker.AsyncClientStreamingCall(__Method_DoAction, null, options);
      }
      public virtual global::y3d.e.ResponseEvent DoUnwrap(global::y3d.e.EUnwrap request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return DoUnwrap(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.e.ResponseEvent DoUnwrap(global::y3d.e.EUnwrap request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_DoUnwrap, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResponseEvent> DoUnwrapAsync(global::y3d.e.EUnwrap request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return DoUnwrapAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.e.ResponseEvent> DoUnwrapAsync(global::y3d.e.EUnwrap request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_DoUnwrap, null, options, request);
      }
      /// <summary>Creates a new instance of client from given <c>ClientBaseConfiguration</c>.</summary>
      protected override YActionClient NewInstance(ClientBaseConfiguration configuration)
      {
        return new YActionClient(configuration);
      }
    }

    /// <summary>Creates service definition that can be registered with a server</summary>
    /// <param name="serviceImpl">An object implementing the server-side handling logic.</param>
    public static ServerServiceDefinition BindService(YActionBase serviceImpl)
    {
      return ServerServiceDefinition.CreateBuilder()
          .AddMethod(__Method_DoAction, serviceImpl.DoAction)
          .AddMethod(__Method_DoUnwrap, serviceImpl.DoUnwrap).Build();
    }

  }
}
#endregion
