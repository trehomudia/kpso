
#ifndef MUTEX_H
#define MUTEX_H


//=================// INCLUDES //================//

#ifdef _WIN32
  #define _WINSOCKAPI_

  #ifndef _WIN32_WINNT              // Allow use of features specific to Windows XP or later.
  #define _WIN32_WINNT 0x0501       // Change this to the appropriate value to target other versions of Windows.
  #endif

  #include "windows.h"
#endif

#ifndef _WIN32
  #include <pthread.h>
  #include <errno.h>
#endif

//====================================================//

// Макрос, определяющий, поведение библиотеки при неудачной
// попытке СОЗДАНИЯ ресурса. Если макрос определен, то 
// неудачные попытки создания вызывают assert().
#define MUTEX_TREAT_UNSUCCESS_AS_ASSERT

//====================================================//

//====================================================//

#define MYTEX_TRY_ERROR         (-1)
#define MUTEX_TRY_LOCK_OK       0
#define MUTEX_TRY_LOCK_BUSY     1

//=================//	MACRO	//====================//

#ifdef _WIN32
  typedef HANDLE MUTEX;
  typedef HANDLE PS_COND_VAR;
#endif

#ifndef _WIN32
  typedef pthread_mutex_t MUTEX;
  typedef pthread_cond_t PS_COND_VAR;
#endif

//=================//	PROTOTYPES	//================//

#ifdef __cplusplus
extern "C" {
#endif
//==========================================// MUTEX //==========================================//
int InitMutex(MUTEX* pMutex);       // result: -1 - ошибка, 0 - успех
int LockMutex(MUTEX* pMutex);       // result: -1 - ошибка, 0 - успех
int UnlockMutex(MUTEX* pMutex);     // result: -1 - ошибка, 0 - успех
int DestroyMutex(MUTEX* pMutex);    // result: -1 - ошибка, 0 - успех

// попытка захватить мьютекс
int TryLockMutex(MUTEX* pMutex);    // result: -1 - ошибка (MYTEX_TRY_ERROR), 0 - мьютекс захвачен (MUTEX_TRY_LOCK_OK),
                  // 1 - мьютекс уже кем-то занят (MUTEX_TRY_LOCK_BUSY)
//===============================================================================================//

//================================// PSEUDO CONDITION VARIABLE //================================//

int PsCondVarInit(PS_COND_VAR* pCondVar);                   // result: -1 - ошибка, 0 - успех
int PsCondVarDestroy(PS_COND_VAR* pCondVar);                // result: -1 - ошибка, 0 - успех
int PsCondVarWait(PS_COND_VAR* pCondVar, MUTEX* pMutex);    // result: -1 - ошибка, 0 - успех
int PsCondVarTimedWait(PS_COND_VAR* pCondVar, MUTEX* pMutex, unsigned int timeMilSec); // result: 0 - event, -1 - critical, -2 - timeout
int PsCondVarWakeUpAll(PS_COND_VAR* pCondVar);              // result: -1 - ошибка, 0 - успех
//===============================================================================================//


#ifdef __cplusplus
}
#endif
//===============================================================================================//

#endif


