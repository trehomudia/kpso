#include <stdio.h>
#include "..//include//matrix.h"
#include "..//include//matrix3.h"
#include "..//include//err.h"



/* _m_copy -- copies matrix into new area
	-- out(i0:m,j0:n) <- in(i0:m,j0:n) */

MAT	*_m_copy(const MAT *in, MAT *out, unsigned int i0, unsigned int j0)
{
	unsigned int	i;

	if ( in==MNULL )
		error(E_NULL,"_m_copy");
	if ( in==out )
		return (out);
	if ( out==MNULL || out->m < in->m || out->n < in->n )
		out = mat_resize(out,in->m,in->n);

	for ( i=i0; i < in->m; i++ )
		MEM_COPY(&(in->me[i][j0]),&(out->me[i][j0]),
				(in->n - j0)*sizeof(MatrixReal));
		/* for ( j=j0; j < in->n; j++ )
			out->me[i][j] = in->me[i][j]; */

	return (out);
}

/* _v_copy -- copies vector into new area
	-- out(i0:dim) <- in(i0:dim) */
VEC	*_v_copy(const VEC *in, VEC *out, unsigned int i0)
{
	
	if ( in==VNULL )
		error(E_NULL,"_v_copy");
	if ( in==out )
		return (out);
	if ( out==VNULL || out->dim < in->dim )
		out = vec_resize(out,in->dim);

	MEM_COPY(&(in->ve[i0]),&(out->ve[i0]),(in->dim - i0)*sizeof(MatrixReal));

	return (out);
}

/* px_copy -- copies permutation 'in' to 'out'
	-- out is resized to in->size */

PERM	*px_copy(const PERM *in, PERM *out)
{
	if ( in == PNULL )
		error(E_NULL,"px_copy");
	if ( in == out )
		return out;
	if ( out == PNULL || out->size != in->size )
		out = px_resize(out,in->size);

	MEM_COPY(in->pe,out->pe,in->size*sizeof(unsigned int));

	return out;
}

/*
	The .._move() routines are for moving blocks of memory around
	within Meschach data structures and for re-arranging matrices,
	vectors etc.
*/

/* m_move -- copies selected pieces of a matrix
	-- moves the m0 x n0 submatrix with top-left cor-ordinates (i0,j0)
	   to the corresponding submatrix of out with top-left co-ordinates
	   (i1,j1)
	-- out is resized (& created) if necessary */
MAT	*m_move(const MAT *in, unsigned int i0,unsigned int j0, unsigned int m0,unsigned int n0,
		MAT *out, unsigned int i1, unsigned int j1)
{
    unsigned int		i;

    if ( ! in )
	error(E_NULL,"m_move");
    if ( (int)i0 < 0 || (int)j0 < 0 || (int)i1 < 0 || (int)j1 < 0 || (int)m0 < 0 || (int)n0 < 0 ||
	 i0+m0 > in->m || j0+n0 > in->n )
	error(E_BOUNDS,"m_move");

    if ( ! out )
	out = mat_resize(out,i1+m0,j1+n0);
    else if ( i1+m0 > out->m || j1+n0 > out->n )
	out = mat_resize(out,mat_max(out->m,i1+m0),mat_max(out->n,j1+n0));

    for ( i = 0; i < m0; i++ )
	MEM_COPY(&(in->me[i0+i][j0]),&(out->me[i1+i][j1]),
		 n0*sizeof(MatrixReal));

    return out;
}

/* v_move -- copies selected pieces of a vector
	-- moves the length dim0 subvector with initial index i0
	   to the corresponding subvector of out with initial index i1
	-- out is resized if necessary */
VEC	*v_move(const VEC *in, unsigned int i0, unsigned int dim0,
		VEC *out, unsigned int i1)
{
    if ( ! in )
	error(E_NULL,"v_move");
    if ( (int)i0 < 0 || (int)dim0 < 0 || (int)i1 < 0 ||
	 i0+dim0 > in->dim )
	error(E_BOUNDS,"v_move");

    if ( (! out) || i1+dim0 > out->dim )
	out = vec_resize(out,i1+dim0);

    MEM_COPY(&(in->ve[i0]),&(out->ve[i1]),dim0*sizeof(MatrixReal));

    return out;
}

/* mv_move -- copies selected piece of matrix to a vector
	-- moves the m0 x n0 submatrix with top-left co-ordinate (i0,j0) to
	   the subvector with initial index i1 (and length m0*n0)
	-- rows are copied contiguously
	-- out is resized if necessary */
VEC	*mv_move(const MAT *in, unsigned int i0, unsigned int j0, unsigned int m0, unsigned int n0,
		 VEC *out, unsigned int i1)
{
    unsigned int		dim1, i;

    if ( ! in )
	error(E_NULL,"mv_move");
    if ( (int)i0 < 0 || (int)j0 < 0 || (int)m0 < 0 || (int)n0 < 0 || (int)i1 < 0 ||
	 i0+m0 > in->m || j0+n0 > in->n )
	error(E_BOUNDS,"mv_move");

    dim1 = m0*n0;
    if ( (! out) || i1+dim1 > out->dim )
	out = vec_resize(out,i1+dim1);

    for ( i = 0; i < m0; i++ )
	MEM_COPY(&(in->me[i0+i][j0]),&(out->ve[i1+i*n0]),n0*sizeof(MatrixReal));

    return out;
}

/* vm_move -- copies selected piece of vector to a matrix
	
-- moves the subvector with initial index i0 and length m1*n1 to
	   the m1 x n1 submatrix with top-left co-ordinate (i1,j1)
        -- copying is done by rows
	-- out is resized if necessary */
MAT	*vm_move(const VEC *in, unsigned int i0,
		 MAT *out, unsigned int i1, unsigned int j1, unsigned int m1, unsigned int n1)
{
    unsigned int		/*dim0,*/ i;

    if ( ! in )
	error(E_NULL,"vm_move");
    if ( (int)i0 < 0 || (int)i1 < 0 || (int)j1 < 0 || (int)m1 < 0 || (int)n1 < 0 ||
	 i0+m1*n1 > in->dim )
	error(E_BOUNDS,"vm_move");

    if ( ! out )
	out = mat_resize(out,i1+m1,j1+n1);
    else
	out = mat_resize(out,mat_max(i1+m1,out->m),mat_max(j1+n1,out->n));

    /*dim0 = m1*n1;*/
    for ( i = 0; i < m1; i++ )
	MEM_COPY(&(in->ve[i0+i*n1]),&(out->me[i1+i][j1]),n1*sizeof(MatrixReal));

    return out;
}


void m_print(MAT *A)                              
{
	unsigned int i,j;

	for (i = 0; i < A->max_m; i++)
   {
	   for (j = 0; j < A->max_n; j++)
        printf ("   %15.8e", A -> me [i][j]);
      printf ("\n");
   }
}



void v_print(VEC *v)
{
   unsigned int i;
   for (i = 0; i < v->max_dim; i++)
   {
      printf ("   %15.8e", v -> ve [i]);
      printf ("\n");
   }
}
