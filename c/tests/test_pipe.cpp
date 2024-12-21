#include <ciokomb.h>

int32 main(int32 argc, String argv[]){
  if (!ciokomb_init()) return EXIT_FAILURE;
  CiokombPipe* pipe = ciokomb_pipe_open();
  CIOKOMB_ASSERT(pipe != null, "pipe is null");
  char writeBytes[] = "pipe";
  char readBytes[5];
  int32 writeSize = ciokomb_pipe_write(pipe, writeBytes, sizeof(writeBytes));
  CIOKOMB_ASSERT(writeSize == 5, "writeSize=%d", writeSize);
  int32 readSize = ciokomb_pipe_read(pipe, readBytes, sizeof(readBytes));
  CIOKOMB_ASSERT(readSize == 5, "readSize=%d", readSize);
  CIOKOMB_ASSERT(memcmp(writeBytes, readBytes, 5) == 0, "writeBytes='%s' readSize='%s'", writeBytes, readSize);
  ciokomb_pipe_close(pipe);
  ciokomb_pipe_close(null);
  ciokomb_exit();
  return EXIT_SUCCESS;
}
