
#include <assert.h>


#ifndef _WIN32
// Для PsCondVarTimedWait():
#include <time.h>
#endif
#include "..//include//mutex.h"

#ifdef __QNX__
#include <stdio.h>
#endif


//===============================================================================================//

#ifdef MUTEX_TREAT_UNSUCCESS_AS_ASSERT
#define MUTEX_UNSUCCESS_ASSERT(expr) assert(expr)
#else
#define MUTEX_UNSUCCESS_ASSERT(expr) ((void)0)
#endif

//===============================================================================================//
//==========================================// MUTEX //==========================================//
//===============================================================================================//

//========================================// InitMutex //========================================//
int InitMutex(MUTEX* pMutex)
{
#ifdef __QNX__
  int result;
#endif
  assert(pMutex);
  if (0 == pMutex)
  {
    return -1;
  }

#if (defined _WIN32)
  *pMutex = CreateMutex(0, 0, 0);
  if (0 == (*pMutex))
  {
    MUTEX_UNSUCCESS_ASSERT(0);
    return -1;
  }
#elif (defined __QNX__)
  result = pthread_mutex_init(pMutex, 0);
  if (0 != result)
  {
    // обходной маневр для QNX: переоткрываем мьютекс
    if (EBUSY == result)
    {
      printf("WARNING: Mutex BUSY\n");
      result = pthread_mutex_destroy(pMutex);
      if (0 != result)
      {
        assert(0);
        return -1;
      }

      result = pthread_mutex_init(pMutex, 0);
      if (0 != result)
      {
        MUTEX_UNSUCCESS_ASSERT(0);
        return -1;
      }
    }
    else
    {
      MUTEX_UNSUCCESS_ASSERT(0);
      return -1;
    }
  }
#else
  if (0 != pthread_mutex_init(pMutex, 0))
  {
    MUTEX_UNSUCCESS_ASSERT(0);
    return -1;
  }
#endif

  return 0;
}
//===============================================================================================//

//======================================//  DestroyMutex //======================================//
int DestroyMutex(MUTEX* pMutex)
{
  int result;

  assert(pMutex);
  if (0 == pMutex)
  {
    return -1;
  }

#ifdef _WIN32
  assert(*pMutex);
  if (0 == (*pMutex))
  {
    return -1;
  }

  result = CloseHandle(*pMutex);
  if (0 == result)
  {
    assert(0);
    return -1;
  }
#else
  result = pthread_mutex_destroy(pMutex);
  if (0 != result)
  {
    assert(0);
    return -1;
  }
#endif

  return 0;

}
//===============================================================================================//

//========================================// LockMutex //========================================//
int LockMutex(MUTEX* pMutex)
{
#ifdef _WIN32
  DWORD result;
#else
  int result;
#endif

  assert(pMutex);
  if (0 == pMutex)
  {
    return -1;
  }

#ifdef _WIN32
  assert(*pMutex);
  if (0 == (*pMutex))
  {
    return -1;
  }

  result = WaitForSingleObject(*pMutex, INFINITE);      // lock mutex
  if (WAIT_OBJECT_0 != result)
  {
    assert(0);
    return -1;
  }
#else
  result = pthread_mutex_lock(pMutex);
  if (0 != result)
  {
    assert(0);
    return -1;
  }
#endif

  return 0;
}
//===============================================================================================//

//=======================================// UnlockMutex //=======================================//
int UnlockMutex(MUTEX* pMutex)
{
  int result;

  assert(pMutex);
  if (0 == pMutex)
  {
    return -1;
  }

#ifdef _WIN32
  assert(*pMutex);
  if (0 == (*pMutex))
  {
    return -1;
  }

  result = ReleaseMutex(*pMutex);       // unlock mutex
  if (0 == result)
  {
    assert(0);
    return -1;
  }
#else
  result = pthread_mutex_unlock(pMutex);
  if (0 != result)
  {
    assert(0);
    return -1;
  }
#endif

  return 0;
}
//===============================================================================================//

//======================================//  TryLockMutex //======================================//
// попытка захватить мьютекс
// result: -1 - ошибка (MYTEX_TRY_ERROR), 0 - мьютекс захвачен (MUTEX_TRY_LOCK_OK),
// 1 - мьютекс уже кем-то занят (MUTEX_TRY_LOCK_BUSY)
int TryLockMutex(MUTEX* pMutex)
{
#ifdef _WIN32
  DWORD result;
#else
  int result;
#endif

  assert(pMutex);
  if (0 == pMutex)
  {
    return MYTEX_TRY_ERROR;
  }

#ifdef _WIN32
  assert(*pMutex);
  if (0 == (*pMutex))
  {
    return MYTEX_TRY_ERROR;
  }

  result = WaitForSingleObject(*pMutex, 0);
  switch (result)
  {
  case WAIT_OBJECT_0:
    return MUTEX_TRY_LOCK_OK;       // мьютекс успешно захвачен

  case WAIT_TIMEOUT:
    return MUTEX_TRY_LOCK_BUSY;     // мьютекс не может быть захвачен, так как он был захвачен ранее

  default:
    assert(0);
    return MYTEX_TRY_ERROR;         // ошибка при захвате мьютекса
  }
#else
  result = pthread_mutex_trylock(pMutex);
  switch (result)
  {
  case 0:
    return MUTEX_TRY_LOCK_OK;       // мьютекс успешно захвачен

  case EBUSY:
    return MUTEX_TRY_LOCK_BUSY;     // мьютекс не может быть захвачен, так как он был захвачен ранее

  default:
    assert(0);
    return MYTEX_TRY_ERROR;         // ошибка при захвате мьютекса
  }
#endif
}
//===============================================================================================//



//===============================================================================================//
//================================// PSEUDO CONDITION VARIABLE //================================//
//===============================================================================================//

//======================================// PsCondVarInit //======================================//
int PsCondVarInit(PS_COND_VAR* pCondVar)
{
  assert(pCondVar);
  if (0 == pCondVar)
  {
    return -1;
  }

#ifdef _WIN32
  *pCondVar = CreateEvent(0, TRUE, 0, 0);
  if (0 == (*pCondVar))
  {
    MUTEX_UNSUCCESS_ASSERT(0);
    return -1;
  }
#else
  if (0 != pthread_cond_init(pCondVar, 0))
  {
    MUTEX_UNSUCCESS_ASSERT(0);
    return -1;
  }
#endif

  return 0;
}
//===============================================================================================//

//====================================//  PsCondVarDestroy //====================================//
int PsCondVarDestroy(PS_COND_VAR* pCondVar)
{
  int result;

  assert(pCondVar);
  if (0 == pCondVar)
  {
    return -1;
  }

#ifdef _WIN32
  assert(*pCondVar);
  if (0 == (*pCondVar))
  {
    return -1;
  }

  result = CloseHandle(*pCondVar);
  if (0 == result)
  {
    assert(0);
    return -1;
  }
#else
  result = pthread_cond_destroy(pCondVar);
  if (0 != result)
  {
    assert(0);
    return -1;
  }
#endif

  return 0;
}
//===============================================================================================//

//======================================// PsCondVarWait //======================================//
int PsCondVarWait(PS_COND_VAR* pCondVar, MUTEX* pMutex)
{
  int result;
#ifdef _WIN32
  DWORD waitResult;
#endif

  assert(pCondVar);
  assert(pMutex);
  if ((0 == pMutex) || (0 == pCondVar))
  {
    return -1;
  }

#ifdef _WIN32
  assert(*pCondVar);
  assert(*pMutex);
  if ((0 == (*pCondVar)) || (0 == (*pMutex)))
  {
    return -1;
  }

  waitResult = SignalObjectAndWait(*pMutex, *pCondVar, INFINITE, 0);
  if (waitResult != WAIT_OBJECT_0)
  {
    assert(0);
    return -1;
  }

  result = LockMutex(pMutex);
  if (0 != result)
  {
    assert(0);
    return -1;
  }
#else
  result = pthread_cond_wait(pCondVar, pMutex);
  if (0 != result)
  {
    assert(0);
    return -1;
  }
#endif

  return 0;
}
//===============================================================================================//

//===================================// PsCondVarTimedWait //====================================//
// result: 0 - event, -1 - critical, -2 - timeout
int PsCondVarTimedWait(PS_COND_VAR* pCondVar, MUTEX* pMutex, unsigned int timeMilSec)
{
  int result, retRes;
#ifdef _WIN32
  DWORD waitResult;
#else
  struct timespec absTime;
#endif

  assert(pCondVar);
  assert(pMutex);
  if ((0 == pMutex) || (0 == pCondVar))
  {
    return -1;
  }

#ifdef _WIN32
  assert(*pCondVar);
  assert(*pMutex);
  if ((0 == (*pCondVar)) || (0 == (*pMutex)))
  {
    return -1;
  }

  waitResult = SignalObjectAndWait(*pMutex, *pCondVar, timeMilSec, 0);
  switch (waitResult)
  {
  case WAIT_OBJECT_0:
    retRes = 0;
    break;

  case WAIT_TIMEOUT:
    retRes = -2;
    break;

  default:
    assert(0);
    return -1;
  }

  result = LockMutex(pMutex);
  if (0 != result)
  {
    assert(0);
    return -1;
  }
#else
  result = clock_gettime(CLOCK_REALTIME, &absTime);
  if (0 != result)
  {
    assert(0);
    return -1;
  }
  absTime.tv_nsec += (timeMilSec % 1000) * 1000000;
  absTime.tv_sec += (timeMilSec / 1000) + (absTime.tv_nsec / 1000000000);
  absTime.tv_nsec = absTime.tv_nsec % 1000000000;

  result = pthread_cond_timedwait(pCondVar, pMutex, &absTime);
  switch (result)
  {
  case 0:
    retRes = 0;
    break;

  case ETIMEDOUT:
    retRes = -2;
    break;

  default:
    assert(0);
    return -1;
  }
#endif

  return retRes;
}
//===============================================================================================//

//===================================//  PsCondVarWakeUpAll //===================================//
int PsCondVarWakeUpAll(PS_COND_VAR* pCondVar)
{
  int result;

  assert(pCondVar);
  if (0 == pCondVar)
  {
    return -1;
  }

#ifdef _WIN32
  assert(*pCondVar);
  if (0 == (*pCondVar))
  {
    return -1;
  }

  result = SetEvent(*pCondVar);
  if (0 == result)
  {
    assert(0);
    return -1;
  }

  if (0 == ResetEvent(*pCondVar))
  {
    assert(0);
    return -1;
  }
#else
  result = pthread_cond_broadcast(pCondVar);
  if (0 != result)
  {
    assert(0);
    return -1;
  }
#endif

  return 0;
}


//===============================================================================================//
