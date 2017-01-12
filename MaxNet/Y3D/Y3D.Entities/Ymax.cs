// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ymax.proto
#pragma warning disable 1591, 0612, 3021
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace yproto {

  /// <summary>Holder for reflection information generated from ymax.proto</summary>
  public static partial class YmaxReflection {

    #region Descriptor
    /// <summary>File descriptor for ymax.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbr::FileDescriptor descriptor;

    static YmaxReflection() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "Cgp5bWF4LnByb3RvEgZ5cHJvdG8iGQoMTnVtRmFjZVJhbmdlEgkKAWYYASAD",
            "KAUiKQoGUG9pbnQzEgkKAXgYASABKAISCQoBeRgCIAEoAhIJCgF6GAMgASgC",
            "IkIKBEJveDMSHAoEcG1pbhgBIAEoCzIOLnlwcm90by5Qb2ludDMSHAoEcG1h",
            "eBgCIAEoCzIOLnlwcm90by5Qb2ludDMiPgoFTU5NYXASCQoBZhgBIAMoBRIZ",
            "CgF2GAIgAygLMg4ueXByb3RvLlBvaW50MxIPCgdjaGFubmVsGAMgASgFIn8K",
            "B09iakl0ZW0SDQoFb25hbWUYASABKAkSCwoDaWR4GAIgASgNEhEKCW51bV9m",
            "YWNlcxgDIAEoBRIbCgNwb3MYBCABKAsyDi55cHJvdG8uUG9pbnQzEhoKBGJi",
            "b3gYBSABKAsyDC55cHJvdG8uQm94MxIMCgR4cmVmGAYgAygFIigKB09iakxp",
            "c3QSHQoEb2JqcxgBIAMoCzIPLnlwcm90by5PYmpJdGVtMj4KCFlQcmVwYXJl",
            "EjIKB01ha2VCb3gSFC55cHJvdG8uTnVtRmFjZVJhbmdlGg8ueXByb3RvLk9i",
            "akxpc3QiAEIPogIDWTNEqgIGeXByb3RvYgZwcm90bzM="));
      descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { },
          new pbr::GeneratedClrTypeInfo(null, new pbr::GeneratedClrTypeInfo[] {
            new pbr::GeneratedClrTypeInfo(typeof(global::yproto.NumFaceRange), global::yproto.NumFaceRange.Parser, new[]{ "F" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::yproto.Point3), global::yproto.Point3.Parser, new[]{ "X", "Y", "Z" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::yproto.Box3), global::yproto.Box3.Parser, new[]{ "Pmin", "Pmax" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::yproto.MNMap), global::yproto.MNMap.Parser, new[]{ "F", "V", "Channel" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::yproto.ObjItem), global::yproto.ObjItem.Parser, new[]{ "Oname", "Idx", "NumFaces", "Pos", "Bbox", "Xref" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::yproto.ObjList), global::yproto.ObjList.Parser, new[]{ "Objs" }, null, null, null)
          }));
    }
    #endregion

  }
  #region Messages
  /// <summary>
  ///  The request message containing the user's name.
  /// </summary>
  public sealed partial class NumFaceRange : pb::IMessage<NumFaceRange> {
    private static readonly pb::MessageParser<NumFaceRange> _parser = new pb::MessageParser<NumFaceRange>(() => new NumFaceRange());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<NumFaceRange> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::yproto.YmaxReflection.Descriptor.MessageTypes[0]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public NumFaceRange() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public NumFaceRange(NumFaceRange other) : this() {
      f_ = other.f_.Clone();
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public NumFaceRange Clone() {
      return new NumFaceRange(this);
    }

    /// <summary>Field number for the "f" field.</summary>
    public const int FFieldNumber = 1;
    private static readonly pb::FieldCodec<int> _repeated_f_codec
        = pb::FieldCodec.ForInt32(10);
    private readonly pbc::RepeatedField<int> f_ = new pbc::RepeatedField<int>();
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public pbc::RepeatedField<int> F {
      get { return f_; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as NumFaceRange);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(NumFaceRange other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if(!f_.Equals(other.f_)) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      hash ^= f_.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      f_.WriteTo(output, _repeated_f_codec);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      size += f_.CalculateSize(_repeated_f_codec);
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(NumFaceRange other) {
      if (other == null) {
        return;
      }
      f_.Add(other.f_);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 10:
          case 8: {
            f_.AddEntriesFrom(input, _repeated_f_codec);
            break;
          }
        }
      }
    }

  }

  public sealed partial class Point3 : pb::IMessage<Point3> {
    private static readonly pb::MessageParser<Point3> _parser = new pb::MessageParser<Point3>(() => new Point3());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<Point3> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::yproto.YmaxReflection.Descriptor.MessageTypes[1]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Point3() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Point3(Point3 other) : this() {
      x_ = other.x_;
      y_ = other.y_;
      z_ = other.z_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Point3 Clone() {
      return new Point3(this);
    }

    /// <summary>Field number for the "x" field.</summary>
    public const int XFieldNumber = 1;
    private float x_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public float X {
      get { return x_; }
      set {
        x_ = value;
      }
    }

    /// <summary>Field number for the "y" field.</summary>
    public const int YFieldNumber = 2;
    private float y_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public float Y {
      get { return y_; }
      set {
        y_ = value;
      }
    }

    /// <summary>Field number for the "z" field.</summary>
    public const int ZFieldNumber = 3;
    private float z_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public float Z {
      get { return z_; }
      set {
        z_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as Point3);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(Point3 other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (X != other.X) return false;
      if (Y != other.Y) return false;
      if (Z != other.Z) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (X != 0F) hash ^= X.GetHashCode();
      if (Y != 0F) hash ^= Y.GetHashCode();
      if (Z != 0F) hash ^= Z.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (X != 0F) {
        output.WriteRawTag(13);
        output.WriteFloat(X);
      }
      if (Y != 0F) {
        output.WriteRawTag(21);
        output.WriteFloat(Y);
      }
      if (Z != 0F) {
        output.WriteRawTag(29);
        output.WriteFloat(Z);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (X != 0F) {
        size += 1 + 4;
      }
      if (Y != 0F) {
        size += 1 + 4;
      }
      if (Z != 0F) {
        size += 1 + 4;
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(Point3 other) {
      if (other == null) {
        return;
      }
      if (other.X != 0F) {
        X = other.X;
      }
      if (other.Y != 0F) {
        Y = other.Y;
      }
      if (other.Z != 0F) {
        Z = other.Z;
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
          case 13: {
            X = input.ReadFloat();
            break;
          }
          case 21: {
            Y = input.ReadFloat();
            break;
          }
          case 29: {
            Z = input.ReadFloat();
            break;
          }
        }
      }
    }

  }

  public sealed partial class Box3 : pb::IMessage<Box3> {
    private static readonly pb::MessageParser<Box3> _parser = new pb::MessageParser<Box3>(() => new Box3());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<Box3> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::yproto.YmaxReflection.Descriptor.MessageTypes[2]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Box3() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Box3(Box3 other) : this() {
      Pmin = other.pmin_ != null ? other.Pmin.Clone() : null;
      Pmax = other.pmax_ != null ? other.Pmax.Clone() : null;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Box3 Clone() {
      return new Box3(this);
    }

    /// <summary>Field number for the "pmin" field.</summary>
    public const int PminFieldNumber = 1;
    private global::yproto.Point3 pmin_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::yproto.Point3 Pmin {
      get { return pmin_; }
      set {
        pmin_ = value;
      }
    }

    /// <summary>Field number for the "pmax" field.</summary>
    public const int PmaxFieldNumber = 2;
    private global::yproto.Point3 pmax_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::yproto.Point3 Pmax {
      get { return pmax_; }
      set {
        pmax_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as Box3);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(Box3 other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (!object.Equals(Pmin, other.Pmin)) return false;
      if (!object.Equals(Pmax, other.Pmax)) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (pmin_ != null) hash ^= Pmin.GetHashCode();
      if (pmax_ != null) hash ^= Pmax.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (pmin_ != null) {
        output.WriteRawTag(10);
        output.WriteMessage(Pmin);
      }
      if (pmax_ != null) {
        output.WriteRawTag(18);
        output.WriteMessage(Pmax);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (pmin_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(Pmin);
      }
      if (pmax_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(Pmax);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(Box3 other) {
      if (other == null) {
        return;
      }
      if (other.pmin_ != null) {
        if (pmin_ == null) {
          pmin_ = new global::yproto.Point3();
        }
        Pmin.MergeFrom(other.Pmin);
      }
      if (other.pmax_ != null) {
        if (pmax_ == null) {
          pmax_ = new global::yproto.Point3();
        }
        Pmax.MergeFrom(other.Pmax);
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
          case 10: {
            if (pmin_ == null) {
              pmin_ = new global::yproto.Point3();
            }
            input.ReadMessage(pmin_);
            break;
          }
          case 18: {
            if (pmax_ == null) {
              pmax_ = new global::yproto.Point3();
            }
            input.ReadMessage(pmax_);
            break;
          }
        }
      }
    }

  }

  public sealed partial class MNMap : pb::IMessage<MNMap> {
    private static readonly pb::MessageParser<MNMap> _parser = new pb::MessageParser<MNMap>(() => new MNMap());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<MNMap> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::yproto.YmaxReflection.Descriptor.MessageTypes[3]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public MNMap() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public MNMap(MNMap other) : this() {
      f_ = other.f_.Clone();
      v_ = other.v_.Clone();
      channel_ = other.channel_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public MNMap Clone() {
      return new MNMap(this);
    }

    /// <summary>Field number for the "f" field.</summary>
    public const int FFieldNumber = 1;
    private static readonly pb::FieldCodec<int> _repeated_f_codec
        = pb::FieldCodec.ForInt32(10);
    private readonly pbc::RepeatedField<int> f_ = new pbc::RepeatedField<int>();
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public pbc::RepeatedField<int> F {
      get { return f_; }
    }

    /// <summary>Field number for the "v" field.</summary>
    public const int VFieldNumber = 2;
    private static readonly pb::FieldCodec<global::yproto.Point3> _repeated_v_codec
        = pb::FieldCodec.ForMessage(18, global::yproto.Point3.Parser);
    private readonly pbc::RepeatedField<global::yproto.Point3> v_ = new pbc::RepeatedField<global::yproto.Point3>();
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public pbc::RepeatedField<global::yproto.Point3> V {
      get { return v_; }
    }

    /// <summary>Field number for the "channel" field.</summary>
    public const int ChannelFieldNumber = 3;
    private int channel_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int Channel {
      get { return channel_; }
      set {
        channel_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as MNMap);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(MNMap other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if(!f_.Equals(other.f_)) return false;
      if(!v_.Equals(other.v_)) return false;
      if (Channel != other.Channel) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      hash ^= f_.GetHashCode();
      hash ^= v_.GetHashCode();
      if (Channel != 0) hash ^= Channel.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      f_.WriteTo(output, _repeated_f_codec);
      v_.WriteTo(output, _repeated_v_codec);
      if (Channel != 0) {
        output.WriteRawTag(24);
        output.WriteInt32(Channel);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      size += f_.CalculateSize(_repeated_f_codec);
      size += v_.CalculateSize(_repeated_v_codec);
      if (Channel != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(Channel);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(MNMap other) {
      if (other == null) {
        return;
      }
      f_.Add(other.f_);
      v_.Add(other.v_);
      if (other.Channel != 0) {
        Channel = other.Channel;
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
          case 10:
          case 8: {
            f_.AddEntriesFrom(input, _repeated_f_codec);
            break;
          }
          case 18: {
            v_.AddEntriesFrom(input, _repeated_v_codec);
            break;
          }
          case 24: {
            Channel = input.ReadInt32();
            break;
          }
        }
      }
    }

  }

  public sealed partial class ObjItem : pb::IMessage<ObjItem> {
    private static readonly pb::MessageParser<ObjItem> _parser = new pb::MessageParser<ObjItem>(() => new ObjItem());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<ObjItem> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::yproto.YmaxReflection.Descriptor.MessageTypes[4]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ObjItem() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ObjItem(ObjItem other) : this() {
      oname_ = other.oname_;
      idx_ = other.idx_;
      numFaces_ = other.numFaces_;
      Pos = other.pos_ != null ? other.Pos.Clone() : null;
      Bbox = other.bbox_ != null ? other.Bbox.Clone() : null;
      xref_ = other.xref_.Clone();
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ObjItem Clone() {
      return new ObjItem(this);
    }

    /// <summary>Field number for the "oname" field.</summary>
    public const int OnameFieldNumber = 1;
    private string oname_ = "";
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string Oname {
      get { return oname_; }
      set {
        oname_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
      }
    }

    /// <summary>Field number for the "idx" field.</summary>
    public const int IdxFieldNumber = 2;
    private uint idx_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public uint Idx {
      get { return idx_; }
      set {
        idx_ = value;
      }
    }

    /// <summary>Field number for the "num_faces" field.</summary>
    public const int NumFacesFieldNumber = 3;
    private int numFaces_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int NumFaces {
      get { return numFaces_; }
      set {
        numFaces_ = value;
      }
    }

    /// <summary>Field number for the "pos" field.</summary>
    public const int PosFieldNumber = 4;
    private global::yproto.Point3 pos_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::yproto.Point3 Pos {
      get { return pos_; }
      set {
        pos_ = value;
      }
    }

    /// <summary>Field number for the "bbox" field.</summary>
    public const int BboxFieldNumber = 5;
    private global::yproto.Box3 bbox_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::yproto.Box3 Bbox {
      get { return bbox_; }
      set {
        bbox_ = value;
      }
    }

    /// <summary>Field number for the "xref" field.</summary>
    public const int XrefFieldNumber = 6;
    private static readonly pb::FieldCodec<int> _repeated_xref_codec
        = pb::FieldCodec.ForInt32(50);
    private readonly pbc::RepeatedField<int> xref_ = new pbc::RepeatedField<int>();
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public pbc::RepeatedField<int> Xref {
      get { return xref_; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as ObjItem);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(ObjItem other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (Oname != other.Oname) return false;
      if (Idx != other.Idx) return false;
      if (NumFaces != other.NumFaces) return false;
      if (!object.Equals(Pos, other.Pos)) return false;
      if (!object.Equals(Bbox, other.Bbox)) return false;
      if(!xref_.Equals(other.xref_)) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (Oname.Length != 0) hash ^= Oname.GetHashCode();
      if (Idx != 0) hash ^= Idx.GetHashCode();
      if (NumFaces != 0) hash ^= NumFaces.GetHashCode();
      if (pos_ != null) hash ^= Pos.GetHashCode();
      if (bbox_ != null) hash ^= Bbox.GetHashCode();
      hash ^= xref_.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (Oname.Length != 0) {
        output.WriteRawTag(10);
        output.WriteString(Oname);
      }
      if (Idx != 0) {
        output.WriteRawTag(16);
        output.WriteUInt32(Idx);
      }
      if (NumFaces != 0) {
        output.WriteRawTag(24);
        output.WriteInt32(NumFaces);
      }
      if (pos_ != null) {
        output.WriteRawTag(34);
        output.WriteMessage(Pos);
      }
      if (bbox_ != null) {
        output.WriteRawTag(42);
        output.WriteMessage(Bbox);
      }
      xref_.WriteTo(output, _repeated_xref_codec);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (Oname.Length != 0) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(Oname);
      }
      if (Idx != 0) {
        size += 1 + pb::CodedOutputStream.ComputeUInt32Size(Idx);
      }
      if (NumFaces != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(NumFaces);
      }
      if (pos_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(Pos);
      }
      if (bbox_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(Bbox);
      }
      size += xref_.CalculateSize(_repeated_xref_codec);
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(ObjItem other) {
      if (other == null) {
        return;
      }
      if (other.Oname.Length != 0) {
        Oname = other.Oname;
      }
      if (other.Idx != 0) {
        Idx = other.Idx;
      }
      if (other.NumFaces != 0) {
        NumFaces = other.NumFaces;
      }
      if (other.pos_ != null) {
        if (pos_ == null) {
          pos_ = new global::yproto.Point3();
        }
        Pos.MergeFrom(other.Pos);
      }
      if (other.bbox_ != null) {
        if (bbox_ == null) {
          bbox_ = new global::yproto.Box3();
        }
        Bbox.MergeFrom(other.Bbox);
      }
      xref_.Add(other.xref_);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 10: {
            Oname = input.ReadString();
            break;
          }
          case 16: {
            Idx = input.ReadUInt32();
            break;
          }
          case 24: {
            NumFaces = input.ReadInt32();
            break;
          }
          case 34: {
            if (pos_ == null) {
              pos_ = new global::yproto.Point3();
            }
            input.ReadMessage(pos_);
            break;
          }
          case 42: {
            if (bbox_ == null) {
              bbox_ = new global::yproto.Box3();
            }
            input.ReadMessage(bbox_);
            break;
          }
          case 50:
          case 48: {
            xref_.AddEntriesFrom(input, _repeated_xref_codec);
            break;
          }
        }
      }
    }

  }

  public sealed partial class ObjList : pb::IMessage<ObjList> {
    private static readonly pb::MessageParser<ObjList> _parser = new pb::MessageParser<ObjList>(() => new ObjList());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<ObjList> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::yproto.YmaxReflection.Descriptor.MessageTypes[5]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ObjList() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ObjList(ObjList other) : this() {
      objs_ = other.objs_.Clone();
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ObjList Clone() {
      return new ObjList(this);
    }

    /// <summary>Field number for the "objs" field.</summary>
    public const int ObjsFieldNumber = 1;
    private static readonly pb::FieldCodec<global::yproto.ObjItem> _repeated_objs_codec
        = pb::FieldCodec.ForMessage(10, global::yproto.ObjItem.Parser);
    private readonly pbc::RepeatedField<global::yproto.ObjItem> objs_ = new pbc::RepeatedField<global::yproto.ObjItem>();
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public pbc::RepeatedField<global::yproto.ObjItem> Objs {
      get { return objs_; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as ObjList);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(ObjList other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if(!objs_.Equals(other.objs_)) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      hash ^= objs_.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      objs_.WriteTo(output, _repeated_objs_codec);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      size += objs_.CalculateSize(_repeated_objs_codec);
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(ObjList other) {
      if (other == null) {
        return;
      }
      objs_.Add(other.objs_);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 10: {
            objs_.AddEntriesFrom(input, _repeated_objs_codec);
            break;
          }
        }
      }
    }

  }

  #endregion

}

#endregion Designer generated code
