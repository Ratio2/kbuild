/* config.hin.  Manually edited */
#ifndef INCLUDED_CONFIG_WIN_H
#define INCLUDED_CONFIG_WIN_H

#if 1

/* HACK ALERT! Make inlined CRT functions static to avoid getting duplicate
   symbols errors for things like _vprintf_l.  */
# define _CRT_STDIO_INLINE static __inline

/* HACK ALERT! Ugly ugly hack for making __local_stdio_printf_options static
   and not to cause stupid stupid duplicate symbols linker errors. */
# include <corecrt.h>
# undef _CRT_INLINE_PURE_SECURITYCRITICAL_ATTRIBUTE
# define _CRT_INLINE_PURE_SECURITYCRITICAL_ATTRIBUTE static

/* HACK ALERT! wchar.h requires __inline temporarily redefined to static _inline or
   we'll end up with mbsinit wmemchr, wmemcmp, wmemcpy and friends as duplicates.  */
# include <stdio.h>
# include <conio.h>
# include <string.h>
# include <time.h>
# include <sys/stat.h>
# define __inline static _inline
# include <wchar.h>
# undef __inline

#endif

/* CPU and C ABI indicator */
#ifndef __i386__
#undef __i386__
#endif
#ifndef __x86_64_x32__
#undef __x86_64_x32__
#endif
#ifndef __x86_64__
#undef __x86_64__
#endif
#ifndef __alpha__
#undef __alpha__
#endif
#ifndef __arm__
#undef __arm__
#endif
#ifndef __armhf__
#undef __armhf__
#endif
#ifndef __arm64_ilp32__
#undef __arm64_ilp32__
#endif
#ifndef __arm64__
#undef __arm64__
#endif
#ifndef __hppa__
#undef __hppa__
#endif
#ifndef __hppa64__
#undef __hppa64__
#endif
#ifndef __ia64_ilp32__
#undef __ia64_ilp32__
#endif
#ifndef __ia64__
#undef __ia64__
#endif
#ifndef __m68k__
#undef __m68k__
#endif
#ifndef __mips__
#undef __mips__
#endif
#ifndef __mipsn32__
#undef __mipsn32__
#endif
#ifndef __mips64__
#undef __mips64__
#endif
#ifndef __powerpc__
#undef __powerpc__
#endif
#ifndef __powerpc64__
#undef __powerpc64__
#endif
#ifndef __powerpc64_elfv2__
#undef __powerpc64_elfv2__
#endif
#ifndef __riscv32__
#undef __riscv32__
#endif
#ifndef __riscv64__
#undef __riscv64__
#endif
#ifndef __riscv32_ilp32__
#undef __riscv32_ilp32__
#endif
#ifndef __riscv32_ilp32f__
#undef __riscv32_ilp32f__
#endif
#ifndef __riscv32_ilp32d__
#undef __riscv32_ilp32d__
#endif
#ifndef __riscv64_ilp32__
#undef __riscv64_ilp32__
#endif
#ifndef __riscv64_ilp32f__
#undef __riscv64_ilp32f__
#endif
#ifndef __riscv64_ilp32d__
#undef __riscv64_ilp32d__
#endif
#ifndef __riscv64_lp64__
#undef __riscv64_lp64__
#endif
#ifndef __riscv64_lp64f__
#undef __riscv64_lp64f__
#endif
#ifndef __riscv64_lp64d__
#undef __riscv64_lp64d__
#endif
#ifndef __s390__
#undef __s390__
#endif
#ifndef __s390x__
#undef __s390x__
#endif
#ifndef __sh__
#undef __sh__
#endif
#ifndef __sparc__
#undef __sparc__
#endif
#ifndef __sparc64__
#undef __sparc64__
#endif


/* Define if building universal (internal helper macro) */
#undef AC_APPLE_UNIVERSAL_BUILD

/* Define to the function xargmatch calls on failures. */
#undef ARGMATCH_DIE

/* Define to the declaration of the xargmatch failure function. */
#undef ARGMATCH_DIE_DECL

/* Define to the number of bits in type 'ptrdiff_t'. */
#undef BITSIZEOF_PTRDIFF_T

/* Define to the number of bits in type 'sig_atomic_t'. */
#undef BITSIZEOF_SIG_ATOMIC_T

/* Define to the number of bits in type 'size_t'. */
#undef BITSIZEOF_SIZE_T

/* Define to the number of bits in type 'wchar_t'. */
#undef BITSIZEOF_WCHAR_T

/* Define to the number of bits in type 'wint_t'. */
#undef BITSIZEOF_WINT_T

/* Define to 1 if the `closedir' function returns void instead of int. */
#undef CLOSEDIR_VOID

/* Define to 1 if using 'alloca.c'. */
#undef C_ALLOCA

/* Define as the bit index in the word where to find bit 0 of the exponent of
   'double'. */
#undef DBL_EXPBIT0_BIT

/* Define as the word index where to find the exponent of 'double'. */
#undef DBL_EXPBIT0_WORD

/* the name of the file descriptor member of DIR */
#undef DIR_FD_MEMBER_NAME

#ifdef DIR_FD_MEMBER_NAME
# define DIR_TO_FD(Dir_p) ((Dir_p)->DIR_FD_MEMBER_NAME)
#else
# define DIR_TO_FD(Dir_p) -1
#endif


/* Define to 1 if // is a file system root distinct from /. */
#undef DOUBLE_SLASH_IS_DISTINCT_ROOT

/* Define if struct dirent has a member d_ino that actually works. */
#undef D_INO_IN_DIRENT

/* Define to 1 if translation of program messages to the user's native
   language is requested. */
#undef ENABLE_NLS

/* Define this to 1 if F_DUPFD behavior does not match POSIX */
#undef FCNTL_DUPFD_BUGGY

/* Define to nothing if C supports flexible array members, and to 1 if it does
   not. That way, with a declaration like 'struct s { int n; short
   d[FLEXIBLE_ARRAY_MEMBER]; };', the struct hack can be used with pre-C99
   compilers. Use 'FLEXSIZEOF (struct s, d, N * sizeof (short))' to calculate
   the size in bytes of such a struct containing an N-element array. */
#define FLEXIBLE_ARRAY_MEMBER 1

/* Define to 1 if fopen() fails to recognize a trailing slash. */
#undef FOPEN_TRAILING_SLASH_BUG

/* Define to 1 if nl_langinfo (YESEXPR) returns a non-empty string. */
#undef FUNC_NL_LANGINFO_YESEXPR_WORKS

/* Define this to 'void' or 'struct timezone' to match the system's
   declaration of the second argument to gettimeofday. */
#undef GETTIMEOFDAY_TIMEZONE

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module close-stream shall be considered present. */
#undef GNULIB_CLOSE_STREAM

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module fcntl-safer shall be considered present. */
#undef GNULIB_FCNTL_SAFER

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module fdopendir shall be considered present. */
#undef GNULIB_FDOPENDIR

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module fd-safer-flag shall be considered present. */
#define GNULIB_FD_SAFER_FLAG 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module fopen-gnu shall be considered present. */
#undef GNULIB_FOPEN_GNU

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module fscanf shall be considered present. */
#undef GNULIB_FSCANF

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module isblank shall be considered present. */
#undef GNULIB_ISBLANK

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module lock shall be considered present. */
#undef GNULIB_LOCK

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module msvc-nothrow shall be considered present. */
#undef GNULIB_MSVC_NOTHROW

/* Define to 1 to disable use of VLAs */
#undef GNULIB_NO_VLA

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module openat shall be considered present. */
#define GNULIB_OPENAT 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module openat-safer shall be considered present. */
#define GNULIB_OPENAT_SAFER 1

/* enable some gnulib portability checks */
#undef GNULIB_PORTCHECK

/* Enable compile-time and run-time bounds-checking, and some warnings,
      without upsetting glibc 2.15+. */
   #if (defined GNULIB_PORTCHECK && !defined _FORTIFY_SOURCE \
        && defined __OPTIMIZE__ && __OPTIMIZE__)
   # define _FORTIFY_SOURCE 2
   #endif
  

/* Define to 1 if printf and friends should be labeled with attribute
   "__gnu_printf__" instead of "__printf__" */
#undef GNULIB_PRINTF_ATTRIBUTE_FLAVOR_GNU

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module reallocarray shall be considered present. */
#undef GNULIB_REALLOCARRAY

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module scanf shall be considered present. */
#undef GNULIB_SCANF

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module snprintf shall be considered present. */
#undef GNULIB_SNPRINTF

/* Define to 1 if you want the FILE stream functions getc, putc, etc. to use
   unlocked I/O if available, throughout the package. Unlocked I/O can improve
   performance, sometimes dramatically. But unlocked I/O is safe only in
   single-threaded programs, as well as in multithreaded programs for which
   you can guarantee that every FILE stream, including stdin, stdout, stderr,
   is used only in a single thread. */
#undef GNULIB_STDIO_SINGLE_THREAD

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module strerror shall be considered present. */
#undef GNULIB_STRERROR

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module strerror_r-posix shall be considered present. */
#define GNULIB_STRERROR_R_POSIX 0

/* Define to 1 when the gnulib module accept should be tested. */
#undef GNULIB_TEST_ACCEPT

/* Define to 1 when the gnulib module bind should be tested. */
#undef GNULIB_TEST_BIND

/* Define to 1 when the gnulib module btowc should be tested. */
#undef GNULIB_TEST_BTOWC

/* Define to 1 when the gnulib module calloc-posix should be tested. */
#undef GNULIB_TEST_CALLOC_POSIX

/* Define to 1 when the gnulib module chdir should be tested. */
#undef GNULIB_TEST_CHDIR

/* Define to 1 when the gnulib module cloexec should be tested. */
#undef GNULIB_TEST_CLOEXEC

/* Define to 1 when the gnulib module close should be tested. */
#undef GNULIB_TEST_CLOSE

/* Define to 1 when the gnulib module closedir should be tested. */
#undef GNULIB_TEST_CLOSEDIR

/* Define to 1 when the gnulib module connect should be tested. */
#undef GNULIB_TEST_CONNECT

/* Define to 1 when the gnulib module dirfd should be tested. */
#undef GNULIB_TEST_DIRFD

/* Define to 1 when the gnulib module dup should be tested. */
#undef GNULIB_TEST_DUP

/* Define to 1 when the gnulib module dup2 should be tested. */
#undef GNULIB_TEST_DUP2

/* Define to 1 when the gnulib module environ should be tested. */
#undef GNULIB_TEST_ENVIRON

/* Define to 1 when the gnulib module fchdir should be tested. */
#undef GNULIB_TEST_FCHDIR

/* Define to 1 when the gnulib module fcntl should be tested. */
#undef GNULIB_TEST_FCNTL

/* Define to 1 when the gnulib module fdopen should be tested. */
#undef GNULIB_TEST_FDOPEN

/* Define to 1 when the gnulib module fdopendir should be tested. */
#undef GNULIB_TEST_FDOPENDIR

/* Define to 1 when the gnulib module fgetc should be tested. */
#undef GNULIB_TEST_FGETC

/* Define to 1 when the gnulib module fgets should be tested. */
#undef GNULIB_TEST_FGETS

/* Define to 1 when the gnulib module fnmatch should be tested. */
#undef GNULIB_TEST_FNMATCH

/* Define to 1 when the gnulib module fopen should be tested. */
#undef GNULIB_TEST_FOPEN

/* Define to 1 when the gnulib module fprintf should be tested. */
#undef GNULIB_TEST_FPRINTF

/* Define to 1 when the gnulib module fputc should be tested. */
#undef GNULIB_TEST_FPUTC

/* Define to 1 when the gnulib module fputs should be tested. */
#undef GNULIB_TEST_FPUTS

/* Define to 1 when the gnulib module fread should be tested. */
#undef GNULIB_TEST_FREAD

/* Define to 1 when the gnulib module free-posix should be tested. */
#undef GNULIB_TEST_FREE_POSIX

/* Define to 1 when the gnulib module fscanf should be tested. */
#undef GNULIB_TEST_FSCANF

/* Define to 1 when the gnulib module fstat should be tested. */
#undef GNULIB_TEST_FSTAT

/* Define to 1 when the gnulib module fstatat should be tested. */
#undef GNULIB_TEST_FSTATAT

/* Define to 1 when the gnulib module ftruncate should be tested. */
#undef GNULIB_TEST_FTRUNCATE

/* Define to 1 when the gnulib module fwrite should be tested. */
#undef GNULIB_TEST_FWRITE

/* Define to 1 when the gnulib module getc should be tested. */
#undef GNULIB_TEST_GETC

/* Define to 1 when the gnulib module getchar should be tested. */
#undef GNULIB_TEST_GETCHAR

/* Define to 1 when the gnulib module getcwd should be tested. */
#undef GNULIB_TEST_GETCWD

/* Define to 1 when the gnulib module getdtablesize should be tested. */
#undef GNULIB_TEST_GETDTABLESIZE

/* Define to 1 when the gnulib module getopt-posix should be tested. */
#undef GNULIB_TEST_GETOPT_POSIX

/* Define to 1 when the gnulib module getpagesize should be tested. */
#undef GNULIB_TEST_GETPAGESIZE

/* Define to 1 when the gnulib module gettimeofday should be tested. */
#undef GNULIB_TEST_GETTIMEOFDAY

/* Define to 1 when the gnulib module ioctl should be tested. */
#undef GNULIB_TEST_IOCTL

/* Define to 1 when the gnulib module isatty should be tested. */
#undef GNULIB_TEST_ISATTY

/* Define to 1 when the gnulib module iswblank should be tested. */
#undef GNULIB_TEST_ISWBLANK

/* Define to 1 when the gnulib module iswctype should be tested. */
#undef GNULIB_TEST_ISWCTYPE

/* Define to 1 when the gnulib module iswdigit should be tested. */
#undef GNULIB_TEST_ISWDIGIT

/* Define to 1 when the gnulib module iswxdigit should be tested. */
#undef GNULIB_TEST_ISWXDIGIT

/* Define to 1 when the gnulib module listen should be tested. */
#undef GNULIB_TEST_LISTEN

/* Define to 1 when the gnulib module localeconv should be tested. */
#undef GNULIB_TEST_LOCALECONV

/* Define to 1 when the gnulib module localename should be tested. */
#undef GNULIB_TEST_LOCALENAME

/* Define to 1 when the gnulib module lseek should be tested. */
#undef GNULIB_TEST_LSEEK

/* Define to 1 when the gnulib module lstat should be tested. */
#undef GNULIB_TEST_LSTAT

/* Define to 1 when the gnulib module malloc-posix should be tested. */
#undef GNULIB_TEST_MALLOC_POSIX

/* Define to 1 when the gnulib module mbrlen should be tested. */
#undef GNULIB_TEST_MBRLEN

/* Define to 1 when the gnulib module mbrtowc should be tested. */
#undef GNULIB_TEST_MBRTOWC

/* Define to 1 when the gnulib module mbscasecmp should be tested. */
#undef GNULIB_TEST_MBSCASECMP

/* Define to 1 when the gnulib module mbsinit should be tested. */
#undef GNULIB_TEST_MBSINIT

/* Define to 1 when the gnulib module mbslen should be tested. */
#undef GNULIB_TEST_MBSLEN

/* Define to 1 when the gnulib module mbsrtowcs should be tested. */
#undef GNULIB_TEST_MBSRTOWCS

/* Define to 1 when the gnulib module mbsstr should be tested. */
#undef GNULIB_TEST_MBSSTR

/* Define to 1 when the gnulib module mbtowc should be tested. */
#undef GNULIB_TEST_MBTOWC

/* Define to 1 when the gnulib module memchr should be tested. */
#undef GNULIB_TEST_MEMCHR

/* Define to 1 when the gnulib module mempcpy should be tested. */
#undef GNULIB_TEST_MEMPCPY

/* Define to 1 when the gnulib module memrchr should be tested. */
#undef GNULIB_TEST_MEMRCHR

/* Define to 1 when the gnulib module nanosleep should be tested. */
#undef GNULIB_TEST_NANOSLEEP

/* Define to 1 when the gnulib module nl_langinfo should be tested. */
#undef GNULIB_TEST_NL_LANGINFO

/* Define to 1 when the gnulib module open should be tested. */
#undef GNULIB_TEST_OPEN

/* Define to 1 when the gnulib module openat should be tested. */
#undef GNULIB_TEST_OPENAT

/* Define to 1 when the gnulib module opendir should be tested. */
#undef GNULIB_TEST_OPENDIR

/* Define to 1 when the gnulib module perror should be tested. */
#undef GNULIB_TEST_PERROR

/* Define to 1 when the gnulib module pipe should be tested. */
#undef GNULIB_TEST_PIPE

/* Define to 1 when the gnulib module printf should be tested. */
#undef GNULIB_TEST_PRINTF

/* Define to 1 when the gnulib module pthread_sigmask should be tested. */
#undef GNULIB_TEST_PTHREAD_SIGMASK

/* Define to 1 when the gnulib module pthread-thread should be tested. */
#undef GNULIB_TEST_PTHREAD_THREAD

/* Define to 1 when the gnulib module putc should be tested. */
#undef GNULIB_TEST_PUTC

/* Define to 1 when the gnulib module putchar should be tested. */
#undef GNULIB_TEST_PUTCHAR

/* Define to 1 when the gnulib module putenv should be tested. */
#undef GNULIB_TEST_PUTENV

/* Define to 1 when the gnulib module puts should be tested. */
#undef GNULIB_TEST_PUTS

/* Define to 1 when the gnulib module raise should be tested. */
#undef GNULIB_TEST_RAISE

/* Define to 1 when the gnulib module rawmemchr should be tested. */
#undef GNULIB_TEST_RAWMEMCHR

/* Define to 1 when the gnulib module read should be tested. */
#undef GNULIB_TEST_READ

/* Define to 1 when the gnulib module readdir should be tested. */
#undef GNULIB_TEST_READDIR

/* Define to 1 when the gnulib module reallocarray should be tested. */
#undef GNULIB_TEST_REALLOCARRAY

/* Define to 1 when the gnulib module realloc-posix should be tested. */
#undef GNULIB_TEST_REALLOC_POSIX

/* Define to 1 when the gnulib module scanf should be tested. */
#undef GNULIB_TEST_SCANF

/* Define to 1 when the gnulib module select should be tested. */
#undef GNULIB_TEST_SELECT

/* Define to 1 when the gnulib module setenv should be tested. */
#undef GNULIB_TEST_SETENV

/* Define to 1 when the gnulib module setlocale should be tested. */
#undef GNULIB_TEST_SETLOCALE

/* Define to 1 when the gnulib module setlocale_null should be tested. */
#undef GNULIB_TEST_SETLOCALE_NULL

/* Define to 1 when the gnulib module setsockopt should be tested. */
#undef GNULIB_TEST_SETSOCKOPT

/* Define to 1 when the gnulib module sigaction should be tested. */
#undef GNULIB_TEST_SIGACTION

/* Define to 1 when the gnulib module sigprocmask should be tested. */
#undef GNULIB_TEST_SIGPROCMASK

/* Define to 1 when the gnulib module sleep should be tested. */
#undef GNULIB_TEST_SLEEP

/* Define to 1 when the gnulib module snprintf should be tested. */
#undef GNULIB_TEST_SNPRINTF

/* Define to 1 when the gnulib module socket should be tested. */
#undef GNULIB_TEST_SOCKET

/* Define to 1 when the gnulib module stat should be tested. */
#undef GNULIB_TEST_STAT

/* Define to 1 when the gnulib module stpcpy should be tested. */
#undef GNULIB_TEST_STPCPY

/* Define to 1 when the gnulib module strdup should be tested. */
#undef GNULIB_TEST_STRDUP

/* Define to 1 when the gnulib module strerror should be tested. */
#undef GNULIB_TEST_STRERROR

/* Define to 1 when the gnulib module strerror_r should be tested. */
#undef GNULIB_TEST_STRERROR_R

/* Define to 1 when the gnulib module strnlen should be tested. */
#undef GNULIB_TEST_STRNLEN

/* Define to 1 when the gnulib module strstr should be tested. */
#undef GNULIB_TEST_STRSTR

/* Define to 1 when the gnulib module strtoll should be tested. */
#undef GNULIB_TEST_STRTOLL

/* Define to 1 when the gnulib module strtoull should be tested. */
#undef GNULIB_TEST_STRTOULL

/* Define to 1 when the gnulib module symlink should be tested. */
#undef GNULIB_TEST_SYMLINK

/* Define to 1 when the gnulib module unsetenv should be tested. */
#undef GNULIB_TEST_UNSETENV

/* Define to 1 when the gnulib module vfprintf should be tested. */
#undef GNULIB_TEST_VFPRINTF

/* Define to 1 when the gnulib module vprintf should be tested. */
#undef GNULIB_TEST_VPRINTF

/* Define to 1 when the gnulib module wcrtomb should be tested. */
#undef GNULIB_TEST_WCRTOMB

/* Define to 1 when the gnulib module wctob should be tested. */
#undef GNULIB_TEST_WCTOB

/* Define to 1 when the gnulib module wctomb should be tested. */
#undef GNULIB_TEST_WCTOMB

/* Define to 1 when the gnulib module wcwidth should be tested. */
#undef GNULIB_TEST_WCWIDTH

/* Define to 1 when the gnulib module wmemchr should be tested. */
#undef GNULIB_TEST_WMEMCHR

/* Define to 1 when the gnulib module wmempcpy should be tested. */
#undef GNULIB_TEST_WMEMPCPY

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module unistr/u8-mbtoucr shall be considered present. */
#undef GNULIB_UNISTR_U8_MBTOUCR

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module unistr/u8-uctomb shall be considered present. */
#undef GNULIB_UNISTR_U8_UCTOMB

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module xalloc shall be considered present. */
#define GNULIB_XALLOC 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module xalloc-die shall be considered present. */
#define GNULIB_XALLOC_DIE 1

/* We are building grep */
#define GREP

/* Define to 1 if you have 'alloca' after including <alloca.h>, a header that
   may be supplied by this distribution. */
#undef HAVE_ALLOCA

/* Define to 1 if <alloca.h> works. */
#undef HAVE_ALLOCA_H

/* Define to 1 if you have the <arpa/inet.h> header file. */
#undef HAVE_ARPA_INET_H

/* Define to 1 if you have the <bp-sym.h> header file. */
#undef HAVE_BP_SYM_H

/* Define to 1 if you have the `btowc' function. */
#undef HAVE_BTOWC

/* Define to 1 if nanosleep mishandles large arguments. */
#undef HAVE_BUG_BIG_NANOSLEEP

/* Define to 1 if you have the `catgets' function. */
#undef HAVE_CATGETS

/* Define to 1 if you have the Mac OS X function
   CFLocaleCopyPreferredLanguages in the CoreFoundation framework. */
#undef HAVE_CFLOCALECOPYPREFERREDLANGUAGES

/* Define to 1 if you have the Mac OS X function CFPreferencesCopyAppValue in
   the CoreFoundation framework. */
#undef HAVE_CFPREFERENCESCOPYAPPVALUE

/* Define to 1 if you have the `closedir' function. */
#define HAVE_CLOSEDIR 1

/* Define to 1 if you have the <crtdefs.h> header file. */
#undef HAVE_CRTDEFS_H

/* Define if the GNU dcgettext() function is already present or preinstalled.
   */
#undef HAVE_DCGETTEXT

/* Define to 1 if you have the declaration of `alarm', and to 0 if you don't.
   */
#undef HAVE_DECL_ALARM

/* Define to 1 if you have the declaration of `clearerr_unlocked', and to 0 if
   you don't. */
#undef HAVE_DECL_CLEARERR_UNLOCKED

/* Define to 1 if you have the declaration of `dirfd', and to 0 if you don't.
   */
#undef HAVE_DECL_DIRFD

/* Define to 1 if you have the declaration of `ecvt', and to 0 if you don't.
   */
#undef HAVE_DECL_ECVT

/* Define to 1 if you have the declaration of `execvpe', and to 0 if you
   don't. */
#undef HAVE_DECL_EXECVPE

/* Define to 1 if you have the declaration of `fchdir', and to 0 if you don't.
   */
#define HAVE_DECL_FCHDIR 0

/* Define to 1 if you have the declaration of `fcloseall', and to 0 if you
   don't. */
#undef HAVE_DECL_FCLOSEALL

/* Define to 1 if you have the declaration of `fcvt', and to 0 if you don't.
   */
#undef HAVE_DECL_FCVT

/* Define to 1 if you have the declaration of `fdopendir', and to 0 if you
   don't. */
#undef HAVE_DECL_FDOPENDIR

/* Define to 1 if you have the declaration of `feof_unlocked', and to 0 if you
   don't. */
#undef HAVE_DECL_FEOF_UNLOCKED

/* Define to 1 if you have the declaration of `ferror_unlocked', and to 0 if
   you don't. */
#undef HAVE_DECL_FERROR_UNLOCKED

/* Define to 1 if you have the declaration of `fflush_unlocked', and to 0 if
   you don't. */
#undef HAVE_DECL_FFLUSH_UNLOCKED

/* Define to 1 if you have the declaration of `fgets_unlocked', and to 0 if
   you don't. */
#undef HAVE_DECL_FGETS_UNLOCKED

/* Define to 1 if you have the declaration of `fputc_unlocked', and to 0 if
   you don't. */
#undef HAVE_DECL_FPUTC_UNLOCKED

/* Define to 1 if you have the declaration of `fputs_unlocked', and to 0 if
   you don't. */
#undef HAVE_DECL_FPUTS_UNLOCKED

/* Define to 1 if you have the declaration of `fread_unlocked', and to 0 if
   you don't. */
#undef HAVE_DECL_FREAD_UNLOCKED

/* Define to 1 if you have the declaration of `fwrite_unlocked', and to 0 if
   you don't. */
#undef HAVE_DECL_FWRITE_UNLOCKED

/* Define to 1 if you have the declaration of `gcvt', and to 0 if you don't.
   */
#undef HAVE_DECL_GCVT

/* Define to 1 if you have the declaration of `getchar_unlocked', and to 0 if
   you don't. */
#define HAVE_DECL_GETCHAR_UNLOCKED 0

/* Define to 1 if you have the declaration of `getc_unlocked', and to 0 if you
   don't. */
#define HAVE_DECL_GETC_UNLOCKED 0

/* Define to 1 if you have the declaration of `getdtablesize', and to 0 if you
   don't. */
#undef HAVE_DECL_GETDTABLESIZE

/* Define to 1 if you have the declaration of `inet_pton', and to 0 if you
   don't. */
#undef HAVE_DECL_INET_PTON

/* Define to 1 if you have the declaration of `isblank', and to 0 if you
   don't. */
#define HAVE_DECL_ISBLANK 0

/* Define to 1 if you have the declaration of `iswblank', and to 0 if you
   don't. */
#define HAVE_DECL_ISWBLANK 1

/* Define to 1 if you have the declaration of `mbrlen', and to 0 if you don't.
   */
#define HAVE_DECL_MBRLEN 1

/* Define to 1 if you have the declaration of `mbrtowc', and to 0 if you
   don't. */
#define HAVE_DECL_MBRTOWC 1

/* Define to 1 if you have the declaration of `mbsinit', and to 0 if you
   don't. */
#define HAVE_DECL_MBSINIT 0

/* Define to 1 if you have the declaration of `mbsrtowcs', and to 0 if you
   don't. */
#define HAVE_DECL_MBSRTOWCS 0

/* Define to 1 if you have the declaration of `memrchr', and to 0 if you
   don't. */
#define HAVE_DECL_MEMRCHR 0

/* Define to 1 if you have the declaration of `program_invocation_name', and
   to 0 if you don't. */
#define HAVE_DECL_PROGRAM_INVOCATION_NAME 0

/* Define to 1 if you have the declaration of `program_invocation_short_name',
   and to 0 if you don't. */
#define HAVE_DECL_PROGRAM_INVOCATION_SHORT_NAME 0

/* Define to 1 if you have the declaration of `putchar_unlocked', and to 0 if
   you don't. */
#define HAVE_DECL_PUTCHAR_UNLOCKED 0

/* Define to 1 if you have the declaration of `putc_unlocked', and to 0 if you
   don't. */
#define HAVE_DECL_PUTC_UNLOCKED 0

/* Define to 1 if you have the declaration of `setenv', and to 0 if you don't.
   */
#define HAVE_DECL_SETENV 0

/* Define to 1 if you have the declaration of `sleep', and to 0 if you don't.
   */
#define HAVE_DECL_SLEEP 0

/* Define to 1 if you have the declaration of `snprintf', and to 0 if you
   don't. */
#define HAVE_DECL_SNPRINTF 1

/* Define to 1 if you have the declaration of `strdup', and to 0 if you don't.
   */
#define HAVE_DECL_STRDUP 1

/* Define to 1 if you have the declaration of `strerror_r', and to 0 if you
   don't. */
#define HAVE_DECL_STRERROR_R 1

/* Define to 1 if you have the declaration of `strnlen', and to 0 if you
   don't. */
#define HAVE_DECL_STRNLEN 1

/* Define to 1 if you have the declaration of `strtoimax', and to 0 if you
   don't. */
#define HAVE_DECL_STRTOIMAX 1

/* Define to 1 if you have the declaration of `strtoll', and to 0 if you
   don't. */
#define HAVE_DECL_STRTOLL 1

/* Define to 1 if you have the declaration of `strtoull', and to 0 if you
   don't. */
#define HAVE_DECL_STRTOULL 1

/* Define to 1 if you have the declaration of `strtoumax', and to 0 if you
   don't. */
#define HAVE_DECL_STRTOUMAX 1

/* Define to 1 if you have the declaration of `towlower', and to 0 if you
   don't. */
#define HAVE_DECL_TOWLOWER 1

/* Define to 1 if you have the declaration of `unsetenv', and to 0 if you
   don't. */
#define HAVE_DECL_UNSETENV 0

/* Define to 1 if you have the declaration of `wcrtomb', and to 0 if you
   don't. */
#define HAVE_DECL_WCRTOMB 0

/* Define to 1 if you have the declaration of `wcsdup', and to 0 if you don't.
   */
#define HAVE_DECL_WCSDUP 1

/* Define to 1 if you have the declaration of `wctob', and to 0 if you don't.
   */
#define HAVE_DECL_WCTOB 1

/* Define to 1 if you have the declaration of `wcwidth', and to 0 if you
   don't. */
#define HAVE_DECL_WCWIDTH 0

/* Define to 1 if you have the declaration of `_putenv', and to 0 if you
   don't. */
#define HAVE_DECL__PUTENV 1

/* Define to 1 if you have the declaration of `_snprintf', and to 0 if you
   don't. */
#undef HAVE_DECL__SNPRINTF

/* Define to 1 if you have the declaration of `__argv', and to 0 if you don't.
   */
#define HAVE_DECL___ARGV 1

/* Define to 1 if you have the declaration of `__fpending', and to 0 if you
   don't. */
#define HAVE_DECL___FPENDING 0

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#undef HAVE_DIRENT_H

/* Define to 1 if you have the `dirfd' function. */
#undef HAVE_DIRFD

/* Define to 1 if you have the `duplocale' function. */
#undef HAVE_DUPLOCALE

/* Define if you have the declaration of environ. */
#undef HAVE_ENVIRON_DECL

/* Define if the locale_t type contains insufficient information, as on
   OpenBSD. */
#undef HAVE_FAKE_LOCALES

/* Define to 1 if you have the `fchdir' function. */
#undef HAVE_FCHDIR

/* Define to 1 if you have the `fcntl' function. */
#undef HAVE_FCNTL

/* Define to 1 if you have the `fdopendir' function. */
#undef HAVE_FDOPENDIR

/* Define to 1 if you have the <features.h> header file. */
#undef HAVE_FEATURES_H

/* Define to 1 if you have the `fnmatch' function. */
#undef HAVE_FNMATCH

/* Define to 1 if you have the <fnmatch.h> header file. */
#undef HAVE_FNMATCH_H

/* Define to 1 if you have the `freelocale' function. */
#undef HAVE_FREELOCALE

/* Define if the 'free' function is guaranteed to preserve errno. */
#undef HAVE_FREE_POSIX

/* Define to 1 if you have the `fstatat' function. */
#undef HAVE_FSTATAT

/* Define to 1 if you have the `fstatfs' function. */
#undef HAVE_FSTATFS

/* Define to 1 if you have the `ftruncate' function. */
#undef HAVE_FTRUNCATE

/* Define to 1 if you have the `getdtablesize' function. */
#undef HAVE_GETDTABLESIZE

/* Define to 1 if you have the `getexecname' function. */
#undef HAVE_GETEXECNAME

/* Define to 1 if you have the `getlocalename_l' function. */
#undef HAVE_GETLOCALENAME_L

/* Define to 1 if you have the <getopt.h> header file. */
#undef HAVE_GETOPT_H

/* Define to 1 if you have the `getopt_long_only' function. */
#undef HAVE_GETOPT_LONG_ONLY

/* Define to 1 if you have the `getprogname' function. */
#define HAVE_GETPROGNAME 1
const char *getprogname(void);

/* Define to 1 if you have the `getrlimit' function. */
#undef HAVE_GETRLIMIT

/* Define if the GNU gettext() function is already present or preinstalled. */
#undef HAVE_GETTEXT

/* Define to 1 if you have the `gettimeofday' function. */
#undef HAVE_GETTIMEOFDAY

/* Define if the uselocale exists, may be safely called, and returns
   sufficient information. */
#undef HAVE_GOOD_USELOCALE

/* Define if you have the iconv() function and it works. */
#undef HAVE_ICONV

/* Define to 1 if you have the <iconv.h> header file. */
#undef HAVE_ICONV_H

/* Define to 1 if you have the `inet_pton' function. */
#undef HAVE_INET_PTON

/* Define to 1 if the compiler supports one of the keywords 'inline',
   '__inline__', '__inline' and effectively inlines functions marked as such.
   */
#undef HAVE_INLINE

/* Define if you have the 'intmax_t' type in <stdint.h> or <inttypes.h>. */
#undef HAVE_INTMAX_T

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define if <inttypes.h> exists, doesn't clash with <sys/types.h>, and
   declares uintmax_t. */
#undef HAVE_INTTYPES_H_WITH_UINTMAX

/* Define to 1 if you have the `ioctl' function. */
#undef HAVE_IOCTL

/* Define to 1 if <sys/socket.h> defines AF_INET. */
#undef HAVE_IPV4

/* Define to 1 if <sys/socket.h> defines AF_INET6. */
#undef HAVE_IPV6

/* Define to 1 if you have the `isascii' function. */
#undef HAVE_ISASCII

/* Define to 1 if you have the `isblank' function. */
#undef HAVE_ISBLANK

/* Define to 1 if you have the `iswblank' function. */
#undef HAVE_ISWBLANK

/* Define to 1 if you have the `iswcntrl' function. */
#undef HAVE_ISWCNTRL

/* Define to 1 if you have the `iswctype' function. */
#undef HAVE_ISWCTYPE

/* Define if you have <langinfo.h> and nl_langinfo(CODESET). */
#undef HAVE_LANGINFO_CODESET

/* Define to 1 if you have the <langinfo.h> header file. */
#undef HAVE_LANGINFO_H

/* Define if your <locale.h> file defines LC_MESSAGES. */
#undef HAVE_LC_MESSAGES

/* Define to 1 if you have the <libintl.h> header file. */
#undef HAVE_LIBINTL_H

/* Define to 1 if you have the Perl Compatible Regular Expressions library
   (-lpcre). */
#undef HAVE_LIBPCRE

/* Define if you have the libsigsegv library. */
#undef HAVE_LIBSIGSEGV

/* Define to 1 if you have the <limits.h> header file. */
#undef HAVE_LIMITS_H

/* Define to 1 if the system has the type 'long long int'. */
#undef HAVE_LONG_LONG_INT

/* Define to 1 if you have the `lstat' function. */
#undef HAVE_LSTAT

/* Define to 1 if you have the <malloc.h> header file. */
#undef HAVE_MALLOC_H

/* Define if malloc, realloc, and calloc set errno on allocation failure. */
#undef HAVE_MALLOC_POSIX

/* Define to 1 if mmap()'s MAP_ANONYMOUS flag is available after including
   config.h and <sys/mman.h>. */
#undef HAVE_MAP_ANONYMOUS

/* Define to 1 if you have the `mbrlen' function. */
#undef HAVE_MBRLEN

/* Define to 1 if you have the `mbrtowc' function. */
#undef HAVE_MBRTOWC

/* Define to 1 if you have the `mbsinit' function. */
#undef HAVE_MBSINIT

/* Define to 1 if you have the `mbslen' function. */
#undef HAVE_MBSLEN

/* Define to 1 if you have the `mbsrtowcs' function. */
#undef HAVE_MBSRTOWCS

/* Define to 1 if <wchar.h> declares mbstate_t. */
#undef HAVE_MBSTATE_T

/* Define to 1 if you have the `mbtowc' function. */
#undef HAVE_MBTOWC

/* Define to 1 if you have the `mempcpy' function. */
#define HAVE_MEMPCPY 1

#include <string.h>
static _inline void *mempcpy(void *pvDst, const void *pvSrc, size_t cb)
{
    return (char *)memcpy(pvDst, pvSrc, cb) + cb;
}

/* Define to 1 if you have the `memrchr' function. */
#undef HAVE_MEMRCHR
void *memrchr(const void *, int, size_t);

/* Define to 1 if you have the <minix/config.h> header file. */
#undef HAVE_MINIX_CONFIG_H

/* Define to 1 if <limits.h> defines the MIN and MAX macros. */
#undef HAVE_MINMAX_IN_LIMITS_H

/* Define to 1 if <sys/param.h> defines the MIN and MAX macros. */
#undef HAVE_MINMAX_IN_SYS_PARAM_H

/* Define to 1 if you have the `mprotect' function. */
#undef HAVE_MPROTECT

/* Define to 1 on MSVC platforms that have the "invalid parameter handler"
   concept. */
#undef HAVE_MSVC_INVALID_PARAMETER_HANDLER

/* Define if the locale_t type does not contain the name of each locale
   category. */
#undef HAVE_NAMELESS_LOCALES

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
#undef HAVE_NDIR_H

/* Define to 1 if you have the <netdb.h> header file. */
#undef HAVE_NETDB_H

/* Define to 1 if you have the <netinet/in.h> header file. */
#undef HAVE_NETINET_IN_H

/* Define to 1 if you have the `newlocale' function. */
#undef HAVE_NEWLOCALE

/* Define to 1 if you have the `nl_langinfo' function. */
#undef HAVE_NL_LANGINFO

/* Define to 1 if the system has obstacks that work with any size object. */
#undef HAVE_OBSTACK

/* Define to 1 if you have the `openat' function. */
#define HAVE_OPENAT 1

/* Define to 1 if you have the `opendir' function. */
#define HAVE_OPENDIR 1

/* Define to 1 if you have the <OS.h> header file. */
#undef HAVE_OS_H

/* Define to 1 if you have the `pipe' function. */
#undef HAVE_PIPE

/* Define if you have the <pthread.h> header and the POSIX threads API. */
#undef HAVE_PTHREAD_API

/* Define to 1 if you have the `pthread_atfork' function. */
#undef HAVE_PTHREAD_ATFORK

/* Define to 1 if you have the <pthread.h> header file. */
#undef HAVE_PTHREAD_H

/* Define if the <pthread.h> defines PTHREAD_MUTEX_RECURSIVE. */
#undef HAVE_PTHREAD_MUTEX_RECURSIVE

/* Define if the POSIX multithreading library has read/write locks. */
#undef HAVE_PTHREAD_RWLOCK

/* Define if the 'pthread_rwlock_rdlock' function prefers a writer to a
   reader. */
#undef HAVE_PTHREAD_RWLOCK_RDLOCK_PREFER_WRITER

/* Define to 1 if the pthread_sigmask function can be used (despite bugs). */
#undef HAVE_PTHREAD_SIGMASK

/* Define to 1 if the system has the type `pthread_spinlock_t'. */
#undef HAVE_PTHREAD_SPINLOCK_T

/* Define to 1 if the system has the type `pthread_t'. */
#undef HAVE_PTHREAD_T

/* Define to 1 if you have the `raise' function. */
#undef HAVE_RAISE

/* Define to 1 if you have the `rawmemchr' function. */
#define HAVE_RAWMEMCHR 1


#include <string.h>
static _inline void *rawmemchr(const void *pv, int ch)
{
    return memchr(pv, ch, ~(size_t)0 / 4);
}

/* Define to 1 if you have the `readdir' function. */
#define HAVE_READDIR 1

/* Define to 1 if you have the `reallocarray' function. */
#undef HAVE_REALLOCARRAY

/* Define to 1 if the system has the type `sa_family_t'. */
#undef HAVE_SA_FAMILY_T

/* Define to 1 if you have the <sched.h> header file. */
#undef HAVE_SCHED_H

/* Define to 1 if you have the <sdkddkver.h> header file. */
#undef HAVE_SDKDDKVER_H

/* Define to 1 if you have the <search.h> header file. */
#undef HAVE_SEARCH_H

/* Define to 1 if you have the `setdtablesize' function. */
#undef HAVE_SETDTABLESIZE

/* Define to 1 if you have the `setenv' function. */
#undef HAVE_SETENV

/* Define to 1 if you have the `setlocale' function. */
#define HAVE_SETLOCALE 1

/* Define to 1 if you have the `setrlimit' function. */
#undef HAVE_SETRLIMIT

/* Define to 1 if you have the `shutdown' function. */
#undef HAVE_SHUTDOWN

/* Define to 1 if you have the `sigaction' function. */
#undef HAVE_SIGACTION

/* Define to 1 if you have the `sigaltstack' function. */
#undef HAVE_SIGALTSTACK

/* Define to 1 if the system has the type `siginfo_t'. */
#undef HAVE_SIGINFO_T

/* Define to 1 if you have the `siginterrupt' function. */
#undef HAVE_SIGINTERRUPT

/* Define to 1 if 'sig_atomic_t' is a signed integer type. */
#undef HAVE_SIGNED_SIG_ATOMIC_T

/* Define to 1 if 'wchar_t' is a signed integer type. */
#undef HAVE_SIGNED_WCHAR_T

/* Define to 1 if 'wint_t' is a signed integer type. */
#undef HAVE_SIGNED_WINT_T

/* Define to 1 if the system has the type `sigset_t'. */
#undef HAVE_SIGSET_T

/* Define to 1 if you have the `sleep' function. */
#undef HAVE_SLEEP

/* Define to 1 if you have the `snprintf' function. */
#undef HAVE_SNPRINTF

/* Define if the return value of the snprintf function is the number of of
   bytes (excluding the terminating NUL) that would have been produced if the
   buffer had been large enough. */
#undef HAVE_SNPRINTF_RETVAL_C99

/* Define if the string produced by the snprintf function is always NUL
   terminated. */
#undef HAVE_SNPRINTF_TRUNCATION_C99

/* Define if the locale_t type is as on Solaris 11.4. */
#undef HAVE_SOLARIS114_LOCALES

/* Define to 1 if you have the <stdint.h> header file. */
#undef HAVE_STDINT_H

/* Define if <stdint.h> exists, doesn't clash with <sys/types.h>, and declares
   uintmax_t. */
#undef HAVE_STDINT_H_WITH_UINTMAX

/* Define to 1 if you have the <stdio_ext.h> header file. */
#undef HAVE_STDIO_EXT_H

/* Define to 1 if you have the <stdio.h> header file. */
#undef HAVE_STDIO_H

/* Define to 1 if you have the <stdlib.h> header file. */
#undef HAVE_STDLIB_H

/* Define to 1 if you have the `stpcpy' function. */
#undef HAVE_STPCPY

/* Define to 1 if you have the `strerror_r' function. */
#define HAVE_STRERROR_R 1
#include <string.h>
static _inline int strerror_r(int iErr, char *pszDst, size_t cbDst)
{
    return strerror_s(pszDst, cbDst, iErr);
}

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#undef HAVE_STRING_H

/* Define to 1 if you have the `strnlen' function. */
#undef HAVE_STRNLEN

/* Define to 1 if you have the `strtoimax' function. */
#undef HAVE_STRTOIMAX

/* Define to 1 if you have the `strtoll' function. */
#undef HAVE_STRTOLL

/* Define to 1 if you have the `strtoull' function. */
#undef HAVE_STRTOULL

/* Define to 1 if you have the `strtoumax' function. */
#undef HAVE_STRTOUMAX

/* Define if there is a member named d_type in the struct describing directory
   headers. */
#undef HAVE_STRUCT_DIRENT_D_TYPE

/* Define to 1 if `decimal_point' is a member of `struct lconv'. */
#undef HAVE_STRUCT_LCONV_DECIMAL_POINT

/* Define to 1 if `sa_sigaction' is a member of `struct sigaction'. */
#undef HAVE_STRUCT_SIGACTION_SA_SIGACTION

/* Define to 1 if the system has the type `struct sockaddr_storage'. */
#undef HAVE_STRUCT_SOCKADDR_STORAGE

/* Define to 1 if `ss_family' is a member of `struct sockaddr_storage'. */
#undef HAVE_STRUCT_SOCKADDR_STORAGE_SS_FAMILY

/* Define to 1 if `f_type' is a member of `struct statfs'. */
#undef HAVE_STRUCT_STATFS_F_TYPE

/* Define to 1 if `st_atimensec' is a member of `struct stat'. */
#undef HAVE_STRUCT_STAT_ST_ATIMENSEC

/* Define to 1 if `st_atimespec.tv_nsec' is a member of `struct stat'. */
#undef HAVE_STRUCT_STAT_ST_ATIMESPEC_TV_NSEC

/* Define to 1 if `st_atim.st__tim.tv_nsec' is a member of `struct stat'. */
#undef HAVE_STRUCT_STAT_ST_ATIM_ST__TIM_TV_NSEC

/* Define to 1 if `st_atim.tv_nsec' is a member of `struct stat'. */
#undef HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC

/* Define to 1 if `st_birthtimensec' is a member of `struct stat'. */
#undef HAVE_STRUCT_STAT_ST_BIRTHTIMENSEC

/* Define to 1 if `st_birthtimespec.tv_nsec' is a member of `struct stat'. */
#undef HAVE_STRUCT_STAT_ST_BIRTHTIMESPEC_TV_NSEC

/* Define to 1 if `st_birthtim.tv_nsec' is a member of `struct stat'. */
#undef HAVE_STRUCT_STAT_ST_BIRTHTIM_TV_NSEC

/* Define to 1 if you have the `symlink' function. */
#undef HAVE_SYMLINK

/* Define to 1 if you have the <sys/bitypes.h> header file. */
#undef HAVE_SYS_BITYPES_H

/* Define to 1 if you have the <sys/cdefs.h> header file. */
#undef HAVE_SYS_CDEFS_H

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
#undef HAVE_SYS_DIR_H

/* Define to 1 if you have the <sys/inttypes.h> header file. */
#undef HAVE_SYS_INTTYPES_H

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#undef HAVE_SYS_IOCTL_H

/* Define to 1 if you have the <sys/mman.h> header file. */
#undef HAVE_SYS_MMAN_H

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
#undef HAVE_SYS_NDIR_H

/* Define to 1 if you have the <sys/param.h> header file. */
#undef HAVE_SYS_PARAM_H

/* Define to 1 if you have the <sys/select.h> header file. */
#undef HAVE_SYS_SELECT_H

/* Define to 1 if you have the <sys/single_threaded.h> header file. */
#undef HAVE_SYS_SINGLE_THREADED_H

/* Define to 1 if you have the <sys/socket.h> header file. */
#undef HAVE_SYS_SOCKET_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#undef HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/time.h> header file. */
#undef HAVE_SYS_TIME_H

/* Define to 1 if you have the <sys/types.h> header file. */
#undef HAVE_SYS_TYPES_H

/* Define to 1 if you have the <sys/uio.h> header file. */
#undef HAVE_SYS_UIO_H

/* Define to 1 if you have the <sys/vfs.h> header file. */
#undef HAVE_SYS_VFS_H

/* Define to 1 if you have the <sys/wait.h> header file. */
#undef HAVE_SYS_WAIT_H

/* Define to 1 if you have the `thrd_create' function. */
#undef HAVE_THRD_CREATE

/* Define to 1 if you have the <threads.h> header file. */
#undef HAVE_THREADS_H

/* Define to 1 if you have the `towlower' function. */
#undef HAVE_TOWLOWER

/* Define to 1 if you have the `tsearch' function. */
#undef HAVE_TSEARCH

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define to 1 if you have the `unsetenv' function. */
#undef HAVE_UNSETENV

/* Define to 1 if the system has the type 'unsigned long long int'. */
#undef HAVE_UNSIGNED_LONG_LONG_INT

/* Define to 1 if you have the `uselocale' function. */
#undef HAVE_USELOCALE

/* Define if you have a global __progname variable */
#undef HAVE_VAR___PROGNAME

/* Define to 1 if you have the `vasnprintf' function. */
#undef HAVE_VASNPRINTF

/* Define to 1 or 0, depending whether the compiler supports simple visibility
   declarations. */
#undef HAVE_VISIBILITY

/* Define to 1 if you have the <wchar.h> header file. */
#undef HAVE_WCHAR_H

/* Define if you have the 'wchar_t' type. */
#undef HAVE_WCHAR_T

/* Define to 1 if you have the `wcrtomb' function. */
#undef HAVE_WCRTOMB

/* Define to 1 if you have the `wcslen' function. */
#undef HAVE_WCSLEN

/* Define to 1 if you have the `wcsnlen' function. */
#undef HAVE_WCSNLEN

/* Define to 1 if you have the `wctob' function. */
#undef HAVE_WCTOB

/* Define to 1 if you have the <wctype.h> header file. */
#undef HAVE_WCTYPE_H

/* Define to 1 if you have the `wcwidth' function. */
#undef HAVE_WCWIDTH

/* Define to 1 if the compiler and linker support weak declarations of
   symbols. */
#undef HAVE_WEAK_SYMBOLS

/* Define to 1 if you have the <winsock2.h> header file. */
#undef HAVE_WINSOCK2_H

/* Define if you have the 'wint_t' type. */
#undef HAVE_WINT_T

/* Define to 1 if you have the `wmempcpy' function. */
#undef HAVE_WMEMPCPY

/* Define to 1 if fstatat (..., 0) works. For example, it does not work in AIX
   7.1. */
#undef HAVE_WORKING_FSTATAT_ZERO_FLAG

/* Define to 1 if O_NOATIME works. */
#undef HAVE_WORKING_O_NOATIME

/* Define to 1 if O_NOFOLLOW works. */
#undef HAVE_WORKING_O_NOFOLLOW

/* Define if you have the sigaltstack() function and it works. */
#undef HAVE_WORKING_SIGALTSTACK

/* Define if the uselocale function exists and may safely be called. */
#undef HAVE_WORKING_USELOCALE

/* Define to 1 if you have the <ws2tcpip.h> header file. */
#undef HAVE_WS2TCPIP_H

/* Define to 1 if you have the <xlocale.h> header file. */
#undef HAVE_XLOCALE_H

/* Define to 1 if the system has the type `_Bool'. */
#undef HAVE__BOOL

/* Define to 1 if you have the `_chsize' function. */
#undef HAVE__CHSIZE

/* Define to 1 if you have the `_set_invalid_parameter_handler' function. */
#undef HAVE__SET_INVALID_PARAMETER_HANDLER

/* Define to 1 if the compiler supports __builtin_expect,
   and to 2 if <builtins.h> does.  */
#undef HAVE___BUILTIN_EXPECT
#ifndef HAVE___BUILTIN_EXPECT
# define __builtin_expect(e, c) (e)
#elif HAVE___BUILTIN_EXPECT == 2
# include <builtins.h>
#endif
    

/* Define to 1 if the system has the type `__fsword_t'. */
#undef HAVE___FSWORD_T

/* Define to 1 if the compiler supports the keyword '__inline'. */
#undef HAVE___INLINE

/* Define to 1 if you have the `__xpg_strerror_r' function. */
#undef HAVE___XPG_STRERROR_R

/* Define as const if the declaration of iconv() needs const. */
#undef ICONV_CONST

/* Define to a symbolic name denoting the flavor of iconv_open()
   implementation. */
#undef ICONV_FLAVOR

/* Define if localename.c overrides newlocale(), duplocale(), freelocale(). */
#undef LOCALENAME_ENHANCE_LOCALE_FUNCS

/* Define to 1 if lseek does not detect pipes. */
#undef LSEEK_PIPE_BROKEN

/* Define to 1 if 'lstat' dereferences a symlink specified with a trailing
   slash. */
#undef LSTAT_FOLLOWS_SLASHED_SYMLINK

/* If malloc(0) is != NULL, define this to 1. Otherwise define this to 0. */
#undef MALLOC_0_IS_NONNULL

/* Define to a substitute value for mmap()'s MAP_ANONYMOUS flag. */
#undef MAP_ANONYMOUS

/* Define if the mbrtowc function does not return (size_t) -2 for empty input.
   */
#undef MBRTOWC_EMPTY_INPUT_BUG

/* Define if the mbrtowc function may signal encoding errors in the C locale.
   */
#undef MBRTOWC_IN_C_LOCALE_MAYBE_EILSEQ

/* Define if the mbrtowc function has the NULL pwc argument bug. */
#undef MBRTOWC_NULL_ARG1_BUG

/* Define if the mbrtowc function has the NULL string argument bug. */
#undef MBRTOWC_NULL_ARG2_BUG

/* Define if the mbrtowc function does not return 0 for a NUL character. */
#undef MBRTOWC_NUL_RETVAL_BUG

/* Define if the mbrtowc function returns a wrong return value. */
#undef MBRTOWC_RETVAL_BUG

/* Define if the mbrtowc function stores a wide character when reporting
   incomplete input. */
#undef MBRTOWC_STORES_INCOMPLETE_BUG

/* Use GNU style printf and scanf.  */
#ifndef __USE_MINGW_ANSI_STDIO
# undef __USE_MINGW_ANSI_STDIO
#endif


/* Define to 1 on musl libc. */
#undef MUSL_LIBC

/* Define to 1 if assertions should be disabled. */
#undef NDEBUG

/* Define to 1 to enable general improvements of setlocale. */
#undef NEED_SETLOCALE_IMPROVED

/* Define to 1 to enable a multithread-safety fix of setlocale. */
#undef NEED_SETLOCALE_MTSAFE

/* Define to 1 if nl_langinfo is multithread-safe. */
#undef NL_LANGINFO_MTSAFE

/* Define to 1 if open() fails to recognize a trailing slash. */
#undef OPEN_TRAILING_SLASH_BUG

/* Name of package */
#define PACKAGE "kmk_grep"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "bird-kbuild-spam-ixx@anduin.net"

/* Define to the full name of this package. */
#define PACKAGE_NAME "kmk_grep"

/* String identifying the packager of this software */
#undef PACKAGE_PACKAGER

/* Packager info for bug reports (URL/e-mail/...) */
#undef PACKAGE_PACKAGER_BUG_REPORTS

/* Packager-specific version information */
#undef PACKAGE_PACKAGER_VERSION

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the home page for this package. */
#undef PACKAGE_URL

/* Define to the version of this package. */
#define PACKAGE_VERSION "3.7"

/* Define to the type that is the result of default argument promotions of
   type mode_t. */
#define PROMOTED_MODE_T uintptr_t

/* Define if pthread_create is an inline function. */
#undef PTHREAD_CREATE_IS_INLINE

/* Define if the pthread_in_use() detection is hard. */
#undef PTHREAD_IN_USE_DETECTION_HARD

/* Define to 1 if pthread_sigmask(), when it fails, returns -1 and sets errno.
   */
#undef PTHREAD_SIGMASK_FAILS_WITH_ERRNO

/* Define to 1 if pthread_sigmask may return 0 and have no effect. */
#undef PTHREAD_SIGMASK_INEFFECTIVE

/* Define to 1 if pthread_sigmask() unblocks signals incorrectly. */
#undef PTHREAD_SIGMASK_UNBLOCK_BUG

/* Define to l, ll, u, ul, ull, etc., as suitable for constants of type
   'ptrdiff_t'. */
#undef PTRDIFF_T_SUFFIX

/* Define to 1 if gnulib's dirfd() replacement is used. */
#undef REPLACE_DIRFD

/* Define to 1 if gnulib's fchdir() replacement is used. */
#define REPLACE_FCHDIR 1

/* Define to 1 if stat needs help when passed a file name with a trailing
   slash */
#undef REPLACE_FUNC_STAT_FILE

/* Define if nl_langinfo exists but is overridden by gnulib. */
#undef REPLACE_NL_LANGINFO

/* Define to 1 if open() should work around the inability to open a directory.
   */
#undef REPLACE_OPEN_DIRECTORY

/* Define to 1 if strerror(0) does not return a message implying success. */
#undef REPLACE_STRERROR_0

/* Define if vasnprintf exists but is overridden by gnulib. */
#undef REPLACE_VASNPRINTF

/* Define to 1 if setlocale (LC_ALL, NULL) is multithread-safe. */
#undef SETLOCALE_NULL_ALL_MTSAFE

/* Define to 1 if setlocale (category, NULL) is multithread-safe. */
#undef SETLOCALE_NULL_ONE_MTSAFE

/* Define if sigaltstack() interprets the stack_t.ss_sp field incorrectly, as
   the highest address of the alternate stack range rather than as the lowest
   address. */
#undef SIGALTSTACK_SS_REVERSED

/* Define to l, ll, u, ul, ull, etc., as suitable for constants of type
   'sig_atomic_t'. */
#undef SIG_ATOMIC_T_SUFFIX

/* Define as the maximum value of type 'size_t', if the system doesn't define
   it. */
#ifndef SIZE_MAX
# undef SIZE_MAX
#endif

/* Define to l, ll, u, ul, ull, etc., as suitable for constants of type
   'size_t'. */
#undef SIZE_T_SUFFIX

/* Define on Solaris 11 and its derivates. */
#undef SOLARIS11

/* Define as the direction of stack growth for your system. STACK_DIRECTION >
   0 => grows toward higher addresses STACK_DIRECTION < 0 => grows toward
   lower addresses STACK_DIRECTION = 0 => spaghetti stack. */
#undef STACK_DIRECTION

/* Define to 1 if the `S_IS*' macros in <sys/stat.h> do not work properly. */
#undef STAT_MACROS_BROKEN

/* Define to 1 if all of the C90 standard headers exist (not just the ones
   required in a freestanding environment). This macro is provided for
   backward compatibility; new code need not use it. */
#undef STDC_HEADERS

/* Define to 1 if strerror_r returns char *. */
#undef STRERROR_R_CHAR_P

/* Define to 1 if the type of the st_atim member of a struct stat is struct
   timespec. */
#undef TYPEOF_STRUCT_STAT_ST_ATIM_IS_STRUCT_TIMESPEC

/* Define if the combination of the ISO C and POSIX multithreading APIs can be
   used. */
#undef USE_ISOC_AND_POSIX_THREADS

/* Define if the ISO C multithreading library can be used. */
#undef USE_ISOC_THREADS

/* Define if the POSIX multithreading library can be used. */
#undef USE_POSIX_THREADS

/* Define if references to the POSIX multithreading library are satisfied by
   libc. */
#undef USE_POSIX_THREADS_FROM_LIBC

/* Define if references to the POSIX multithreading library should be made
   weak. */
#undef USE_POSIX_THREADS_WEAK

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# undef _ALL_SOURCE
#endif
/* Enable general extensions on macOS.  */
#ifndef _DARWIN_C_SOURCE
# undef _DARWIN_C_SOURCE
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# undef __EXTENSIONS__
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
/* Enable X/Open compliant socket functions that do not require linking
   with -lxnet on HP-UX 11.11.  */
#ifndef _HPUX_ALT_XOPEN_SOCKET_API
# undef _HPUX_ALT_XOPEN_SOCKET_API
#endif
/* Identify the host operating system as Minix.
   This macro does not affect the system headers' behavior.
   A future release of Autoconf may stop defining this macro.  */
#ifndef _MINIX
# undef _MINIX
#endif
/* Enable general extensions on NetBSD.
   Enable NetBSD compatibility extensions on Minix.  */
#ifndef _NETBSD_SOURCE
# undef _NETBSD_SOURCE
#endif
/* Enable OpenBSD compatibility extensions on NetBSD.
   Oddly enough, this does nothing on OpenBSD.  */
#ifndef _OPENBSD_SOURCE
# undef _OPENBSD_SOURCE
#endif
/* Define to 1 if needed for POSIX-compatible behavior.  */
#ifndef _POSIX_SOURCE
# undef _POSIX_SOURCE
#endif
/* Define to 2 if needed for POSIX-compatible behavior.  */
#ifndef _POSIX_1_SOURCE
# undef _POSIX_1_SOURCE
#endif
/* Enable POSIX-compatible threading on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# undef _POSIX_PTHREAD_SEMANTICS
#endif
/* Enable extensions specified by ISO/IEC TS 18661-5:2014.  */
#ifndef __STDC_WANT_IEC_60559_ATTRIBS_EXT__
# undef __STDC_WANT_IEC_60559_ATTRIBS_EXT__
#endif
/* Enable extensions specified by ISO/IEC TS 18661-1:2014.  */
#ifndef __STDC_WANT_IEC_60559_BFP_EXT__
# undef __STDC_WANT_IEC_60559_BFP_EXT__
#endif
/* Enable extensions specified by ISO/IEC TS 18661-2:2015.  */
#ifndef __STDC_WANT_IEC_60559_DFP_EXT__
# undef __STDC_WANT_IEC_60559_DFP_EXT__
#endif
/* Enable extensions specified by ISO/IEC TS 18661-4:2015.  */
#ifndef __STDC_WANT_IEC_60559_FUNCS_EXT__
# undef __STDC_WANT_IEC_60559_FUNCS_EXT__
#endif
/* Enable extensions specified by ISO/IEC TS 18661-3:2015.  */
#ifndef __STDC_WANT_IEC_60559_TYPES_EXT__
# undef __STDC_WANT_IEC_60559_TYPES_EXT__
#endif
/* Enable extensions specified by ISO/IEC TR 24731-2:2010.  */
#ifndef __STDC_WANT_LIB_EXT2__
# undef __STDC_WANT_LIB_EXT2__
#endif
/* Enable extensions specified by ISO/IEC 24747:2009.  */
#ifndef __STDC_WANT_MATH_SPEC_FUNCS__
# undef __STDC_WANT_MATH_SPEC_FUNCS__
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# undef _TANDEM_SOURCE
#endif
/* Enable X/Open extensions.  Define to 500 only if necessary
   to make mbstate_t available.  */
#ifndef _XOPEN_SOURCE
# undef _XOPEN_SOURCE
#endif


/* An alias of GNULIB_STDIO_SINGLE_THREAD. */
#undef USE_UNLOCKED_IO

/* Define if the native Windows multithreading API can be used. */
#undef USE_WINDOWS_THREADS

/* Version number of package */
#define VERSION PACKAGE_VERSION

/* Define to 1 if unsetenv returns void instead of int. */
#undef VOID_UNSETENV

/* Define to l, ll, u, ul, ull, etc., as suitable for constants of type
   'wchar_t'. */
#undef WCHAR_T_SUFFIX

/* Define if the wcrtomb function does not work in the C locale. */
#undef WCRTOMB_C_LOCALE_BUG

/* Define if the wcrtomb function has an incorrect return value. */
#undef WCRTOMB_RETVAL_BUG

/* Define if WSAStartup is needed. */
#undef WINDOWS_SOCKETS

/* Define to l, ll, u, ul, ull, etc., as suitable for constants of type
   'wint_t'. */
#undef WINT_T_SUFFIX

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
#  undef WORDS_BIGENDIAN
# endif
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
#undef _FILE_OFFSET_BITS

/* True if the compiler says it groks GNU C version MAJOR.MINOR.  */
#if defined __GNUC__ && defined __GNUC_MINOR__
# define _GL_GNUC_PREREQ(major, minor) \
    ((major) < __GNUC__ + ((minor) <= __GNUC_MINOR__))
#else
# define _GL_GNUC_PREREQ(major, minor) 0
#endif


/* Define to enable the declarations of ISO C 11 types and functions. */
#undef _ISOC11_SOURCE

/* Define for large files, on AIX-style hosts. */
#undef _LARGE_FILES

/* Define to 1 on Solaris. */
#undef _LCONV_C99

/* The _Noreturn keyword of C11.  */
#ifndef _Noreturn
# if (defined __cplusplus \
      && ((201103 <= __cplusplus && !(__GNUC__ == 4 && __GNUC_MINOR__ == 7)) \
          || (defined _MSC_VER && 1900 <= _MSC_VER)) \
      && 0)
    /* [[noreturn]] is not practically usable, because with it the syntax
         extern _Noreturn void func (...);
       would not be valid; such a declaration would only be valid with 'extern'
       and '_Noreturn' swapped, or without the 'extern' keyword.  However, some
       AIX system header files and several gnulib header files use precisely
       this syntax with 'extern'.  */
#  define _Noreturn [[noreturn]]
# elif ((!defined __cplusplus || defined __clang__) \
        && (201112 <= (defined __STDC_VERSION__ ? __STDC_VERSION__ : 0) \
            || (!defined __STRICT_ANSI__ \
                && (_GL_GNUC_PREREQ (4, 7) \
                    || (defined __apple_build_version__ \
                        ? 6000000 <= __apple_build_version__ \
                        : 3 < __clang_major__ + (5 <= __clang_minor__))))))
   /* _Noreturn works as-is.  */
# elif _GL_GNUC_PREREQ (2, 8) || defined __clang__ || 0x5110 <= __SUNPRO_C
#  define _Noreturn __attribute__ ((__noreturn__))
# elif 1200 <= (defined _MSC_VER ? _MSC_VER : 0)
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn
# endif
#endif


/* Define to 1 in order to get the POSIX compatible declarations of socket
   functions. */
#undef _POSIX_PII_SOCKET

/* Define if you want <regex.h> to include <limits.h>, so that it consistently
   overrides <limits.h>'s RE_DUP_MAX. */
#undef _REGEX_INCLUDE_LIMITS_H

/* Define if you want regoff_t to be at least as wide POSIX requires. */
#undef _REGEX_LARGE_OFFSETS

/* Number of bits in a timestamp, on hosts where this is settable. */
#undef _TIME_BITS

/* For standard stat data types on VMS. */
#undef _USE_STD_STAT

/* Define to rpl_ if the getopt replacement functions and variables should be
   used. */
#undef __GETOPT_PREFIX

/* For 64-bit time_t on 32-bit mingw. */
#undef __MINGW_USE_VC2005_COMPAT

/* Define to 1 if the system <stdint.h> predates C++11. */
#undef __STDC_CONSTANT_MACROS

/* Define to 1 if the system <stdint.h> predates C++11. */
#undef __STDC_LIMIT_MACROS

/* The _GL_ASYNC_SAFE marker should be attached to functions that are
   signal handlers (for signals other than SIGABRT, SIGPIPE) or can be
   invoked from such signal handlers.  Such functions have some restrictions:
     * All functions that it calls should be marked _GL_ASYNC_SAFE as well,
       or should be listed as async-signal-safe in POSIX
       <https://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_04>
       section 2.4.3.  Note that malloc(), sprintf(), and fwrite(), in
       particular, are NOT async-signal-safe.
     * All memory locations (variables and struct fields) that these functions
       access must be marked 'volatile'.  This holds for both read and write
       accesses.  Otherwise the compiler might optimize away stores to and
       reads from such locations that occur in the program, depending on its
       data flow analysis.  For example, when the program contains a loop
       that is intended to inspect a variable set from within a signal handler
           while (!signal_occurred)
             ;
       the compiler is allowed to transform this into an endless loop if the
       variable 'signal_occurred' is not declared 'volatile'.
   Additionally, recall that:
     * A signal handler should not modify errno (except if it is a handler
       for a fatal signal and ends by raising the same signal again, thus
       provoking the termination of the process).  If it invokes a function
       that may clobber errno, it needs to save and restore the value of
       errno.  */
#define _GL_ASYNC_SAFE


/* Attributes.  */
#if (defined __has_attribute \
     && (!defined __clang_minor__ \
         || 3 < __clang_major__ + (5 <= __clang_minor__)))
# define _GL_HAS_ATTRIBUTE(attr) __has_attribute (__##attr##__)
#else
# define _GL_HAS_ATTRIBUTE(attr) _GL_ATTR_##attr
# define _GL_ATTR_alloc_size _GL_GNUC_PREREQ (4, 3)
# define _GL_ATTR_always_inline _GL_GNUC_PREREQ (3, 2)
# define _GL_ATTR_artificial _GL_GNUC_PREREQ (4, 3)
# define _GL_ATTR_cold _GL_GNUC_PREREQ (4, 3)
# define _GL_ATTR_const _GL_GNUC_PREREQ (2, 95)
# define _GL_ATTR_deprecated _GL_GNUC_PREREQ (3, 1)
# define _GL_ATTR_diagnose_if 0
# define _GL_ATTR_error _GL_GNUC_PREREQ (4, 3)
# define _GL_ATTR_externally_visible _GL_GNUC_PREREQ (4, 1)
# define _GL_ATTR_fallthrough _GL_GNUC_PREREQ (7, 0)
# define _GL_ATTR_format _GL_GNUC_PREREQ (2, 7)
# define _GL_ATTR_leaf _GL_GNUC_PREREQ (4, 6)
# define _GL_ATTR_malloc _GL_GNUC_PREREQ (3, 0)
# ifdef _ICC
#  define _GL_ATTR_may_alias 0
# else
#  define _GL_ATTR_may_alias _GL_GNUC_PREREQ (3, 3)
# endif
# define _GL_ATTR_noinline _GL_GNUC_PREREQ (3, 1)
# define _GL_ATTR_nonnull _GL_GNUC_PREREQ (3, 3)
# define _GL_ATTR_nonstring _GL_GNUC_PREREQ (8, 0)
# define _GL_ATTR_nothrow _GL_GNUC_PREREQ (3, 3)
# define _GL_ATTR_packed _GL_GNUC_PREREQ (2, 7)
# define _GL_ATTR_pure _GL_GNUC_PREREQ (2, 96)
# define _GL_ATTR_returns_nonnull _GL_GNUC_PREREQ (4, 9)
# define _GL_ATTR_sentinel _GL_GNUC_PREREQ (4, 0)
# define _GL_ATTR_unused _GL_GNUC_PREREQ (2, 7)
# define _GL_ATTR_warn_unused_result _GL_GNUC_PREREQ (3, 4)
#endif

#ifdef __has_c_attribute
# define _GL_HAS_C_ATTRIBUTE(attr) __has_c_attribute (__##attr##__)
#else
# define _GL_HAS_C_ATTRIBUTE(attr) 0
#endif


#if _GL_HAS_ATTRIBUTE (alloc_size)
# define _GL_ATTRIBUTE_ALLOC_SIZE(args) __attribute__ ((__alloc_size__ args))
#else
# define _GL_ATTRIBUTE_ALLOC_SIZE(args)
#endif

#if _GL_HAS_ATTRIBUTE (always_inline)
# define _GL_ATTRIBUTE_ALWAYS_INLINE __attribute__ ((__always_inline__))
#else
# define _GL_ATTRIBUTE_ALWAYS_INLINE
#endif

#if _GL_HAS_ATTRIBUTE (artificial)
# define _GL_ATTRIBUTE_ARTIFICIAL __attribute__ ((__artificial__))
#else
# define _GL_ATTRIBUTE_ARTIFICIAL
#endif

/* Avoid __attribute__ ((cold)) on MinGW; see thread starting at
   <https://lists.gnu.org/r/emacs-devel/2019-04/msg01152.html>.
   Also, Oracle Studio 12.6 requires 'cold' not '__cold__'.  */
#if _GL_HAS_ATTRIBUTE (cold) && !defined __MINGW32__
# ifndef __SUNPRO_C
#  define _GL_ATTRIBUTE_COLD __attribute__ ((__cold__))
# else
#  define _GL_ATTRIBUTE_COLD __attribute__ ((cold))
# endif
#else
# define _GL_ATTRIBUTE_COLD
#endif

#if _GL_HAS_ATTRIBUTE (const)
# define _GL_ATTRIBUTE_CONST __attribute__ ((__const__))
#else
# define _GL_ATTRIBUTE_CONST
#endif

/* _GL_ATTRIBUTE_DEALLOC (F, I) is for functions returning pointers
   that can be freed by passing them as the Ith argument to the
   function F.  _GL_ATTRIBUTE_DEALLOC_FREE is for functions that
   return pointers that can be freed via 'free'; it can be used
   only after including stdlib.h.  These macros cannot be used on
   inline functions.  */
#if _GL_GNUC_PREREQ (11, 0)
# define _GL_ATTRIBUTE_DEALLOC(f, i) __attribute__ ((__malloc__ (f, i)))
#else
# define _GL_ATTRIBUTE_DEALLOC(f, i)
#endif
#define _GL_ATTRIBUTE_DEALLOC_FREE _GL_ATTRIBUTE_DEALLOC (free, 1)

#if _GL_HAS_C_ATTRIBUTE (deprecated)
# define _GL_ATTRIBUTE_DEPRECATED [[__deprecated__]]
#elif _GL_HAS_ATTRIBUTE (deprecated)
# define _GL_ATTRIBUTE_DEPRECATED __attribute__ ((__deprecated__))
#else
# define _GL_ATTRIBUTE_DEPRECATED
#endif

#if _GL_HAS_ATTRIBUTE (error)
# define _GL_ATTRIBUTE_ERROR(msg) __attribute__ ((__error__ (msg)))
# define _GL_ATTRIBUTE_WARNING(msg) __attribute__ ((__warning__ (msg)))
#elif _GL_HAS_ATTRIBUTE (diagnose_if)
# define _GL_ATTRIBUTE_ERROR(msg) __attribute__ ((__diagnose_if__ (1, msg, "error")))
# define _GL_ATTRIBUTE_WARNING(msg) __attribute__ ((__diagnose_if__ (1, msg, "warning")))
#else
# define _GL_ATTRIBUTE_ERROR(msg)
# define _GL_ATTRIBUTE_WARNING(msg)
#endif

#if _GL_HAS_ATTRIBUTE (externally_visible)
# define _GL_ATTRIBUTE_EXTERNALLY_VISIBLE __attribute__ ((externally_visible))
#else
# define _GL_ATTRIBUTE_EXTERNALLY_VISIBLE
#endif

/* FALLTHROUGH is special, because it always expands to something.  */
#if _GL_HAS_C_ATTRIBUTE (fallthrough)
# define _GL_ATTRIBUTE_FALLTHROUGH [[__fallthrough__]]
#elif _GL_HAS_ATTRIBUTE (fallthrough)
# define _GL_ATTRIBUTE_FALLTHROUGH __attribute__ ((__fallthrough__))
#else
# define _GL_ATTRIBUTE_FALLTHROUGH ((void) 0)
#endif

#if _GL_HAS_ATTRIBUTE (format)
# define _GL_ATTRIBUTE_FORMAT(spec) __attribute__ ((__format__ spec))
#else
# define _GL_ATTRIBUTE_FORMAT(spec)
#endif

#if _GL_HAS_ATTRIBUTE (leaf)
# define _GL_ATTRIBUTE_LEAF __attribute__ ((__leaf__))
#else
# define _GL_ATTRIBUTE_LEAF
#endif

#if _GL_HAS_ATTRIBUTE (malloc)
# define _GL_ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#else
# define _GL_ATTRIBUTE_MALLOC
#endif

/* Oracle Studio 12.6 mishandles may_alias despite __has_attribute OK.  */
#if _GL_HAS_ATTRIBUTE (may_alias) && !defined __SUNPRO_C
# define _GL_ATTRIBUTE_MAY_ALIAS __attribute__ ((__may_alias__))
#else
# define _GL_ATTRIBUTE_MAY_ALIAS
#endif

#if _GL_HAS_C_ATTRIBUTE (maybe_unused)
# define _GL_ATTRIBUTE_MAYBE_UNUSED [[__maybe_unused__]]
#else
# define _GL_ATTRIBUTE_MAYBE_UNUSED _GL_ATTRIBUTE_UNUSED
#endif
/* Earlier spellings of this macro.  */
#define _UNUSED_PARAMETER_ _GL_ATTRIBUTE_MAYBE_UNUSED

#if _GL_HAS_C_ATTRIBUTE (nodiscard)
# define _GL_ATTRIBUTE_NODISCARD [[__nodiscard__]]
#elif _GL_HAS_ATTRIBUTE (warn_unused_result)
# define _GL_ATTRIBUTE_NODISCARD __attribute__ ((__warn_unused_result__))
#else
# define _GL_ATTRIBUTE_NODISCARD
#endif

#if _GL_HAS_ATTRIBUTE (noinline)
# define _GL_ATTRIBUTE_NOINLINE __attribute__ ((__noinline__))
#else
# define _GL_ATTRIBUTE_NOINLINE
#endif

#if _GL_HAS_ATTRIBUTE (nonnull)
# define _GL_ATTRIBUTE_NONNULL(args) __attribute__ ((__nonnull__ args))
#else
# define _GL_ATTRIBUTE_NONNULL(args)
#endif

#if _GL_HAS_ATTRIBUTE (nonstring)
# define _GL_ATTRIBUTE_NONSTRING __attribute__ ((__nonstring__))
#else
# define _GL_ATTRIBUTE_NONSTRING
#endif

/* There is no _GL_ATTRIBUTE_NORETURN; use _Noreturn instead.  */

#if _GL_HAS_ATTRIBUTE (nothrow) && !defined __cplusplus
# define _GL_ATTRIBUTE_NOTHROW __attribute__ ((__nothrow__))
#else
# define _GL_ATTRIBUTE_NOTHROW
#endif

#if _GL_HAS_ATTRIBUTE (packed)
# define _GL_ATTRIBUTE_PACKED __attribute__ ((__packed__))
#else
# define _GL_ATTRIBUTE_PACKED
#endif

#if _GL_HAS_ATTRIBUTE (pure)
# define _GL_ATTRIBUTE_PURE __attribute__ ((__pure__))
#else
# define _GL_ATTRIBUTE_PURE
#endif

#if _GL_HAS_ATTRIBUTE (returns_nonnull)
# define _GL_ATTRIBUTE_RETURNS_NONNULL __attribute__ ((__returns_nonnull__))
#else
# define _GL_ATTRIBUTE_RETURNS_NONNULL
#endif

#if _GL_HAS_ATTRIBUTE (sentinel)
# define _GL_ATTRIBUTE_SENTINEL(pos) __attribute__ ((__sentinel__ pos))
#else
# define _GL_ATTRIBUTE_SENTINEL(pos)
#endif

#if _GL_HAS_ATTRIBUTE (unused)
# define _GL_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
#else
# define _GL_ATTRIBUTE_UNUSED
#endif
/* Earlier spellings of this macro.  */
#define _GL_UNUSED _GL_ATTRIBUTE_UNUSED


/* To support C++ as well as C, use _GL_UNUSED_LABEL with trailing ';'.  */
#if !defined __cplusplus || _GL_GNUC_PREREQ (4, 5)
# define _GL_UNUSED_LABEL _GL_ATTRIBUTE_UNUSED
#else
# define _GL_UNUSED_LABEL
#endif


/* Define to empty if `const' does not conform to ANSI C. */
#undef const

/* Please see the Gnulib manual for how to use these macros.

   Suppress extern inline with HP-UX cc, as it appears to be broken; see
   <https://lists.gnu.org/r/bug-texinfo/2013-02/msg00030.html>.

   Suppress extern inline with Sun C in standards-conformance mode, as it
   mishandles inline functions that call each other.  E.g., for 'inline void f
   (void) { } inline void g (void) { f (); }', c99 incorrectly complains
   'reference to static identifier "f" in extern inline function'.
   This bug was observed with Sun C 5.12 SunOS_i386 2011/11/16.

   Suppress extern inline (with or without __attribute__ ((__gnu_inline__)))
   on configurations that mistakenly use 'static inline' to implement
   functions or macros in standard C headers like <ctype.h>.  For example,
   if isdigit is mistakenly implemented via a static inline function,
   a program containing an extern inline function that calls isdigit
   may not work since the C standard prohibits extern inline functions
   from calling static functions (ISO C 99 section 6.7.4.(3).
   This bug is known to occur on:

     OS X 10.8 and earlier; see:
     https://lists.gnu.org/r/bug-gnulib/2012-12/msg00023.html

     DragonFly; see
     http://muscles.dragonflybsd.org/bulk/clang-master-potential/20141111_102002/logs/ah-tty-0.3.12.log

     FreeBSD; see:
     https://lists.gnu.org/r/bug-gnulib/2014-07/msg00104.html

   OS X 10.9 has a macro __header_inline indicating the bug is fixed for C and
   for clang but remains for g++; see <https://trac.macports.org/ticket/41033>.
   Assume DragonFly and FreeBSD will be similar.

   GCC 4.3 and above with -std=c99 or -std=gnu99 implements ISO C99
   inline semantics, unless -fgnu89-inline is used.  It defines a macro
   __GNUC_STDC_INLINE__ to indicate this situation or a macro
   __GNUC_GNU_INLINE__ to indicate the opposite situation.
   GCC 4.2 with -std=c99 or -std=gnu99 implements the GNU C inline
   semantics but warns, unless -fgnu89-inline is used:
     warning: C99 inline functions are not supported; using GNU89
     warning: to disable this warning use -fgnu89-inline or the gnu_inline function attribute
   It defines a macro __GNUC_GNU_INLINE__ to indicate this situation.
 */
#if (((defined __APPLE__ && defined __MACH__) \
      || defined __DragonFly__ || defined __FreeBSD__) \
     && (defined __header_inline \
         ? (defined __cplusplus && defined __GNUC_STDC_INLINE__ \
            && ! defined __clang__) \
         : ((! defined _DONT_USE_CTYPE_INLINE_ \
             && (defined __GNUC__ || defined __cplusplus)) \
            || (defined _FORTIFY_SOURCE && 0 < _FORTIFY_SOURCE \
                && defined __GNUC__ && ! defined __cplusplus))))
# define _GL_EXTERN_INLINE_STDHEADER_BUG
#endif
#if ((__GNUC__ \
      ? defined __GNUC_STDC_INLINE__ && __GNUC_STDC_INLINE__ \
      : (199901L <= __STDC_VERSION__ \
         && !defined __HP_cc \
         && !defined __PGI \
         && !(defined __SUNPRO_C && __STDC__))) \
     && !defined _GL_EXTERN_INLINE_STDHEADER_BUG)
# define _GL_INLINE inline
# define _GL_EXTERN_INLINE extern inline
# define _GL_EXTERN_INLINE_IN_USE
#elif (2 < __GNUC__ + (7 <= __GNUC_MINOR__) && !defined __STRICT_ANSI__ \
       && !defined _GL_EXTERN_INLINE_STDHEADER_BUG)
# if defined __GNUC_GNU_INLINE__ && __GNUC_GNU_INLINE__
   /* __gnu_inline__ suppresses a GCC 4.2 diagnostic.  */
#  define _GL_INLINE extern inline __attribute__ ((__gnu_inline__))
# else
#  define _GL_INLINE extern inline
# endif
# define _GL_EXTERN_INLINE extern
# define _GL_EXTERN_INLINE_IN_USE
#else
# define _GL_INLINE static _GL_UNUSED
# define _GL_EXTERN_INLINE static _GL_UNUSED
#endif

/* In GCC 4.6 (inclusive) to 5.1 (exclusive),
   suppress bogus "no previous prototype for 'FOO'"
   and "no previous declaration for 'FOO'" diagnostics,
   when FOO is an inline function in the header; see
   <https://gcc.gnu.org/bugzilla/show_bug.cgi?id=54113> and
   <https://gcc.gnu.org/bugzilla/show_bug.cgi?id=63877>.  */
#if __GNUC__ == 4 && 6 <= __GNUC_MINOR__
# if defined __GNUC_STDC_INLINE__ && __GNUC_STDC_INLINE__
#  define _GL_INLINE_HEADER_CONST_PRAGMA
# else
#  define _GL_INLINE_HEADER_CONST_PRAGMA \
     _Pragma ("GCC diagnostic ignored \"-Wsuggest-attribute=const\"")
# endif
# define _GL_INLINE_HEADER_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wmissing-prototypes\"") \
    _Pragma ("GCC diagnostic ignored \"-Wmissing-declarations\"") \
    _GL_INLINE_HEADER_CONST_PRAGMA
# define _GL_INLINE_HEADER_END \
    _Pragma ("GCC diagnostic pop")
#else
# define _GL_INLINE_HEADER_BEGIN
# define _GL_INLINE_HEADER_END
#endif

/* Define to the overridden function name */
#undef fts_children

/* Define to the overridden function name */
#undef fts_close

/* Define to the overridden function name */
#undef fts_cross_check

/* Define to the overridden function name */
#undef fts_open

/* Define to the overridden function name */
#undef fts_read

/* Define to the overridden function name */
#undef fts_set

/* Define to `int' if <sys/types.h> doesn't define. */
#undef gid_t

/* A replacement for va_copy, if needed.  */
#define gl_va_copy(a,b) ((a) = (b))

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#undef inline
#endif

/* Define to long or long long if <stdint.h> and <inttypes.h> don't define. */
#undef intmax_t

/* Work around a bug in Apple GCC 4.0.1 build 5465: In C99 mode, it supports
   the ISO C 99 semantics of 'extern inline' (unlike the GNU C semantics of
   earlier versions), but does not display it by setting __GNUC_STDC_INLINE__.
   __APPLE__ && __MACH__ test for Mac OS X.
   __APPLE_CC__ tests for the Apple compiler and its version.
   __STDC_VERSION__ tests for the C99 mode.  */
#if defined __APPLE__ && defined __MACH__ && __APPLE_CC__ >= 5465 && !defined __cplusplus && __STDC_VERSION__ >= 199901L && !defined __GNUC_STDC_INLINE__
# define __GNUC_STDC_INLINE__ 1
#endif

/* Define to 1 if the compiler is checking for lint. */
#undef lint

/* Define to a type if <wchar.h> does not define. */
#undef mbstate_t

/* _GL_CMP (n1, n2) performs a three-valued comparison on n1 vs. n2, where
   n1 and n2 are expressions without side effects, that evaluate to real
   numbers (excluding NaN).
   It returns
     1  if n1 > n2
     0  if n1 == n2
     -1 if n1 < n2
   The naïve code   (n1 > n2 ? 1 : n1 < n2 ? -1 : 0)  produces a conditional
   jump with nearly all GCC versions up to GCC 10.
   This variant     (n1 < n2 ? -1 : n1 > n2)  produces a conditional with many
   GCC versions up to GCC 9.
   The better code  (n1 > n2) - (n1 < n2)  from Hacker's Delight § 2-9
   avoids conditional jumps in all GCC versions >= 3.4.  */
#define _GL_CMP(n1, n2) (((n1) > (n2)) - ((n1) < (n2)))


/* Define to `int' if <sys/types.h> does not define. */
#undef mode_t

/* Define to the type of st_nlink in struct stat, or a supertype. */
#undef nlink_t

/* Define to `long int' if <sys/types.h> does not define. */
#undef off_t

/* Define as a signed integer type capable of holding a process identifier. */
#undef pid_t

/* Define as the type of the result of subtracting two pointers, if the system
   doesn't define it. */
#undef ptrdiff_t

/* Define to rpl_re_comp if the replacement should be used. */
#undef re_comp

/* Define to rpl_re_compile_fastmap if the replacement should be used. */
#undef re_compile_fastmap

/* Define to rpl_re_compile_pattern if the replacement should be used. */
#undef re_compile_pattern

/* Define to rpl_re_exec if the replacement should be used. */
#undef re_exec

/* Define to rpl_re_match if the replacement should be used. */
#undef re_match

/* Define to rpl_re_match_2 if the replacement should be used. */
#undef re_match_2

/* Define to rpl_re_search if the replacement should be used. */
#undef re_search

/* Define to rpl_re_search_2 if the replacement should be used. */
#undef re_search_2

/* Define to rpl_re_set_registers if the replacement should be used. */
#undef re_set_registers

/* Define to rpl_re_set_syntax if the replacement should be used. */
#undef re_set_syntax

/* Define to rpl_re_syntax_options if the replacement should be used. */
#undef re_syntax_options

/* Define to rpl_regcomp if the replacement should be used. */
#undef regcomp

/* Define to rpl_regerror if the replacement should be used. */
#undef regerror

/* Define to rpl_regexec if the replacement should be used. */
#undef regexec

/* Define to rpl_regfree if the replacement should be used. */
#undef regfree

/* Define to the equivalent of the C99 'restrict' keyword, or to
   nothing if this is not supported.  Do not define if restrict is
   supported only directly.  */
#define restrict
/* Work around a bug in older versions of Sun C++, which did not
   #define __restrict__ or support _Restrict or __restrict__
   even though the corresponding Sun C compiler ended up with
   "#define restrict _Restrict" or "#define restrict __restrict__"
   in the previous line.  This workaround can be removed once
   we assume Oracle Developer Studio 12.5 (2016) or later.  */
#if defined __SUNPRO_CC && !defined __RESTRICT && !defined __restrict__
# define _Restrict
# define __restrict__
#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
#undef size_t

/* type to use in place of socklen_t if not defined */
#undef socklen_t

/* Define as a signed type of the same size as size_t. */
#define ssize_t ptrdiff_t

/* Define to 'struct sigaltstack' if that's the type of the argument to
   sigaltstack */
#undef stack_t

/* Define to `int' if <sys/types.h> doesn't define. */
#undef uid_t


  /* This definition is a duplicate of the one in unitypes.h.
     It is here so that we can cope with an older version of unitypes.h
     that does not contain this definition and that is pre-installed among
     the public header files.  */
  # if defined __restrict \
       || 2 < __GNUC__ + (95 <= __GNUC_MINOR__) \
       || __clang_major__ >= 3
  #  define _UC_RESTRICT __restrict
  # elif 199901L <= __STDC_VERSION__ || defined restrict
  #  define _UC_RESTRICT restrict
  # else
  #  define _UC_RESTRICT
  # endif
  

/* Define as a macro for copying va_list variables. */
#undef va_copy

typedef unsigned mode_t;

/* Include windows.h here before DATADIR is defined, as it conflicts with objidl.h. */
#include <windows.h>

/* Hack! */
#include <arg-nonnull.h>

/* More hacks */
#define _GL_ATTRIBUTE_FORMAT_PRINTF_STANDARD(a, b)

/* S_ISXXX macros: */
#include <sys/stat.h>
#include <nt/ntstat.h>
#ifndef S_ISCHR
# define S_ISCHR(x) 0
#endif
#ifndef S_ISBLK
# define S_ISBLK(x) 0
#endif
#ifndef S_ISSOCK
# define S_ISSOCK(x) 0
#endif
#ifndef S_ISFIFO
# define S_ISFIFO(x) 0
#endif
#ifndef S_TYPEISSHM
# define S_TYPEISSHM(x) 0
#endif
#ifndef S_TYPEISTMO
# define S_TYPEISTMO(x) 0
#endif

/* Missing these: */
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

/* Missing open flags.*/
#include <nt/ntopenat.h>
#ifndef O_NOCTTY
# define O_NOCTTY 0
#endif
#ifndef O_NOFOLLOW
# define O_NOFOLLOW 0
#endif
#ifndef O_NOLINK
# define O_NOLINK 0
#endif
#ifndef O_NOLINKS
# define O_NOLINKS 0
#endif
#ifndef O_NOTRANS
# define O_NOTRANS 0
#endif
#ifndef O_NONBLOCK
# define O_NONBLOCK 0
#endif
#ifndef O_SEARCH
# define O_SEARCH 0
#endif
#ifndef O_CLOEXEC
# define O_CLOEXEC _O_NOINHERIT
#endif

/* Some prototypes of gnulib functions put here to avoid needing to mess with
   stdlib.in.h, string.in.h and wchar.in.h since MSC has no include_next facility. */
extern void *reallocarray(void *, size_t, size_t);
extern int mbscasecmp(const char *, const char *);
extern wchar_t *wmempcpy(wchar_t *, const wchar_t *, size_t);
extern int wcwidth(wchar_t);

#define strerror gl_strerror

/* Override initialize_main to do wildcard expansion. */
#define initialize_main w32_initialize_main
void w32_initialize_main(int *pcArgs, char ***ppapszArgs);

#include "get_codepage.h"

#endif /* !INCLUDED_CONFIG_WIN_H */

