/*
		Files for matrix computations

	Givens operations file. Contains routines for calculating and
	applying givens rotations for/to vectors and also to matrices by
	row and by column.
*/


#include	<stdio.h>
#include	<math.h>
#include "..//include//matrix.h"
#include "..//include//matrix2.h"
#include "..//include//err.h"

/* givens -- returns c,s parameters for Givens rotation to
		eliminate y in the vector [ x y ]' */
void	givens(double x, double y, MatrixReal *c, MatrixReal *s)
{
	MatrixReal	norm;

	norm = (MatrixReal)sqrt(x*x+y*y);
	if ( norm == 0.0 )
	{	*c = 1.0;	*s = 0.0;	}	/* identity */
	else
	{	*c = x/norm;	*s = y/norm;	}
}

/* rot_vec -- apply Givens rotation to x's i & k components */
VEC	*rot_vec(const VEC *x, unsigned int i,unsigned int k, double c, double s,
		 VEC *out)
{
	MatrixReal	temp;

	if ( x==VNULL )
		error(E_NULL,"rot_vec");
	if ( i >= x->dim || k >= x->dim )
		error(E_RANGE,"rot_vec");
	out = v_copy(x,out);

	/* temp = c*out->ve[i] + s*out->ve[k]; */
	temp = c*v_entry(out,i) + s*v_entry(out,k);
	/* out->ve[k] = -s*out->ve[i] + c*out->ve[k]; */
	v_set_val(out,k,-s*v_entry(out,i)+c*v_entry(out,k));
	/* out->ve[i] = temp; */
	v_set_val(out,i,temp);

	return (out);
}

/* rot_rows -- premultiply mat by givens rotation described by c,s */
MAT	*rot_rows(const MAT *mat, unsigned int i, unsigned int k,
		  double c, double s, MAT *out)
{
	unsigned int	j;
	MatrixReal	temp;

	if ( mat==(MAT *)NULL )
		error(E_NULL,"rot_rows");
	if ( i >= mat->m || k >= mat->m )
		error(E_RANGE,"rot_rows");
	if ( mat != out )
		out = m_copy(mat,m_resize(out,mat->m,mat->n));

	for ( j=0; j<mat->n; j++ )
	{
		/* temp = c*out->me[i][j] + s*out->me[k][j]; */
		temp = c*m_entry(out,i,j) + s*m_entry(out,k,j);
		/* out->me[k][j] = -s*out->me[i][j] + c*out->me[k][j]; */
		m_set_val(out,k,j, -s*m_entry(out,i,j) + c*m_entry(out,k,j));
		/* out->me[i][j] = temp; */
		m_set_val(out,i,j, temp);
	}

	return (out);
}

/* rot_cols -- postmultiply mat by givens rotation described by c,s */
MAT	*rot_cols(const MAT *mat,unsigned int i,unsigned int k,
		  double c, double s, MAT *out)
{
	unsigned int	j;
	MatrixReal	temp;

	if ( mat==(MAT *)NULL )
		error(E_NULL,"rot_cols");
	if ( i >= mat->n || k >= mat->n )
		error(E_RANGE,"rot_cols");
	if ( mat != out )
		out = m_copy(mat,m_resize(out,mat->m,mat->n));

	for ( j=0; j<mat->m; j++ )
	{
		/* temp = c*out->me[j][i] + s*out->me[j][k]; */
		temp = c*m_entry(out,j,i) + s*m_entry(out,j,k);
		/* out->me[j][k] = -s*out->me[j][i] + c*out->me[j][k]; */
		m_set_val(out,j,k, -s*m_entry(out,j,i) + c*m_entry(out,j,k));
		/* out->me[j][i] = temp; */
		m_set_val(out,j,i,temp);
	}

	return (out);
}

