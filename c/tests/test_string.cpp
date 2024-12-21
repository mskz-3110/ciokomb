#include <ciokomb.h>

#define STRING_ASSERT(_string, _check_chars) string_assert(__FILE__, __LINE__, _string, _check_chars)
void string_assert(String file, int32 line, CiokombString* string, String checkChars){
  String chars = ciokomb_string_get_chars(string);
  int32 length = ciokomb_string_get_length(string);
  ciokomb_assert(file, line, length == (int32)strlen(checkChars) && ciokomb_string_equals(chars, checkChars), "length=%d chars='%s'", length, chars);
}

void test_int32(CiokombString* string){
  ciokomb_string_clear(string);
  ciokomb_string_add_int32(string, "%d:", -1);
  ciokomb_string_add_int32(string, "%d", 1);
  STRING_ASSERT(string, "-1:1");
}

void test_int64(CiokombString* string){
  ciokomb_string_clear(string);
  ciokomb_string_add_int64(string, "%lld:", -1);
  ciokomb_string_add_int64(string, "%lld", 1);
  STRING_ASSERT(string, "-1:1");
}

void test_float(CiokombString* string){
  ciokomb_string_clear(string);
  ciokomb_string_add_float(string, "%.1f:", -1.5f);
  ciokomb_string_add_float(string, "%.1f", 1.5f);
  STRING_ASSERT(string, "-1.5:1.5");
}

void test_double(CiokombString* string){
  ciokomb_string_clear(string);
  ciokomb_string_add_double(string, "%.1f:", -1.5f);
  ciokomb_string_add_double(string, "%.1f", 1.5f);
  STRING_ASSERT(string, "-1.5:1.5");
}

void test_string(CiokombString* string){
  ciokomb_string_clear(string);
  ciokomb_string_add_string(string, "%s:", "abcde");
  ciokomb_string_add_string(string, "%s", "あいうえお");
  STRING_ASSERT(string, "abcde:あいうえお");
  ciokomb_string_clear(string);
  ciokomb_string_add_string_repeat(string, "ABC", 3);
  STRING_ASSERT(string, "ABCABCABC");
}

void test_escape(){
  char encodeFixedString[32];
  CiokombString encodeString = ciokomb_string_fixed(encodeFixedString, sizeof(encodeFixedString));
  char decodeFixedString[32];
  CiokombString decodeString = ciokomb_string_fixed(decodeFixedString, sizeof(decodeFixedString));
  ciokomb_string_escape_encode(&encodeString, "1\\2\'3\"4\t5\n6", 11);
  STRING_ASSERT(&encodeString, "1\\\\2\\\'3\\\"4\\t5\\n6");
  ciokomb_string_escape_decode(&decodeString, ciokomb_string_get_chars(&encodeString), ciokomb_string_get_length(&encodeString));
  STRING_ASSERT(&decodeString, "1\\2\'3\"4\t5\n6");
}

#define YAML_LINE_ASSERT(_yaml_line, _check_key, _check_value) yaml_line_assert(__FILE__, __LINE__, _yaml_line, _check_key, _check_value)
void yaml_line_assert(String file, int32 line, String yamlLine, String checkKey, String checkValue){
  char keyFixedString[32];
  CiokombString keyString = ciokomb_string_fixed(keyFixedString, sizeof(keyFixedString));
  char valueFixedString[32];
  CiokombString valueString = ciokomb_string_fixed(valueFixedString, sizeof(valueFixedString));
  ciokomb_assert(file, line, ciokomb_string_yaml_line_split(&keyString, &valueString, "Key: Value"), "yaml line split error");
  string_assert(file, line, &keyString, "Key");
  string_assert(file, line, &valueString, "Value");
}

void test_yaml_line(){
  YAML_LINE_ASSERT("Key: Value", "Key", "Value");
  YAML_LINE_ASSERT("Japanese: あいうえお", "Japanese", "あいうえお");
  YAML_LINE_ASSERT("Empty: ''", "Empty", "");
  YAML_LINE_ASSERT("Escape: 1\\\\2\\\'3\\\"4\\t5\\n6", "Escape", "1\\2\'3\"4\t5\n6");
}

int32 main(int32 argc, String argv[]){
  if (!ciokomb_init()) return EXIT_FAILURE;
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
  STRING_ASSERT(string, "");
  test_int32(string);
  test_int64(string);
  test_float(string);
  test_double(string);
  test_string(string);
  test_escape();
  test_yaml_line();
  ciokomb_string_delete(string);
  ciokomb_string_delete(null);
  ciokomb_exit();
  return EXIT_SUCCESS;
}
