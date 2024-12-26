#ifndef __CIOKOMB_THREAD_H__
#define __CIOKOMB_THREAD_H__
#include <ciokomb/api.h>
#ifdef CIOKOMB_WINDOWS
  #include <process.h>
#else
  #include <pthread.h>
#endif

CIOKOMB_CALLBACK(void, CiokombThreadActionEvent, void* arg)

CIOKOMB_STRUCT_BEGIN
  CiokombThreadActionEvent OnAction;
  void*                    Arg;
#ifdef CIOKOMB_WINDOWS
  HANDLE                   Thread;
#else
  pthread_t                Thread;
#endif
CIOKOMB_STRUCT_END(CiokombThread)

CIOKOMB_API_BEGIN
  CIOKOMB_API_INC(CiokombThread*, ciokomb_thread_run, CiokombThreadActionEvent onAction, void* arg)
  CIOKOMB_API_INC(void, ciokomb_thread_join, CiokombThread* thread)
CIOKOMB_API_END
#endif
