//   **************   тут объявлены типы матриц и векторов ****************//

#ifndef	MATRIX_H
#define	MATRIX_H

#include	"machine.h"
//#include  "err.h"
#include 	"meminfo.h"
#ifndef _WIN32
#include  "mutex/mutex.h"
#endif

/* vector definition */
typedef	struct{
	unsigned int  dim;
  unsigned int  max_dim;
	MatrixReal	  *ve;
	MatrixReal    *align;
}VEC;

/* matrix definition */
typedef	struct{
  unsigned int	m;
  unsigned int	n;
  unsigned int	max_m;
  unsigned int	max_n;
  unsigned int	max_size;
  unsigned int  align;
  MatrixReal	  **me;
  MatrixReal    *base;	  /* base is base of alloc'd mem */
}MAT;

/* permutation definition */
typedef	struct{
  unsigned int	size;
  unsigned int	max_size;
  unsigned int	*pe;
  unsigned int  *align;
}PERM;

/* integer vector definition */
typedef struct{
	unsigned int	dim, max_dim;
	int	*ive;
	int  *align;
}IVEC;

#ifndef MALLOCDECL
#ifndef ANSI_C
extern	char	*malloc(), *calloc(), *realloc();
#else
extern	void	*malloc(size_t),
		*calloc(size_t,size_t),
		*realloc(void *,size_t);
#endif
#endif /* MALLOCDECL */


#ifdef THREADSAFE
#define	STATIC
#else
#define	STATIC	static
#endif /* THREADSAFE */


void	m_version( void );



/* allocate one object of given type */
#define	NEW(type)	((type *)calloc((size_t)1,(size_t)sizeof(type)))

/* allocate num objects of given type */
#define	NEW_A(num,type)	((type *)calloc((size_t)(num),(size_t)sizeof(type)))

 /* re-allocate arry to have num objects of the given type */
#define	RENEW(var,num,type) \
    ((type *)((var) ? \
		    realloc((char *)(var),(size_t)((num)*sizeof(type))) : \
		    calloc((size_t)(num),(size_t)sizeof(type))))

#define	MEMCOPY(from,to,n_items,type) \
 MEM_COPY((char *)(from),(char *)(to),(unsigned)(n_items)*sizeof(type))



/* type independent min and max operations */
#ifndef mat_max
#define	mat_max(a,b)	((a) > (b) ? (a) : (b))
#endif /* max */
#ifndef mat_min
#define	mat_min(a,b)	((a) > (b) ? (b) : (a))
#endif /* min */


#undef TRUE
#define	TRUE	1
#undef FALSE
#define	FALSE	0


/* for input routines */
#define MAXLINE 81


/* Dynamic memory allocation */

/* Should use M_FREE/V_FREE/PX_FREE in programs instead of m/v/px_free()
   as this is considerably safer -- also provides a simple type check ! */

extern	VEC *v_get(), *v_resize();
extern	MAT *matrix_get(), *m_resize();
extern	PERM *px_get(), *px_resize();
extern	IVEC *iv_get(), *iv_resize();
extern	int matrix_free(),v_free();
extern  int px_free();
extern  int iv_free();
extern  int bd_free();


//---------------------------START CODE SPECIFIC (by Okunev)
#ifdef __cplusplus
extern "C"
{
#endif
//---------------------------
/* get/resize vector to given dimension */
extern	VEC *v_get(int), *v_resize(VEC *,unsigned int);
/* get/resize matrix to be m x n */
extern	MAT *matrix_get(int,int), *m_resize(MAT *,unsigned int,unsigned int);
/* get/resize permutation to have the given size */
extern	PERM *px_get(int), *px_resize(PERM *,unsigned int);
/* get/resize an integer vector to given dimension */
extern	IVEC *iv_get(int), *iv_resize(IVEC *,unsigned int);
/* get/resize a band matrix to given dimension */
//extern  BAND *bd_get(int,int,int), *bd_resize(BAND *,int,int,int);

/* free (de-allocate) (band) matrices, vectors, permutations and
   integer vectors */
extern  int iv_free(IVEC *);
extern	int matrix_free(MAT *),	v_free(VEC *),px_free(PERM *);


/* MACROS */

// ------------------------------------------------------------------
/* macros that also check types and sets pointers to NULL */
#define	M_FREE(mat)	( matrix_free(mat),	(mat)=(MAT *)NULL )
#define V_FREE(vec)	( v_free(vec),	(vec)=(VEC *)NULL )
#define	PX_FREE(px)	( px_free(px),	(px)=(PERM *)NULL )
#define	IV_FREE(iv)	( iv_free(iv),	(iv)=(IVEC *)NULL )

#define MAXDIM  	10000001


/* Entry level access to data structures */
/* routines to check indexes */
#define	m_chk_idx(A,i,j)	((i)>=0 && (i)<(A)->m && (j)>=0 && (j)<=(A)->n)
#define	v_chk_idx(x,i)		((i)>=0 && (i)<(x)->dim)
#define	bd_chk_idx(A,i,j)	((i)>=mat_max(0,(j)-(A)->ub) && \
		(j)>=mat_max(0,(i)-(A)->lb) && (i)<(A)->mat->n && (j)<(A)->mat->n)

#define	m_entry(A,i,j)		m_get_val(A,i,j)
#define	v_entry(x,i)		v_get_val(x,i)
#define	bd_entry(A,i,j)		bd_get_val(A,i,j)
#ifdef DEBUG
#define	m_set_val(A,i,j,val)	( m_chk_idx(A,i,j) ? \
	(A)->me[(i)][(j)] = (val) : (error(E_BOUNDS,"m_set_val"), 0.0))
#define	m_add_val(A,i,j,val)	( m_chk_idx(A,i,j) ? \
	(A)->me[(i)][(j)] += (val) : (error(E_BOUNDS,"m_add_val"), 0.0))
#define	m_sub_val(A,i,j,val)	( m_chk_idx(A,i,j) ? \
	(A)->me[(i)][(j)] -= (val) : (error(E_BOUNDS,"m_sub_val"), 0.0))
#define	m_get_val(A,i,j)	( m_chk_idx(A,i,j) ? \
	(A)->me[(i)][(j)] : (error(E_BOUNDS,"m_get_val"), 0.0))
#define	v_set_val(x,i,val)	( v_chk_idx(x,i) ? (x)->ve[(i)] = (val) : \
	(error(E_BOUNDS,"v_set_val"), 0.0))
#define	v_add_val(x,i,val)	( v_chk_idx(x,i) ? (x)->ve[(i)] += (val) : \
	(error(E_BOUNDS,"v_set_val"), 0.0))
#define	v_sub_val(x,i,val)	( v_chk_idx(x,i) ? (x)->ve[(i)] -= (val) : \
	(error(E_BOUNDS,"v_set_val"), 0.0))
#define	v_get_val(x,i)	( v_chk_idx(x,i) ? (x)->ve[(i)] : \
	(error(E_BOUNDS,"v_get_val"), 0.0))
#define	bd_set_val(A,i,j,val)	( bd_chk_idx(A,i,j) ? \
	(A)->mat->me[(A)->lb+(j)-(i)][(j)] = (val) : \
	(error(E_BOUNDS,"bd_set_val"), 0.0))
#define	bd_add_val(A,i,j,val)	( bd_chk_idx(A,i,j) ? \
	(A)->mat->me[(A)->lb+(j)-(i)][(j)] += (val) : \
	(error(E_BOUNDS,"bd_set_val"), 0.0))
#define	bd_get_val(A,i,j)	( bd_chk_idx(A,i,j) ? \
	(A)->mat->me[(A)->lb+(j)-(i)][(j)] : \
	(error(E_BOUNDS,"bd_get_val"), 0.0))
#else /* no DEBUG */
#define	m_set_val(A,i,j,val)	((A)->me[(i)][(j)] = (val))
#define	m_add_val(A,i,j,val)	((A)->me[(i)][(j)] += (val))
#define	m_sub_val(A,i,j,val)	((A)->me[(i)][(j)] -= (val))
#define	m_get_val(A,i,j)	((A)->me[(i)][(j)])
#define	v_set_val(x,i,val)	((x)->ve[(i)] = (val))
#define	v_add_val(x,i,val)	((x)->ve[(i)] += (val))
#define	v_sub_val(x,i,val)	((x)->ve[(i)] -= (val))
#define	v_get_val(x,i)		((x)->ve[(i)])
#define	bd_set_val(A,i,j,val)	((A)->mat->me[(A)->lb+(j)-(i)][(j)] = (val))
#define	bd_add_val(A,i,j,val)	((A)->mat->me[(A)->lb+(j)-(i)][(j)] += (val))
#define	bd_get_val(A,i,j)	((A)->mat->me[(A)->lb+(j)-(i)][(j)])
#endif /* DEBUG */


/* I/O routines */

/* print x on file fp */
void v_foutput(FILE *fp,const VEC *x),
       /* print A on file fp */
	m_foutput(FILE *fp,const MAT *A),
       /* print px on file fp */
	px_foutput(FILE *fp,const PERM *px);
/* print ix on file fp */
void iv_foutput(FILE *fp,const IVEC *ix);

/* Note: if out is NULL, then returned object is newly allocated;
        Also: if out is not NULL, then that size is assumed */

/* read in vector from fp */
VEC *v_finput(FILE *fp,VEC *out);
/* read in matrix from fp */
MAT *m_finput(FILE *fp,MAT *out);
/* read in permutation from fp */
PERM *px_finput(FILE *fp,PERM *out);
/* read in int vector from fp */
IVEC *iv_finput(FILE *fp,IVEC *out);

/* fy_or_n -- yes-or-no to question in string s
        -- question written to stderr, input from fp
        -- if fp is NOT a tty then return y_n_dflt */
int fy_or_n(FILE *fp, const char *s);

/* yn_dflt -- sets the value of y_n_dflt to val */
int yn_dflt(int val);

/* fin_int -- return integer read from file/stream fp
        -- prompt s on stderr if fp is a tty
        -- check that x lies between low and high: re-prompt if
                fp is a tty, error exit otherwise
        -- ignore check if low > high           */
int fin_int(FILE *fp,const char *s,int low,int high);

/* fin_double -- return double read from file/stream fp
        -- prompt s on stderr if fp is a tty
        -- check that x lies between low and high: re-prompt if
                fp is a tty, error exit otherwise
        -- ignore check if low > high           */
double fin_double(FILE *fp,const char *s,double low,double high);

/* it skips white spaces and strings of the form #....\n
   Here .... is a comment string */
int skipjunk(FILE *fp);



/* MACROS */

/* macros to use stdout and stdin instead of explicit fp */
#define	v_output(vec)	v_foutput(stdout,vec)
#define	v_input(vec)	v_finput(stdin,vec)
#define	m_output(mat)	m_foutput(stdout,mat)
#define	m_input(mat)	m_finput(stdin,mat)
#define	px_output(px)	px_foutput(stdout,px)
#define	px_input(px)	px_finput(stdin,px)
#define	iv_output(iv)	iv_foutput(stdout,iv)
#define	iv_input(iv)	iv_finput(stdin,iv)

/* general purpose input routine; skips comments # ... \n */
#define	finput(fp,prompt,fmt,var) \
	( ( isatty(fileno(fp)) ? fprintf(stderr,prompt) : skipjunk(fp) ), \
							fscanf(fp,fmt,var) )
#define	input(prompt,fmt,var)	finput(stdin,prompt,fmt,var)
#define	fprompter(fp,prompt) \
	( isatty(fileno(fp)) ? fprintf(stderr,prompt) : skipjunk(fp) )
#define	prompter(prompt)	fprompter(stdin,prompt)
#define	y_or_n(s)	fy_or_n(stdin,s)
#define	in_int(s,lo,hi)	fin_int(stdin,s,lo,hi)
#define	in_double(s,lo,hi)	fin_double(stdin,s,lo,hi)


/* special purpose access routines */

/* Copying routines */
/* copy in to out starting at out[i0][j0] */
extern	MAT	*_m_copy(const MAT *in,MAT *out,unsigned int i0,unsigned int j0),
		* m_move(const MAT *in, unsigned int, unsigned int, unsigned int, unsigned int, MAT *out, unsigned int, unsigned int),
		*vm_move(const VEC *in, unsigned int, MAT *out, unsigned int, unsigned int, unsigned int, unsigned int);
/* copy in to out starting at out[i0] */
extern	VEC	*_v_copy(const VEC *in,VEC *out,unsigned int i0),
		* v_move(const VEC *in, unsigned int, unsigned int, VEC *out, unsigned int),
		*mv_move(const MAT *in, unsigned int, unsigned int, unsigned int, unsigned int, VEC *out, unsigned int);
extern	PERM	*px_copy(const PERM *in,PERM *out);
extern	IVEC	*iv_copy(const IVEC *in,IVEC *out),
		*iv_move(const IVEC *in, unsigned int, unsigned int, IVEC *out, unsigned int);


/* MACROS */
#define	m_copy(in,out)	_m_copy(in,out,0,0)
#define	v_copy(in,out)	_v_copy(in,out,0)


/* Initialisation routines -- to be zero, ones, random or identity */
//Функция инициализации матричной библиотеки
extern  int InitMatrix();
extern  int DestroyMatrix();
extern	VEC     *v_zero(VEC *), *v_rand(VEC *), *v_ones(VEC *);
extern	MAT     *m_zero(MAT *), *m_ident(MAT *), *m_rand(MAT *),
						*m_ones(MAT *);
extern	PERM    *px_ident(PERM *);
extern  IVEC    *iv_zero(IVEC *);


/* Basic vector operations */
extern	VEC	*sv_mlt(double s,const VEC *x,VEC *out),	/* out <- s.x */
		*mv_mlt(const MAT *A,const VEC *s,VEC *out),	/* out <- A.x */
		*vm_mlt(const MAT *A,const VEC *x,VEC *out),	/* out^T <- x^T.A */
		*v_add(const VEC *x,const VEC *y,VEC *out), 	/* out <- x + y */
                *v_sub(const VEC *x,const VEC *y,VEC *out),	/* out <- x - y */
		*px_vec(PERM *px,const VEC *x,VEC *out),	/* out <- P.x */
		*pxinv_vec(PERM *px,const VEC *x,VEC *out),	/* out <- P^{-1}.x */
		*v_mltadd(const VEC *x,const VEC *y,double s,VEC *out),   /* out <- x + s.y */
#ifdef PROTOTYPES_IN_STRUCT
		*v_map(double (*f)(double),const VEC *x,VEC *y),
                                                 /* out[i] <- f(x[i]) */
		*_v_map(double (*f)(void *,double),void *p,const VEC *x,VEC *y),
#else
		*v_map(double (*f)(),const VEC *,VEC *), /* out[i] <- f(x[i]) */
		*_v_map(double (*f)(),void *,const VEC *,VEC *),
#endif /* PROTOTYPES_IN_STRUCT */
		*v_lincomb(int,const VEC **,const MatrixReal *,VEC *),
                                                 /* out <- sum_i s[i].x[i] */
                *v_linlist(VEC *out,VEC *v1,double a1,...);
                                              /* out <- s1.x1 + s2.x2 + ... */

/* returns min_j x[j] (== x[i]) */
extern	double	v_min(const VEC *, int *),
     /* returns max_j x[j] (== x[i]) */
        v_max(const VEC *, int *),
        /* returns sum_i x[i] */
        v_sum(const VEC *);

/* Hadamard product: out[i] <- x[i].y[i] */
extern	VEC	*v_star(const VEC *, const VEC *, VEC *),
                 /* out[i] <- x[i] / y[i] */
		*v_slash(const VEC *, const VEC *, VEC *),
               /* sorts x, and sets order so that sorted x[i] = x[order[i]] */
		*v_sort(VEC *, PERM *);

/* returns inner product starting at component i0 */
extern	double	_in_prod(const VEC *x, const VEC *y,unsigned int i0),
                /* returns sum_{i=0}^{len-1} x[i].y[i] */
                __ip__(const MatrixReal *,const MatrixReal *,int);

/* see v_mltadd(), v_add(), v_sub() and v_zero() */
extern	void	__mltadd__(MatrixReal *,const MatrixReal *,double,int),
		__add__(const MatrixReal *,const MatrixReal *,MatrixReal *,int),
		__sub__(const MatrixReal *,const MatrixReal *,MatrixReal *,int),
                __smlt__(const MatrixReal *,double,MatrixReal *,int),
		__zero__(MatrixReal *,int);



/* MACRO */
/* usual way of computing the inner product */
#define	in_prod(a,b)	_in_prod(a,b,0)

/* Norms */
/* scaled vector norms -- scale == NULL implies unscaled */

               /* returns sum_i |x[i]/scale[i]| */
extern	double	_v_norm1(const VEC *x,const VEC *scale),
               /* returns (scaled) Euclidean norm */
                _v_norm2(const VEC *x,const VEC *scale),
               /* returns max_i |x[i]/scale[i]| */
		_v_norm_inf(const VEC *x,const VEC *scale);

/* unscaled matrix norms */
extern double m_norm1(const MAT *A),
	m_norm_inf(const MAT *A),
	m_norm_frob(const MAT *A);

// Функции вывода на экран или в файл матриц и векторов
int PrintMat(MAT *pMat, FILE *pFile);
int PrintVec(VEC *pVec, FILE *pFile);


/* MACROS */
/* unscaled vector norms */
#define	v_norm1(x)	_v_norm1(x,VNULL)
#define	v_norm2(x)	_v_norm2(x,VNULL)
#define	v_norm_inf(x)	_v_norm_inf(x,VNULL)

/* Basic matrix operations */
extern	MAT	*sm_mlt(double s, const MAT *A,MAT *out), 	/* out <- s.A */
		*m_mlt(const MAT *A,const MAT *B,MAT *out),	/* out <- A.B */
		*mmtr_mlt(const MAT *A,const MAT *B,MAT *out),	/* out <- A.B^T */
		*mtrm_mlt(const MAT *A,const MAT *B,MAT *out),	/* out <- A^T.B */
		*m_add(const MAT *A,const MAT *B,MAT *out),	/* out <- A + B */
		*m_sub(const MAT *A,const MAT *B,MAT *out),	/* out <- A - B */
		*sub_mat(const MAT *A,unsigned int,unsigned int,unsigned int,
			 unsigned int,MAT *out),
		*m_transp(const MAT *A,MAT *out),		/* out <- A^T */
                /* out <- A + s.B */
								*ms_mltadd(const MAT *A,const MAT *B,double s,MAT *out);


extern	MAT	*px_rows(const PERM *px,const MAT *A,MAT *out),	/* out <- P.A */
		*px_cols(const PERM *px,const MAT *A,MAT *out),	/* out <- A.P^T */
		*swap_rows(MAT *,unsigned int,unsigned int,unsigned int,unsigned int),
		*swap_cols(MAT *,unsigned int,unsigned int,unsigned int,unsigned int),
                 /* A[i][j] <- out[j], j >= j0 */
		*_set_col(MAT *A,unsigned int i,const VEC *col,unsigned int j0),
                 /* A[i][j] <- out[i], i >= i0 */
		*_set_row(MAT *A,unsigned int j,const VEC *row,unsigned int i0);

extern	VEC	*get_row(const MAT *,unsigned int,VEC *),
		*get_col(const MAT *,unsigned int,VEC *),
		*sub_vec(const VEC *, unsigned int, unsigned int,VEC *),
                   /* mv_mltadd: out <- x + s.A.y */
		*mv_mltadd(const VEC *x,const VEC *y,const MAT *A,
			  double s,VEC *out),
                  /* vm_mltadd: out^T <- x^T + s.y^T.A */
		*vm_mltadd(const VEC *x,const VEC *y,const MAT *A,
			   double s,VEC *out);
                  /* bdv_mltadd: out <- x + s.A.y */


/* MACROS */
/* row i of A <- vec */
#define	set_row(mat,row,vec)	_set_row(mat,row,vec,0)
/* col j of A <- vec */
#define	set_col(mat,col,vec)	_set_col(mat,col,vec,0)


/* Basic permutation operations */
extern	PERM	*px_mlt(const PERM *px1,const PERM *px2,PERM *out),	/* out <- px1.px2 */
		*px_inv(const PERM *px,PERM *out),	/* out <- px^{-1} */
                 /* swap px[i] and px[j] */
		*px_transp(PERM *px,unsigned int i,unsigned int j);

     /* returns sign(px) = +1 if px product of even # transpositions
                           -1 if ps product of odd  # transpositions */
extern	int	px_sign(const PERM *);


/* Basic integer vector operations */
extern	IVEC	*iv_add(const IVEC *ix,const IVEC *iy,IVEC *out),
  /* out <- ix + iy */
		*iv_sub(const IVEC *ix,const IVEC *iy,IVEC *out),
  /* out <- ix - iy */
  /* sorts ix & sets order so that sorted ix[i] = old ix[order[i]] */
		*iv_sort(IVEC *ix, PERM *order);

//Вычисление факториала
extern	unsigned int Fact(unsigned int n);

/* miscellaneous functions */

double	square(double x), 	/* returns x^2 */
  cube(double x), 		/* returns x^3 */
  mrand(void);                  /* returns random # in [0,1) */

void	smrand(int seed),            /* seeds mrand() */
  mrandlist(MatrixReal *x, int len);       /* generates len random numbers */

void    m_dump(FILE *fp,const MAT *a), px_dump(FILE *fp, const PERM *px),
        v_dump(FILE *fp,const VEC *x), iv_dump(FILE *fp, const IVEC *ix);


/* miscellaneous constants */
#define	VNULL	((VEC *)NULL)
#define	MNULL	((MAT *)NULL)
#define	PNULL	((PERM *)NULL)
#define	IVNULL	((IVEC *)NULL)


/* varying number of arguments */

#ifdef ANSI_C
#include <stdarg.h>

/* prototypes */

int v_get_vars(int dim,...);
int iv_get_vars(int dim,...);
int m_get_vars(int m,int n,...);
int px_get_vars(int dim,...);

int v_resize_vars(int new_dim,...);
int iv_resize_vars(int new_dim,...);
int m_resize_vars(int m,int n,...);
int px_resize_vars(int new_dim,...);

int v_free_vars(VEC **,...);
int iv_free_vars(IVEC **,...);
int px_free_vars(PERM **,...);
int m_free_vars(MAT **,...);

#elif VARARGS
/* old varargs is used */

#include  <varargs.h>

/* prototypes */

int v_get_vars();
int iv_get_vars();
int m_get_vars();
int px_get_vars();

int v_resize_vars();
int iv_resize_vars();
int m_resize_vars();
int px_resize_vars();

int v_free_vars();
int iv_free_vars();
int px_free_vars();
int m_free_vars();

#endif /* ANSI_C */

#ifdef __cplusplus
}
#endif

#endif /* MATRIXH */
