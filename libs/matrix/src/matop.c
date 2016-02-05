
#include	<stdio.h>
#include "..//include//matrix.h"
#include "..//include//err.h"


/* m_add -- matrix addition -- may be in-situ */
MAT	*m_add(const MAT *mat1, const MAT *mat2, MAT *out)
{
	unsigned int	m,n,i;

	if ( mat1==(MAT *)NULL || mat2==(MAT *)NULL )
		error(E_NULL,"m_add");
	if ( mat1->m != mat2->m || mat1->n != mat2->n )
		error(E_SIZES,"m_add");
	if ( out==(MAT *)NULL || out->m != mat1->m || out->n != mat1->n )
		out = m_resize(out,mat1->m,mat1->n);
	m = mat1->m;	n = mat1->n;
	for ( i=0; i<m; i++ )
	{
		__add__(mat1->me[i],mat2->me[i],out->me[i],(int)n);
	}

	return (out);
}

/* m_sub -- matrix subtraction -- may be in-situ */
MAT	*m_sub(const MAT *mat1, const MAT *mat2, MAT *out)
{
	unsigned int	m,n,i;

	if ( mat1==(MAT *)NULL || mat2==(MAT *)NULL )
		error(E_NULL,"m_sub");
	if ( mat1->m != mat2->m || mat1->n != mat2->n )
		error(E_SIZES,"m_sub");
	if ( out==(MAT *)NULL || out->m != mat1->m || out->n != mat1->n )
		out = m_resize(out,mat1->m,mat1->n);
	m = mat1->m;	n = mat1->n;
	for ( i=0; i<m; i++ )
	{
		__sub__(mat1->me[i],mat2->me[i],out->me[i],(int)n);
	}

	return (out);
}

/* m_mlt -- matrix-matrix multiplication */

MAT	*m_mlt(const MAT *A, const MAT *B, MAT *OUT)
{
	unsigned int	i, /* j, */ k, m, n, p;
	MatrixReal	**A_v, **B_v /*, *B_row, *OUT_row, sum, tmp */;

	if ( A==(MAT *)NULL || B==(MAT *)NULL )
		error(E_NULL,"m_mlt");
	if ( A->n != B->m )
		error(E_SIZES,"m_mlt");
	if ( A == OUT || B == OUT )
		error(E_INSITU,"m_mlt");
	m = A->m;	n = A->n;	p = B->n;
	A_v = A->me;		B_v = B->me;

	if ( OUT==(MAT *)NULL || OUT->m != A->m || OUT->n != B->n )
		OUT = m_resize(OUT,A->m,B->n);

	m_zero(OUT);
	for ( i=0; i<m; i++ )
		for ( k=0; k<n; k++ )
		{
		    if ( A_v[i][k] != 0.0 )
		        __mltadd__(OUT->me[i],B_v[k],A_v[i][k],(int)p);
		}

	return OUT;
}

/* mmtr_mlt -- matrix-matrix transposed multiplication
	-- A.B^T is returned, and stored in OUT */

MAT	*mmtr_mlt(const MAT *A, const MAT *B, MAT *OUT)
{
	unsigned int	i, j, limit;
	
	if ( ! A || ! B )
		error(E_NULL,"mmtr_mlt");
	if ( A == OUT || B == OUT )
		error(E_INSITU,"mmtr_mlt");
	if ( A->n != B->n )
		error(E_SIZES,"mmtr_mlt");
	if ( ! OUT || OUT->m != A->m || OUT->n != B->m )
		OUT = m_resize(OUT,A->m,B->m);

	limit = A->n;
	for ( i = 0; i < A->m; i++ )
		for ( j = 0; j < B->m; j++ )
		{
		    OUT->me[i][j] = __ip__(A->me[i],B->me[j],(int)limit);
		}

	return OUT;
}

/* mtrm_mlt -- matrix transposed-matrix multiplication
	-- A^T.B is returned, result stored in OUT */
MAT	*mtrm_mlt(const MAT *A, const MAT *B, MAT *OUT)
{
	unsigned int	i, k, limit;
	
	if ( ! A || ! B )
		error(E_NULL,"mmtr_mlt");
	if ( A == OUT || B == OUT )
		error(E_INSITU,"mtrm_mlt");
	if ( A->m != B->m )
		error(E_SIZES,"mmtr_mlt");
	if ( ! OUT || OUT->m != A->n || OUT->n != B->n )
		OUT = m_resize(OUT,A->n,B->n);

	limit = B->n;
	m_zero(OUT);
	for ( k = 0; k < A->m; k++ )
		for ( i = 0; i < A->n; i++ )
		{
		    if ( A->me[k][i] != 0.0 )
			__mltadd__(OUT->me[i],B->me[k],A->me[k][i],(int)limit);
		}

	return OUT;
}

/* mv_mlt -- matrix-vector multiplication 
		-- Note: b is treated as a column vector */
VEC	*mv_mlt(const MAT *A, const VEC *b, VEC *out)
{
	unsigned int	i, m, n;
	MatrixReal	**A_v, *b_v;
	

	if ( A==(MAT *)NULL || b==(VEC *)NULL )
		error(E_NULL,"mv_mlt");
	if ( A->n != b->dim )
		error(E_SIZES,"mv_mlt");
	if ( b == out )
		error(E_INSITU,"mv_mlt");
	if ( out == (VEC *)NULL || out->dim != A->m )
		out = v_resize(out,A->m);

	m = A->m;		n = A->n;
	A_v = A->me;		b_v = b->ve;
	for ( i=0; i<m; i++ )
	{

		out->ve[i] = __ip__(A_v[i],b_v,(int)n);
	}

	return out;
}

/* sm_mlt -- scalar-matrix multiply -- may be in-situ */

MAT	*sm_mlt(double scalar, const MAT *matrix, MAT *out)
{
	unsigned int	m,n,i;

	if ( matrix==(MAT *)NULL )
		error(E_NULL,"sm_mlt");
	if ( out==(MAT *)NULL || out->m != matrix->m || out->n != matrix->n )
		out = m_resize(out,matrix->m,matrix->n);
	m = matrix->m;	n = matrix->n;
	for ( i=0; i<m; i++ )
		__smlt__(matrix->me[i],(double)scalar,out->me[i],(int)n);
	return (out);
}

/* vm_mlt -- vector-matrix multiplication 
		-- Note: b is treated as a row vector */

VEC	*vm_mlt(const MAT *A, const VEC *b, VEC *out)
{
	unsigned int	j,m,n;
	
	if ( A==(MAT *)NULL || b==(VEC *)NULL )
		error(E_NULL,"vm_mlt");
	if ( A->m != b->dim )
		error(E_SIZES,"vm_mlt");
	if ( b == out )
		error(E_INSITU,"vm_mlt");
	if ( out == (VEC *)NULL || out->dim != A->n )
		out = v_resize(out,A->n);

	m = A->m;		n = A->n;

	v_zero(out);
	for ( j = 0; j < m; j++ )
		if ( b->ve[j] != 0.0 )
		    __mltadd__(out->ve,A->me[j],b->ve[j],(int)n);
	return out;
}

/* m_transp -- transpose matrix */

MAT	*m_transp(const MAT *in, MAT *out)
{
	unsigned int	i, j;
	int	in_situ;
	MatrixReal	tmp;

	if ( in == (MAT *)NULL )
		error(E_NULL,"m_transp");
	if ( in == out && in->n != in->m )
		error(E_INSITU2,"m_transp");
	in_situ = ( in == out );
	if ( out == (MAT *)NULL || out->m != in->n || out->n != in->m )
		out = m_resize(out,in->n,in->m);

	if ( ! in_situ )
		for ( i = 0; i < in->m; i++ )
			for ( j = 0; j < in->n; j++ )
				out->me[j][i] = in->me[i][j];
	else
		for ( i = 1; i < in->m; i++ )
			for ( j = 0; j < i; j++ )
			{	tmp = in->me[i][j];
				in->me[i][j] = in->me[j][i];
				in->me[j][i] = tmp;
			}

	return out;
}

/* swap_rows -- swaps rows i and j of matrix A for cols lo through hi */

MAT	*swap_rows(MAT *A, unsigned int i, unsigned int j, unsigned int lo, unsigned int hi)
{
	unsigned int	k;
	MatrixReal	**A_me, tmp;

	if ( ! A )
		error(E_NULL,"swap_rows");
	if ( (int)i < 0 || (int)j < 0 || i >= A->m || j >= A->m )
		error(E_SIZES,"swap_rows");
	lo = mat_max(0,(int)lo);
	hi = mat_min(hi,A->n-1);
	A_me = A->me;

	for ( k = lo; k <= hi; k++ )
	{
		tmp = A_me[k][i];
		A_me[k][i] = A_me[k][j];
		A_me[k][j] = tmp;
	}
	return A;
}

/* swap_cols -- swap columns i and j of matrix A for cols lo through hi */

MAT	*swap_cols(MAT *A, unsigned int i, unsigned int j, unsigned int lo, unsigned int hi)
{
	unsigned int	k;
	MatrixReal	**A_me, tmp;

	if ( ! A )
		error(E_NULL,"swap_cols");
	if ( (int)i < 0 || (int)j < 0 || i >= A->n || j >= A->n )
		error(E_SIZES,"swap_cols");
	lo = mat_max(0,(int)lo);
	hi = mat_min(hi,A->m-1);
	A_me = A->me;

	for ( k = lo; k <= hi; k++ )
	{
		tmp = A_me[i][k];
		A_me[i][k] = A_me[j][k];
		A_me[j][k] = tmp;
	}
	return A;
}

/* ms_mltadd -- matrix-scalar multiply and add
	-- may be in situ
	-- returns out == A1 + s*A2 */

MAT	*ms_mltadd(const MAT *A1, const MAT *A2, double s, MAT *out)
{
	int	i, m, n;

	if ( ! A1 || ! A2 )
		error(E_NULL,"ms_mltadd");
	if ( A1->m != A2->m || A1->n != A2->n )
		error(E_SIZES,"ms_mltadd");

	if ( out != A1 && out != A2 )
		out = m_resize(out,A1->m,A1->n);

	if ( s == 0.0 )
		return m_copy(A1,out);
	if ( s == 1.0 )
		return m_add(A1,A2,out);

	tracecatch_matrix(out = m_copy(A1,out),"ms_mltadd");

	m = A1->m;	n = A1->n;
	for ( i = 0; i < m; i++ )
	{
		__mltadd__(out->me[i],A2->me[i],s,(int)n);
	}

	return out;
}

/* mv_mltadd -- matrix-vector multiply and add
	-- may not be in situ
	-- returns out == v1 + alpha*A*v2 */

VEC	*mv_mltadd(const VEC *v1, const VEC *v2, const MAT *A,
		   double alpha, VEC *out)
{
	int	i, m, n;
	MatrixReal	*v2_ve, *out_ve;

	if ( ! v1 || ! v2 || ! A )
		error(E_NULL,"mv_mltadd");
	if ( out == v2 )
		error(E_INSITU,"mv_mltadd");
	if ( v1->dim != A->m || v2->dim != A->n )
		error(E_SIZES,"mv_mltadd");

	tracecatch_matrix(out = v_copy(v1,out),"mv_mltadd");

	v2_ve = v2->ve;	out_ve = out->ve;
	m = A->m;	n = A->n;

	if ( alpha == 0.0 )
	    return out;

	for ( i = 0; i < m; i++ )
	{
		out_ve[i] += alpha*__ip__(A->me[i],v2_ve,(int)n);
	}

	return out;
}

/* vm_mltadd -- vector-matrix multiply and add
	-- may not be in situ
	-- returns out' == v1' + v2'*A */
VEC	*vm_mltadd(const VEC *v1, const VEC *v2, const MAT *A,
		   double alpha, VEC *out)
{
	int	j, m, n;
	MatrixReal	tmp, *out_ve;

	if ( ! v1 || ! v2 || ! A )
		error(E_NULL,"vm_mltadd");
	if ( v2 == out )
		error(E_INSITU,"vm_mltadd");
	if ( v1->dim != A->n || A->m != v2->dim )
		error(E_SIZES,"vm_mltadd");

	tracecatch_matrix(out = v_copy(v1,out),"vm_mltadd");

	out_ve = out->ve;	m = A->m;	n = A->n;
	for ( j = 0; j < m; j++ )
	{
		tmp = v2->ve[j]*alpha;
		if ( tmp != 0.0 )
		    __mltadd__(out_ve,A->me[j],tmp,(int)n);
	}

	return out;
}

