// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: yloader.proto
#region Designer generated code

using System;
using System.Threading;
using System.Threading.Tasks;
using Grpc.Core;

namespace y3d.s {
  public static partial class Loader
  {
    static readonly string __ServiceName = "y3d.Loader";

    static readonly Marshaller<global::y3d.s.LibInfo> __Marshaller_LibInfo = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.s.LibInfo.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.s.ResultType> __Marshaller_ResultType = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.s.ResultType.Parser.ParseFrom);

    static readonly Method<global::y3d.s.LibInfo, global::y3d.s.ResultType> __Method_Shutdown = new Method<global::y3d.s.LibInfo, global::y3d.s.ResultType>(
        MethodType.Unary,
        __ServiceName,
        "Shutdown",
        __Marshaller_LibInfo,
        __Marshaller_ResultType);

    static readonly Method<global::y3d.s.LibInfo, global::y3d.s.ResultType> __Method_LoadDll = new Method<global::y3d.s.LibInfo, global::y3d.s.ResultType>(
        MethodType.Unary,
        __ServiceName,
        "LoadDll",
        __Marshaller_LibInfo,
        __Marshaller_ResultType);

    /// <summary>Service descriptor</summary>
    public static global::Google.Protobuf.Reflection.ServiceDescriptor Descriptor
    {
      get { return global::y3d.s.YloaderReflection.Descriptor.Services[0]; }
    }

    /// <summary>Base class for server-side implementations of Loader</summary>
    public abstract partial class LoaderBase
    {
      public virtual global::System.Threading.Tasks.Task<global::y3d.s.ResultType> Shutdown(global::y3d.s.LibInfo request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.s.ResultType> LoadDll(global::y3d.s.LibInfo request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

    }

    /// <summary>Client for Loader</summary>
    public partial class LoaderClient : ClientBase<LoaderClient>
    {
      /// <summary>Creates a new client for Loader</summary>
      /// <param name="channel">The channel to use to make remote calls.</param>
      public LoaderClient(Channel channel) : base(channel)
      {
      }
      /// <summary>Creates a new client for Loader that uses a custom <c>CallInvoker</c>.</summary>
      /// <param name="callInvoker">The callInvoker to use to make remote calls.</param>
      public LoaderClient(CallInvoker callInvoker) : base(callInvoker)
      {
      }
      /// <summary>Protected parameterless constructor to allow creation of test doubles.</summary>
      protected LoaderClient() : base()
      {
      }
      /// <summary>Protected constructor to allow creation of configured clients.</summary>
      /// <param name="configuration">The client configuration.</param>
      protected LoaderClient(ClientBaseConfiguration configuration) : base(configuration)
      {
      }

      public virtual global::y3d.s.ResultType Shutdown(global::y3d.s.LibInfo request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return Shutdown(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.s.ResultType Shutdown(global::y3d.s.LibInfo request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_Shutdown, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.s.ResultType> ShutdownAsync(global::y3d.s.LibInfo request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return ShutdownAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.s.ResultType> ShutdownAsync(global::y3d.s.LibInfo request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_Shutdown, null, options, request);
      }
      public virtual global::y3d.s.ResultType LoadDll(global::y3d.s.LibInfo request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return LoadDll(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.s.ResultType LoadDll(global::y3d.s.LibInfo request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_LoadDll, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.s.ResultType> LoadDllAsync(global::y3d.s.LibInfo request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return LoadDllAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.s.ResultType> LoadDllAsync(global::y3d.s.LibInfo request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_LoadDll, null, options, request);
      }
      /// <summary>Creates a new instance of client from given <c>ClientBaseConfiguration</c>.</summary>
      protected override LoaderClient NewInstance(ClientBaseConfiguration configuration)
      {
        return new LoaderClient(configuration);
      }
    }

    /// <summary>Creates service definition that can be registered with a server</summary>
    /// <param name="serviceImpl">An object implementing the server-side handling logic.</param>
    public static ServerServiceDefinition BindService(LoaderBase serviceImpl)
    {
      return ServerServiceDefinition.CreateBuilder()
          .AddMethod(__Method_Shutdown, serviceImpl.Shutdown)
          .AddMethod(__Method_LoadDll, serviceImpl.LoadDll).Build();
    }

  }
  /// <summary>
  /// server run in 3dsmax order to control Tool service
  /// </summary>
  public static partial class YServiceMaxLoader
  {
    static readonly string __ServiceName = "y3d.YServiceMaxLoader";

    static readonly Marshaller<global::y3d.s.LibInfo> __Marshaller_LibInfo = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.s.LibInfo.Parser.ParseFrom);
    static readonly Marshaller<global::y3d.s.ResultType> __Marshaller_ResultType = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::y3d.s.ResultType.Parser.ParseFrom);

    static readonly Method<global::y3d.s.LibInfo, global::y3d.s.ResultType> __Method_Shutdown = new Method<global::y3d.s.LibInfo, global::y3d.s.ResultType>(
        MethodType.Unary,
        __ServiceName,
        "Shutdown",
        __Marshaller_LibInfo,
        __Marshaller_ResultType);

    static readonly Method<global::y3d.s.LibInfo, global::y3d.s.ResultType> __Method_LoadDll = new Method<global::y3d.s.LibInfo, global::y3d.s.ResultType>(
        MethodType.Unary,
        __ServiceName,
        "LoadDll",
        __Marshaller_LibInfo,
        __Marshaller_ResultType);

    static readonly Method<global::y3d.s.LibInfo, global::y3d.s.ResultType> __Method_CloseApp = new Method<global::y3d.s.LibInfo, global::y3d.s.ResultType>(
        MethodType.Unary,
        __ServiceName,
        "CloseApp",
        __Marshaller_LibInfo,
        __Marshaller_ResultType);

    /// <summary>Service descriptor</summary>
    public static global::Google.Protobuf.Reflection.ServiceDescriptor Descriptor
    {
      get { return global::y3d.s.YloaderReflection.Descriptor.Services[1]; }
    }

    /// <summary>Base class for server-side implementations of YServiceMaxLoader</summary>
    public abstract partial class YServiceMaxLoaderBase
    {
      public virtual global::System.Threading.Tasks.Task<global::y3d.s.ResultType> Shutdown(global::y3d.s.LibInfo request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.s.ResultType> LoadDll(global::y3d.s.LibInfo request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual global::System.Threading.Tasks.Task<global::y3d.s.ResultType> CloseApp(global::y3d.s.LibInfo request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

    }

    /// <summary>Client for YServiceMaxLoader</summary>
    public partial class YServiceMaxLoaderClient : ClientBase<YServiceMaxLoaderClient>
    {
      /// <summary>Creates a new client for YServiceMaxLoader</summary>
      /// <param name="channel">The channel to use to make remote calls.</param>
      public YServiceMaxLoaderClient(Channel channel) : base(channel)
      {
      }
      /// <summary>Creates a new client for YServiceMaxLoader that uses a custom <c>CallInvoker</c>.</summary>
      /// <param name="callInvoker">The callInvoker to use to make remote calls.</param>
      public YServiceMaxLoaderClient(CallInvoker callInvoker) : base(callInvoker)
      {
      }
      /// <summary>Protected parameterless constructor to allow creation of test doubles.</summary>
      protected YServiceMaxLoaderClient() : base()
      {
      }
      /// <summary>Protected constructor to allow creation of configured clients.</summary>
      /// <param name="configuration">The client configuration.</param>
      protected YServiceMaxLoaderClient(ClientBaseConfiguration configuration) : base(configuration)
      {
      }

      public virtual global::y3d.s.ResultType Shutdown(global::y3d.s.LibInfo request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return Shutdown(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.s.ResultType Shutdown(global::y3d.s.LibInfo request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_Shutdown, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.s.ResultType> ShutdownAsync(global::y3d.s.LibInfo request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return ShutdownAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.s.ResultType> ShutdownAsync(global::y3d.s.LibInfo request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_Shutdown, null, options, request);
      }
      public virtual global::y3d.s.ResultType LoadDll(global::y3d.s.LibInfo request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return LoadDll(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.s.ResultType LoadDll(global::y3d.s.LibInfo request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_LoadDll, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.s.ResultType> LoadDllAsync(global::y3d.s.LibInfo request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return LoadDllAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.s.ResultType> LoadDllAsync(global::y3d.s.LibInfo request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_LoadDll, null, options, request);
      }
      public virtual global::y3d.s.ResultType CloseApp(global::y3d.s.LibInfo request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return CloseApp(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::y3d.s.ResultType CloseApp(global::y3d.s.LibInfo request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_CloseApp, null, options, request);
      }
      public virtual AsyncUnaryCall<global::y3d.s.ResultType> CloseAppAsync(global::y3d.s.LibInfo request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return CloseAppAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::y3d.s.ResultType> CloseAppAsync(global::y3d.s.LibInfo request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_CloseApp, null, options, request);
      }
      /// <summary>Creates a new instance of client from given <c>ClientBaseConfiguration</c>.</summary>
      protected override YServiceMaxLoaderClient NewInstance(ClientBaseConfiguration configuration)
      {
        return new YServiceMaxLoaderClient(configuration);
      }
    }

    /// <summary>Creates service definition that can be registered with a server</summary>
    /// <param name="serviceImpl">An object implementing the server-side handling logic.</param>
    public static ServerServiceDefinition BindService(YServiceMaxLoaderBase serviceImpl)
    {
      return ServerServiceDefinition.CreateBuilder()
          .AddMethod(__Method_Shutdown, serviceImpl.Shutdown)
          .AddMethod(__Method_LoadDll, serviceImpl.LoadDll)
          .AddMethod(__Method_CloseApp, serviceImpl.CloseApp).Build();
    }

  }
}
#endregion