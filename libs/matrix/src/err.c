/*
  File with basic error-handling operations
*/


#include	<stdio.h>
#include	<setjmp.h>
#include	<ctype.h>
#include  "..//include//err.h"


#ifdef SYSV
/* AT&T System V */
#include	<sys/signal.h>
#else
/* something else -- assume BSD or ANSI C */
#include	<signal.h>
#endif



#define		FALSE	0
#define		TRUE	1

#define	EF_EXIT		0
#define	EF_ABORT	1
#define	EF_JUMP		2
#define	EF_SILENT	3

/* The only error caught in this file! */

static	char	*err_mesg[] =
{	  "unknown error",			                            /* 0 */
	  "sizes of objects don't match",	                    /* 1 */
	  "index out of bounds",		                        /* 2 */
	  "can't allocate memory",		                        /* 3 */
	  "singular matrix",			                        /* 4 */
	  "matrix not positive definite",	                    /* 5 */
	  "incorrect format input",		                        /* 6 */
	  "bad input file/device",		                        /* 7 */
	  "NULL objects passed",		                        /* 8 */
	  "matrix not square",			                        /* 9 */
	  "object out of range",		                        /* 10 */
	  "can't do operation in situ for non-square matrix",   /* 11 */
	  "can't do operation in situ",		                    /* 12 */
	  "excessive number of iterations",	                    /* 13 */
	  "convergence criterion failed",	                    /* 14 */
	  "bad starting value",			                        /* 15 */
	  "floating exception",			                        /* 16 */
	  "internal inconsistency (data structure)",            /* 17 */
	  "unexpected end-of-file",		                        /* 18 */
	  "shared vectors (cannot release them)",               /* 19 */  
	  "negative argument",			                        /* 20 */
	  "cannot overwrite object",                            /* 21 */
	  "breakdown in iterative method"                       /* 22 */
	 };

#define	MAXERR	(sizeof(err_mesg)/sizeof(char *))                   //макс.колл. ошибок

static char *warn_mesg[] = {                                        // ---//--- предупреждений
   "unknown warning_matrix",				                        /* 0 */
   "wrong type number (use macro TYPE_*)",	                /* 1 */
   "no corresponding mem_stat_mark",	                    /* 2 */
   "computed norm of a residual is less than 0",            /* 3 */
   "resizing a shared vector"			                    /* 4 */
};

#define MAXWARN  (sizeof(warn_mesg)/sizeof(char *))                  



#define	MAX_ERRS	100

#ifdef NO_THREAD_SECURITY
jmp_buf restart;
#elif _WIN32
__declspec(thread) jmp_buf	restart;
#else
__thread jmp_buf	restart;
#endif


/* array of pointers to lists of errors */

typedef struct {
   char **listp;    /* pointer to a list of errors */          // указатель на лист ошибок
   unsigned len;    /* length of the list */                   
   unsigned warn;   /* =FALSE - errors, =TRUE - warning_matrixs */
}  Err_list;

static Err_list     err_list[ERR_LIST_MAX_LEN] = {
 {err_mesg,MAXERR,FALSE},	/* basic errors list */
 {warn_mesg,MAXWARN,TRUE}	/* basic warning_matrixs list */
};


static int err_list_end = 2;   /* number of elements in err_list */

/* attach a new list of errors pointed by err_ptr
   or change a previous one;
   list_len is the number of elements in the list;
   list_num is the list number;
   warn == FALSE - errors (stop the program),
   warn == TRUE - warning_matrixs (continue the program);
   Note: lists numbered 0 and 1 are attached automatically,
   you do not need to do it
   */
int err_list_attach(int list_num, int list_len, char **err_ptr, int warn)
{
   if (list_num < 0 || list_len <= 0 ||
       err_ptr == (char **)NULL) 
     return -1;
   
   if (list_num >= ERR_LIST_MAX_LEN) {
	fprintf(stderr,"\n file \"%s\": %s %s\n",
		"err.c","increase the value of ERR_LIST_MAX_LEN",
		"in matrix.h and zmatdef.h");
	if ( ! isatty(fileno(stdout)) )
	  fprintf(stderr,"\n file \"%s\": %s %s\n",
		  "err.c","increase the value of ERR_LIST_MAX_LEN",
		  "in matrix.h and zmatdef.h");
	printf("Exiting program\n");
	//exit(0);
  return 0;
     }

   if (err_list[list_num].listp != (char **)NULL &&
       err_list[list_num].listp != err_ptr)
     free((char *)err_list[list_num].listp);
   err_list[list_num].listp = err_ptr;
   err_list[list_num].len = list_len;
   err_list[list_num].warn = warn;
   err_list_end = list_num+1;
   
   return list_num;
}


/* release the error list numbered list_num */
int err_list_free(int list_num)
{
   if (list_num < 0 || list_num >= err_list_end) return -1;
   if (err_list[list_num].listp != (char **)NULL) {
      err_list[list_num].listp = (char **)NULL;
      err_list[list_num].len = 0;
      err_list[list_num].warn = 0;
   }
   return 0;
}


/* check if list_num is attached;
   return FALSE if not;
   return TRUE if yes
   */
int err_is_list_attached(int list_num)
{
   if (list_num < 0 || list_num >= err_list_end)
     return FALSE;
   
   if (err_list[list_num].listp != (char **)NULL)
     return TRUE;
   
   return FALSE;
}

/* other local variables */

static	int	err_flag = EF_EXIT, /*num_errs = 0,*/ cnt_errs = 1;
#ifdef _DEBUG
static	int	num_errs = 0;
#endif

/* set_err_flag -- sets err_flag -- returns old err_flag */

int	set_err_flag(int flag)
{
   int	tmp;
   
   tmp = err_flag;
   err_flag = flag;
   return tmp;
}

/* count_errs -- sets cnt_errs (TRUE/FALSE) & returns old value */

int	count_errs(int flag)
{
   int	tmp;
   
   tmp = cnt_errs;
   cnt_errs = flag;
   return tmp;
}

/* ev_err -- reports error (err_num) in file "file" at line "line_num" and
   returns to user error handler;
   list_num is an error list number (0 is the basic list 
   pointed by err_mesg, 1 is the basic list of warning_matrixs)
 */
int	ev_err(const char *file, int err_num, int line_num,
	       const char *fn_name, int list_num)
{
#ifdef _DEBUG
   unsigned int	num;
   
   if ( err_num < 0 ) err_num = 0;
   
   if (list_num < 0 || list_num >= err_list_end ||
       err_list[list_num].listp == (char **)NULL) {
      fprintf(stderr,
	      "\n Not (properly) attached list of errors: list_num = %d\n",
	      list_num);
      fprintf(stderr," Call \"err_list_attach\" in your program\n");
      if ( ! isatty(fileno(stdout)) ) {
	 fprintf(stderr,
		 "\n Not (properly) attached list of errors: list_num = %d\n",
		 list_num);
	 fprintf(stderr," Call \"err_list_attach\" in your program\n");
      }
      printf("\nExiting program\n");
      //exit(0);
      return 0;
   }
   
   num = err_num;
   if ( num >= err_list[list_num].len ) num = 0;
   
   if ( cnt_errs && ++num_errs >= MAX_ERRS )	/* too many errors */
   {
	      fprintf(stderr,"\n\"%s\", line %d: %s in function %s()\n",
	      file,line_num,err_list[list_num].listp[num],
	      isascii(*fn_name) ? fn_name : "???");
      if ( ! isatty(fileno(stdout)) )
	fprintf(stdout,"\n\"%s\", line %d: %s in function %s()\n",
		file,line_num,err_list[list_num].listp[num],
		isascii(*fn_name) ? fn_name : "???");
      printf("Sorry, too many errors: %d\n",num_errs);
      printf("Exiting program\n");
      //exit(0);
      return 0;
   }
   if ( err_list[list_num].warn )
       switch ( err_flag )
       {
	   case EF_SILENT: break;
	   default:
	   fprintf(stderr,"\n\"%s\", line %d: %s in function %s()\n\n",
		   file,line_num,err_list[list_num].listp[num],
		   isascii(*fn_name) ? fn_name : "???");
	   if ( ! isatty(fileno(stdout)) )
	       fprintf(stdout,"\n\"%s\", line %d: %s in function %s()\n\n",
		       file,line_num,err_list[list_num].listp[num],
		       isascii(*fn_name) ? fn_name : "???");
	   break;
       }
   else
       switch ( err_flag )
       {
	   case EF_SILENT:
	   longjmp(restart,(err_num==0)? -1 : err_num);
	   break;
	   case EF_ABORT:
	   fprintf(stderr,"\n\"%s\", line %d: %s in function %s()\n",
		   file,line_num,err_list[list_num].listp[num],
		   isascii(*fn_name) ? fn_name : "???");
	   if ( ! isatty(fileno(stdout)) )
	       fprintf(stdout,"\n\"%s\", line %d: %s in function %s()\n",
		       file,line_num,err_list[list_num].listp[num],
		       isascii(*fn_name) ? fn_name : "???");
	   abort();
	   break;
	   case EF_JUMP:
	   fprintf(stderr,"\n\"%s\", line %d: %s in function %s()\n",
		   file,line_num,err_list[list_num].listp[num],
		   isascii(*fn_name) ? fn_name : "???");
	   if ( ! isatty(fileno(stdout)) )
	       fprintf(stdout,"\n\"%s\", line %d: %s in function %s()\n",
		       file,line_num,err_list[list_num].listp[num],
		       isascii(*fn_name) ? fn_name : "???");
	   longjmp(restart,(err_num==0)? -1 : err_num);
	   break;
	   default:
	   fprintf(stderr,"\n\"%s\", line %d: %s in function %s()\n\n",
		   file,line_num,err_list[list_num].listp[num],
		   isascii(*fn_name) ? fn_name : "???");
	   if ( ! isatty(fileno(stdout)) )
	       fprintf(stdout,"\n\"%s\", line %d: %s in function %s()\n\n",
		       file,line_num,err_list[list_num].listp[num],
		       isascii(*fn_name) ? fn_name : "???");
	   
	   break;
       }
   
   /* ensure exit if fall through */
   if ( ! err_list[list_num].warn )  /*exit(0)*/return 0;
#else
   (void)file;
   (void)err_num;
   (void)line_num;
   (void)fn_name;
   (void)list_num;
#endif

   return 0;
}

