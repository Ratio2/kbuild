/*
 * This file was generated by the mkinit program.
 */

#include "shell.h"
#include "mystring.h"
#include "init.h"
#include "eval.h"
#include <stdio.h>
#include "input.h"
#include "error.h"
#include <stdlib.h>
#include "options.h"
#include "output.h"
#include "memalloc.h"
#include "redir.h"
#include <signal.h>
#include "trap.h"
#include "var.h"



#undef  ATABSIZE
#define ATABSIZE 39
#undef  MAXPWD
#define MAXPWD 256
#undef  ALL
#define ALL (E_OPEN|E_CREAT|E_EXEC)
#undef  EV_EXIT
#define EV_EXIT 01		/* exit after evaluating tree */
#undef  EV_TESTED
#define EV_TESTED 02		/* exit status is checked; ignore -e flag */
#undef  EV_BACKCMD
#define EV_BACKCMD 04		/* command executing within back quotes */
#undef  CMDTABLESIZE
#define CMDTABLESIZE 31		/* should be prime */
#undef  ARB
#define ARB 1			/* actual size determined at run time */
#undef  NEWARGS
#define NEWARGS 5
#undef  MAXHISTLOOPS
#define MAXHISTLOOPS	4	/* max recursions through fc */
#undef  DEFEDITOR
#define DEFEDITOR	"ed"	/* default editor *should* be $EDITOR */
#undef  editing
#define editing (Eflag(psh) || Vflag(psh))
#undef  EOF_NLEFT
#define EOF_NLEFT -99		/* value of parsenleft when EOF pushed back */
#undef  MAXMBOXES
#define MAXMBOXES 10
#undef  PROFILE
#define PROFILE 0
#undef  SIGSSIZE
#define SIGSSIZE (sizeof(sigs)/sizeof(sigs[0]))
#undef  MINSIZE
#define MINSIZE 504		/* minimum size of a block */
#undef  DEFINE_OPTIONS
#define DEFINE_OPTIONS
#undef  OUTBUFSIZ
#define OUTBUFSIZ BUFSIZ
#undef  BLOCK_OUT
#define BLOCK_OUT -2		/* output to a fixed block of memory */
#undef  MEM_OUT
#define MEM_OUT -3		/* output to dynamically allocated memory */
#undef  OUTPUT_ERR
#define OUTPUT_ERR 01		/* error occurred on output */
#undef  TEMPSIZE
#define TEMPSIZE 24
#undef  HAVE_VASPRINTF
#define HAVE_VASPRINTF 1
#undef  EOFMARKLEN
#define EOFMARKLEN 79
#undef  OPENBRACE
#define OPENBRACE '{'
#undef  CLOSEBRACE
#define CLOSEBRACE '}'
#undef  EMPTY
#define EMPTY -2		/* marks an unused slot in redirtab */
#undef  S_DFL
#define S_DFL 1			/* default signal handling (SIG_DFL) */
#undef  S_CATCH
#define S_CATCH 2		/* signal is caught */
#undef  S_IGN
#define S_IGN 3			/* signal is ignored (SIG_IGN) */
#undef  S_HARD_IGN
#define S_HARD_IGN 4		/* signal is ignored permenantly */
#undef  S_RESET
#define S_RESET 5		/* temporary - to reset a hard ignored sig */
#undef  INCL_BASE
#define INCL_BASE
#undef  LIBPATHSTRICT
#define LIBPATHSTRICT 3
#undef  QHINF_EXEINFO
#define QHINF_EXEINFO       1 /* NE exeinfo. */
#undef  QHINF_READRSRCTBL
#define QHINF_READRSRCTBL   2 /* Reads from the resource table. */
#undef  QHINF_READFILE
#define QHINF_READFILE      3 /* Reads from the executable file. */
#undef  QHINF_LIBPATHLENGTH
#define QHINF_LIBPATHLENGTH 4 /* Gets the libpath length. */
#undef  QHINF_LIBPATH
#define QHINF_LIBPATH       5 /* Gets the entire libpath. */
#undef  QHINF_FIXENTRY
#define QHINF_FIXENTRY      6 /* NE only */
#undef  QHINF_STE
#define QHINF_STE           7 /* NE only */
#undef  QHINF_MAPSEL
#define QHINF_MAPSEL        8 /* NE only */
#undef  VTABSIZE
#define VTABSIZE 39
#undef  VTABSIZE
#define VTABSIZE 517
#undef  main
#define main echocmd
#undef  main
#define main killcmd



extern void rmaliases(struct shinstance *);

extern int loopnest;		/* current loop nesting level */

extern void deletefuncs(struct shinstance *);
extern void hash_special_builtins(struct shinstance *);

struct strpush {
	struct strpush *prev;	/* preceding string on stack */
	char *prevstring;
	int prevnleft;
	int prevlleft;
	struct alias *ap;	/* if push was associated with an alias */
};

struct parsefile {
	struct parsefile *prev;	/* preceding file on stack */
	int linno;		/* current line */
	int fd;			/* file descriptor (or -1 if string) */
	int nleft;		/* number of chars left in this line */
	int lleft;		/* number of chars left in this buffer */
	char *nextc;		/* next char in buffer */
	char *buf;		/* input buffer */
	struct strpush *strpush; /* for pushing strings at this level */
	struct strpush basestrpush; /* so pushing one is fast */
};

extern int parselleft;		/* copy of parsefile->lleft */
extern struct parsefile basepf;	/* top level input file */
extern char basebuf[BUFSIZ];	/* buffer for top level input file */

extern pid_t backgndpid;	/* pid of last background process */
extern int jobctl;

extern int tokpushback;		/* last token pushed back */
extern int checkkwd;            /* 1 == check for kwds, 2 == also eat newlines */

struct redirtab {
	struct redirtab *next;
	short renamed[10];
};

extern struct redirtab *redirlist;

extern char sigmode[NSIG];	/* current value of signal */

extern char **environ;



/*
 * Initialization code.
 */

void
init(shinstance *psh) {

      /* from exec.c: */
      {
	      hash_special_builtins();
      }

      /* from input.c: */
      {
	      basepf.nextc = basepf.buf = basebuf;
      }

      /* from var.c: */
      {
	      char **envp;

	      initvar(psh);
	      for (envp = environ ; *envp ; envp++) {
		      if (strchr(*envp, '=')) {
			      setvareq(psh, *envp, VEXPORT|VTEXTFIXED);
		      }
	      }
      }
}



/*
 * This routine is called when an error or an interrupt occurs in an
 * interactive shell and control is returned to the main command loop.
 */

void
reset(shinstance *psh) {

      /* from eval.c: */
      {
	      psh->evalskip = 0;
	      psh->loopnest = 0;
	      psh->funcnest = 0;
      }

      /* from input.c: */
      {
	      if (psh->exception != EXSHELLPROC)
		      psh->parselleft = psh->parsenleft = 0;	/* clear input buffer */
	      popallfiles(psh);
      }

      /* from output.c: */
      {
	      psh->out1 = &psh->output;
	      psh->out2 = &psh->errout;
	      if (memout.buf != NULL) {
		      ckfree(memout.buf);
		      memout.buf = NULL;
	      }
      }

      /* from parser.c: */
      {
	      psh->tokpushback = 0;
	      psh->checkkwd = 0;
      }

      /* from redir.c: */
      {
	      while (psh->redirlist)
		      popredir(psh);
      }
}



/*
 * This routine is called to initialize the shell to run a shell procedure.
 */

void
initshellproc(shinstance *psh) {

      /* from alias.c: */
      {
	      rmaliases(psh);
      }

      /* from eval.c: */
      {
	      psh->exitstatus = 0;
      }

      /* from exec.c: */
      {
	      deletefuncs(psh);
      }

      /* from input.c: */
      {
	      popallfiles(psh);
      }

      /* from jobs.c: */
      {
	      psh->backgndpid = -1;
#if JOBS
	      jobctl = 0;
#endif
      }

      /* from options.c: */
      {
	      int i;

	      for (i = 0; optlist[i].name; i++)
		      optlist[i].val = 0;
	      optschanged(psh);

      }

      /* from redir.c: */
      {
	      clearredir(psh, 0);
      }

      /* from trap.c: */
      {
	      char *sm;

	      clear_traps(psh, 0);
	      for (sm = psh->sigmode ; sm < psh->sigmode + NSIG ; sm++) {
		      if (*sm == S_IGN)
			      *sm = S_HARD_IGN;
	      }
      }

      /* from var.c: */
      {
	      shprocvar(psh);
      }
}
