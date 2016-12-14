// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: yservice.proto
#pragma warning disable 1591, 0612, 3021
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace Y3D.YService {

  /// <summary>Holder for reflection information generated from yservice.proto</summary>
  public static partial class YserviceReflection {

    #region Descriptor
    /// <summary>File descriptor for yservice.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbr::FileDescriptor descriptor;

    static YserviceReflection() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "Cg55c2VydmljZS5wcm90bxIDeTNkGhlnb29nbGUvcHJvdG9idWYvYW55LnBy",
            "b3RvGgl5M2QucHJvdG8iLQoLUmVzdWx0UmVwbHkSDQoFZXJyb3IYASABKAgS",
            "DwoHbWVzc2FnZRgCIAEoCSIgCgtSZW5hbWVQYXJhbRIRCgl1c2VTZWxlY3QY",
            "ASABKAgyPQoFVG9vbHMSNAoMUmVuYW1lT2JqZWN0EhAueTNkLlJlbmFtZVBh",
            "cmFtGhAueTNkLlJlc3VsdFJlcGx5IgBCD6oCDFkzRC5ZU2VydmljZWIGcHJv",
            "dG8z"));
      descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { global::Google.Protobuf.WellKnownTypes.AnyReflection.Descriptor, global::Y3D.Entities.Y3DReflection.Descriptor, },
          new pbr::GeneratedClrTypeInfo(null, new pbr::GeneratedClrTypeInfo[] {
            new pbr::GeneratedClrTypeInfo(typeof(global::Y3D.YService.ResultReply), global::Y3D.YService.ResultReply.Parser, new[]{ "Error", "Message" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Y3D.YService.RenameParam), global::Y3D.YService.RenameParam.Parser, new[]{ "UseSelect" }, null, null, null)
          }));
    }
    #endregion

  }
  #region Messages
  public sealed partial class ResultReply : pb::IMessage<ResultReply> {
    private static readonly pb::MessageParser<ResultReply> _parser = new pb::MessageParser<ResultReply>(() => new ResultReply());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<ResultReply> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Y3D.YService.YserviceReflection.Descriptor.MessageTypes[0]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ResultReply() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ResultReply(ResultReply other) : this() {
      error_ = other.error_;
      message_ = other.message_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ResultReply Clone() {
      return new ResultReply(this);
    }

    /// <summary>Field number for the "error" field.</summary>
    public const int ErrorFieldNumber = 1;
    private bool error_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Error {
      get { return error_; }
      set {
        error_ = value;
      }
    }

    /// <summary>Field number for the "message" field.</summary>
    public const int MessageFieldNumber = 2;
    private string message_ = "";
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string Message {
      get { return message_; }
      set {
        message_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as ResultReply);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(ResultReply other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (Error != other.Error) return false;
      if (Message != other.Message) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (Error != false) hash ^= Error.GetHashCode();
      if (Message.Length != 0) hash ^= Message.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (Error != false) {
        output.WriteRawTag(8);
        output.WriteBool(Error);
      }
      if (Message.Length != 0) {
        output.WriteRawTag(18);
        output.WriteString(Message);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (Error != false) {
        size += 1 + 1;
      }
      if (Message.Length != 0) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(Message);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(ResultReply other) {
      if (other == null) {
        return;
      }
      if (other.Error != false) {
        Error = other.Error;
      }
      if (other.Message.Length != 0) {
        Message = other.Message;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 8: {
            Error = input.ReadBool();
            break;
          }
          case 18: {
            Message = input.ReadString();
            break;
          }
        }
      }
    }

  }

  public sealed partial class RenameParam : pb::IMessage<RenameParam> {
    private static readonly pb::MessageParser<RenameParam> _parser = new pb::MessageParser<RenameParam>(() => new RenameParam());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<RenameParam> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Y3D.YService.YserviceReflection.Descriptor.MessageTypes[1]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public RenameParam() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public RenameParam(RenameParam other) : this() {
      useSelect_ = other.useSelect_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public RenameParam Clone() {
      return new RenameParam(this);
    }

    /// <summary>Field number for the "useSelect" field.</summary>
    public const int UseSelectFieldNumber = 1;
    private bool useSelect_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool UseSelect {
      get { return useSelect_; }
      set {
        useSelect_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as RenameParam);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(RenameParam other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (UseSelect != other.UseSelect) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (UseSelect != false) hash ^= UseSelect.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (UseSelect != false) {
        output.WriteRawTag(8);
        output.WriteBool(UseSelect);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (UseSelect != false) {
        size += 1 + 1;
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(RenameParam other) {
      if (other == null) {
        return;
      }
      if (other.UseSelect != false) {
        UseSelect = other.UseSelect;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 8: {
            UseSelect = input.ReadBool();
            break;
          }
        }
      }
    }

  }

  #endregion

}

#endregion Designer generated code
