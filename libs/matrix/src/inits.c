#include "..//include//matrix.h"
#include "..//include//matrix3.h"
#include "..//include//err.h"

#define ALIGN_ADD( buf_t, asize ) ( ( (size_t)buf_t % asize ) ? ( asize - ((size_t)buf_t % asize) ) : 0 )

// новая функция инициализации вектора
void vec_get(VEC **x, void *pMem, int size)
{
	char *pMemData = 0;
	
  if (size < 0)
    return;

	//pMem = calloc(1, SET_VEC_SIZE(size));

  if (pMem == NULL )
  {
      error1(E_MEM,"vec_get");
      return;
  }
/*  else if (mem_info_is_on()) {
      mem_bytes(TYPE_VEC,0,sizeof(VEC));
      mem_numvar(TYPE_VEC,1);
  }
  */
  //Очищаем память
  memset(pMem, 0, SET_VEC_SIZE(size));
	// память для структуры
	*x = (VEC*)pMem;
	// память для чисел вектора
  //Выравниваем указатель под данные
  pMemData = (char*)(pMem) + sizeof(VEC);
  pMemData += ( ( size_t )pMemData ) % sizeof( MatrixReal ) ? ( sizeof( MatrixReal ) - ( ( size_t )pMemData ) % sizeof( MatrixReal ) ) : 0;
  (*x)->ve = (MatrixReal*)pMemData;
/*
	if (mem_info_is_on()) {
    mem_bytes(TYPE_VEC,0,size*sizeof(MatrixReal));
  }
  */
	// заполнене вспомогательных полей
	(*x)->dim = (*x)->max_dim = size;
}

//
void perm_get(PERM	**permute, void *pMem, int size )
{
  /*PERM	*permute;*/
  int	i;

  if (size < 0)
    error1(E_NEG,"px_get");

  if (pMem == NULL )
  {
      error1(E_MEM,"perm_get");
      return;
  }

  /*if ((permute=NEW(PERM)) == (PERM *)NULL )
  error(E_MEM,"px_get");
  else if (mem_info_is_on()) {
  mem_bytes(TYPE_PERM,0,sizeof(PERM));
  mem_numvar(TYPE_PERM,1);
  }*/
  //Очищаем память
  memset( pMem, 0, SET_PERM_SIZE( size ) );
  *permute = (PERM *)pMem;
  (* permute)->pe = (unsigned int *)((char *)pMem + SET_PERM_SIZE( size ));
  (* permute)->max_size = size;
  (* permute)->size = size;

  /*permute->size = permute->max_size = size;
  if ((permute->pe = NEW_A(size,unsigned int)) == (unsigned int *)NULL )
    error(E_MEM,"px_get");
  else if (mem_info_is_on()) {
    mem_bytes(TYPE_PERM,0,size*sizeof(unsigned int));
  }*/

  for ( i=0; i<size; i++ )
    (*permute)->pe[i] = i;

  /*return (permute);*/
}


// -------------------------------------------------------------
// новая функция инициализации матрицы
void mat_get(MAT **A, void *pMem, int m, int n)
{
	int  i = 0;
  char *pMemData = 0;

  if (m < 0 || n < 0)
    return;

	if ( pMem == NULL )
    return;

  //Очищаем память
  memset(pMem, 0, SET_MAT_SIZE(m, n));
  
	// память для структуры
	*A = (MAT*)pMem;
	
  // память для чисел
  //Выравниваем указатель под данные
  pMemData = (char*)(pMem) + sizeof(MAT);
  pMemData += ( ( size_t )pMemData ) % sizeof( MatrixReal ) ? ( sizeof( MatrixReal ) - ( ( size_t )pMemData ) % sizeof( MatrixReal ) ) : 0;
  
  (*A)->base = (MatrixReal*)pMemData;
	// память для указателей
  (*A)->me = (MatrixReal**)(pMemData + m*n*sizeof(MatrixReal));
	
	// установка указателей
	for ( i=0; i<m; i++ )
    (*A)->me[i] = &((*A)->base[i*n]);

  // заполнение вспомогательных полей матрицы
  (*A)->m = m;
  (*A)->n = (*A)->max_n = n;
  (*A)->max_m = m;
  (*A)->max_size = m*n;
}

//! @brief функция размещения структуры VEC на выделенной памяти с вычислением смещения
//! @details vec_get_off( &pVEC, 6, pBuf, sizeBuf, &offset );
//! расположение в памяти: pBuf + offset_in ->| VEC |align_MatrixReal| VEC.ve[ dim ] |<- pBuf + offset_out
//! @return код ошибки < 0, 0 = OK.
int vec_get_off(
    VEC ** const ppVec,       //! [out] указатель на возвращаемую структуру вектора
    const unsigned int dim,   //! [in]  размерность вектора
    char * const pBuf,        //! [in]  указатель на выделенную память
    const size_t size,        //! [in]  размер выделенной памяти
    size_t * const pOffset    //! [in/out] смещение по памяти
)
{
  if( ppVec == NULL || pBuf == NULL || pOffset == NULL ) {
    return -1;
  }

  if( *pOffset + SET_VEC_SIZE( dim ) > size ) {
    return -2;
  }

  memset( pBuf + *pOffset, 0, SET_VEC_SIZE( dim ) );

  (*ppVec)          = (VEC *)( pBuf + *pOffset );
  (*ppVec)->dim     = dim;
  (*ppVec)->max_dim = dim;

  (*ppVec)->ve  = (MatrixReal *)( pBuf + *pOffset + sizeof( VEC ) + ALIGN_ADD(pBuf, sizeof( MatrixReal )) );

  *pOffset += SET_VEC_SIZE( dim );

  return 0;
}

//! @brief функция проверки размещения структуры VEC на выделенной памяти с вычислением смещения
//! @details vec_chk_off( &pVEC, 6, pBuf, sizeBuf, &offset );
//! расположение в памяти: pBuf + offset_in ->| VEC |align_MatrixReal| VEC.ve[ dim ] |<- pBuf + offset_out
//! @return код ошибки < 0, 0 = OK, 1 = ошибка размещения в памяти.
int vec_chk_off(
    const VEC * pVec,       //! [in]  указатель на возвращаемую структуру вектора
    const unsigned int dim,   //! [in]  размерность вектора
    char * const pBuf,        //! [in]  указатель на выделенную память
    const size_t size,        //! [in]  размер выделенной памяти
    size_t * const pOffset    //! [in/out] смещение по памяти
)
{
  int res = 0;
  if( pVec == NULL || pBuf == NULL || pOffset == NULL ) {
    return -1;
  }

  if( *pOffset + SET_VEC_SIZE( dim ) > size ) {
    return -2;
  }

  res = 0;
  if( pVec != (VEC *)( pBuf + *pOffset ) ) {
    res = 1;
  } else if( pVec->ve != (MatrixReal *)( pBuf + *pOffset + sizeof( VEC ) + ALIGN_ADD(pBuf, sizeof( MatrixReal )) ) ) {
    res = 1;
  }

  if( res == 0 )
    *pOffset += SET_VEC_SIZE( dim );

  return res;
}

//! @brief функция размещения структуры MAT на выделенной памяти с вычислением смещения
//! @details mat_get_off( &pMAT, 6, 6, pBuf, sizeBuf, &offset );
//! расположение в памяти: pBuf + offset_in ->| MAT |align_MatrixReal| MAT.base[ m*n ] | MAT.me[ m ] |<- pBuf + offset_out
//! @return код ошибки < 0, 0 = OK.
int mat_get_off(
    MAT ** const ppMat,       //! [out] указатель на возвращаемую структуру матрицы
    const unsigned int m,     //! [in]  количество строк
    const unsigned int n,     //! [in]  количество столбцов
    char * const pBuf,        //! [in]  указатель на выделенную память
    const size_t size,        //! [in]  размер выделенной памяти
    size_t * const pOffset    //! [in/out] смещение по памяти
)
{
  unsigned int i;

  if( ppMat == NULL || pBuf == NULL || pOffset == NULL ) {
    return -1;
  }

  if( *pOffset + SET_MAT_SIZE( m, n ) > size ) {
    return -2;
  }

  memset( pBuf + *pOffset, 0, SET_MAT_SIZE( m, n ) );

  (*ppMat)          = (MAT *)( pBuf + *pOffset );
  (*ppMat)->m       = m;
  (*ppMat)->max_m   = m;
  (*ppMat)->n       = n;
  (*ppMat)->max_n   = m;
  (*ppMat)->max_size= m * n;

  (*ppMat)->base    = (MatrixReal *)( pBuf + *pOffset + sizeof( MAT ) + ALIGN_ADD(pBuf, sizeof( MatrixReal )) );

  (*ppMat)->me      = (MatrixReal **)( pBuf + *pOffset + sizeof( MAT ) + ALIGN_ADD(pBuf, sizeof( MatrixReal )) + m*n*sizeof( MatrixReal ) );

  for( i = 0; i < m; i++ ) {
    (*ppMat)->me[ i ] = &((*ppMat)->base[ i*n ]);
  }

  *pOffset += SET_MAT_SIZE( m, n );

  return 0;

}

//! @brief функция проверки размещения структуры MAT на выделенной памяти с вычислением смещения
//! @details mat_chk_off( &pMAT, 6, 6, pBuf, sizeBuf, &offset );
//! расположение в памяти: pBuf + offset_in ->| MAT |align_MatrixReal| MAT.base[ m*n ] | MAT.me[ m ] |<- pBuf + offset_out
//! @return код ошибки < 0, 0 = OK, 1 = ошибка размещения в памяти.
int mat_chk_off(
    const MAT * pMat,       //! [in]  указатель на возвращаемую структуру матрицы
    const unsigned int m,     //! [in]  количество строк
    const unsigned int n,     //! [in]  количество столбцов
    char * const pBuf,        //! [in]  указатель на выделенную память
    const size_t size,        //! [in]  размер выделенной памяти
    size_t * const pOffset    //! [in/out] смещение по памяти
)
{
  unsigned int i;
  int res = 0;

  if( pMat == NULL || pBuf == NULL || pOffset == NULL ) {
    return -1;
  }

  if( *pOffset + SET_MAT_SIZE( m, n ) > size ) {
    return -2;
  }

  res = 0;
  if( pMat != (MAT *)( pBuf + *pOffset ) ) {
    res = 1;
  } else if( pMat->base != (MatrixReal *)( pBuf + *pOffset + sizeof( MAT ) + ALIGN_ADD(pBuf, sizeof( MatrixReal )) ) ) {
    res = 1;
  } else {
    for( i = 0; i < m; i++ ) {
      if( pMat->me[ i ] != &(pMat->base[ i*n ]) ) {
        res = 1;
      }
    }
  }
  if( res == 0 )
    *pOffset += SET_MAT_SIZE( m, n );

  return res;

}

//! @brief функция размещения структуры PERM на выделенной памяти с вычислением смещения
//! @details perm_get_off( &pPERM, 6, pBuf, sizeBuf, &offset );
//! расположение в памяти: pBuf + offset_in ->| PERM | PERM.pe[ dim ] |<- pBuf + offset_out
//! @return код ошибки < 0, 0 = OK.
int perm_get_off(
    PERM ** const ppPerm,     //! [out] указатель на возвращаемую структуру вектора
    const unsigned int dim,   //! [in]  размерность вектора
    char * const pBuf,        //! [in]  указатель на выделенную память
    const size_t size,        //! [in]  размер выделенной памяти
    size_t * const pOffset    //! [in/out] смещение по памяти
)
{

  if( ppPerm == NULL || pBuf == NULL || pOffset == NULL ) {
    return -1;
  }

  if( *pOffset + SET_PERM_SIZE( dim ) > size ) {
    return -2;
  }

  memset( pBuf + *pOffset, 0, SET_PERM_SIZE( dim ) );

  (*ppPerm)           = (PERM *)( pBuf + *pOffset );
  (*ppPerm)->size     = dim;
  (*ppPerm)->max_size = dim;

  (*ppPerm)->pe  = (unsigned int *)( pBuf + *pOffset + sizeof( PERM ) );

  *pOffset += SET_PERM_SIZE( dim );

  return 0;

}

//! @brief функция проверки размещения структуры PERM на выделенной памяти с вычислением смещения
//! @details perm_chk_off( &pPERM, 6, pBuf, sizeBuf, &offset );
//! расположение в памяти: pBuf + offset_in ->| PERM | PERM.pe[ dim ] |<- pBuf + offset_out
//! @return код ошибки < 0, 0 = OK, 1 = ошибка размещения в памяти.
int perm_chk_off(
    const PERM * pPerm,     //! [in]  указатель на возвращаемую структуру вектора
    const unsigned int dim,   //! [in]  размерность вектора
    char * const pBuf,        //! [in]  указатель на выделенную память
    const size_t size,        //! [in]  размер выделенной памяти
    size_t * const pOffset    //! [in/out] смещение по памяти
)
{
  int res = 0;
  if( pPerm == NULL || pBuf == NULL || pOffset == NULL ) {
    return -1;
  }

  if( *pOffset + SET_PERM_SIZE( dim ) > size ) {
    return -2;
  }

  if( pPerm != (PERM *)( pBuf + *pOffset ) ) {
    res = 1;
  }

  if( pPerm->pe  != (unsigned int *)( pBuf + *pOffset + sizeof( PERM ) ) ) {
    res = 1;
  }

  if( res == 0 )
    *pOffset += SET_PERM_SIZE( dim );

  return res;

}


