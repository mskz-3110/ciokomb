#ifndef __CIOKOMB_H__
#define __CIOKOMB_H__
#include <ciokomb/string.h>
#include <ciokomb/arist.h>

#define CIOKOMB_DPRINTF(_format, ...) ciokomb_dprintf(__FILE__, __LINE__, _format, __VA_ARGS__)
#define CIOKOMB_ASSERT(_expression, _format, ...) ciokomb_assert(__FILE__, __LINE__, _expression, _format, __VA_ARGS__)

CIOKOMB_API_BEGIN
  CIOKOMB_API_INC(void, ciokomb_vdprintf, String file, int32 line, String format, va_list list)
  CIOKOMB_API_INC(void, ciokomb_dprintf, String file, int32 line, String format, ...)
  CIOKOMB_API_INC(void, ciokomb_assert, String file, int32 line, bool expression, String format, ...)
CIOKOMB_API_END
#endif
