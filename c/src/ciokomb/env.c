#include <ciokomb.h>

CIOKOMB_API_SRC(String, ciokomb_env_get, CiokombString* string, String name){
  ciokomb_string_clear(string);
#ifdef CIOKOMB_WINDOWS
  size_t needSize = 0;
  getenv_s(&needSize, null, 0, name);
  if (0 < needSize){
    ciokomb_string_add_string_repeat(string, "\0", (int32)needSize);
    ciokomb_string_clear(string);
    getenv_s(&needSize, string->Chars, needSize, name);
  }
#else
  ciokomb_string_add_string(string, "%s", ciokomb_string_guard(getenv(name)));
#endif
  return string->Chars;
}
