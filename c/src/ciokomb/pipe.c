#include <ciokomb.h>
#ifdef CIOKOMB_WINDOWS
  #include <fcntl.h>
  #include <io.h>
#else
  #include <unistd.h>
#endif

CIOKOMB_API_SRC(CiokombPipeIO*, ciokomb_pipe_open){
  CiokombPipeIO* pipeIO = malloc(sizeof(CiokombPipeIO));
  if (pipeIO == null) return null;
#ifdef CIOKOMB_WINDOWS
  int32 result = _pipe(pipeIO->Fds, 65536, O_BINARY);
#else
  int32 result = pipe(pipeIO->Fds);
#endif
  if (result != 0){
    pipeIO->Fds[0] = pipeIO->Fds[1] = -1;
  }
  return pipeIO;
}

CIOKOMB_API_SRC(void, ciokomb_pipe_close, CiokombPipeIO* pipeIO){
  if (pipeIO == null) return;
#ifdef CIOKOMB_WINDOWS
  if (0 <= pipeIO->Fds[0]) _close(pipeIO->Fds[0]);
  if (0 <= pipeIO->Fds[1]) _close(pipeIO->Fds[1]);
#else
  if (0 <= pipeIO->Fds[0]) close(pipeIO->Fds[0]);
  if (0 <= pipeIO->Fds[1]) close(pipeIO->Fds[1]);
#endif
  free(pipeIO);
}

CIOKOMB_API_SRC(int32, ciokomb_pipe_write, CiokombPipeIO* pipeIO, void* bytes, int32 size){
#ifdef CIOKOMB_WINDOWS
  return _write(pipeIO->Fds[1], bytes, size);
#else
  return write(pipeIO->Fds[1], bytes, (size_t)size);
#endif
}

CIOKOMB_API_SRC(int32, ciokomb_pipe_read, CiokombPipeIO* pipeIO, void* bytes, int32 size){
#ifdef CIOKOMB_WINDOWS
  return _read(pipeIO->Fds[0], bytes, size);
#else
  return read(pipeIO->Fds[0], bytes, (size_t)size);
#endif
}
