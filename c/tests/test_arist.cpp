#include <ciokomb.h>

static bool g_Verbose = false;

typedef struct {
  int32 Value;
} Element;

#define ARIST_ASSERT(_arist, _check_count) arist_assert(__FILE__, __LINE__, _arist, _check_count)
void arist_assert(String file, int32 line, CiokombArist* arist, int32 checkCount){
  int32 count = ciokomb_arist_get_count(arist);
  ciokomb_assert(file, line, count == checkCount, "count=%d", count);
}

#define ARIST_LOCK_ASSERT(_arist, _check_index) arist_lock_assert(__FILE__, __LINE__, _arist, _check_index)
void arist_lock_assert(String file, int32 line, CiokombArist* arist, int32 checkIndex){
  int32 index = ciokomb_arist_lock(arist);
  ciokomb_assert(file, line, index == checkIndex, "index=%d", index);
}

#define ELEMENT_ASSERT(_element, _check_value) element_assert(__FILE__, __LINE__, _element, _check_value)
void element_assert(String file, int32 line, Element* element, int32 checkValue){
  ciokomb_assert(file, line, element != null, "element is null");
  ciokomb_assert(file, line, element->Value == checkValue, "value=%d", element->Value);
}

#define ARIST_DUMP(_arist, _format, ...) arist_dump(__FILE__, __LINE__, _arist, _format, ## __VA_ARGS__)
void arist_dump(String file, int32 line, CiokombArist* arist, String format, ...){
  if (g_Verbose){
    va_list list;
    va_start(list, format);
    ciokomb_vdprintf(file, line, format, list);
    va_end(list);
  }

  for (int32 i = ciokomb_arist_each_root_index(arist); i != -1; i = ciokomb_arist_each_next_index(arist, i)){
    Element* element = (Element*)ciokomb_arist_get_element(arist, i);
    CIOKOMB_ASSERT(element != null, "element is null");
    if (g_Verbose) printf("[%02d] Value=%d\n", i, element->Value);
  }
}

int32 main(int32 argc, String argv[]){
  if (!ciokomb_init()) return EXIT_FAILURE;
  g_Verbose = ciokomb_string_to_int64(ciokomb_string_guard(getenv("VERBOSE"))) != 0;

  CiokombArist* arist = ciokomb_arist_new(sizeof(Element));
  ARIST_ASSERT(arist, 0);
  for (int32 i = 0; i < 10; ++i){
    ARIST_LOCK_ASSERT(arist, i);
    ARIST_ASSERT(arist, i + 1);
    Element* element = (Element*)ciokomb_arist_get_element(arist, i);
    CIOKOMB_ASSERT(element != null, "element is null");
    element->Value = i;
  }
  ARIST_DUMP(arist, "10(+10)");

  int32 index = 5;
  ciokomb_arist_unlock(arist, index);
  ARIST_ASSERT(arist, 9);
  ARIST_DUMP(arist, "9(-1)");

  ARIST_LOCK_ASSERT(arist, index);
  ARIST_ASSERT(arist, 10);
  ELEMENT_ASSERT((Element*)ciokomb_arist_get_element(arist, index), index);
  ARIST_DUMP(arist, "10(+1)");

  ciokomb_arist_unlock(arist, 0);
  ciokomb_arist_unlock(arist, 9);
  ciokomb_arist_unlock(arist, 6);
  ciokomb_arist_unlock(arist, 3);
  ciokomb_arist_unlock(arist, 1);
  ARIST_ASSERT(arist, 5);
  ARIST_DUMP(arist, "5(-5)");

  CIOKOMB_ASSERT(ciokomb_arist_get_element(arist, -1) == null, "element is not null");
  CIOKOMB_ASSERT(ciokomb_arist_get_element(arist, 0) == null, "element is not null");
  CIOKOMB_ASSERT(ciokomb_arist_get_element(arist, 1) == null, "element is not null");
  ELEMENT_ASSERT((Element*)ciokomb_arist_get_element(arist, 2), 2);
  CIOKOMB_ASSERT(ciokomb_arist_get_element(arist, 3) == null, "element is not null");
  ELEMENT_ASSERT((Element*)ciokomb_arist_get_element(arist, 4), 4);
  ELEMENT_ASSERT((Element*)ciokomb_arist_get_element(arist, 5), 5);
  CIOKOMB_ASSERT(ciokomb_arist_get_element(arist, 6) == null, "element is not null");
  ELEMENT_ASSERT((Element*)ciokomb_arist_get_element(arist, 7), 7);
  ELEMENT_ASSERT((Element*)ciokomb_arist_get_element(arist, 8), 8);
  CIOKOMB_ASSERT(ciokomb_arist_get_element(arist, 9) == null, "element is not null");
  CIOKOMB_ASSERT(ciokomb_arist_get_element(arist, 16) == null, "element is not null");

  ciokomb_arist_delete(arist);
  ciokomb_arist_delete(null);
  ciokomb_exit();
  return EXIT_SUCCESS;
}
