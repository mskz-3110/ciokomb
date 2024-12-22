#ifndef __CIOKOMB_API_H__
#define __CIOKOMB_API_H__
#include <ciokomb/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef CIOKOMB_WINDOWS
  #define __CIOKOMB_API_PREFIX __declspec(dllexport)
#else
  #define __CIOKOMB_API_PREFIX
#endif
#define CIOKOMB_API_SRC(_return_type, _name, ...) _return_type _name(__VA_ARGS__)
#define CIOKOMB_API_INC(_return_type, _name, ...) __CIOKOMB_API_PREFIX CIOKOMB_API_SRC(_return_type, _name, ## __VA_ARGS__);

#ifdef __cplusplus
  #define CIOKOMB_API_BEGIN extern "C" {
  #define CIOKOMB_API_END   }
#else
  #define CIOKOMB_API_BEGIN
  #define CIOKOMB_API_END
#endif

#ifndef null
  #define null NULL
#endif

#ifndef memzero
  #define memzero(_addr, _size) memset(_addr, 0, _size)
#endif
#endif
