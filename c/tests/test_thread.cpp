#include <ciokomb.h>

static void on_thread(void* arg){
  int32* value = (int32*)arg;
  *value = 1;
}

int32 main(int32 argc, String argv[]){
  if (!ciokomb_init()) return EXIT_FAILURE;
  int32 value = 0;
  CiokombThread* thread = ciokomb_thread_run(on_thread, &value);
  CIOKOMB_ASSERT(thread != null, "thread is null");
  ciokomb_thread_join(thread);
  CIOKOMB_ASSERT(value == 1, "value=%d", value);
  ciokomb_exit();
  return EXIT_SUCCESS;
}
