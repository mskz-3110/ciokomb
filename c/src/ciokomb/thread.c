#include <ciokomb.h>

#ifdef CIOKOMB_WINDOWS
static CIOKOMB_API_SRC(unsigned __stdcall, __on_thread_run, void* arg){
#else
static CIOKOMB_API_SRC(void*, __on_thread_run, void* arg){
#endif
  CiokombThread* thread = arg;
  thread->OnAction(thread->Arg);
#ifdef CIOKOMB_WINDOWS
  CloseHandle(thread->Thread);
  return 0;
#else
  return null;
#endif
}

CIOKOMB_API_SRC(CiokombThread*, ciokomb_thread_run, CiokombThreadActionEvent onAction, void* arg){
  CiokombThread* thread = malloc(sizeof(CiokombThread));
  if (thread == null) return null;

  thread->OnAction = onAction;
  thread->Arg = arg;
#ifdef CIOKOMB_WINDOWS
  thread->Thread = (HANDLE)_beginthreadex(null, 0, __on_thread_run, thread, 0, null);
  if (thread->Thread != 0) return thread;
#else
  do{
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0) break;
    if (pthread_create(&(thread->Thread), &attr, __on_thread_run, thread) == 0) return thread;
  }while (0);
#endif

  free(thread);
  return null;
}

CIOKOMB_API_SRC(void, ciokomb_thread_join, CiokombThread* thread){
  if (thread == null) return;
#ifdef CIOKOMB_WINDOWS
  WaitForSingleObject(thread->Thread, INFINITE);
#else
  pthread_join(thread->Thread, null);
#endif
  free(thread);
}
