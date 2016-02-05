/*
	A collection of functions for computing norms: scaled and unscaled
*/

#include	<stdio.h>
#include	<math.h>
#include "..//include//matrix.h"
#include "..//include//err.h"


/* _v_norm1 -- computes (scaled) 1-norms of vectors */

double	_v_norm1(const VEC *x, const VEC *scale)
{
	unsigned int	i, dim;
	double	s, sum;

	if ( x == (VEC *)NULL )
		error(E_NULL,"_v_norm1");
	dim = x->dim;

	sum = 0.0;
	if ( scale == (VEC *)NULL )
		for ( i = 0; i < dim; i++ )
			sum += fabs(x->ve[i]);
	else if ( scale->dim < dim )
		error(E_SIZES,"_v_norm1");
	else
		for ( i = 0; i < dim; i++ )
		{	s = scale->ve[i];
			sum += ( s== 0.0 ) ? fabs(x->ve[i]) : fabs(x->ve[i]/s);
		}

	return sum;
}

/* square -- returns x^2 */

double	square(double x)
{	return x*x;	}

/* cube -- returns x^3 */

double cube(double x)
{  return x*x*x;   }

/* _v_norm2 -- computes (scaled) 2-norm (Euclidean norm) of vectors */

double	_v_norm2(const VEC *x, const VEC *scale)
{
	unsigned int	i, dim;
	double	s, sum;

	if ( x == (VEC *)NULL )
		error(E_NULL,"_v_norm2");
	dim = x->dim;

	sum = 0.0;
	if ( scale == (VEC *)NULL )
		for ( i = 0; i < dim; i++ )
			sum += square(x->ve[i]);
	else if ( scale->dim < dim )
		error(E_SIZES,"_v_norm2");
	else
		for ( i = 0; i < dim; i++ )
		{	s = scale->ve[i];
			sum += ( s== 0.0 ) ? square(x->ve[i]) :
							square(x->ve[i]/s);
		}

	return sqrt(sum);
}

//#define	max(a,b)	((a) > (b) ? (a) : (b))

/* _v_norm_inf -- computes (scaled) infinity-norm (supremum norm) of vectors */
double	_v_norm_inf(const VEC *x, const VEC *scale)
{
	unsigned int	i, dim;
	double	s, maxval, tmp;

	if ( x == (VEC *)NULL )
		error(E_NULL,"_v_norm_inf");
	dim = x->dim;

	maxval = 0.0;
	if ( scale == (VEC *)NULL )
		for ( i = 0; i < dim; i++ )
		{	tmp = fabs(x->ve[i]);
			maxval = mat_max(maxval,tmp);
		}
	else if ( scale->dim < dim )
		error(E_SIZES,"_v_norm_inf");
	else
		for ( i = 0; i < dim; i++ )
		{	s = scale->ve[i];
			tmp = ( s== 0.0 ) ? fabs(x->ve[i]) : fabs(x->ve[i]/s);
			maxval = mat_max(maxval,tmp);
		}

	return maxval;
}

/* m_norm1 -- compute matrix 1-norm -- unscaled */
double	m_norm1(const MAT *A)
{
	int	i, j, m, n;
	double	maxval, sum;

	if ( A == (MAT *)NULL )
		error(E_NULL,"m_norm1");

	m = A->m;	n = A->n;
	maxval = 0.0;

	for ( j = 0; j < n; j++ )
	{
		sum = 0.0;
		for ( i = 0; i < m; i ++ )
			sum += fabs(A->me[i][j]);
		maxval = mat_max(maxval,sum);
	}

	return maxval;
}

/* m_norm_inf -- compute matrix infinity-norm -- unscaled */

double	m_norm_inf(const MAT *A)
{
	int	i, j, m, n;
	double	maxval, sum;

	if ( A == (MAT *)NULL )
		error(E_NULL,"m_norm_inf");

	m = A->m;	n = A->n;
	maxval = 0.0;

	for ( i = 0; i < m; i++ )
	{
		sum = 0.0;
		for ( j = 0; j < n; j ++ )
			sum += fabs(A->me[i][j]);
		maxval = mat_max(maxval,sum);
	}

	return maxval;
}

/* m_norm_frob -- compute matrix frobenius-norm -- unscaled */
double	m_norm_frob(const MAT *A)
{
	int	i, j, m, n;
	double	sum;

	if ( A == (MAT *)NULL )
		error(E_NULL,"m_norm_frob");

	m = A->m;	n = A->n;
	sum = 0.0;

	for ( i = 0; i < m; i++ )
		for ( j = 0; j < n; j ++ )
			sum += square(A->me[i][j]);

	return sqrt(sum);
}

