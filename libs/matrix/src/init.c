
/*
	This is a file of routines for zero-ing, and initialising
	vectors, matrices and permutations.
	This is to be included in the matrix.a library
*/


#include	<stdio.h>
#include "mutex/mutex.h"
#include "..//include//matrix.h"
#include "..//include//err.h"

//Глобальная переменная
//Мьютекс
#ifndef _WIN32
MUTEX         matrixMutex;
#endif
//Флаг инициализации
unsigned int  initMatrix = 0;

//Функция инициализации матричной библиотеки
int InitMatrix()
{
  int result = 0;

  if(!initMatrix){
#ifndef _WIN32
    result = InitMutex(&matrixMutex);
#endif
    initMatrix = 1;
  }

  return result;
}

//Функция уничтожения матричного мьютекса
int DestroyMatrix()
{
  int result = 0;

  if(initMatrix){
#ifndef _WIN32
    result = DestroyMutex(&matrixMutex);
#endif
    initMatrix = 0;
  }

  return result;
}

/* v_zero -- zero the vector x */
VEC	*v_zero(VEC *x)
{
	if ( x == VNULL )
		error(E_NULL,"v_zero");

	__zero__(x->ve,x->dim);

	return x;
}


/* iv_zero -- zero the vector ix */
IVEC	*iv_zero(IVEC *ix)
{
   unsigned int i;

   if ( ix == IVNULL )
     error(E_NULL,"iv_zero");

   for ( i = 0; i < ix->dim; i++ )
     ix->ive[i] = 0;

   return ix;
}


/* m_zero -- zero the matrix A */
MAT	*m_zero(MAT *A)
{
	int	i, A_m, A_n;
	MatrixReal	**A_me;

	if ( A == MNULL )
		error(E_NULL,"m_zero");

	A_m = A->m;	A_n = A->n;	A_me = A->me;
	for ( i = 0; i < A_m; i++ )
		__zero__(A_me[i],A_n);
		/* for ( j = 0; j < A_n; j++ )
			A_me[i][j] = 0.0; */

	return A;
}

/* mat_id -- set A to being closest to identity matrix as possible
	-- i.e. A[i][j] == 1 if i == j and 0 otherwise */
MAT	*m_ident(MAT *A)
{
	int	i, size;

	if ( A == MNULL )
		error(E_NULL,"m_ident");

	m_zero(A);
	size = mat_min(A->m,A->n);
	for ( i = 0; i < size; i++ )
		A->me[i][i] = 1.0;

	return A;
}

/* px_ident -- set px to identity permutation */
PERM	*px_ident(PERM *px)
{
	int	i, px_size;
	unsigned int	*px_pe;

	if ( px == PNULL )
		error(E_NULL,"px_ident");

	px_size = px->size;	px_pe = px->pe;
	for ( i = 0; i < px_size; i++ )
		px_pe[i] = i;

	return px;
}

/* Pseudo random number generator data structures */
/* Knuth's lagged Fibonacci-based generator: See "Seminumerical Algorithms:
   The Art of Computer Programming" sections 3.2-3.3 */


#ifndef LONG_MAX
#include	<limits.h>
#endif


#ifdef LONG_MAX
#define MODULUS	LONG_MAX
#else
#define MODULUS	1000000000L	/* assuming long's at least 32 bits long */
#endif
#define MZ	0L

static long mrand_list[56];
static int  started = FALSE;
static int  inext = 0, inextp = 31;


/* mrand -- pseudo-random number generator */
double mrand(void)
{
    long	lval;
    static MatrixReal  factor = 1.0/((MatrixReal)MODULUS);

    if ( ! started )
	smrand(3127);

    inext = (inext >= 54) ? 0 : inext+1;
    inextp = (inextp >= 54) ? 0 : inextp+1;

    lval = mrand_list[inext]-mrand_list[inextp];
    if ( lval < 0L )
	lval += MODULUS;
    mrand_list[inext] = lval;

    return (double)lval*factor;
}

/* mrandlist -- fills the array a[] with len random numbers */

void	mrandlist(MatrixReal a[], int len)
{
    int		i;
    long	lval;
    static MatrixReal  factor = 1.0/((MatrixReal)MODULUS);

    if ( ! started )
	smrand(3127);

    for ( i = 0; i < len; i++ )
    {
	inext = (inext >= 54) ? 0 : inext+1;
	inextp = (inextp >= 54) ? 0 : inextp+1;

	lval = mrand_list[inext]-mrand_list[inextp];
	if ( lval < 0L )
	    lval += MODULUS;
	mrand_list[inext] = lval;

	a[i] = (MatrixReal)lval*factor;
    }
}


/* smrand -- set seed for mrand() */
void smrand(int seed)
{
    int		i;

    mrand_list[0] = (123413*seed) % MODULUS;
    for ( i = 1; i < 55; i++ )
	mrand_list[i] = (123413*mrand_list[i-1]) % MODULUS;

    started = TRUE;

    /* run mrand() through the list sufficient times to
       thoroughly randomise the array */
    for ( i = 0; i < 55*55; i++ )
	mrand();
}
#undef MODULUS
#undef MZ
#undef FAC

/* v_rand -- initialises x to be a random vector, components
	independently & uniformly ditributed between 0 and 1 */

VEC	*v_rand(VEC *x)
{
	if ( ! x )
		error(E_NULL,"v_rand");

	mrandlist(x->ve,x->dim);

	return x;
}

/* m_rand -- initialises A to be a random vector, components
	independently & uniformly distributed between 0 and 1 */

MAT	*m_rand(MAT *A)
{
	unsigned int	i /* , j */;

	if ( ! A )
		error(E_NULL,"m_rand");

	for ( i = 0; i < A->m; i++ )

	    mrandlist(A->me[i],A->n);

	return A;
}

/* v_ones -- fills x with one's */

VEC	*v_ones(VEC *x)
{
	unsigned int	i;

	if ( ! x )
		error(E_NULL,"v_ones");

	for ( i = 0; i < x->dim; i++ )
		x->ve[i] = 1.0;

	return x;
}

/* m_ones -- fills matrix with one's */

MAT	*m_ones(MAT *A)
{
	unsigned int	i, j;

	if ( ! A )
		error(E_NULL,"m_ones");

	for ( i = 0; i < A->m; i++ )
		for ( j = 0; j < A->n; j++ )
		    A->me[i][j] = 1.0;

	return A;
}

/* v_count -- initialises x so that x->ve[i] == i */

VEC	*v_count(VEC *x)
{
	unsigned int	i;

	if ( ! x )
	    error(E_NULL,"v_count");

	for ( i = 0; i < x->dim; i++ )
	    x->ve[i] = (MatrixReal)i;

	return x;
}
