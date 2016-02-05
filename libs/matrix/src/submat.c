
#include	<stdio.h>
#include "..//include//matrix.h"
#include "..//include//err.h"

/* get_col -- gets a specified column of a matrix and retruns it as a vector */

VEC	*get_col(const MAT *mat, unsigned int col, VEC *vec)
{
   unsigned int	i;
   
   if ( mat==(MAT *)NULL )
     error(E_NULL,"get_col");
   if ( col >= mat->n )
     error(E_RANGE,"get_col");
   if ( vec==(VEC *)NULL || vec->dim<mat->m )
     vec = v_resize(vec,mat->m);
   
   for ( i=0; i<mat->m; i++ )
     vec->ve[i] = mat->me[i][col];
   
   return (vec);
}

/* get_row -- gets a specified row of a matrix and retruns it as a vector */

VEC	*get_row(const MAT *mat, unsigned int row, VEC *vec)
{
   unsigned int	i;
   
   if ( mat==(MAT *)NULL )
     error(E_NULL,"get_row");
   if ( row >= mat->m )
     error(E_RANGE,"get_row");
   if ( vec==(VEC *)NULL || vec->dim<mat->n )
     vec = v_resize(vec,mat->n);
   
   for ( i=0; i<mat->n; i++ )
     vec->ve[i] = mat->me[row][i];
   
   return (vec);
}

/* _set_col -- sets column of matrix to values given in vec (in situ)
	-- that is, mat(i0:lim,col) <- vec(i0:lim) */
MAT	*_set_col(MAT *mat, unsigned int col, const VEC *vec, unsigned int i0)
{
   unsigned int	i,lim;
   
   if ( mat==(MAT *)NULL || vec==(VEC *)NULL )
     error(E_NULL,"_set_col");
   if ( col >= mat->n )
     error(E_RANGE,"_set_col");
   lim = mat_min(mat->m,vec->dim);
   for ( i=i0; i<lim; i++ )
     mat->me[i][col] = vec->ve[i];
   
   return (mat);
}

/* _set_row -- sets row of matrix to values given in vec (in situ) */

MAT	*_set_row(MAT *mat, unsigned int row, const VEC *vec, unsigned int j0)
{
   unsigned int	j,lim;
   
   if ( mat==(MAT *)NULL || vec==(VEC *)NULL )
     error(E_NULL,"_set_row");
   if ( row >= mat->m )
     error(E_RANGE,"_set_row");
   lim = mat_min(mat->n,vec->dim);
   for ( j=j0; j<lim; j++ )
     mat->me[row][j] = vec->ve[j];
   
   return (mat);
}

/* sub_mat -- returns sub-matrix of old which is formed by the rectangle
   from (row1,col1) to (row2,col2)
   -- Note: storage is shared so that altering the "new"
   matrix will alter the "old" matrix */

MAT	*sub_mat(const MAT *old, 
		 unsigned int row1, unsigned int col1,
		 unsigned int row2, unsigned int col2,
		 MAT *new)
{
   unsigned int	i;
   
   if ( old==(MAT *)NULL )
     error(E_NULL,"sub_mat");
   if ( row1 > row2 || col1 > col2 || row2 >= old->m || col2 >= old->n )
     error(E_RANGE,"sub_mat");
   if ( new==(MAT *)NULL || new->m < row2-row1+1 )
   {
      new = NEW(MAT);
      new->me = NEW_A(row2-row1+1,MatrixReal *);
      if ( new==(MAT *)NULL || new->me==(MatrixReal **)NULL )
	error(E_MEM,"sub_mat");
      else if (mem_info_is_on()) {
	 mem_bytes(TYPE_MAT,0,sizeof(MAT)+
		      (row2-row1+1)*sizeof(MatrixReal *));
      }
      
   }
   new->m = row2-row1+1;
   
   new->n = col2-col1+1;
   
   new->base = (MatrixReal *)NULL;
   
   for ( i=0; i < new->m; i++ )
     new->me[i] = (old->me[i+row1]) + col1;
   
   return (new);
}


/* sub_vec -- returns sub-vector which is formed by the elements i1 to i2
   -- as for sub_mat, storage is shared */

VEC	*sub_vec(const VEC *old, unsigned int i1, unsigned int i2, VEC *new)
{
   if ( old == (VEC *)NULL )
     error(E_NULL,"sub_vec");
   if ( i1 > i2 || old->dim < i2 )
     error(E_RANGE,"sub_vec");
   
   if ( new == (VEC *)NULL )
     new = NEW(VEC);
   if ( new == (VEC *)NULL )
     error(E_MEM,"sub_vec");
   else if (mem_info_is_on()) {
      mem_bytes(TYPE_VEC,0,sizeof(VEC));
   }
   
   
   new->dim = i2 - i1 + 1;
   new->ve = &(old->ve[i1]);
   
   return new;
}
