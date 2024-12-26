#ifndef __CIOKOMB_STRING_H__
#define __CIOKOMB_STRING_H__
#include <ciokomb/api.h>

CIOKOMB_STRUCT_BEGIN
  char* Chars;
  int32 Length;
  int32 __AllocatedSize;
CIOKOMB_STRUCT_END(CiokombString)

CIOKOMB_API_BEGIN
  CIOKOMB_API_INC(String, ciokomb_string_empty)
  CIOKOMB_API_INC(String, ciokomb_string_guard, String value)
  CIOKOMB_API_INC(bool, ciokomb_string_equals, String value1, String value2)
  CIOKOMB_API_INC(bool, ciokomb_string_starts_with, String targetValue, String checkValue)
  CIOKOMB_API_INC(bool, ciokomb_string_ends_with, String targetValue, String checkValue)
  CIOKOMB_API_INC(int64, ciokomb_string_to_int64, String value)
  CIOKOMB_API_INC(double, ciokomb_string_to_double, String value)

  CIOKOMB_API_INC(CiokombString, ciokomb_string_fixed, char* chars, int32 size)
  CIOKOMB_API_INC(CiokombString*, ciokomb_string_new)
  CIOKOMB_API_INC(void, ciokomb_string_delete, CiokombString* string)
  CIOKOMB_API_INC(String, ciokomb_string_get_chars, CiokombString* string)
  CIOKOMB_API_INC(int32, ciokomb_string_get_length, CiokombString* string)
  CIOKOMB_API_INC(void, ciokomb_string_clear, CiokombString* string)
  CIOKOMB_API_INC(void, ciokomb_string_add_int32, CiokombString* string, String format, int32 value)
  CIOKOMB_API_INC(void, ciokomb_string_add_int64, CiokombString* string, String format, int64 value)
  CIOKOMB_API_INC(void, ciokomb_string_add_float, CiokombString* string, String format, float value)
  CIOKOMB_API_INC(void, ciokomb_string_add_double, CiokombString* string, String format, double value)
  CIOKOMB_API_INC(void, ciokomb_string_add_string, CiokombString* string, String format, String value)
  CIOKOMB_API_INC(void, ciokomb_string_add_string_repeat, CiokombString* string, String value, int32 repeatCount)
  CIOKOMB_API_INC(void, ciokomb_string_vprintf, CiokombString* string, String format, va_list list)
  CIOKOMB_API_INC(void, ciokomb_string_printf, CiokombString* string, String format, ...)
  CIOKOMB_API_INC(void, ciokomb_string_escape_encode, CiokombString* string, String value, int32 length)
  CIOKOMB_API_INC(void, ciokomb_string_escape_decode, CiokombString* string, String value, int32 length)
  CIOKOMB_API_INC(bool, ciokomb_string_yaml_line_split, CiokombString* keyString, CiokombString* valueString, String line)
CIOKOMB_API_END
#endif
