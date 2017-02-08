// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: yloader.proto

#ifndef PROTOBUF_yloader_2eproto__INCLUDED
#define PROTOBUF_yloader_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000002 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace y3d {

// Internal implementation detail -- do not call these.
void Y3D_EXPORT_MACRO protobuf_AddDesc_yloader_2eproto();
void protobuf_AssignDesc_yloader_2eproto();
void protobuf_ShutdownFile_yloader_2eproto();

class LibInfo;
class ResultType;

// ===================================================================

class Y3D_EXPORT_MACRO LibInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:y3d.LibInfo) */ {
 public:
  LibInfo();
  virtual ~LibInfo();

  LibInfo(const LibInfo& from);

  inline LibInfo& operator=(const LibInfo& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const LibInfo& default_instance();

  void Swap(LibInfo* other);

  // implements Message ----------------------------------------------

  inline LibInfo* New() const { return New(NULL); }

  LibInfo* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const LibInfo& from);
  void MergeFrom(const LibInfo& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(LibInfo* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 id = 1;
  void clear_id();
  static const int kIdFieldNumber = 1;
  ::google::protobuf::int32 id() const;
  void set_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:y3d.LibInfo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::int32 id_;
  mutable int _cached_size_;
  friend void Y3D_EXPORT_MACRO protobuf_AddDesc_yloader_2eproto();
  friend void protobuf_AssignDesc_yloader_2eproto();
  friend void protobuf_ShutdownFile_yloader_2eproto();

  void InitAsDefaultInstance();
  static LibInfo* default_instance_;
};
// -------------------------------------------------------------------

class Y3D_EXPORT_MACRO ResultType : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:y3d.ResultType) */ {
 public:
  ResultType();
  virtual ~ResultType();

  ResultType(const ResultType& from);

  inline ResultType& operator=(const ResultType& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ResultType& default_instance();

  void Swap(ResultType* other);

  // implements Message ----------------------------------------------

  inline ResultType* New() const { return New(NULL); }

  ResultType* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ResultType& from);
  void MergeFrom(const ResultType& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(ResultType* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional bool error = 1;
  void clear_error();
  static const int kErrorFieldNumber = 1;
  bool error() const;
  void set_error(bool value);

  // optional string message = 2;
  void clear_message();
  static const int kMessageFieldNumber = 2;
  const ::std::string& message() const;
  void set_message(const ::std::string& value);
  void set_message(const char* value);
  void set_message(const char* value, size_t size);
  ::std::string* mutable_message();
  ::std::string* release_message();
  void set_allocated_message(::std::string* message);

  // @@protoc_insertion_point(class_scope:y3d.ResultType)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr message_;
  bool error_;
  mutable int _cached_size_;
  friend void Y3D_EXPORT_MACRO protobuf_AddDesc_yloader_2eproto();
  friend void protobuf_AssignDesc_yloader_2eproto();
  friend void protobuf_ShutdownFile_yloader_2eproto();

  void InitAsDefaultInstance();
  static ResultType* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// LibInfo

// optional int32 id = 1;
inline void LibInfo::clear_id() {
  id_ = 0;
}
inline ::google::protobuf::int32 LibInfo::id() const {
  // @@protoc_insertion_point(field_get:y3d.LibInfo.id)
  return id_;
}
inline void LibInfo::set_id(::google::protobuf::int32 value) {
  
  id_ = value;
  // @@protoc_insertion_point(field_set:y3d.LibInfo.id)
}

// -------------------------------------------------------------------

// ResultType

// optional bool error = 1;
inline void ResultType::clear_error() {
  error_ = false;
}
inline bool ResultType::error() const {
  // @@protoc_insertion_point(field_get:y3d.ResultType.error)
  return error_;
}
inline void ResultType::set_error(bool value) {
  
  error_ = value;
  // @@protoc_insertion_point(field_set:y3d.ResultType.error)
}

// optional string message = 2;
inline void ResultType::clear_message() {
  message_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& ResultType::message() const {
  // @@protoc_insertion_point(field_get:y3d.ResultType.message)
  return message_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ResultType::set_message(const ::std::string& value) {
  
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:y3d.ResultType.message)
}
inline void ResultType::set_message(const char* value) {
  
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:y3d.ResultType.message)
}
inline void ResultType::set_message(const char* value, size_t size) {
  
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:y3d.ResultType.message)
}
inline ::std::string* ResultType::mutable_message() {
  
  // @@protoc_insertion_point(field_mutable:y3d.ResultType.message)
  return message_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ResultType::release_message() {
  // @@protoc_insertion_point(field_release:y3d.ResultType.message)
  
  return message_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ResultType::set_allocated_message(::std::string* message) {
  if (message != NULL) {
    
  } else {
    
  }
  message_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), message);
  // @@protoc_insertion_point(field_set_allocated:y3d.ResultType.message)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace y3d

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_yloader_2eproto__INCLUDED