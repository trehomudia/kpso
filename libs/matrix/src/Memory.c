
#include	"..//include//matrix.h"
#include 	"..//include//matrix3.h"
#include "..//include//err.h"


/* matrix_get -- gets an mxn matrix (in MAT form) by dynamic memory allocation
	-- normally ALL matrices should be obtained this way
	-- if either m or n is negative this will raise an error
	-- note that 0 x n and m x 0 matrices can be created */


MAT	*matrix_get(int m, int n)
{
   MAT	*matrix = MNULL;
   int	i;
   
   if (m < 0 || n < 0)
     error(E_NEG,"matrix_get");

   if ((matrix=NEW(MAT)) == (MAT *)NULL )
      error(E_MEM,"matrix_get");
   else if (mem_info_is_on()) {
      mem_bytes(TYPE_MAT,0,sizeof(MAT));
      mem_numvar(TYPE_MAT,1);
   }
   
   matrix->m = m;		matrix->n = matrix->max_n = n;
   matrix->max_m = m;	matrix->max_size = m*n;
#ifndef SEGMENTED
   if ((matrix->base = NEW_A(m*n,MatrixReal)) == (MatrixReal *)NULL )
   {
      free(matrix);
      error(E_MEM,"matrix_get");
   }
   else if (mem_info_is_on()) {
      mem_bytes(TYPE_MAT,0,m*n*sizeof(MatrixReal));
   }
#else
   matrix->base = (MatrixReal *)NULL;
#endif
   if ((matrix->me = (MatrixReal **)calloc(m,sizeof(MatrixReal *))) == 
       (MatrixReal **)NULL )
   {	free(matrix->base);	free(matrix);
	error(E_MEM,"matrix_get");
     }
   else if (mem_info_is_on()) {
      mem_bytes(TYPE_MAT,0,m*sizeof(MatrixReal *));
   }
   
#ifndef SEGMENTED
   /* set up pointers */
   for ( i=0; i<m; i++ )
     matrix->me[i] = &(matrix->base[i*n]);
#else
   for ( i = 0; i < m; i++ )
     if ( (matrix->me[i]=NEW_A(n,MatrixReal)) == (MatrixReal *)NULL )
       error(E_MEM,"matrix_get");
     else if (mem_info_is_on()) {
	mem_bytes(TYPE_MAT,0,n*sizeof(MatrixReal));
       }
#endif
   
   return (matrix);
}


/* px_get -- gets a PERM of given 'size' by dynamic memory allocation
	-- Note: initialized to the identity permutation
	-- the permutation is on the set {0,1,2,...,size-1} */

PERM	*px_get(int size)
{
   PERM	*permute;
   int	i;

   if (size < 0)
     error(E_NEG,"px_get");

   if ((permute=NEW(PERM)) == (PERM *)NULL )
     error(E_MEM,"px_get");
   else if (mem_info_is_on()) {
      mem_bytes(TYPE_PERM,0,sizeof(PERM));
      mem_numvar(TYPE_PERM,1);
   }
   
   permute->size = permute->max_size = size;
   if ((permute->pe = NEW_A(size,unsigned int)) == (unsigned int *)NULL )
     error(E_MEM,"px_get");
   else if (mem_info_is_on()) {
      mem_bytes(TYPE_PERM,0,size*sizeof(unsigned int));
   }
   
   for ( i=0; i<size; i++ )
     permute->pe[i] = i;
   
   return (permute);
}




VEC	*v_get(int size)
{
   VEC	*vector;
   
   if (size < 0)
     error(E_NEG,"v_get");

   if ((vector=NEW(VEC)) == (VEC *)NULL )
     error(E_MEM,"v_get");
   else if (mem_info_is_on()) {
      mem_bytes(TYPE_VEC,0,sizeof(VEC));
      mem_numvar(TYPE_VEC,1);
   }
   
   vector->dim = vector->max_dim = size;
   if ((vector->ve=NEW_A(size,MatrixReal)) == (MatrixReal *)NULL )
   {
      free(vector);
      error(E_MEM,"v_get");
   }
   else if (mem_info_is_on()) {
      mem_bytes(TYPE_VEC,0,size*sizeof(MatrixReal));
   }
   
   return (vector);
}
//------------------------------------------------------------------
/* matrix_free -- returns MAT & asoociated memory back to memory heap */

int	matrix_free(MAT *mat)
{
#ifdef SEGMENTED
   int	i;
#endif
   
   if ( mat==(MAT *)NULL || (int)(mat->m) < 0 ||
       (int)(mat->n) < 0 )
     return (-1);
   
#ifndef SEGMENTED
   if ( mat->base != (MatrixReal *)NULL ) {
      if (mem_info_is_on()) {
	 mem_bytes(TYPE_MAT,mat->max_m*mat->max_n*sizeof(MatrixReal),0);
      }
      free((char *)(mat->base));
   }
#else
   for ( i = 0; i < mat->max_m; i++ )
     if ( mat->me[i] != (MatrixReal *)NULL ) {
	if (mem_info_is_on()) {
	   mem_bytes(TYPE_MAT,mat->max_n*sizeof(MatrixReal),0);
	}
	free((char *)(mat->me[i]));
     }
#endif
   if ( mat->me != (MatrixReal **)NULL ) {
      if (mem_info_is_on()) {
	 mem_bytes(TYPE_MAT,mat->max_m*sizeof(MatrixReal *),0);
      }
      free((char *)(mat->me));
   }
   
   if (mem_info_is_on()) {
      mem_bytes(TYPE_MAT,sizeof(MAT),0);
      mem_numvar(TYPE_MAT,-1);
   }
   free((char *)mat);
   
   return (0);
}



/* px_free -- returns PERM & asoociated memory back to memory heap */

int	px_free(PERM *px)
{
   if ( px==(PERM *)NULL || (int)(px->size) < 0 )
     return (-1);
   
   if ( px->pe == (unsigned int *)NULL ) {
      if (mem_info_is_on()) {
	 mem_bytes(TYPE_PERM,sizeof(PERM),0);
	 mem_numvar(TYPE_PERM,-1);
      }      
      free((char *)px);
   }
   else
   {
      if (mem_info_is_on()) {
	 mem_bytes(TYPE_PERM,sizeof(PERM)+px->max_size*sizeof(unsigned int),0);
	 mem_numvar(TYPE_PERM,-1);
      }
      free((char *)px->pe);
      free((char *)px);
   }
   
   return (0);
}



/* v_free -- returns VEC & asoociated memory back to memory heap */
int	v_free(VEC *vec)
{
   if ( vec==(VEC *)NULL || (int)(vec->dim) < 0 )
     return (-1);
   
   if ( vec->ve == (MatrixReal *)NULL ) {
      if (mem_info_is_on()) {
	 mem_bytes(TYPE_VEC,sizeof(VEC),0);
	 mem_numvar(TYPE_VEC,-1);
      }
      free((char *)vec);
   }
   else
   {
      if (mem_info_is_on()) {
	 mem_bytes(TYPE_VEC,sizeof(VEC)+vec->max_dim*sizeof(MatrixReal),0);
	 mem_numvar(TYPE_VEC,-1);
      }
      free((char *)vec->ve);
      free((char *)vec);
   }
   
   return (0);
}



MAT	*mat_resize(MAT *A, unsigned int new_m, unsigned int new_n)
{
	if ( (new_m <= A->max_m ) && ( new_n <= A->max_n ) )
	{
		A->m = new_m;
		A->n = new_n;
		return A;
	}else{
		printf("Error: Can't resize matrix!\n");
		return A;
	}

}

/* m_resize -- returns the matrix A of size new_m x new_n; A is zeroed
   -- if A == NULL on entry then the effect is equivalent to matrix_get() */

MAT	*m_resize(MAT *A, unsigned int new_m, unsigned int new_n)
{
   unsigned int	i;
   unsigned int	new_max_m, new_max_n, new_size, old_m, old_n;
   
   if ((int)new_m < 0 || (int)new_n < 0)
     error(E_NEG,"m_resize");

   if ( ! A ){
	  return matrix_get(new_m,new_n);
   }

   /* nothing was changed */
   if (new_m == A->m && new_n == A->n)
     return A;

   old_m = A->m;	old_n = A->n;
   if ( new_m > A->max_m )
   {	/* re-allocate A->me */
      if (mem_info_is_on()) {
	 mem_bytes(TYPE_MAT,A->max_m*sizeof(MatrixReal *),
		      new_m*sizeof(MatrixReal *));
      }

      A->me = RENEW(A->me,new_m,MatrixReal *);
      if ( ! A->me )
	error(E_MEM,"m_resize");
   }
   new_max_m = mat_max(new_m,A->max_m);
   new_max_n = mat_max(new_n,A->max_n);
   
#ifndef SEGMENTED
   new_size = new_max_m*new_max_n;
   if ( new_size > A->max_size )
   {	/* re-allocate A->base */
      if (mem_info_is_on()) {
	 mem_bytes(TYPE_MAT,A->max_m*A->max_n*sizeof(MatrixReal),
		      new_size*sizeof(MatrixReal));
      }

      A->base = RENEW(A->base,new_size,MatrixReal);
      if ( ! A->base )
	error(E_MEM,"m_resize");
      A->max_size = new_size;
   }
   
   /* now set up A->me[i] */
   for ( i = 0; i < new_m; i++ )
     A->me[i] = &(A->base[i*new_n]);
   
   /* now shift data in matrix */
   if ( old_n > new_n )
   {
      for ( i = 1; i < mat_min(old_m,new_m); i++ )
	MEM_COPY((char *)&(A->base[i*old_n]),
		 (char *)&(A->base[i*new_n]),
		 sizeof(MatrixReal)*new_n);
   }
   else if ( old_n < new_n )
   {
      for ( i = (int)(mat_min(old_m,new_m))-1; i > 0; i-- )
      {   /* copy & then zero extra space */
	 MEM_COPY((char *)&(A->base[i*old_n]),
		  (char *)&(A->base[i*new_n]),
		  sizeof(MatrixReal)*old_n);
	 __zero__(&(A->base[i*new_n+old_n]),(new_n-old_n));
      }
      __zero__(&(A->base[old_n]),(new_n-old_n));
      A->max_n = new_n;
   }
   /* zero out the new rows.. */
   for ( i = old_m; i < new_m; i++ )
     __zero__(&(A->base[i*new_n]),new_n);
#else
   if ( A->max_n < new_n )
   {
      MatrixReal	*tmp;
      
      for ( i = 0; i < A->max_m; i++ )
      {
	 if (mem_info_is_on()) {
	    mem_bytes(TYPE_MAT,A->max_n*sizeof(MatrixReal),
			 new_max_n*sizeof(MatrixReal));
	 }	

	 if ( (tmp = RENEW(A->me[i],new_max_n,MatrixReal)) == NULL )
	   error(E_MEM,"m_resize");
	 else {	
	    A->me[i] = tmp;
	 }
      }
      for ( i = A->max_m; i < new_max_m; i++ )
      {
	 if ( (tmp = NEW_A(new_max_n,MatrixReal)) == NULL )
	   error(E_MEM,"m_resize");
	 else {
	    A->me[i] = tmp;

	    if (mem_info_is_on()) {
	       mem_bytes(TYPE_MAT,0,new_max_n*sizeof(MatrixReal));
	    }	    
	 }
      }
   }
   else if ( A->max_m < new_m )
   {
      for ( i = A->max_m; i < new_m; i++ ) 
	if ( (A->me[i] = NEW_A(new_max_n,MatrixReal)) == NULL )
	  error(E_MEM,"m_resize");
	else if (mem_info_is_on()) {
	   mem_bytes(TYPE_MAT,0,new_max_n*sizeof(MatrixReal));
	}
      
   }
   
   if ( old_n < new_n )
   {
      for ( i = 0; i < old_m; i++ )
	__zero__(&(A->me[i][old_n]),new_n-old_n);
   }
   
   /* zero out the new rows.. */
   for ( i = old_m; i < new_m; i++ )
     __zero__(A->me[i],new_n);
#endif
   
   A->max_m = new_max_m;
   A->max_n = new_max_n;
   A->max_size = A->max_m*A->max_n;
   A->m = new_m;	A->n = new_n;
   
   return A;
}

/* px_resize -- returns the permutation px with size new_size
   -- px is set to the identity permutation */

PERM	*px_resize(PERM *px, unsigned int new_size)
{
   unsigned int	i;
   
   if ((int)new_size < 0)
     error(E_NEG,"px_resize");

   if ( ! px )
     return px_get(new_size);
   
   /* nothing is changed */
   if (new_size == px->size)
     return px;

   if ( new_size > px->max_size )
   {
      if (mem_info_is_on()) {
	 mem_bytes(TYPE_PERM,px->max_size*sizeof(unsigned int),
		      new_size*sizeof(unsigned int));
      }
      px->pe = RENEW(px->pe,new_size,unsigned int);
      if ( ! px->pe )
	error(E_MEM,"px_resize");
      px->max_size = new_size;
   }
   if ( px->size <= new_size )
     /* extend permutation */
     for ( i = px->size; i < new_size; i++ )
       px->pe[i] = i;
   else
     for ( i = 0; i < new_size; i++ )
       px->pe[i] = i;
   
   px->size = new_size;
   
   return px;
}

VEC	*vec_resize(VEC *x, unsigned int new_dim)
{
	if (new_dim <= x->max_dim)
	{
		x->dim = new_dim;
		return x;
	}else{
		printf("Can't resize vector!\n");
		return x;
	}
}


/* v_resize -- returns the vector x with dim new_dim
   -- x is set to the zero vector */

VEC	*v_resize(VEC *x, unsigned int new_dim)
{
   
   if ((int)new_dim < 0)
     error(E_NEG,"v_resize");

   if ( ! x )
     return v_get(new_dim);

   /* nothing is changed */
   if (new_dim == x->dim)
     return x;

   if ( x->max_dim == 0 )	/* assume that it's from sub_vec */
     return v_get(new_dim);
   
   if ( new_dim > x->max_dim )
   {
      if (mem_info_is_on()) { 
	 mem_bytes(TYPE_VEC,x->max_dim*sizeof(MatrixReal),
			 new_dim*sizeof(MatrixReal));
      }

      x->ve = RENEW(x->ve,new_dim,MatrixReal);
      if ( ! x->ve )
	error(E_MEM,"v_resize");
      x->max_dim = new_dim;
   }
   
   if ( new_dim > x->dim )
     __zero__(&(x->ve[x->dim]),new_dim - x->dim);
   x->dim = new_dim;
   
   return x;
}




/* Varying number of arguments */
/* other functions of this type are in sparse.c and zmemory.c */

int v_get_vars(int dim,...) 
{
   va_list ap;
   int i=0;
   VEC **par;
   
   va_start(ap, dim);
   while ((par = va_arg(ap,VEC **))) {   /* NULL ends the list*/
      *par = v_get(dim);
      i++;
   } 

   va_end(ap);
   return i;
}


int iv_get_vars(int dim,...) 
{
   va_list ap;
   int i=0;
   IVEC **par;
   
   va_start(ap, dim);
   while ((par = va_arg(ap,IVEC **))) {   /* NULL ends the list*/
      *par = iv_get(dim);
      i++;
   } 

   va_end(ap);
   return i;
}

int m_get_vars(int m,int n,...) 
{
   va_list ap;
   int i=0;
   MAT **par;
   
   va_start(ap, n);
   while ((par = va_arg(ap,MAT **))) {   /* NULL ends the list*/
      *par = matrix_get(m,n);
      i++;
   } 

   va_end(ap);
   return i;
}

int px_get_vars(int dim,...) 
{
   va_list ap;
   int i=0;
   PERM **par;
   
   va_start(ap, dim);
   while ((par = va_arg(ap,PERM **))) {   /* NULL ends the list*/
      *par = px_get(dim);
      i++;
   } 

   va_end(ap);
   return i;
}


int v_resize_vars(int new_dim,...)
{
   va_list ap;
   int i=0;
   VEC **par;
   
   va_start(ap, new_dim);
   while ((par = va_arg(ap,VEC **))) {   /* NULL ends the list*/
      *par = v_resize(*par,new_dim);
      i++;
   } 

   va_end(ap);
   return i;
}



int iv_resize_vars(int new_dim,...) 
{
   va_list ap;
   int i=0;
   IVEC **par;
   
   va_start(ap, new_dim);
   while ((par = va_arg(ap,IVEC **))) {   /* NULL ends the list*/
      *par = iv_resize(*par,new_dim);
      i++;
   } 

   va_end(ap);
   return i;
}

int m_resize_vars(int m,int n,...) 
{
   va_list ap;
   int i=0;
   MAT **par;
   
   va_start(ap, n);
   while ((par = va_arg(ap,MAT **))) {   /* NULL ends the list*/
      *par = m_resize(*par,m,n);
      i++;
   } 

   va_end(ap);
   return i;
}


int px_resize_vars(int new_dim,...) 
{
   va_list ap;
   int i=0;
   PERM **par;
   
   va_start(ap, new_dim);
   while ((par = va_arg(ap,PERM **))) {   /* NULL ends the list*/
      *par = px_resize(*par,new_dim);
      i++;
   } 

   va_end(ap);
   return i;
}

int v_free_vars(VEC **pv,...)
{
   va_list ap;
   int i=1;
   VEC **par;
   
   v_free(*pv);
   *pv = VNULL;
   va_start(ap, pv);
   while ((par = va_arg(ap,VEC **))) {   /* NULL ends the list*/
      v_free(*par); 
      *par = VNULL;
      i++;
   } 

   va_end(ap);
   return i;
}


int iv_free_vars(IVEC **ipv,...)
{
   va_list ap;
   int i=1;
   IVEC **par;
   
   iv_free(*ipv);
   *ipv = IVNULL;
   va_start(ap, ipv);
   while ((par = va_arg(ap,IVEC **))) {   /* NULL ends the list*/
      iv_free(*par); 
      *par = IVNULL;
      i++;
   } 

   va_end(ap);
   return i;
}


int px_free_vars(PERM **vpx,...)
{
   va_list ap;
   int i=1;
   PERM **par;
   
   px_free(*vpx);
   *vpx = PNULL;
   va_start(ap, vpx);
   while ((par = va_arg(ap,PERM **))) {   /* NULL ends the list*/
      px_free(*par); 
      *par = PNULL;
      i++;
   } 

   va_end(ap);
   return i;
}

int m_free_vars(MAT **va,...)
{
   va_list ap;
   int i=1;
   MAT **par;
   
   matrix_free(*va);
   *va = MNULL;
   va_start(ap, va);
   while ((par = va_arg(ap,MAT **))) {   /* NULL ends the list*/
      matrix_free(*par); 
      *par = MNULL;
      i++;
   } 

   va_end(ap);
   return i;
}
