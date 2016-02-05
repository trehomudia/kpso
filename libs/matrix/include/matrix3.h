/* МАКРОСЫ */
#ifndef MATRIX3_H
#define MATRIX3_H
// Размеры для матрицы и массива

#define SET_MAT_SIZE(m, n) (sizeof(MAT) + \
                           (m*n*sizeof(MatrixReal)) + \
                           (m*sizeof(MatrixReal *)) + \
                           sizeof(MatrixReal)       + \
                           (((m*sizeof(MatrixReal*)) % sizeof(MatrixReal)) ? sizeof(MatrixReal) - ((m*sizeof(MatrixReal *)) % sizeof(MatrixReal)) : 0 ))
#define SET_VEC_SIZE(size) (sizeof(VEC) + size*sizeof(MatrixReal) + sizeof(MatrixReal))

#define SET_PERM_SIZE(size) (sizeof(PERM) + size*sizeof(unsigned int) + sizeof(MatrixReal) + ((size%2)?sizeof(unsigned int):0))


// ********************************************************

/* ПРОТОТИПЫ */
#ifdef __cplusplus
extern "C" 
{
#endif
// функции распаковки
int m_unpack(MAT **mat, MatrixReal *buffer, int m, int n);
int v_unpack(VEC **vec, MatrixReal *buffer, int size);

// функции инициализации
void vec_get(VEC **x, void *pMem, int size);
void mat_get(MAT **A, void *pMem, int m, int n);
void perm_get(PERM	**permute, void *pMem, int size );

VEC	*vec_resize(VEC *x, unsigned int new_dim);
MAT	*mat_resize(MAT *A, unsigned int new_m, unsigned int new_n);

// функция сдвига указателя
void *pshift(void *pMem, int n, int m);

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
);

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
);

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
);

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
);

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
);

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
);

#ifdef __cplusplus
}
#endif
#endif
