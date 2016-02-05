#ifndef MATRIX2H
#define MATRIX2H

#include "matrix.h"

#ifdef __cplusplus
extern "C" 
{
#endif
/* Unless otherwise specified, factorisation routines overwrite the
   matrix that is being factorised */

                 /* forms Bunch-Kaufman-Parlett factorisation for
                        symmetric indefinite matrices */
extern	MAT	*BKPfactor(MAT *A,PERM *pivot,PERM *blocks),
                 /* Cholesky factorisation of A
                        (symmetric, positive definite) */
		*CHfactor(MAT *A),
                /* LU factorisation of A (with partial pivoting) */ 
                *LUfactor(MAT *A,PERM *pivot),
                /* QR factorisation of A; need dim(diag) >= # rows of A */
		*QRfactor(MAT *A,VEC *diag),
                /* QR factorisation of A with column pivoting */
		*QRCPfactor(MAT *A,VEC *diag,PERM *pivot),
                /* L.D.L^T factorisation of A */
		*LDLfactor(MAT *A), 
                /* Hessenberg factorisation of A -- for schur() */
                *Hfactor(MAT *A,VEC *diag1,VEC *diag2),
                /* modified Cholesky factorisation of A;
                        actually factors A+D, D diagonal with no
                        diagonal entry in the factor < sqrt(tol) */
                *MCHfactor(MAT *A,double tol),
		*m_inverse(const MAT *A,MAT *out);

                /* returns condition estimate for A after LUfactor() */
extern	double	LUcondest(const MAT *A, PERM *pivot),
                /* returns condition estimate for Q after QRfactor() */
                QRcondest(const MAT *A);

/* Note: The make..() and ..update() routines assume that the factorisation
        has already been carried out */

     /* Qout is the "Q" (orthongonal) matrix from QR factorisation */
extern	MAT	*makeQ(const MAT *QR,const VEC *diag,MAT *Qout),
                /* Rout is the "R" (upper triangular) matrix
                        from QR factorisation */
		*makeR(const MAT *A,MAT *Rout),
                /* Qout is orthogonal matrix in Hessenberg factorisation */
		*makeHQ(MAT *A,VEC *diag1,VEC *diag2,MAT *Qout),
                /* Hout is the Hessenberg matrix in Hessenberg factorisation */
		*makeH(const MAT *A,MAT *Hout);

                /* updates L.D.L^T factorisation for A <- A + alpha.u.u^T */
extern	MAT	*LDLupdate(MAT *A,VEC *u,double alpha),
                /* updates QR factorisation for QR <- Q.(R+u.v^T)
		   Note: we need explicit Q & R matrices,
                        from makeQ() and makeR() */
		*QRupdate(MAT *Q,MAT *R,VEC *u,VEC *v);

/* Solve routines assume that the corresponding factorisation routine
        has already been applied to the matrix along with auxiliary
        objects (such as pivot permutations)

        These solve the system A.x = b,
        except for LUTsolve and QRTsolve which solve the transposed system
                                A^T.x. = b.
        If x is NULL on entry, then it is created.
*/

extern	VEC	*BKPsolve(const MAT *A,PERM *pivot,const PERM *blocks,
			  const VEC *b,VEC *x),
		*CHsolve(const MAT *A,const VEC *b,VEC *x),
		*LDLsolve(const MAT *A,const VEC *b,VEC *x),
		*LUsolve(const MAT *A, PERM *pivot, const VEC *b,VEC *x),
		*_Qsolve(const MAT *A, const VEC *diag, const VEC *b, 
			 VEC *x, VEC *tmp),
		*QRsolve(const MAT *A, const VEC *diag, const VEC *b,VEC *x),
    		*QRTsolve(const MAT *A,const VEC *,const VEC *b,VEC *x),


     /* Triangular equations solve routines;
        U for upper triangular, L for lower traingular, D for diagonal
        if diag_val == 0.0 use that values in the matrix */

		*Usolve(const MAT *A,const VEC *b,VEC *x,double diag_val),
		*Lsolve(const MAT *A,const VEC *b,VEC *x,double diag_val),
		*Dsolve(const MAT *A,const VEC *b,VEC *x),
		*LTsolve(const MAT *A,const VEC *b,VEC *x,double diag_val),
		*UTsolve(const MAT *A,const VEC *b,VEC *x,double diag_val),
                *LUTsolve(const MAT *A,PERM *pivot,const VEC *b, VEC *x),
                *QRCPsolve(const MAT *QR,const VEC *diag,PERM *pivot,
			   const VEC *b,VEC *x);

/* matrix powers and exponent */
MAT  *_m_pow(const MAT *A, int p, MAT *tmp,MAT *out);
MAT  *m_pow(const MAT *A, int p, MAT *out);
MAT  *m_exp(MAT *,double,MAT *);
MAT  *_m_exp(MAT *A, double eps, MAT *out, int *q_out, int *j_out);
MAT  *m_poly(const MAT *,const VEC *,MAT *);

extern	VEC	*hhvec(const VEC *,unsigned int,MatrixReal *,VEC *,MatrixReal *);
extern	VEC	*hhtrvec(const VEC *,double,unsigned int,const VEC *,VEC *);
extern	MAT	*hhtrrows(MAT *,unsigned int,unsigned int,const VEC *,double);
extern	MAT	*hhtrcols(MAT *,unsigned int,unsigned int,const VEC *,double);
extern	MAT	*_hhtrcols(MAT *,unsigned int,unsigned int,const VEC *,double,VEC *);


extern	void	givens(double, double, MatrixReal *, MatrixReal *);
extern	VEC	*rot_vec(const VEC *,unsigned int,unsigned int,
			 double,double,VEC *); /* in situ */
extern	MAT	*rot_rows(const MAT *,unsigned int,unsigned int,
			  double,double,MAT *); /* in situ */
extern	MAT	*rot_cols(const MAT *,unsigned int,unsigned int,
			  double,double,MAT *); /* in situ */

/* eigenvalue routines */

               /* compute eigenvalues of tridiagonal matrix
                  with diagonal entries a[i], super & sub diagonal entries
                  b[i]; eigenvectors stored in Q (if not NULL) */
extern	VEC	*trieig(VEC *a,VEC *b,MAT *Q);
                 /* sets out to be vector of eigenvectors; eigenvectors
                   stored in Q (if not NULL). A is unchanged */
extern	VEC	*symmeig(const MAT *A,MAT *Q,VEC *out);

VEC	*v_cross(const VEC *x1, const VEC *x2, VEC *out);

extern int symm_inv6(const double *matA, double* matInvA);
extern double norm_inf6(double *A);

#ifdef __cplusplus
}
#endif

#endif
