#ifndef __CIOKOMB_ENV_H__
#define __CIOKOMB_ENV_H__
#include <ciokomb/api.h>
#include <ciokomb/string.h>

CIOKOMB_API_BEGIN
  CIOKOMB_API_INC(String, ciokomb_env_get, CiokombString* string, String name)
CIOKOMB_API_END
#endif
