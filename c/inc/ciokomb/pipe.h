#ifndef __CIOKOMB_PIPE_H__
#define __CIOKOMB_PIPE_H__
#include <ciokomb/api.h>

typedef struct {
  int32 Fds[2];
} CiokombPipeIO;

CIOKOMB_API_BEGIN
  CIOKOMB_API_INC(CiokombPipeIO*, ciokomb_pipe_open)
  CIOKOMB_API_INC(void, ciokomb_pipe_close, CiokombPipeIO* pipeIO)
  CIOKOMB_API_INC(int32, ciokomb_pipe_write, CiokombPipeIO* pipeIO, void* bytes, int32 size)
  CIOKOMB_API_INC(int32, ciokomb_pipe_read, CiokombPipeIO* pipeIO, void* bytes, int32 size)
CIOKOMB_API_END
#endif
