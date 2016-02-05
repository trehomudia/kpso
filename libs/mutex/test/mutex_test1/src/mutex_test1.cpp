#include <iostream>
#include "mutex/mutex.h"
#include "thread/threadlib.h"
 
int ThreadFunc( void * param )
{
  SleepMilSec( 3000 );
  int result = PsCondVarWakeUpAll( ( PS_COND_VAR* )param );
  if ( 0 == result )
  {
    std::cout << "[PASS] PsCondVarWakeUpAll" << std::endl;
  }
  else
  {
    std::cout << "[FAIL] PsCondVarWakeUpAll" << std::endl;
    return -1;
  }
  return 0;
}

int main(int argc, char *argv[])
{
  int result = 0;
  MUTEX* pMutex = new MUTEX;
  PS_COND_VAR* pCondVar = new PS_COND_VAR;
  int count = 0;
  THREAD_HANDLE* pThread = new THREAD_HANDLE;

  result = InitMutex( pMutex );
  if ( 0 == result )
  {
    std::cout << "[PASS] InitMutex" << std::endl;
  }
  else
  {
    std::cout << "[FAIL] InitMutex" << std::endl;
    count++;
  }

  result = LockMutex( pMutex );
  if ( 0 == result )
  {
    std::cout << "[PASS] TryLockMutex" << std::endl;
  }
  else
  {
    std::cout << "[FAIL] TryLockMutex" << std::endl;
    count++;
  }

  result = PsCondVarInit( pCondVar );
  if ( 0 == result )
  {
    std::cout << "[PASS] PsCondVarInit" << std::endl;
  }
  else
  {
    std::cout << "[FAIL] PsCondVarInit" << std::endl;
    count++;
  }

  CreateThreadMP( &ThreadFunc, (void *)pCondVar, pThread, SCHED_FIFO, 20);

  result = PsCondVarWait( pCondVar, pMutex );
  if ( 0 == result )
  {
    std::cout << "[PASS] PsCondVarWait" << std::endl;
  }
  else
  {
    std::cout << "[FAIL] PsCondVarWait" << std::endl;
    count++;
  }

  //result = PsCondVarTimedWait( pCondVar, pMutex, 1 );
  //if ( -2 == result )
  //{
  //  std::cout << "[PASS] PsCondVarTimedWait" << std::endl;
  //}
  //else
  //{
  //  std::cout << "[FAIL] PsCondVarTimedWait" << std::endl;
  //  count++;
  //}

  result = UnlockMutex( pMutex );
  if ( 0 == result )
  {
    std::cout << "[PASS] UnlockMutex" << std::endl;
  }
  else
  {
    std::cout << "[FAIL] UnlockMutex" << std::endl;
    count++;
  }

  result = PsCondVarDestroy( pCondVar );
  if ( 0 == result )
  {
    std::cout << "[PASS] PsCondVarDestroy" << std::endl;
  }
  else
  {
    std::cout << "[FAIL] PsCondVarDestroy" << std::endl;
    count++;
  }

  result = DestroyMutex( pMutex );
  if ( 0 == result )
  {
    std::cout << "[PASS] DestroyMutex" << std::endl;
  }
  else
  {
    std::cout << "[FAIL] DestroyMutex" << std::endl;
    count++;
  }

  delete pThread;
  delete pMutex;
  delete pCondVar;

  if ( count > 0 )
  {
    return -1;
  }
  return 0;
}