
/*
Matrix factorisation routines to work with the other matrix files.
*/

#include	<stdio.h>
#include	<math.h>
#include "mutex/mutex.h"
#include "..//include//matrix.h"
#include "..//include//matrix2.h"
#include "..//include//matrix3.h"
#include "..//include//meminfo.h"
#include "..//include//err.h"

//Глобальная переменная
//Мьютекс
#ifndef _WIN32
extern MUTEX matrixMutex;
#endif

//char MatrixTempBuffer[ 5 * 1000 ];

/* Most matrix factorisation routines are in-situ unless otherwise specified */

/* LUfactor -- gaussian elimination with scaled partial pivoting
-- Note: returns LU matrix which is A */
MAT	*LUfactor(MAT *A, PERM *pivot)
{
  unsigned int	i, j, m, n;
  unsigned int	k, k_max;
  int i_max;
  char MatrixTempBuffer[ 1000 ];
  MatrixReal	**A_v, *A_piv, *A_row;
  MatrixReal	max1, temp, tiny;
  VEC	*scale = VNULL;

  if ( A==(MAT *)NULL || pivot==(PERM *)NULL ){
    error(E_NULL,"LUfactor");
  }
  if ( pivot->size != A->m )
    error(E_SIZES,"LUfactor");
  m = A->m;	n = A->n;

  if( SET_VEC_SIZE( A->m ) <1000 )
    vec_get( &scale, (void *)MatrixTempBuffer, A->m );
  else
    scale = v_get( A->m );

  //MEM_STAT_REG(scale,TYPE_VEC);
  A_v = A->me;

  tiny = (MatrixReal)(10.0/HUGE_VAL);

  /* initialise pivot with identity permutation */
  for ( i=0; i<m; i++ )
    pivot->pe[i] = i;

  /* set scale parameters */
  for ( i=0; i<m; i++ )
  {
    max1 = 0.0;
    for ( j=0; j<n; j++ )
    {
      temp = (MatrixReal)fabs(A_v[i][j]);
      max1 = mat_max(max1,temp);
    }
    scale->ve[i] = max1;
  }

  /* main loop */
  k_max = mat_min(m,n)-1;
  for ( k=0; k<k_max; k++ )
  {
    /* find best pivot row */
    max1 = 0.0;	i_max = -1;
    for ( i=k; i<m; i++ )
      if ( fabs(scale->ve[i]) >= tiny*fabs(A_v[i][k]) )
      {
        temp = (MatrixReal)fabs(A_v[i][k])/scale->ve[i];
        if ( temp > max1 )
        { max1 = temp;	i_max = i;	}
      }

      /* if no pivot then ignore column k... */
      if ( i_max == -1 )
      {
        /* set pivot entry A[k][k] exactly to zero,
        rather than just "small" */
        A_v[k][k] = 0.0;
        continue;
      }

      /* do we pivot ? */
      if ( i_max != (int)k )	/* yes we do... */
      {
        px_transp(pivot,i_max,k);
        for ( j=0; j<n; j++ )
        {
          temp = A_v[i_max][j];
          A_v[i_max][j] = A_v[k][j];
          A_v[k][j] = temp;
        }
      }

      /* row operations */
      for ( i=k+1; i<m; i++ )	/* for each row do... */
      {	/* Note: divide by zero should never happen */
        temp = A_v[i][k] = A_v[i][k]/A_v[k][k];
        A_piv = &(A_v[k][k+1]);
        A_row = &(A_v[i][k+1]);
        if ( k+1 < n )
          __mltadd__(A_row,A_piv,-temp,(int)(n-(k+1)));
      }

  }

  if( scale != (VEC *)MatrixTempBuffer ) // память выделялась, надо освободить
    V_FREE(scale);

  return A;
}


/* LUsolve -- given an LU factorisation in A, solve Ax=b */

VEC	*LUsolve(const MAT *LU, PERM *pivot, const VEC *b, VEC *x)
{
  if ( ! LU || ! b || ! pivot )
    error(E_NULL,"LUsolve");
  if ( LU->m != LU->n || LU->n != b->dim )
    error(E_SIZES,"LUsolve");

  if( x->max_dim < b->dim )
    x = v_resize( x, b->dim );
  else
    x = vec_resize( x, b->dim );
  px_vec(pivot,b,x);	/* x := P.b */
  Lsolve(LU,x,x,1.0);	/* implicit diagonal = 1 */
  Usolve(LU,x,x,0.0);	/* explicit diagonal */

  return (x);
}


/* m_inverse -- returns inverse of A, provided A is not too rank deficient
-- uses LU factorisation */

MAT	*m_inverse(const MAT *A, MAT *out)
{
  unsigned int	i;
  char MatrixTempBuffer[ 4000 ];
  VEC	*tmp = VNULL, *tmp2 = VNULL;
  MAT	*A_cp = MNULL;
  PERM	*pivot = PNULL;

  if ( ! A )
    error(E_NULL,"m_inverse");
  if ( A->m != A->n )
    error(E_SQUARE,"m_inverse");
  if ( ! out || out->m < A->m || out->n < A->n )
    out = m_resize(out,A->m,A->n);

  if( SET_MAT_SIZE( A->m, A->n ) < 1000 )
    mat_get( &A_cp, (void *)( MatrixTempBuffer + 2000 ), A->m, A->n );
  else
    A_cp = matrix_get( A->m, A->n  );

  A_cp = m_copy( A, A_cp );
  if( SET_VEC_SIZE( A->m ) < 1000 ) {
    vec_get( &tmp, (void *)MatrixTempBuffer, A->m );
    vec_get( &tmp2, (void *)(MatrixTempBuffer + 1000), A->m );
  } else {
    tmp   = v_get( A->m );
    tmp2  = v_get( A->m );
  }

  if( SET_PERM_SIZE( A->m ) < 1000 ) {
    perm_get( &pivot, (void *)( MatrixTempBuffer + 3000 ), A->m );
  } else {
    pivot   = px_get( A->m );
  }

  LUfactor(A_cp,pivot);
  //tracecatch_matrix(LUfactor(A_cp,pivot),"m_inverse");
  for ( i = 0; i < A->n; i++ ){
    v_zero(tmp);
    tmp->ve[i] = 1.0;
    LUsolve(A_cp,pivot,tmp,tmp2);
    //tracecatch_matrix(LUsolve(A_cp,pivot,tmp,tmp2),"m_inverse");
    set_col(out,i,tmp2);
  }
  if( tmp != (VEC *)(MatrixTempBuffer ) ) // память выделялась, надо освободить
    V_FREE(tmp);
  if( tmp2 != (VEC *)(MatrixTempBuffer + 1000) ) // память выделялась, надо освободить
    V_FREE(tmp2);
  if( A_cp != (MAT *)(MatrixTempBuffer + 2000) ) // память выделялась, надо освободить
    M_FREE(A_cp);
  if( pivot != (PERM *)(MatrixTempBuffer + 3000) ) // память выделялась, надо освободить
    PX_FREE( pivot );
  return out;
}
