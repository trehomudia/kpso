#define NOT_SEGMENTED 1
#define HAVE_MALLOC_H 1
#define STDC_HEADERS 1
#define HAVE_BCOPY 1
#define HAVE_BZERO 1
#define CHAR0ISDBL0 1
#define U_INT_DEF 1
#define VARARGS 1


/* for segmented memory */
#ifndef NOT_SEGMENTED
#define	SEGMENTED
#endif

/* if the system has malloc.h */
#ifndef bt202
#ifdef HAVE_MALLOC_H
#define	MALLOCDECL	1
#include	<malloc.h>
#endif
#endif

/* any compiler should have this header */
/* if not, change it */
#include        <stdio.h>


/* Check for ANSI C memmove and memset */
#ifdef STDC_HEADERS

/* standard copy & zero functions */
#define	MEM_COPY(from,to,size)	memmove((to),(from),(size))
#define	MEM_ZERO(where,size)	memset((where),'\0',(size))


#endif

/* standard headers */
#include	<stdlib.h>
#include	<stddef.h>
#include	<string.h>
#include	<float.h>



/* if have bcopy & bzero and no alternatives yet known, use them */
#ifdef HAVE_BCOPY
#ifndef MEM_COPY
/* nonstandard copy function */
#define	MEM_COPY(from,to,size)	bcopy((char *)(from),(char *)(to),(int)(size))
#endif
#endif

#ifdef HAVE_BZERO
#ifndef MEM_ZERO
/* nonstandard zero function */
#define	MEM_ZERO(where,size)	bzero((char *)(where),(int)(size))
#endif
#endif


/* If prototypes are available & ANSI_C not yet defined, then define it,
	but don't include any header files as the proper ANSI C headers
        aren't here */
#define HAVE_PROTOTYPES 1
#ifdef HAVE_PROTOTYPES
#ifndef ANSI_C
#define ANSI_C  1
#endif
#endif

/* floating point precision */

/* you can choose single, double or long double (if available) precision */
#define FLOAT_MATRIX 		1
#define DOUBLE_MATRIX 		2
#define LONG_DOUBLE 	3

/* if nothing is defined, choose double precision */
#ifndef REAL_DBL
#ifndef REAL_FLT
#define REAL_DBL 1
#endif
#endif

/* single precision */
#ifdef REAL_FLT
#define		MatrixReal				float
#define		MatrixLongReal		float
#define		REAL							FLOAT_MATRIX
#define		LONGREAL					FLOAT_MATRIX
#endif

/* double precision */
#ifdef REAL_DBL
#define		MatrixReal				double
#define		MatrixLongReal		double
#define		REAL							DOUBLE_MATRIX
#define		LONGREAL					DOUBLE_MATRIX
#endif


/* machine epsilon or unit roundoff error */
/* This is correct on most IEEE MatrixReal precision systems */
#ifdef DBL_EPSILON
#if REAL == DOUBLE_MATRIX
#define	MACHEPS	DBL_EPSILON
#elif REAL == FLOAT_MATRIX
#define	MACHEPS	FLT_EPSILON
#elif REAL == LONGDOUBLE
#define MACHEPS LDBL_EPSILON
#endif
#endif

#define F_MACHEPS 1.19209e-07
#define D_MACHEPS 2.22045e-16

#ifndef MACHEPS
#if REAL == DOUBLE_MATRIX
#define	MACHEPS	D_MACHEPS
#elif REAL == FLOAT_MATRIX  
#define MACHEPS F_MACHEPS
#elif REAL == LONGDOUBLE
#define MACHEPS D_MACHEPS
#endif
#endif


#define	M_MAX_INT 2147483647
#ifdef	M_MAX_INT
#ifndef MAX_RAND
#define	MAX_RAND ((double)(M_MAX_INT))
#endif
#endif

#ifdef __cplusplus

//extern	int	isatty(int) throw();

#else

extern	int	isatty(int);

#endif
