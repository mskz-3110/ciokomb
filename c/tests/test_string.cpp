#include <ciokomb.h>

#define ASSERT_STRING(_string, _check_chars, _check_length) assert_string(__FILE__, __LINE__, _string, _check_chars, _check_length)

void assert_string(String file, int32 line, CiokombString* string, String checkChars, int32 checkLength){
  String chars = ciokomb_string_get_chars(string);
  int32 length = ciokomb_string_get_length(string);
  ciokomb_assert(file, line, length == checkLength && ciokomb_string_equals(chars, checkChars), "length=%d chars='%s'", length, chars);
}

void test_int32(CiokombString* string){
  ciokomb_string_clear(string);
  ciokomb_string_add_int32(string, "%d:", -1);
  ciokomb_string_add_int32(string, "%d", 1);
  ASSERT_STRING(string, "-1:1", 4);
}

void test_int64(CiokombString* string){
  ciokomb_string_clear(string);
  ciokomb_string_add_int64(string, "%lld:", -1);
  ciokomb_string_add_int64(string, "%lld", 1);
  ASSERT_STRING(string, "-1:1", 4);
}

void test_float(CiokombString* string){
  ciokomb_string_clear(string);
  ciokomb_string_add_float(string, "%.1f:", -1.5f);
  ciokomb_string_add_float(string, "%.1f", 1.5f);
  ASSERT_STRING(string, "-1.5:1.5", 8);
}

void test_double(CiokombString* string){
  ciokomb_string_clear(string);
  ciokomb_string_add_double(string, "%.1f:", -1.5f);
  ciokomb_string_add_double(string, "%.1f", 1.5f);
  ASSERT_STRING(string, "-1.5:1.5", 8);
}

void test_string(CiokombString* string){
  ciokomb_string_clear(string);
  ciokomb_string_add_string(string, "%s:", "abcde");
  ciokomb_string_add_string(string, "%s", "あいうえお");
  ASSERT_STRING(string, "abcde:あいうえお", 16);
  ciokomb_string_clear(string);
  ciokomb_string_add_string_repeat(string, "ABC", 3);
  ASSERT_STRING(string, "ABCABCABC", 9);
}

void test_escape(){
  char encodeFixedString[32];
  CiokombString encodeString = ciokomb_string_fixed(encodeFixedString, sizeof(encodeFixedString));
  char decodeFixedString[32];
  CiokombString decodeString = ciokomb_string_fixed(decodeFixedString, sizeof(decodeFixedString));
  ciokomb_string_escape_encode(&encodeString, "1\\2\'3\"4\t5\n6", 11);
  ASSERT_STRING(&encodeString, "1\\\\2\\\'3\\\"4\\t5\\n6", 16);
  ciokomb_string_escape_decode(&decodeString, ciokomb_string_get_chars(&encodeString), ciokomb_string_get_length(&encodeString));
  ASSERT_STRING(&decodeString, "1\\2\'3\"4\t5\n6", 11);
}

void test_yaml_line(){
  char keyFixedString[32];
  CiokombString keyString = ciokomb_string_fixed(keyFixedString, sizeof(keyFixedString));
  char valueFixedString[32];
  CiokombString valueString = ciokomb_string_fixed(valueFixedString, sizeof(valueFixedString));
  CIOKOMB_ASSERT(ciokomb_string_yaml_line_split(&keyString, &valueString, "Key: Value"), "yaml line split error");
  ASSERT_STRING(&keyString, "Key", 3);
  ASSERT_STRING(&valueString, "Value", 5);
  ciokomb_string_clear(&keyString);
  ciokomb_string_clear(&valueString);
  CIOKOMB_ASSERT(ciokomb_string_yaml_line_split(&keyString, &valueString, "Japanese: 'あいうえお'"), "yaml line split error");
  ASSERT_STRING(&keyString, "Japanese", 8);
  ASSERT_STRING(&valueString, "あいうえお", 10);
  ciokomb_string_clear(&keyString);
  ciokomb_string_clear(&valueString);
  CIOKOMB_ASSERT(ciokomb_string_yaml_line_split(&keyString, &valueString, "Empty: ''"), "yaml line split error");
  ASSERT_STRING(&keyString, "Empty", 5);
  ASSERT_STRING(&valueString, "", 0);
  ciokomb_string_clear(&keyString);
  ciokomb_string_clear(&valueString);
  CIOKOMB_ASSERT(ciokomb_string_yaml_line_split(&keyString, &valueString, "Escape: \"1\\\\2\\\'3\\\"4\\t5\\n6\""), "yaml line split error");
  ASSERT_STRING(&keyString, "Escape", 6);
  ASSERT_STRING(&valueString, "1\\2\'3\"4\t5\n6", 11);
  ciokomb_string_clear(&keyString);
  ciokomb_string_clear(&valueString);
  CIOKOMB_ASSERT(!ciokomb_string_yaml_line_split(&keyString, &valueString, "Error"), "yaml line split error");
}

int32 main(int32 argc, String argv[]){
  CIOKOMB_ASSERT(ciokomb_string_equals(ciokomb_string_guard(null), ""), "guard error");
  CIOKOMB_ASSERT(ciokomb_string_equals(ciokomb_string_guard("abc"), "abc"), "guard error");
  CIOKOMB_ASSERT(ciokomb_string_starts_with("file.ext", ""), "starts with error");
  CIOKOMB_ASSERT(!ciokomb_string_starts_with("", "file"), "starts with error");
  CIOKOMB_ASSERT(ciokomb_string_starts_with("file.ext", "file"), "starts with error");
  CIOKOMB_ASSERT(ciokomb_string_starts_with("file.ext", "file.ext"), "starts with error");
  CIOKOMB_ASSERT(!ciokomb_string_starts_with("file.ext", "file.ext1"), "starts with error");
  CIOKOMB_ASSERT(ciokomb_string_ends_with("file.ext", ""), "ends with error");
  CIOKOMB_ASSERT(!ciokomb_string_ends_with("", ".ext"), "ends with error");
  CIOKOMB_ASSERT(ciokomb_string_ends_with("file.ext", ".ext"), "ends with error");
  CIOKOMB_ASSERT(ciokomb_string_ends_with("file.ext", "file.ext"), "ends with error");
  CIOKOMB_ASSERT(!ciokomb_string_ends_with("file.ext", "file.ext1"), "ends with error");
  CIOKOMB_ASSERT(ciokomb_string_to_int64("1") == 1, "to int64 error");
  CIOKOMB_ASSERT(ciokomb_string_to_int64("-1") == -1, "to int64 error");
  CIOKOMB_ASSERT(ciokomb_string_to_double("1.5") == 1.5, "to double error");
  CIOKOMB_ASSERT(ciokomb_string_to_double("-1.5") == -1.5, "to double error");
  CiokombString* string = ciokomb_string_new();
  CIOKOMB_ASSERT(string != null, "string=%p", string);
  ASSERT_STRING(string, "", 0);
  test_int32(string);
  test_int64(string);
  test_float(string);
  test_double(string);
  test_string(string);
  test_escape();
  test_yaml_line();
  ciokomb_string_delete(string);
  return EXIT_SUCCESS;
}
