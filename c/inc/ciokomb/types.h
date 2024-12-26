#ifndef __CIOKOMB_TYPES_H__
#define __CIOKOMB_TYPES_H__
#if defined(_WIN32) || defined(_WIN64)
  #include <windows.h>
  #define _CRTDBG_MAP_ALLOC
  #include <crtdbg.h>
  #define CIOKOMB_WINDOWS
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>

#ifndef int8
  #define int8 int8_t
#endif
#ifndef uint8
  #define uint8 uint8_t
#endif
#ifndef int16
  #define int16 int16_t
#endif
#ifndef uint16
  #define uint16 uint16_t
#endif
#ifndef int32
  #define int32 int32_t
#endif
#ifndef uint32
  #define uint32 uint32_t
#endif
#ifndef int64
  #define int64 int64_t
#endif
#ifndef uint64
  #define uint64 uint64_t
#endif
#ifndef String
  #define String const char*
#endif
#endif
