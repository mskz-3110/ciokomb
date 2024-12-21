#include <ciokomb.h>

static bool g_Initialized = false;

CIOKOMB_API_SRC(bool, ciokomb_init){
#ifdef CIOKOMB_WINDOWS
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
#endif
  g_Initialized = true;
  return true;
}

CIOKOMB_API_SRC(void, ciokomb_exit){
  if (!g_Initialized) return;
#ifdef CIOKOMB_WINDOWS
  _CrtDumpMemoryLeaks();
#endif
  g_Initialized = false;
}

CIOKOMB_API_SRC(void, ciokomb_vdprintf, String file, int32 line, String format, va_list list){
  char fixedString[1024];
  CiokombString string = ciokomb_string_fixed(fixedString, sizeof(fixedString));
  ciokomb_string_add_string(&string, "[%s:", file);
  ciokomb_string_add_int32(&string, "%d] ", line);
  ciokomb_string_vprintf(&string, format, list);
  printf("%s\n", fixedString);
}

CIOKOMB_API_SRC(void, ciokomb_dprintf, String file, int32 line, String format, ...){
  va_list list;
  va_start(list, format);
  ciokomb_vdprintf(file, line, format, list);
  va_end(list);
}

CIOKOMB_API_SRC(void, ciokomb_assert, String file, int32 line, bool expression, String format, ...){
  if (expression) return;

  va_list list;
  va_start(list, format);
  printf("\033[40m\033[31m");
  ciokomb_vdprintf(file, line, format, list);
  printf("\033[0m");
  va_end(list);
  exit(EXIT_FAILURE);
}
