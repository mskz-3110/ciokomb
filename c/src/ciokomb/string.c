#include <ciokomb.h>

static String g_Empty = "";

static CIOKOMB_API_SRC(int32, __string_get_allocated_size, CiokombString* string){
  return 0 <= string->__AllocatedSize ? string->__AllocatedSize : string->__AllocatedSize * -1;
}

static CIOKOMB_API_SRC(bool, __string_is_fixed, CiokombString* string){
  return string->__AllocatedSize <= 0;
}

static CIOKOMB_API_SRC(String, __string_if_null, String value, String value_if_null){
  return value == null ? value_if_null : value;
}

CIOKOMB_API_SRC(String, ciokomb_string_empty){
  return g_Empty;
}

CIOKOMB_API_SRC(String, ciokomb_string_guard, String value){
  return value != null ? value : g_Empty;
}

CIOKOMB_API_SRC(bool, ciokomb_string_equals, String value1, String value2){
  if ((value1 == null) || (value2 == null)) return (value1 == null) && (value2 == null);

  int32 length = (int32)strlen(value1);
  return length == (int32)strlen(value2) ? memcmp(value1, value2, length) == 0 : false;
}

CIOKOMB_API_SRC(bool, ciokomb_string_starts_with, String targetValue, String checkValue){
  if ((targetValue == null) || (checkValue == null)) return ((targetValue == null) && (checkValue == null));

  int32 targetLength = (int32)strlen(targetValue);
  int32 checkLength = (int32)strlen(checkValue);
  return checkLength <= targetLength ? memcmp(targetValue, checkValue, checkLength) == 0 : false;
}

CIOKOMB_API_SRC(bool, ciokomb_string_ends_with, String targetValue, String checkValue){
  if ((targetValue == null) || (checkValue == null)) return ((targetValue == null) && (checkValue == null));

  int32 length = (int32)strlen(checkValue);
  int32 index = (int32)strlen(targetValue) - length;
  return 0 <= index ? memcmp(&(targetValue[index]), checkValue, length) == 0 : false;
}

CIOKOMB_API_SRC(int64, ciokomb_string_to_int64, String value){
  return strtol(value, null, 0);
}

CIOKOMB_API_SRC(double, ciokomb_string_to_double, String value){
  return strtod(value, null);
}

CIOKOMB_API_SRC(CiokombString, ciokomb_string_fixed, char* chars, int32 size){
  CiokombString string;
  string.Chars = chars;
  string.__AllocatedSize = -size;
  ciokomb_string_clear(&string);
  return string;
}

CIOKOMB_API_SRC(CiokombString*, ciokomb_string_new){
  CiokombString* string = malloc(sizeof(CiokombString));
  if (string == null) return null;

  string->__AllocatedSize = 4;
  string->Chars = malloc(string->__AllocatedSize);
  if (string->Chars == null){
    ciokomb_string_delete(string);
    return null;
  }

  ciokomb_string_clear(string);
  return string;
}

CIOKOMB_API_SRC(void, ciokomb_string_delete, CiokombString* string){
  if (string == null) return;
  if (string->Chars != null){
    free(string->Chars);
  }
  free(string);
}

CIOKOMB_API_SRC(String, ciokomb_string_get_chars, CiokombString* string){
  return string->Chars;
}

CIOKOMB_API_SRC(int32, ciokomb_string_get_length, CiokombString* string){
  return string->Length;
}

CIOKOMB_API_SRC(void, ciokomb_string_clear, CiokombString* string){
  string->Chars[0] = '\0';
  string->Length = 0;
}

CIOKOMB_API_SRC(void, ciokomb_string_add_int32, CiokombString* string, String format, int32 value){
  ciokomb_string_printf(string, format, value);
}

CIOKOMB_API_SRC(void, ciokomb_string_add_int64, CiokombString* string, String format, int64 value){
  ciokomb_string_printf(string, format, value);
}

CIOKOMB_API_SRC(void, ciokomb_string_add_float, CiokombString* string, String format, float value){
  ciokomb_string_printf(string, format, value);
}

CIOKOMB_API_SRC(void, ciokomb_string_add_double, CiokombString* string, String format, double value){
  ciokomb_string_printf(string, format, value);
}

CIOKOMB_API_SRC(void, ciokomb_string_add_string, CiokombString* string, String format, String value){
  ciokomb_string_printf(string, format, value);
}

CIOKOMB_API_SRC(void, ciokomb_string_add_string_repeat, CiokombString* string, String value, int32 repeatCount){
  for (int32 i = 0; i < repeatCount; ++i){
    ciokomb_string_printf(string, "%s", value);
  }
}

CIOKOMB_API_SRC(void, ciokomb_string_vprintf, CiokombString* string, String format, va_list list){
  va_list copiedList;
  va_copy(copiedList, list);
  int32 writableSize = __string_get_allocated_size(string) - string->Length;
  int32 length = vsnprintf(&(string->Chars[string->Length]), writableSize, format, list);
  do{
    if (length <= 0) break;
    if (length < writableSize){
      string->Length += length;
      break;
    }
    if (__string_is_fixed(string)){
      string->Length = (int32)strlen(string->Chars);
      break;
    }

    int32 needSize = string->Length + length + 1;
    int32 allocatedSize = string->__AllocatedSize * 2;
    while (allocatedSize < needSize) allocatedSize *= 2;
    char* allocatedChars = realloc(string->Chars, allocatedSize);
    if (allocatedChars == null) break;

    string->Chars = allocatedChars;
    string->__AllocatedSize = allocatedSize;
    length = vsnprintf(&(string->Chars[string->Length]), string->__AllocatedSize - string->Length, format, copiedList);
    if (0 < length) string->Length += length;
  }while (0);
  va_end(copiedList);
}

CIOKOMB_API_SRC(void, ciokomb_string_printf, CiokombString* string, String format, ...){
  va_list list;
  va_start(list, format);
  ciokomb_string_vprintf(string, format, list);
  va_end(list);
}

CIOKOMB_API_SRC(void, ciokomb_string_escape_encode, CiokombString* string, String value, int32 length){
  for (int32 i = 0; i < length; ++i){
    switch (value[i]){
      case '\\':{ciokomb_string_printf(string, "%s", "\\\\");}break;
      case '\'':{ciokomb_string_printf(string, "%s", "\\\'");}break;
      case '\"':{ciokomb_string_printf(string, "%s", "\\\"");}break;
      case '\t':{ciokomb_string_printf(string, "%s", "\\t");}break;
      case '\n':{ciokomb_string_printf(string, "%s", "\\n");}break;
      default: ciokomb_string_printf(string, "%c", value[i]);
    }
  }
}

CIOKOMB_API_SRC(void, ciokomb_string_escape_decode, CiokombString* string, String value, int32 length){
  for (int32 i = 0; i < length; ++i){
    if (value[i] != '\\'){
      ciokomb_string_printf(string, "%c", value[i]);
      continue;
    }

    ++i;
    switch (value[i]){
      case '\0':break;
      case '\\':{ciokomb_string_printf(string, "%c", '\\');}break;
      case '\'':{ciokomb_string_printf(string, "%c", '\'');}break;
      case '\"':{ciokomb_string_printf(string, "%c", '\"');}break;
      case 't':{ciokomb_string_printf(string, "%c", '\t');}break;
      case 'n':{ciokomb_string_printf(string, "%c", '\n');}break;
      default: ciokomb_string_printf(string, "%c", value[i]);
    }
  }
}

CIOKOMB_API_SRC(bool, ciokomb_string_yaml_line_split, CiokombString* keyString, CiokombString* valueString, String line){
  ciokomb_string_clear(keyString);
  ciokomb_string_clear(valueString);
  int32 keyLength = 0;
  String keyEndString = strchr(line, ':');
  if (keyEndString == null) return false;

  keyLength = (int32)(keyEndString - line);
  ciokomb_string_printf(keyString, "%.*s", keyLength, line);
  String valueEndString = line + strlen(line);
  ++keyEndString;
  int32 count = (int32)(valueEndString - keyEndString);
  for (int32 i = 0; i < count; ++i){
    if (keyEndString[0] != ' ') break;

    ++keyEndString;
  }
  switch (keyEndString[0]){
    case '\'':
    case '\"':{
      ++keyEndString;
      --valueEndString;
    }break;
  }
  ciokomb_string_escape_decode(valueString, keyEndString, (int32)(valueEndString - keyEndString));
  return true;
}
