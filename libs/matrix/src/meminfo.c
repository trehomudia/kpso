
#include	<stdio.h>
#include  "..//include//matrix.h"
#include	"..//include//meminfo.h"
#include "..//include//err.h"

/* this array is defined further in this file */
extern MEM_CONNECT mem_connect[MEM_CONNECT_MAX_LISTS];


/* names of types */
static char *mem_type_names[] = {
   "MAT",
   "PERM",
   "VEC",
   "IVEC"
};


#define MEM_NUM_STD_TYPES  (sizeof(mem_type_names)/sizeof(mem_type_names[0]))


/* local array for keeping track of memory */
static MEM_ARRAY   mem_info_sum[MEM_NUM_STD_TYPES];  


/* for freeing various types */
static int (*mem_free_funcs[MEM_NUM_STD_TYPES])() = {
   matrix_free,
   px_free,    
   v_free,	
   iv_free
};



/* it is a global variable for passing 
   pointers to local arrays defined here */
MEM_CONNECT mem_connect[MEM_CONNECT_MAX_LISTS] = {
 { mem_type_names, mem_free_funcs, MEM_NUM_STD_TYPES, 
     mem_info_sum } 
};

/* attach a new list of types */
int mem_attach_list(int list, int ntypes, 
		    char *type_names[], 
		    int (*free_funcs[])(), 
		    MEM_ARRAY info_sum[])
{
   if (list < 0 || list >= MEM_CONNECT_MAX_LISTS)
     return -1;

   if (type_names == NULL || free_funcs == NULL 
       || info_sum == NULL || ntypes < 0)
     return -1;
   
   /* if a list exists do not overwrite */
   if ( mem_connect[list].ntypes != 0 )
     error(E_OVERWRITE,"mem_attach_list");
   
   mem_connect[list].ntypes = ntypes;
   mem_connect[list].type_names = type_names;
   mem_connect[list].free_funcs = free_funcs;
   mem_connect[list].info_sum = info_sum;
   return 0;
}


/* release a list of types */
int mem_free_vars(int list)
{	
   if (list < 0 || list >= MEM_CONNECT_MAX_LISTS)
     return -1;
   
   mem_connect[list].ntypes = 0;
   mem_connect[list].type_names = NULL;
   mem_connect[list].free_funcs = NULL;
   mem_connect[list].info_sum = NULL;
   
   return 0;
}



/* check if list is attached */
int mem_is_list_attached(int list)
{
   if ( list < 0 || list >= MEM_CONNECT_MAX_LISTS )
   return FALSE;

   if ( mem_connect[list].type_names != NULL &&
        mem_connect[list].free_funcs != NULL &&
        mem_connect[list].info_sum != NULL)
     return TRUE;
   else return FALSE;
}

/* to print out the contents of mem_connect[list] */

/*=============================================================*/

/* local variables */

static int	mem_switched_on = MEM_SWITCH_ON_DEF;  /* on/off */


/* switch on/off memory info */
int mem_info_on(int sw)
{
   int old = mem_switched_on;
   
   mem_switched_on = sw;
   return old;
}

int mem_info_is_on(void)
{
   return mem_switched_on;
}


/* information about allocated memory */

/* return the number of allocated bytes for type 'type' */

long mem_info_bytes(unsigned int type, unsigned int list)
{
   if ( (int)list < 0 || list >= MEM_CONNECT_MAX_LISTS )
     return 0l;
   if ( !mem_switched_on || (int)type < 0
       || type >= mem_connect[list].ntypes
       || mem_connect[list].free_funcs[type] == NULL )
     return 0l;
   
   return mem_connect[list].info_sum[type].bytes;
}

/* return the number of allocated variables for type 'type' */
int mem_info_numvar(unsigned int type, unsigned int list)
{
   if ( (int)list < 0 || list >= MEM_CONNECT_MAX_LISTS )
     return 0l;
   if ( !mem_switched_on || (int)type < 0
       || type >= mem_connect[list].ntypes
       || mem_connect[list].free_funcs[type] == NULL )
     return 0l;
   
   return mem_connect[list].info_sum[type].numvar;
}


#ifndef MEX

/* print out memory info to the file fp */

void mem_info_file(FILE *fp, int list)
{
   unsigned int type;
   long t = 0l, d;
   int n = 0, nt = 0;
   MEM_CONNECT *mlist;
   
   if (!mem_switched_on) return;
   if ( list < 0 || list >= MEM_CONNECT_MAX_LISTS )
     return;
   
   if (list == 0)
     fprintf(fp," MEMORY INFORMATION (standard types):\n");
   else
     fprintf(fp," MEMORY INFORMATION (list no. %d):\n",list);

   mlist = &mem_connect[list];

   for (type=0; type < mlist->ntypes; type++) {
      if (mlist->type_names[type] == NULL ) continue;
      d = mlist->info_sum[type].bytes;
      t += d;
      n = mlist->info_sum[type].numvar;
      nt += n;
      fprintf(fp," type %-7s %10ld alloc. byte%c  %6d alloc. variable%c\n",
	      mlist->type_names[type], d, (d!=1 ? 's' : ' '),
	      n, (n!=1 ? 's' : ' '));
   }

   fprintf(fp," %-12s %10ld alloc. byte%c  %6d alloc. variable%c\n\n",
	   "total:",t, (t!=1 ? 's' : ' '),
	   nt, (nt!=1 ? 's' : ' '));
}
#endif


/* function for memory information */


/* mem_bytes_list
   
   Arguments:
   type - the number of type;
   old_size - old size of allocated memory (in bytes);
   new_size - new size of allocated memory (in bytes);
   list - list of types
   */

void mem_bytes_list(unsigned int type, unsigned int old_size, unsigned int new_size, unsigned int list)
{
   MEM_CONNECT *mlist;
   
   if ( (int)list < 0 || list >= MEM_CONNECT_MAX_LISTS )
     return;
   
   mlist = &mem_connect[list];
   if (  (int)type < 0 || type >= mlist->ntypes
       || mlist->free_funcs[type] == NULL )
     return;

   if ( (int)old_size < 0 || (int)new_size < 0 )
     return;

   mlist->info_sum[type].bytes += new_size - old_size;
}



/* mem_numvar_list
   
   Arguments:
   type - the number of type;
   num - # of variables allocated (> 0) or deallocated ( < 0)
   list - list of types
   */


void mem_numvar_list(unsigned int type, unsigned int num, unsigned int list)
{
   MEM_CONNECT *mlist;
   
   if ( (int)list < 0 || list >= MEM_CONNECT_MAX_LISTS )
     return;
   
   mlist = &mem_connect[list];
   if (  (int)type < 0 || type >= mlist->ntypes
       || mlist->free_funcs[type] == NULL )
     return;

   mlist->info_sum[type].numvar += num;
}

