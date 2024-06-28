// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Enum.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Enum_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Enum_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3017000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3017001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Enum_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Enum_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Enum_2eproto;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE
namespace Protocol {

enum MoveState : int {
  MOVE_STATE_NONE = 0,
  MOVE_STATE_IDLE = 1,
  MOVE_STATE_RUN = 2,
  MoveState_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  MoveState_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool MoveState_IsValid(int value);
constexpr MoveState MoveState_MIN = MOVE_STATE_NONE;
constexpr MoveState MoveState_MAX = MOVE_STATE_RUN;
constexpr int MoveState_ARRAYSIZE = MoveState_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MoveState_descriptor();
template<typename T>
inline const std::string& MoveState_Name(T enum_t_value) {
  static_assert(::std::is_same<T, MoveState>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function MoveState_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    MoveState_descriptor(), enum_t_value);
}
inline bool MoveState_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, MoveState* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<MoveState>(
    MoveState_descriptor(), name, value);
}
enum PlayerType : int {
  PLAYER_TYPE_NONE = 0,
  PLAYER_TYPE_KNIGHT = 1,
  PLAYER_TYPE_MAGE = 2,
  PLAYER_TYPE_ARCHER = 3,
  PlayerType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  PlayerType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool PlayerType_IsValid(int value);
constexpr PlayerType PlayerType_MIN = PLAYER_TYPE_NONE;
constexpr PlayerType PlayerType_MAX = PLAYER_TYPE_ARCHER;
constexpr int PlayerType_ARRAYSIZE = PlayerType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* PlayerType_descriptor();
template<typename T>
inline const std::string& PlayerType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, PlayerType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function PlayerType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    PlayerType_descriptor(), enum_t_value);
}
inline bool PlayerType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, PlayerType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<PlayerType>(
    PlayerType_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace Protocol

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::Protocol::MoveState> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::MoveState>() {
  return ::Protocol::MoveState_descriptor();
}
template <> struct is_proto_enum< ::Protocol::PlayerType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::PlayerType>() {
  return ::Protocol::PlayerType_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Enum_2eproto
