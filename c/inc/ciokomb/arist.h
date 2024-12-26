#ifndef __CIOKOMB_ARIST_H__
#define __CIOKOMB_ARIST_H__
#include <ciokomb/api.h>

CIOKOMB_STRUCT_BEGIN
  char* Bytes;
  int32 Count;
  int32 RootIndex;
  int32 __EmptyIndex;
  int32 __ElementSize;
  int32 __AllocatedSize;
CIOKOMB_STRUCT_END(CiokombArist)

CIOKOMB_API_BEGIN
  CIOKOMB_API_INC(CiokombArist*, ciokomb_arist_new, int32 elementSize)
  CIOKOMB_API_INC(void, ciokomb_arist_delete, CiokombArist* arist)
  CIOKOMB_API_INC(int32, ciokomb_arist_get_count, CiokombArist* arist)
  CIOKOMB_API_INC(void*, ciokomb_arist_get_element, CiokombArist* arist, int32 index)
  CIOKOMB_API_INC(int32, ciokomb_arist_each_root_index, CiokombArist* arist)
  CIOKOMB_API_INC(int32, ciokomb_arist_each_next_index, CiokombArist* arist, int32 index)
  CIOKOMB_API_INC(int32, ciokomb_arist_lock, CiokombArist* arist)
  CIOKOMB_API_INC(void, ciokomb_arist_unlock, CiokombArist* arist, int32 index)
CIOKOMB_API_END
#endif
