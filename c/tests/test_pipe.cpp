#include <ciokomb.h>

int32 main(int32 argc, String argv[]){
  if (!ciokomb_init()) return EXIT_FAILURE;
  CiokombPipeIO* pipeIO = ciokomb_pipe_open();
  CIOKOMB_ASSERT(pipeIO != null, "pipe is null");
  char writeBytes[] = "pipe";
  char readBytes[5];
  int32 writeSize = ciokomb_pipe_write(pipeIO, writeBytes, sizeof(writeBytes));
  CIOKOMB_ASSERT(writeSize == 5, "writeSize=%d", writeSize);
  int32 readSize = ciokomb_pipe_read(pipeIO, readBytes, sizeof(readBytes));
  CIOKOMB_ASSERT(readSize == 5, "readSize=%d", readSize);
  CIOKOMB_ASSERT(memcmp(writeBytes, readBytes, 5) == 0, "writeBytes='%s' readSize='%s'", writeBytes, readSize);
  ciokomb_pipe_close(pipeIO);
  ciokomb_pipe_close(null);
  ciokomb_exit();
  return EXIT_SUCCESS;
}
