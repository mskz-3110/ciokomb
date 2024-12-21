#ifndef __CIOKOMB_PIPE_H__
#define __CIOKOMB_PIPE_H__
#include <ciokomb/api.h>

typedef struct {
  int32 Fds[2];
} CiokombPipe;

CIOKOMB_API_BEGIN
  CIOKOMB_API_INC(CiokombPipe*, ciokomb_pipe_open)
  CIOKOMB_API_INC(void, ciokomb_pipe_close, CiokombPipe* pipe)
  CIOKOMB_API_INC(int32, ciokomb_pipe_write, CiokombPipe* pipe, void* bytes, int32 size)
  CIOKOMB_API_INC(int32, ciokomb_pipe_read, CiokombPipe* pipe, void* bytes, int32 size)
CIOKOMB_API_END
#endif
