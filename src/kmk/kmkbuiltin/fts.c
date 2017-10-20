/*	$NetBSD: __fts13.c,v 1.44 2005/01/19 00:59:48 mycroft Exp $	*/

/*-
 * Copyright (c) 1990, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#if HAVE_NBTOOL_CONFIG_H
#include "nbtool_config.h"
#endif

/*#include <sys/cdefs.h>*/
#if defined(LIBC_SCCS) && !defined(lint)
#if 0
static char sccsid[] = "@(#)fts.c	8.6 (Berkeley) 8/14/94";
#else
__RCSID("$NetBSD: __fts13.c,v 1.44 2005/01/19 00:59:48 mycroft Exp $");
#endif
#endif /* LIBC_SCCS and not lint */

#include "config.h"
/*#include "namespace.h"*/
#ifndef _MSC_VER
#include <sys/param.h>
#endif
#include <sys/stat.h>

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include "ftsfake.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef HAVE_ALLOCA_H
# include <alloca.h>
#endif
#include "kmkbuiltin.h" /* MAXPATHLEN for GNU/hurd */

#ifdef __sun__
# include "solfakes.h"
# ifndef __XOPEN_OR_POSIX
#  define dirfd(dir) ((dir)->dd_fd)
#else
#  define dirfd(dir) ((dir)->d_fd)
#endif
#endif
#ifdef _MSC_VER
# include "mscfakes.h"
# define dirfd(dir) -1
#endif

#if ! HAVE_NBTOOL_CONFIG_H
# if !defined(__sun__) && !defined(__gnu_linux__) && !defined(__HAIKU__)
#  define HAVE_STRUCT_DIRENT_D_NAMLEN 1
# endif
#endif

#if 0
#ifdef __weak_alias
#ifdef __LIBC12_SOURCE__
__weak_alias(fts_children,_fts_children)
__weak_alias(fts_close,_fts_close)
__weak_alias(fts_open,_fts_open)
__weak_alias(fts_read,_fts_read)
__weak_alias(fts_set,_fts_set)
#endif /* __LIBC12_SOURCE__ */
#endif /* __weak_alias */
#endif

#ifdef __LIBC12_SOURCE__
#define	STAT	stat12
#else
#define	STAT	stat
#endif

#ifdef __LIBC12_SOURCE__
__warn_references(fts_children,
    "warning: reference to compatibility fts_children();"
    " include <fts.h> for correct reference")
__warn_references(fts_close,
    "warning: reference to compatibility fts_close();"
    " include <fts.h> for correct reference")
__warn_references(fts_open,
    "warning: reference to compatibility fts_open();"
    " include <fts.h> for correct reference")
__warn_references(fts_read,
    "warning: reference to compatibility fts_read();"
    " include <fts.h> for correct reference")
__warn_references(fts_set,
    "warning: reference to compatibility fts_set();"
    " include <fts.h> for correct reference")
#endif

static FTSENT	*fts_alloc(FTS *, const char *, size_t);
static FTSENT	*fts_build(FTS *, int);
static void	 fts_lfree(FTSENT *);
static void	 fts_load(FTS *, FTSENT *);
static size_t	 fts_maxarglen(char * const *);
static size_t	 fts_pow2(size_t);
static int	 fts_palloc(FTS *, size_t);
static void	 fts_padjust(FTS *, FTSENT *);
static FTSENT	*fts_sort(FTS *, FTSENT *, size_t);
static u_short	 fts_stat(FTS *, FTSENT *, int);
#ifdef _MSC_VER
static u_short   fts_stat_dirent(FTS *sp, FTSENT *p, int follow, struct dirent *pDirEnt);
#endif
static int	 fts_safe_changedir(const FTS *, const FTSENT *, int,
    const char *);

#ifdef _MSC_VER
# undef HAVE_FCHDIR
#endif

#if defined(__EMX__) || defined(_MSC_VER)
# define NEED_STRRSLASH
# define IS_SLASH(ch)   ( (ch) == '/' || (ch) == '\\' )
#else
# define HAVE_FCHDIR
# define IS_SLASH(ch)   ( (ch) == '/' )
#endif

#define	ISDOT(a)	(a[0] == '.' && (!a[1] || (a[1] == '.' && !a[2])))

#define	CLR(opt)	(sp->fts_options &= ~(opt))
#define	ISSET(opt)	(sp->fts_options & (opt))
#define	SET(opt)	(sp->fts_options |= (opt))

#define	CHDIR(sp, path)	(!ISSET(FTS_NOCHDIR) && chdir(path))
#ifdef HAVE_FCHDIR
#define	FCHDIR(sp, fd)	(!ISSET(FTS_NOCHDIR) && fchdir(fd))
#else
#define	FCHDIR(sp, rdir) CHDIR(sp, rdir)
#endif


/* fts_build flags */
#define	BCHILD		1		/* fts_children */
#define	BNAMES		2		/* fts_children, names only */
#define	BREAD		3		/* fts_read */

#ifndef DTF_HIDEW
#undef FTS_WHITEOUT
#endif

#ifndef _DIAGASSERT
#define _DIAGASSERT assert
#endif


FTS *
fts_open(argv, options, compar)
	char * const *argv;
	int options;
	int (*compar)(const FTSENT **, const FTSENT **);
{
	FTS *sp;
	FTSENT *p, *root;
	size_t nitems;
	FTSENT *parent, *tmp = NULL;	/* pacify gcc */
	size_t len;

	_DIAGASSERT(argv != NULL);

	/* Options check. */
	if (options & ~FTS_OPTIONMASK) {
		errno = EINVAL;
		return (NULL);
	}

	/* Allocate/initialize the stream */
	if ((sp = malloc((u_int)sizeof(FTS))) == NULL)
		return (NULL);
	memset(sp, 0, sizeof(FTS));
	sp->fts_compar = compar;
	sp->fts_options = options;

	/* Logical walks turn on NOCHDIR; symbolic links are too hard. */
	if (ISSET(FTS_LOGICAL))
		SET(FTS_NOCHDIR);

	/*
	 * Start out with 1K of path space, and enough, in any case,
	 * to hold the user's paths.
	 */
	if (fts_palloc(sp, MAX(fts_maxarglen(argv), MAXPATHLEN)))
		goto mem1;

	/* Allocate/initialize root's parent. */
	if ((parent = fts_alloc(sp, "", 0)) == NULL)
		goto mem2;
	parent->fts_level = FTS_ROOTPARENTLEVEL;

	/* Allocate/initialize root(s). */
	for (root = NULL, nitems = 0; *argv; ++argv, ++nitems) {
		/* Don't allow zero-length paths. */
		if ((len = strlen(*argv)) == 0) {
			errno = ENOENT;
			goto mem3;
		}

		if ((p = fts_alloc(sp, *argv, len)) == NULL)
			goto mem3;
		p->fts_level = FTS_ROOTLEVEL;
		p->fts_parent = parent;
		p->fts_accpath = p->fts_name;
		p->fts_info = fts_stat(sp, p, ISSET(FTS_COMFOLLOW));

		/* Command-line "." and ".." are real directories. */
		if (p->fts_info == FTS_DOT)
			p->fts_info = FTS_D;

		/*
		 * If comparison routine supplied, traverse in sorted
		 * order; otherwise traverse in the order specified.
		 */
		if (compar) {
			p->fts_link = root;
			root = p;
		} else {
			p->fts_link = NULL;
			if (root == NULL)
				tmp = root = p;
			else {
				tmp->fts_link = p;
				tmp = p;
			}
		}
	}
	if (compar && nitems > 1)
		root = fts_sort(sp, root, nitems);

	/*
	 * Allocate a dummy pointer and make fts_read think that we've just
	 * finished the node before the root(s); set p->fts_info to FTS_INIT
	 * so that everything about the "current" node is ignored.
	 */
	if ((sp->fts_cur = fts_alloc(sp, "", 0)) == NULL)
		goto mem3;
	sp->fts_cur->fts_link = root;
	sp->fts_cur->fts_info = FTS_INIT;

	/*
	 * If using chdir(2), grab a file descriptor pointing to dot to insure
	 * that we can get back here; this could be avoided for some paths,
	 * but almost certainly not worth the effort.  Slashes, symbolic links,
	 * and ".." are all fairly nasty problems.  Note, if we can't get the
	 * descriptor we run anyway, just more slowly.
	 */
	if (!ISSET(FTS_NOCHDIR)) {
#ifdef HAVE_FCHDIR
		if ((sp->fts_rfd = open(".", O_RDONLY, 0)) == -1)
			SET(FTS_NOCHDIR);
		else if (fcntl(sp->fts_rfd, F_SETFD, FD_CLOEXEC) == -1) {
			close(sp->fts_rfd);
			SET(FTS_NOCHDIR);
		}
#else
		if ((sp->fts_rdir = getcwd(NULL, 0)) != NULL)
			SET(FTS_NOCHDIR);
#endif
	}

	return (sp);

mem3:	fts_lfree(root);
	free(parent);
mem2:	free(sp->fts_path);
mem1:	free(sp);
	return (NULL);
}

#ifdef NEED_STRRSLASH
static char *strrslash(register char *psz)
{
    register char ch;
    char *pszLast = NULL;
    for (; (ch = *psz); psz++)
        switch (ch)
        {
            case '/':
            case '\\':
            case ':':
                pszLast = psz;
                break;
        }
    return pszLast;
}
#endif

static void
fts_load(sp, p)
	FTS *sp;
	FTSENT *p;
{
	size_t len;
	char *cp;

	_DIAGASSERT(sp != NULL);
	_DIAGASSERT(p != NULL);

	/*
	 * Load the stream structure for the next traversal.  Since we don't
	 * actually enter the directory until after the preorder visit, set
	 * the fts_accpath field specially so the chdir gets done to the right
	 * place and the user can access the first node.  From fts_open it's
	 * known that the path will fit.
	 */
	len = p->fts_pathlen = p->fts_namelen;
	memmove(sp->fts_path, p->fts_name, len + 1);
#ifdef NEED_STRRSLASH
	if ((cp = strrslash(p->fts_name)) && (cp != p->fts_name || cp[1])) {
#else
	if ((cp = strrchr(p->fts_name, '/')) && (cp != p->fts_name || cp[1])) {
#endif
		len = strlen(++cp);
		memmove(p->fts_name, cp, len + 1);
		p->fts_namelen = len;
	}
	p->fts_accpath = p->fts_path = sp->fts_path;
	sp->fts_dev = p->fts_dev;
}

int
fts_close(sp)
	FTS *sp;
{
	FTSENT *freep, *p;
	int saved_errno = 0;

	_DIAGASSERT(sp != NULL);

	/*
	 * This still works if we haven't read anything -- the dummy structure
	 * points to the root list, so we step through to the end of the root
	 * list which has a valid parent pointer.
	 */
	if (sp->fts_cur) {
#ifndef _MSC_VER
		if (ISSET(FTS_SYMFOLLOW))
			(void)close(sp->fts_cur->fts_symfd);
#endif
		for (p = sp->fts_cur; p->fts_level >= FTS_ROOTLEVEL;) {
			freep = p;
			p = p->fts_link ? p->fts_link : p->fts_parent;
			free(freep);
		}
		free(p);
	}

	/* Free up child linked list, sort array, path buffer. */
	if (sp->fts_child)
		fts_lfree(sp->fts_child);
	if (sp->fts_array)
		free(sp->fts_array);
	free(sp->fts_path);

	/* Return to original directory, save errno if necessary. */
	if (!ISSET(FTS_NOCHDIR)) {
#ifdef HAVE_FCHDIR
		if (fchdir(sp->fts_rfd))
			saved_errno = errno;
		(void)close(sp->fts_rfd);
#else
		if (chdir(sp->fts_rdir))
			saved_errno =  errno;
		free(sp->fts_rdir);
		sp->fts_rdir = NULL;
#endif
	}

	/* Free up the stream pointer. */
	free(sp);
	/* ISSET() is illegal after this, since the macro touches sp */

	/* Set errno and return. */
	if (saved_errno) {
		errno = saved_errno;
		return (-1);
	}
	return (0);
}

/*
 * Special case a root of "/" so that slashes aren't appended which would
 * cause paths to be written as "//foo".
 */
#define	NAPPEND(p)							\
	(p->fts_level == FTS_ROOTLEVEL && p->fts_pathlen == 1 &&	\
	    IS_SLASH(p->fts_path[0]) ? 0 : p->fts_pathlen)

FTSENT *
fts_read(sp)
	FTS *sp;
{
	FTSENT *p, *tmp;
	int instr;
	char *t;
	int saved_errno;

	_DIAGASSERT(sp != NULL);

	/* If finished or unrecoverable error, return NULL. */
	if (sp->fts_cur == NULL || ISSET(FTS_STOP))
		return (NULL);

	/* Set current node pointer. */
	p = sp->fts_cur;

	/* Save and zero out user instructions. */
	instr = p->fts_instr;
	p->fts_instr = FTS_NOINSTR;

	/* Any type of file may be re-visited; re-stat and re-turn. */
	if (instr == FTS_AGAIN) {
		p->fts_info = fts_stat(sp, p, 0);
		return (p);
	}

	/*
	 * Following a symlink -- SLNONE test allows application to see
	 * SLNONE and recover.  If indirecting through a symlink, have
	 * keep a pointer to current location.  If unable to get that
	 * pointer, follow fails.
	 */
	if (instr == FTS_FOLLOW &&
	    (p->fts_info == FTS_SL || p->fts_info == FTS_SLNONE)) {
		p->fts_info = fts_stat(sp, p, 1);
		if (p->fts_info == FTS_D && !ISSET(FTS_NOCHDIR)) {
#ifdef HAVE_FCHDIR
			if ((p->fts_symfd = open(".", O_RDONLY, 0)) == -1) {
				p->fts_errno = errno;
				p->fts_info = FTS_ERR;
			} else if (fcntl(p->fts_symfd, F_SETFD, FD_CLOEXEC) == -1) {
				p->fts_errno = errno;
				p->fts_info = FTS_ERR;
				close(p->fts_symfd);
			} else
				p->fts_flags |= FTS_SYMFOLLOW;
#endif
		}
		return (p);
	}

	/* Directory in pre-order. */
	if (p->fts_info == FTS_D) {
		/* If skipped or crossed mount point, do post-order visit. */
		if (instr == FTS_SKIP ||
		    (ISSET(FTS_XDEV) && p->fts_dev != sp->fts_dev)) {
#ifdef HAVE_FCHDIR
			if (p->fts_flags & FTS_SYMFOLLOW)
				(void)close(p->fts_symfd);
#endif
			if (sp->fts_child) {
				fts_lfree(sp->fts_child);
				sp->fts_child = NULL;
			}
			p->fts_info = FTS_DP;
			return (p);
		}

		/* Rebuild if only read the names and now traversing. */
		if (sp->fts_child && ISSET(FTS_NAMEONLY)) {
			CLR(FTS_NAMEONLY);
			fts_lfree(sp->fts_child);
			sp->fts_child = NULL;
		}

		/*
		 * Cd to the subdirectory.
		 *
		 * If have already read and now fail to chdir, whack the list
		 * to make the names come out right, and set the parent errno
		 * so the application will eventually get an error condition.
		 * Set the FTS_DONTCHDIR flag so that when we logically change
		 * directories back to the parent we don't do a chdir.
		 *
		 * If haven't read do so.  If the read fails, fts_build sets
		 * FTS_STOP or the fts_info field of the node.
		 */
		if (sp->fts_child) {
			if (fts_safe_changedir(sp, p, -1, p->fts_accpath)) {
				p->fts_errno = errno;
				p->fts_flags |= FTS_DONTCHDIR;
				for (p = sp->fts_child; p; p = p->fts_link)
					p->fts_accpath =
					    p->fts_parent->fts_accpath;
			}
		} else if ((sp->fts_child = fts_build(sp, BREAD)) == NULL) {
			if (ISSET(FTS_STOP))
				return (NULL);
			return (p);
		}
		p = sp->fts_child;
		sp->fts_child = NULL;
		goto name;
	}

	/* Move to the next node on this level. */
next:	tmp = p;
	if ((p = p->fts_link) != NULL) {
		free(tmp);

		/*
		 * If reached the top, return to the original directory, and
		 * load the paths for the next root.
		 */
		if (p->fts_level == FTS_ROOTLEVEL) {
#ifdef HAVE_FCHDIR
			if (FCHDIR(sp, sp->fts_rfd)) {
#else
			if (CHDIR(sp, sp->fts_rdir)) {
#endif
				SET(FTS_STOP);
				return (NULL);
			}
			fts_load(sp, p);
			return (sp->fts_cur = p);
		}

		/*
		 * User may have called fts_set on the node.  If skipped,
		 * ignore.  If followed, get a file descriptor so we can
		 * get back if necessary.
		 */
		if (p->fts_instr == FTS_SKIP)
			goto next;
		if (p->fts_instr == FTS_FOLLOW) {
			p->fts_info = fts_stat(sp, p, 1);
			if (p->fts_info == FTS_D && !ISSET(FTS_NOCHDIR)) {
#ifdef HAVE_FCHDIR
				if ((p->fts_symfd =
				    open(".", O_RDONLY, 0)) == -1) {
					p->fts_errno = errno;
					p->fts_info = FTS_ERR;
				} else if (fcntl(p->fts_symfd, F_SETFD, FD_CLOEXEC) == -1) {
					p->fts_errno = errno;
					p->fts_info = FTS_ERR;
					close(p->fts_symfd);
				} else
					p->fts_flags |= FTS_SYMFOLLOW;
#endif
			}
			p->fts_instr = FTS_NOINSTR;
		}

name:		t = sp->fts_path + NAPPEND(p->fts_parent);
		*t++ = '/';
		memmove(t, p->fts_name, (size_t)(p->fts_namelen + 1));
		return (sp->fts_cur = p);
	}

	/* Move up to the parent node. */
	p = tmp->fts_parent;
	free(tmp);

	if (p->fts_level == FTS_ROOTPARENTLEVEL) {
		/*
		 * Done; free everything up and set errno to 0 so the user
		 * can distinguish between error and EOF.
		 */
		free(p);
		errno = 0;
		return (sp->fts_cur = NULL);
	}

	/* Nul terminate the pathname. */
	sp->fts_path[p->fts_pathlen] = '\0';

	/*
	 * Return to the parent directory.  If at a root node or came through
	 * a symlink, go back through the file descriptor.  Otherwise, cd up
	 * one directory.
	 */
	if (p->fts_level == FTS_ROOTLEVEL) {
#ifdef HAVE_FCHDIR
		if (FCHDIR(sp, sp->fts_rfd)) {
#else
		if (CHDIR(sp, sp->fts_rdir)) {
#endif
			SET(FTS_STOP);
			return (NULL);
		}
#ifdef HAVE_FCHDIR
	} else if (p->fts_flags & FTS_SYMFOLLOW) {
		if (FCHDIR(sp, p->fts_symfd)) {
			saved_errno = errno;
			(void)close(p->fts_symfd);
			errno = saved_errno;
			SET(FTS_STOP);
			return (NULL);
		}
		(void)close(p->fts_symfd);
#else
        (void)saved_errno;
#endif
	} else if (!(p->fts_flags & FTS_DONTCHDIR) &&
	    fts_safe_changedir(sp, p->fts_parent, -1, "..")) {
		SET(FTS_STOP);
		return (NULL);
	}
	p->fts_info = p->fts_errno ? FTS_ERR : FTS_DP;
	return (sp->fts_cur = p);
}

/*
 * Fts_set takes the stream as an argument although it's not used in this
 * implementation; it would be necessary if anyone wanted to add global
 * semantics to fts using fts_set.  An error return is allowed for similar
 * reasons.
 */
/* ARGSUSED */
int
fts_set(sp, p, instr)
	FTS *sp;
	FTSENT *p;
	int instr;
{

	_DIAGASSERT(sp != NULL);
	_DIAGASSERT(p != NULL);

	if (instr && instr != FTS_AGAIN && instr != FTS_FOLLOW &&
	    instr != FTS_NOINSTR && instr != FTS_SKIP) {
		errno = EINVAL;
		return (1);
	}
	p->fts_instr = instr;
	return (0);
}

FTSENT *
fts_children(sp, instr)
	FTS *sp;
	int instr;
{
	FTSENT *p;
#ifdef HAVE_FCHDIR
	int fd;
#else
	char *pszRoot;
	int rc;
#endif

	_DIAGASSERT(sp != NULL);

	if (instr && instr != FTS_NAMEONLY) {
		errno = EINVAL;
		return (NULL);
	}

	/* Set current node pointer. */
	p = sp->fts_cur;

	/*
	 * Errno set to 0 so user can distinguish empty directory from
	 * an error.
	 */
	errno = 0;

	/* Fatal errors stop here. */
	if (ISSET(FTS_STOP))
		return (NULL);

	/* Return logical hierarchy of user's arguments. */
	if (p->fts_info == FTS_INIT)
		return (p->fts_link);

	/*
	 * If not a directory being visited in pre-order, stop here.  Could
	 * allow FTS_DNR, assuming the user has fixed the problem, but the
	 * same effect is available with FTS_AGAIN.
	 */
	if (p->fts_info != FTS_D /* && p->fts_info != FTS_DNR */)
		return (NULL);

	/* Free up any previous child list. */
	if (sp->fts_child)
		fts_lfree(sp->fts_child);

	if (instr == FTS_NAMEONLY) {
		SET(FTS_NAMEONLY);
		instr = BNAMES;
	} else
		instr = BCHILD;

	/*
	 * If using chdir on a relative path and called BEFORE fts_read does
	 * its chdir to the root of a traversal, we can lose -- we need to
	 * chdir into the subdirectory, and we don't know where the current
	 * directory is, so we can't get back so that the upcoming chdir by
	 * fts_read will work.
	 */
	if (p->fts_level != FTS_ROOTLEVEL || IS_SLASH(p->fts_accpath[0]) ||
	    ISSET(FTS_NOCHDIR))
		return (sp->fts_child = fts_build(sp, instr));

#ifdef HAVE_FCHDIR
	if ((fd = open(".", O_RDONLY, 0)) == -1)
#else
	if ((pszRoot = getcwd(NULL, 0)) == NULL)
#endif
		return (sp->fts_child = NULL);
	sp->fts_child = fts_build(sp, instr);
#ifdef HAVE_FCHDIR
	if (fchdir(fd)) {
		(void)close(fd);
		return (NULL);
	}
	(void)close(fd);
#else
	rc = chdir(pszRoot);
	free(pszRoot);
	if (rc)
		return (NULL);
#endif

	return (sp->fts_child);
}

/*
 * This is the tricky part -- do not casually change *anything* in here.  The
 * idea is to build the linked list of entries that are used by fts_children
 * and fts_read.  There are lots of special cases.
 *
 * The real slowdown in walking the tree is the stat calls.  If FTS_NOSTAT is
 * set and it's a physical walk (so that symbolic links can't be directories),
 * we can do things quickly.  First, if it's a 4.4BSD file system, the type
 * of the file is in the directory entry.  Otherwise, we assume that the number
 * of subdirectories in a node is equal to the number of links to the parent.
 * The former skips all stat calls.  The latter skips stat calls in any leaf
 * directories and for any files after the subdirectories in the directory have
 * been found, cutting the stat calls by about 2/3.
 */
static FTSENT *
fts_build(sp, type)
	FTS *sp;
	int type;
{
	struct dirent *dp;
	FTSENT *p, *head;
	size_t nitems;
	FTSENT *cur, *tail;
	DIR *dirp;
	int adjust, cderrno, descend, len, level, nlinks, saved_errno, nostat;
	size_t maxlen;
#ifdef FTS_WHITEOUT
	int oflag;
#endif
	char *cp = NULL;	/* pacify gcc */

	_DIAGASSERT(sp != NULL);

	/* Set current node pointer. */
	cur = sp->fts_cur;

	/*
	 * Open the directory for reading.  If this fails, we're done.
	 * If being called from fts_read, set the fts_info field.
	 */
#if defined(FTS_WHITEOUT) && !defined(__OS2__)
	if (ISSET(FTS_WHITEOUT))
		oflag = DTF_NODUP|DTF_REWIND;
	else
		oflag = DTF_HIDEW|DTF_NODUP|DTF_REWIND;
#elif defined(_MSC_VER)
# define __opendir2(path, flag) birdDirOpenExtraInfo(path)
#else
#define __opendir2(path, flag) opendir(path)
#endif
	if ((dirp = __opendir2(cur->fts_accpath, oflag)) == NULL) {
		if (type == BREAD) {
			cur->fts_info = FTS_DNR;
			cur->fts_errno = errno;
		}
		return (NULL);
	}

	/*
	 * Nlinks is the number of possible entries of type directory in the
	 * directory if we're cheating on stat calls, 0 if we're not doing
	 * any stat calls at all, -1 if we're doing stats on everything.
	 */
	if (type == BNAMES) {
		nlinks = 0;
		nostat = 1;
	} else if (ISSET(FTS_NOSTAT) && ISSET(FTS_PHYSICAL)) {
		nlinks = cur->fts_nlink - (ISSET(FTS_SEEDOT) ? 0 : 2);
		nostat = 1;
	} else {
		nlinks = -1;
		nostat = 0;
	}

#ifdef notdef
	(void)printf("nlinks == %d (cur: %d)\n", nlinks, cur->fts_nlink);
	(void)printf("NOSTAT %d PHYSICAL %d SEEDOT %d\n",
	    ISSET(FTS_NOSTAT), ISSET(FTS_PHYSICAL), ISSET(FTS_SEEDOT));
#endif
	/*
	 * If we're going to need to stat anything or we want to descend
	 * and stay in the directory, chdir.  If this fails we keep going,
	 * but set a flag so we don't chdir after the post-order visit.
	 * We won't be able to stat anything, but we can still return the
	 * names themselves.  Note, that since fts_read won't be able to
	 * chdir into the directory, it will have to return different path
	 * names than before, i.e. "a/b" instead of "b".  Since the node
	 * has already been visited in pre-order, have to wait until the
	 * post-order visit to return the error.  There is a special case
	 * here, if there was nothing to stat then it's not an error to
	 * not be able to stat.  This is all fairly nasty.  If a program
	 * needed sorted entries or stat information, they had better be
	 * checking FTS_NS on the returned nodes.
	 */
	cderrno = 0;
	if (nlinks || type == BREAD) {
#ifdef HAVE_FCHDIR
		if (fts_safe_changedir(sp, cur, dirfd(dirp), NULL)) {
#else
		if (fts_safe_changedir(sp, cur, dirfd(dirp), cur->fts_accpath)) {
#endif
			if (nlinks && type == BREAD)
				cur->fts_errno = errno;
			cur->fts_flags |= FTS_DONTCHDIR;
			descend = 0;
			cderrno = errno;
		} else
			descend = 1;
	} else
		descend = 0;

	/*
	 * Figure out the max file name length that can be stored in the
	 * current path -- the inner loop allocates more path as necessary.
	 * We really wouldn't have to do the maxlen calculations here, we
	 * could do them in fts_read before returning the path, but it's a
	 * lot easier here since the length is part of the dirent structure.
	 *
	 * If not changing directories set a pointer so that can just append
	 * each new name into the path.
	 */
	len = NAPPEND(cur);
	if (ISSET(FTS_NOCHDIR)) {
		cp = sp->fts_path + len;
		*cp++ = '/';
	}
	len++;
	maxlen = sp->fts_pathlen - len;

	level = cur->fts_level + 1;

	/* Read the directory, attaching each entry to the `link' pointer. */
	adjust = 0;
	for (head = tail = NULL, nitems = 0; (dp = readdir(dirp)) != NULL;) {
		size_t dlen;

		if (!ISSET(FTS_SEEDOT) && ISDOT(dp->d_name))
			continue;

#if HAVE_STRUCT_DIRENT_D_NAMLEN
		dlen = dp->d_namlen;
#else
		dlen = strlen(dp->d_name);
#endif
		if ((p = fts_alloc(sp, dp->d_name, dlen)) == NULL)
			goto mem1;
		if (dlen >= maxlen) {	/* include space for NUL */
			if (fts_palloc(sp, len + dlen + 1)) {
				/*
				 * No more memory for path or structures.  Save
				 * errno, free up the current structure and the
				 * structures already allocated.
				 */
mem1:				saved_errno = errno;
				if (p)
					free(p);
				fts_lfree(head);
				(void)closedir(dirp);
				errno = saved_errno;
				cur->fts_info = FTS_ERR;
				SET(FTS_STOP);
				return (NULL);
			}
			adjust = 1;
			if (ISSET(FTS_NOCHDIR))
				cp = sp->fts_path + len;
			maxlen = sp->fts_pathlen - len;
		}

		p->fts_pathlen = len + dlen;
		p->fts_parent = sp->fts_cur;
		p->fts_level = level;

#ifdef FTS_WHITEOUT
		if (dp->d_type == DT_WHT)
			p->fts_flags |= FTS_ISW;
#endif

		if (cderrno) {
			if (nlinks) {
				p->fts_info = FTS_NS;
				p->fts_errno = cderrno;
			} else
				p->fts_info = FTS_NSOK;
			p->fts_accpath = cur->fts_accpath;
		} else if (nlinks == 0
#ifdef DT_DIR
		    || (nostat &&
		    dp->d_type != DT_DIR && dp->d_type != DT_UNKNOWN)
#endif
		    ) {
			p->fts_accpath =
			    ISSET(FTS_NOCHDIR) ? p->fts_path : p->fts_name;
			p->fts_info = FTS_NSOK;
		} else {
			/* Build a file name for fts_stat to stat. */
			if (ISSET(FTS_NOCHDIR)) {
				p->fts_accpath = p->fts_path;
				memmove(cp, p->fts_name,
				        (size_t)(p->fts_namelen + 1));
			} else
				p->fts_accpath = p->fts_name;

			/* Stat it. */
#ifdef _MSC_VER
			p->fts_info = fts_stat_dirent(sp, p, 0, dp);
#else
			p->fts_info = fts_stat(sp, p, 0);
#endif

			/* Decrement link count if applicable. */
			if (nlinks > 0 && (p->fts_info == FTS_D ||
			    p->fts_info == FTS_DC || p->fts_info == FTS_DOT))
				--nlinks;
		}

		/* We walk in directory order so "ls -f" doesn't get upset. */
		p->fts_link = NULL;
		if (head == NULL)
			head = tail = p;
		else {
			tail->fts_link = p;
			tail = p;
		}
		++nitems;
	}
	(void)closedir(dirp);

	/*
	 * If had to realloc the path, adjust the addresses for the rest
	 * of the tree.
	 */
	if (adjust)
		fts_padjust(sp, head);

	/*
	 * If not changing directories, reset the path back to original
	 * state.
	 */
	if (ISSET(FTS_NOCHDIR)) {
		if (cp - 1 > sp->fts_path)
			--cp;
		*cp = '\0';
	}

	/*
	 * If descended after called from fts_children or after called from
	 * fts_read and nothing found, get back.  At the root level we use
	 * the saved fd; if one of fts_open()'s arguments is a relative path
	 * to an empty directory, we wind up here with no other way back.  If
	 * can't get back, we're done.
	 */
	if (descend && (type == BCHILD || !nitems) &&
	    (cur->fts_level == FTS_ROOTLEVEL ?
#ifdef HAVE_FCHDIR
	    FCHDIR(sp, sp->fts_rfd) :
#else
	    CHDIR(sp, sp->fts_rdir) :
#endif
	    fts_safe_changedir(sp, cur->fts_parent, -1, ".."))) {
		cur->fts_info = FTS_ERR;
		SET(FTS_STOP);
		return (NULL);
	}

	/* If didn't find anything, return NULL. */
	if (!nitems) {
		if (type == BREAD)
			cur->fts_info = FTS_DP;
		return (NULL);
	}

	/* Sort the entries. */
	if (sp->fts_compar && nitems > 1)
		head = fts_sort(sp, head, nitems);
	return (head);
}

#ifdef _MSC_VER
/** Special version of fts_stat that takes the information from the directory
 *  entry returned by readdir().
 *
 *  Directory listing returns all the stat information on systems likes
 *  Windows and OS/2. */
static u_short
fts_stat_dirent(FTS *sp, FTSENT *p, int follow, struct dirent *pDirEnt)
{
	FTSENT *t;
	dev_t dev;
	ino_t ino;
	struct STAT *sbp, sb;
	int saved_errno;

	_DIAGASSERT(sp != NULL);
	_DIAGASSERT(p != NULL);

	/* If user needs stat info, stat buffer already allocated. */
	sbp = ISSET(FTS_NOSTAT) ? &sb : p->fts_statp;

	/*
	 * Copy over the stat info from the direntry.
	 */
	*sbp = pDirEnt->d_stat;

	/*
	 * If doing a logical walk, or application requested FTS_FOLLOW, do
	 * a stat(2) on symlinks.  If that fails, assume non-existent
	 * symlink and set the errno from the stat call.
	 */
	if (S_ISLNK(sbp->st_mode) && (ISSET(FTS_LOGICAL) || follow)) {
		if (stat(p->fts_accpath, sbp)) {
			saved_errno = errno;
			errno = 0;
			return (FTS_SLNONE);
		}
	}

	if (S_ISDIR(sbp->st_mode)) {
		/*
		 * Set the device/inode.  Used to find cycles and check for
		 * crossing mount points.  Also remember the link count, used
		 * in fts_build to limit the number of stat calls.  It is
		 * understood that these fields are only referenced if fts_info
		 * is set to FTS_D.
		 */
		dev = p->fts_dev = sbp->st_dev;
		ino = p->fts_ino = sbp->st_ino;
		p->fts_nlink = sbp->st_nlink;

		if (ISDOT(p->fts_name))
			return (FTS_DOT);

		/*
		 * Cycle detection is done by brute force when the directory
		 * is first encountered.  If the tree gets deep enough or the
		 * number of symbolic links to directories is high enough,
		 * something faster might be worthwhile.
		 */

#ifdef _MSC_VER
		if (ino && dev) /** @todo ino emulation on windows... */
#endif
		    for (t = p->fts_parent;
			t->fts_level >= FTS_ROOTLEVEL; t = t->fts_parent)
			    if (ino == t->fts_ino && dev == t->fts_dev) {
				    p->fts_cycle = t;
				    return (FTS_DC);
			    }
		return (FTS_D);
	}
	if (S_ISLNK(sbp->st_mode))
		return (FTS_SL);
	if (S_ISREG(sbp->st_mode))
		return (FTS_F);
	return (FTS_DEFAULT);
}

#endif /* fts_stat_dirent */

static u_short
fts_stat(sp, p, follow)
	FTS *sp;
	FTSENT *p;
	int follow;
{
	FTSENT *t;
	dev_t dev;
	ino_t ino;
	struct STAT *sbp, sb;
	int saved_errno;

	_DIAGASSERT(sp != NULL);
	_DIAGASSERT(p != NULL);

	/* If user needs stat info, stat buffer already allocated. */
	sbp = ISSET(FTS_NOSTAT) ? &sb : p->fts_statp;

#ifdef FTS_WHITEOUT
	/* check for whiteout */
	if (p->fts_flags & FTS_ISW) {
		if (sbp != &sb) {
			memset(sbp, '\0', sizeof (*sbp));
			sbp->st_mode = S_IFWHT;
		}
		return (FTS_W);
	}
#endif

	/*
	 * If doing a logical walk, or application requested FTS_FOLLOW, do
	 * a stat(2).  If that fails, check for a non-existent symlink.  If
	 * fail, set the errno from the stat call.
	 */
	if (ISSET(FTS_LOGICAL) || follow) {
		if (stat(p->fts_accpath, sbp)) {
			saved_errno = errno;
			if (!lstat(p->fts_accpath, sbp)) {
				errno = 0;
				return (FTS_SLNONE);
			}
			p->fts_errno = saved_errno;
			goto err;
		}
	} else if (lstat(p->fts_accpath, sbp)) {
		p->fts_errno = errno;
err:		memset(sbp, 0, sizeof(struct STAT));
		return (FTS_NS);
	}

	if (S_ISDIR(sbp->st_mode)) {
		/*
		 * Set the device/inode.  Used to find cycles and check for
		 * crossing mount points.  Also remember the link count, used
		 * in fts_build to limit the number of stat calls.  It is
		 * understood that these fields are only referenced if fts_info
		 * is set to FTS_D.
		 */
		dev = p->fts_dev = sbp->st_dev;
		ino = p->fts_ino = sbp->st_ino;
		p->fts_nlink = sbp->st_nlink;

		if (ISDOT(p->fts_name))
			return (FTS_DOT);

		/*
		 * Cycle detection is done by brute force when the directory
		 * is first encountered.  If the tree gets deep enough or the
		 * number of symbolic links to directories is high enough,
		 * something faster might be worthwhile.
		 */

#ifdef _MSC_VER
		if (ino && dev) /** @todo ino emulation on windows... */
#endif
		for (t = p->fts_parent;
		    t->fts_level >= FTS_ROOTLEVEL; t = t->fts_parent)
			if (ino == t->fts_ino && dev == t->fts_dev) {
				p->fts_cycle = t;
				return (FTS_DC);
			}
		return (FTS_D);
	}
	if (S_ISLNK(sbp->st_mode))
		return (FTS_SL);
	if (S_ISREG(sbp->st_mode))
		return (FTS_F);
	return (FTS_DEFAULT);
}

static FTSENT *
fts_sort(sp, head, nitems)
	FTS *sp;
	FTSENT *head;
	size_t nitems;
{
	FTSENT **ap, *p;

	_DIAGASSERT(sp != NULL);
	_DIAGASSERT(head != NULL);

	/*
	 * Construct an array of pointers to the structures and call qsort(3).
	 * Reassemble the array in the order returned by qsort.  If unable to
	 * sort for memory reasons, return the directory entries in their
	 * current order.  Allocate enough space for the current needs plus
	 * 40 so don't realloc one entry at a time.
	 */
	if (nitems > sp->fts_nitems) {
		FTSENT **new;

		new = realloc(sp->fts_array, sizeof(FTSENT *) * (nitems + 40));
		if (new == 0)
			return (head);
		sp->fts_array = new;
		sp->fts_nitems = nitems + 40;
	}
	for (ap = sp->fts_array, p = head; p; p = p->fts_link)
		*ap++ = p;
	qsort((void *)sp->fts_array, nitems, sizeof(FTSENT *),
		(int (*)(const void *, const void *))sp->fts_compar);
	for (head = *(ap = sp->fts_array); --nitems; ++ap)
		ap[0]->fts_link = ap[1];
	ap[0]->fts_link = NULL;
	return (head);
}

static FTSENT *
fts_alloc(sp, name, namelen)
	FTS *sp;
	const char *name;
	size_t namelen;
{
	FTSENT *p;
	size_t len;

	_DIAGASSERT(sp != NULL);
	_DIAGASSERT(name != NULL);

#if defined(ALIGNBYTES) && defined(ALIGN)
	/*
	 * The file name is a variable length array and no stat structure is
	 * necessary if the user has set the nostat bit.  Allocate the FTSENT
	 * structure, the file name and the stat structure in one chunk, but
	 * be careful that the stat structure is reasonably aligned.  Since the
	 * fts_name field is declared to be of size 1, the fts_name pointer is
	 * namelen + 2 before the first possible address of the stat structure.
	 */
	len = sizeof(FTSENT) + namelen;
	if (!ISSET(FTS_NOSTAT))
		len += sizeof(struct STAT) + ALIGNBYTES;
	if ((p = malloc(len)) == NULL)
		return (NULL);

	if (!ISSET(FTS_NOSTAT))
		p->fts_statp =
		    (struct STAT *)ALIGN((u_long)(p->fts_name + namelen + 2));
#else
	(void)len;
	if ((p = malloc(sizeof(FTSENT) + namelen)) == NULL)
		return (NULL);

	if (!ISSET(FTS_NOSTAT))
		if ((p->fts_statp = malloc(sizeof(struct STAT))) == NULL) {
			free(p);
			return (NULL);
		}
#endif

	/* Copy the name plus the trailing NULL. */
	memmove(p->fts_name, name, namelen + 1);

	p->fts_namelen = namelen;
	p->fts_path = sp->fts_path;
	p->fts_errno = 0;
	p->fts_flags = 0;
	p->fts_instr = FTS_NOINSTR;
	p->fts_number = 0;
	p->fts_pointer = NULL;
	return (p);
}

static void
fts_lfree(head)
	FTSENT *head;
{
	FTSENT *p;

	/* XXX: head may be NULL ? */

	/* Free a linked list of structures. */
	while ((p = head) != NULL) {
		head = head->fts_link;

#if !defined(ALIGNBYTES) || !defined(ALIGN)
		if (p->fts_statp)
			free(p->fts_statp);
#endif
		free(p);
	}
}

static size_t
fts_pow2(x)
	size_t x;
{

	x--;
	x |= x>>1;
	x |= x>>2;
	x |= x>>4;
	x |= x>>8;
	x |= x>>16;
#if LONG_BIT > 32
	x |= x>>32;
#endif
#if LONG_BIT > 64
	x |= x>>64;
#endif
	x++;
	return (x);
}

/*
 * Allow essentially unlimited paths; find, rm, ls should all work on any tree.
 * Most systems will allow creation of paths much longer than MAXPATHLEN, even
 * though the kernel won't resolve them.  Round up the new size to a power of 2,
 * so we don't realloc the path 2 bytes at a time.
 */
static int
fts_palloc(sp, size)
	FTS *sp;
	size_t size;
{
	char *new;

	_DIAGASSERT(sp != NULL);

#if 1
	/* Protect against fts_pathlen overflow. */
	if (size > USHRT_MAX + 1) {
		errno = ENOMEM;
		return (1);
	}
#endif
	size = fts_pow2(size);
	new = realloc(sp->fts_path, size);
	if (new == 0)
		return (1);
	sp->fts_path = new;
	sp->fts_pathlen = size;
	return (0);
}

/*
 * When the path is realloc'd, have to fix all of the pointers in structures
 * already returned.
 */
static void
fts_padjust(sp, head)
	FTS *sp;
	FTSENT *head;
{
	FTSENT *p;
	char *addr;

	_DIAGASSERT(sp != NULL);

#define	ADJUST(p) do {							\
	if ((p)->fts_accpath != (p)->fts_name)				\
		(p)->fts_accpath =					\
		    addr + ((p)->fts_accpath - (p)->fts_path);		\
	(p)->fts_path = addr;						\
} while (/*CONSTCOND*/0)

	addr = sp->fts_path;

	/* Adjust the current set of children. */
	for (p = sp->fts_child; p; p = p->fts_link)
		ADJUST(p);

	/* Adjust the rest of the tree, including the current level. */
	for (p = head; p->fts_level >= FTS_ROOTLEVEL;) {
		ADJUST(p);
		p = p->fts_link ? p->fts_link : p->fts_parent;
	}
}

static size_t
fts_maxarglen(argv)
	char * const *argv;
{
	size_t len, max;

	_DIAGASSERT(argv != NULL);

	for (max = 0; *argv; ++argv)
		if ((len = strlen(*argv)) > max)
			max = len;
	return (max + 1);
}

/*
 * Change to dir specified by fd or p->fts_accpath without getting
 * tricked by someone changing the world out from underneath us.
 * Assumes p->fts_dev and p->fts_ino are filled in.
 */
static int
fts_safe_changedir(sp, p, fd, path)
	const FTS *sp;
	const FTSENT *p;
	int fd;
	const char *path;
{
	int oldfd = fd, ret = -1;
	struct STAT sb;

	if (ISSET(FTS_NOCHDIR))
		return 0;

#ifdef HAVE_FCHDIR
	if (oldfd < 0 && (fd = open(path, O_RDONLY)) == -1)
		return -1;

	if (fstat(fd, &sb) == -1)
		goto bail;
#else
	if (stat(path, &sb))
		goto bail;
#endif

	if (sb.st_ino != p->fts_ino || sb.st_dev != p->fts_dev) {
		errno = ENOENT;
		goto bail;
	}

#ifdef HAVE_FCHDIR
	ret = fchdir(fd);
#else
	ret = chdir(path);
#endif

bail:
#ifdef HAVE_FCHDIR
	if (oldfd < 0) {
		int save_errno = errno;
		(void)close(fd);
		errno = save_errno;
	}
#endif
	return ret;
}
