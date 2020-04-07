//main.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum scheme_port_kind {
	port_free = 0,
	port_file = 1,
	port_string = 2,
	port_input = 16,
	port_output = 32
};

typedef struct port {
	unsigned char kind;
	union {
		struct {
			FILE *file;
			int closeit;
		} stdio;
		struct {
			char *start;
			char *past_the_end;
			char *curr;
		} string;
	} rep;
} port;

typedef struct num {
	char is_fixnum;
	union {
		int64_t ivalue;
		double rvalue;
	} value;
} num;


typedef struct scheme scheme;
typedef struct cell *pointer;

typedef void * (*func_alloc)(size_t);
typedef void(*func_dealloc)(void *);

/* cell structure */
struct cell {
	unsigned int _flag;
	union {
		struct {
			char   *_svalue;
			int   _length;
		} _string;
		num _number;
		port *_port;
		//foreign_func _ff;
		struct {
			struct cell *_car;
			struct cell *_cdr;
		} _cons;
	} _object;
};


struct scheme {
	/* arrays for segments */
	func_alloc malloc;
	func_dealloc free;

	/* return code */
	int retcode;
	int tracing;

#define CELL_SEGSIZE    5000  /* # of cells in one segment */
#define CELL_NSEGMENT   10    /* # of segments for cells */
	char *alloc_seg[CELL_NSEGMENT];
	pointer cell_seg[CELL_NSEGMENT];
	int     last_cell_seg;

	/* We use 4 registers. */
	pointer args;            /* register for arguments of function */
	pointer envir;           /* stack register for current environment */
	pointer code;            /* register for current code */
	pointer dump;            /* stack register for next evaluation */

	int interactive_repl;    /* are we in an interactive REPL? */

	struct cell _sink;
	pointer sink;            /* when mem. alloc. fails */
	struct cell _NIL;
	pointer NIL;             /* special cell representing empty cell */
	struct cell _HASHT;
	pointer T;               /* special cell representing #t */
	struct cell _HASHF;
	pointer F;               /* special cell representing #f */
	struct cell _EOF_OBJ;
	pointer EOF_OBJ;         /* special cell representing end-of-file object */
	pointer oblist;          /* pointer to symbol table */
	pointer global_env;      /* pointer to global environment */

	/* global pointers to special symbols */
	pointer LAMBDA;               /* pointer to syntax lambda */
	pointer QUOTE;           /* pointer to syntax quote */

	pointer QQUOTE;               /* pointer to symbol quasiquote */
	pointer UNQUOTE;         /* pointer to symbol unquote */
	pointer UNQUOTESP;       /* pointer to symbol unquote-splicing */
	pointer FEED_TO;         /* => */
	pointer COLON_HOOK;      /* *colon-hook* */
	pointer ERROR_HOOK;      /* *error-hook* */
	pointer SHARP_HOOK;  /* *sharp-hook* */

	pointer free_cell;       /* pointer to top of free cells */
	long    fcells;          /* # of free cells */

	pointer inport;
	pointer outport;
	pointer save_inport;
	pointer loadport;

#define MAXFIL 64
	port load_stack[MAXFIL];     /* Stack of open files for port -1 (LOADing) */
	int nesting_stack[MAXFIL];
	int file_i;
	int nesting;

	char    gc_verbose;      /* if gc_verbose is not zero, print gc status */
	char    no_memory;       /* Whether mem. alloc. has failed */

#define LINESIZE 1024
	char    linebuff[LINESIZE];
	char    strbuff[256];

	FILE *tmpfp;
	int tok;
	int print_flag;
	pointer value;
	int op;

	void *ext_data;     /* For the benefit of foreign functions */
	long gensym_cnt;

	struct scheme_interface *vptr;
	void *dump_base;	 /* pointer to base of allocated dump stack */
	int dump_size;		 /* number of frames allocated for dump stack */
};

typedef pointer (*dispatch_func)(scheme *, enum scheme_opcodes);

typedef struct {
  dispatch_func func;
  char *name;
  int min_arity;
  int max_arity;
  char *arg_tests_encoding;
} op_code_info;

#define INF_ARG 0xffff

pointer opexe_2(scheme *sc, enum scheme_opcodes op);
pointer opexe_5(scheme *sc, enum scheme_opcodes op);

#define TST_NONE 0
#define TST_ANY "\001"
#define TST_STRING "\002"
#define TST_SYMBOL "\003"
#define TST_PORT "\004"
#define TST_INPORT "\005"
#define TST_OUTPORT "\006"
#define TST_ENVIRONMENT "\007"
#define TST_PAIR "\010"
#define TST_LIST "\011"
#define TST_CHAR "\012"
#define TST_VECTOR "\013"
#define TST_NUMBER "\014"
#define TST_INTEGER "\015"
#define TST_NATURAL "\016"



#ifndef _OP_DEF

static op_code_info dispatch_table[]= {
#define _OP_DEF(A,B,C,D,E,OP) {A,B,C,D,E},
	#include "opdefines.h"
  { 0 }
};

#endif
#undef _OP_DEF

enum scheme_opcodes {
#define _OP_DEF(A,B,C,D,E,OP) OP,
#include "opdefines.h"
	OP_MAXDEFINED
};




pointer opexe_2(scheme *sc, enum scheme_opcodes op) {
	pointer x;
	num v;
#if USE_MATH
	double dd;
#endif

	switch (op) {

	case OP_EXP:    /* inexact->exact */
	{
		printf("you made it\n");
	}

	break;

	default:
		//sprintf(sc->strbuff, "%d: illegal operator", sc->op);
		//Error_0(sc, sc->strbuff);
		exit(1);
	}
	return sc->T;
}




int main()
{
	//dispatch_table[]
	return 0;
}