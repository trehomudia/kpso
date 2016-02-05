
/*
  This file contains basic routines which are used by the functions
  in meschach.a etc.
  These are the routines that should be modified in order to take
  full advantage of specialised architectures (pipelining, vector
  processors etc).
  */


#include "..//include//machine.h"

/* __ip__ -- inner product */

double	__ip__(const MatrixReal *dp1, const MatrixReal *dp2, int len)
{
#ifdef VUNROLL
    register int	len4;
    register double	sum1, sum2, sum3;
#endif
    register int	i;
    register double     sum;

    sum = 0.0;
#ifdef VUNROLL
    sum1 = sum2 = sum3 = 0.0;
    
    len4 = len / 4;
    len  = len % 4;
    
    for ( i = 0; i < len4; i++ )
    {
	sum  += dp1[4*i]*dp2[4*i];
	sum1 += dp1[4*i+1]*dp2[4*i+1];
	sum2 += dp1[4*i+2]*dp2[4*i+2];
	sum3 += dp1[4*i+3]*dp2[4*i+3];
    }
    sum  += sum1 + sum2 + sum3;
    dp1 += 4*len4;	dp2 += 4*len4;
#endif
    
    for ( i = 0; i < len; i++ )
	sum  += dp1[i]*dp2[i];
    
    return sum;
}

/* __mltadd__ -- scalar multiply and add c.f. v_mltadd() */

void	__mltadd__(MatrixReal *dp1, const MatrixReal *dp2, double s, int len)
{
    register int	i;
#ifdef VUNROLL
    register int        len4;
    
    len4 = len / 4;
    len  = len % 4;
    for ( i = 0; i < len4; i++ )
    {
	dp1[4*i]   += s*dp2[4*i];
	dp1[4*i+1] += s*dp2[4*i+1];
	dp1[4*i+2] += s*dp2[4*i+2];
	dp1[4*i+3] += s*dp2[4*i+3];
    }
    dp1 += 4*len4;	dp2 += 4*len4;
#endif
    
    for ( i = 0; i < len; i++ )
	dp1[i] += s*dp2[i];
}

/* __smlt__ scalar multiply array c.f. sv_mlt() */

void	__smlt__(const MatrixReal *dp, double s, MatrixReal *out, int len)
{
    register int	i;
    for ( i = 0; i < len; i++ )
	out[i] = s*dp[i];
}

/* __add__ -- add arrays c.f. v_add() */

void	__add__(const MatrixReal *dp1, const MatrixReal *dp2, MatrixReal *out, int len)
{
    register int	i;
    for ( i = 0; i < len; i++ )
	out[i] = dp1[i] + dp2[i];
}

/* __sub__ -- subtract arrays c.f. v_sub() */
void	__sub__(const MatrixReal *dp1, const MatrixReal *dp2, MatrixReal *out, int len)
{
    register int	i;
    for ( i = 0; i < len; i++ )
	out[i] = dp1[i] - dp2[i];
}

/* __zero__ -- zeros an array of floating point numbers */
void	__zero__(MatrixReal *dp, int len)
{
#ifdef CHAR0ISDBL0
    /* if a floating point zero is equivalent to a string of nulls */
    MEM_ZERO((char *)dp,len*sizeof(MatrixReal));
#else
    /* else, need to zero the array entry by entry */
    int	i;
    for ( i = 0; i < len; i++ )
	dp[i] = 0.0;
#endif
}

