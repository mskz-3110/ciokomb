#include <ciokomb.h>
#ifdef CIOKOMB_WINDOWS
  #include <fcntl.h>
  #include <io.h>
#else
  #include <unistd.h>
#endif

CIOKOMB_API_SRC(CiokombPipe*, ciokomb_pipe_open){
  CiokombPipe* pipe = malloc(sizeof(CiokombPipe));
  if (pipe == null) return null;
#ifdef CIOKOMB_WINDOWS
  int32 result = _pipe(pipe->Fds, 65536, O_BINARY);
#else
  int32 result = pipe(pipe->Fds);
#endif
  if (result != 0){
    pipe->Fds[0] = pipe->Fds[1] = -1;
  }
  return pipe;
}

CIOKOMB_API_SRC(void, ciokomb_pipe_close, CiokombPipe* pipe){
  if (pipe == null) return;
#ifdef CIOKOMB_WINDOWS
  if (0 <= pipe->Fds[0]) _close(pipe->Fds[0]);
  if (0 <= pipe->Fds[1]) _close(pipe->Fds[1]);
#else
  if (0 <= pipe->Fds[0]) close(pipe->Fds[0]);
  if (0 <= pipe->Fds[1]) close(pipe->Fds[1]);
#endif
  free(pipe);
}

CIOKOMB_API_SRC(int32, ciokomb_pipe_write, CiokombPipe* pipe, void* bytes, int32 size){
#ifdef CIOKOMB_WINDOWS
  return _write(pipe->Fds[1], bytes, size);
#else
  return write(pipe->Fds[1], bytes, size);
#endif
}

CIOKOMB_API_SRC(int32, ciokomb_pipe_read, CiokombPipe* pipe, void* bytes, int32 size){
#ifdef CIOKOMB_WINDOWS
  return _read(pipe->Fds[0], bytes, size);
#else
  return read(pipe->Fds[0], bytes, size);
#endif
}
