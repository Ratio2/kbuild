/* $Id$ */
/** @file
 *
 * kDepPre - Dependency Generator using Precompiler output.
 *
 * Copyright (c) 2005-2006 knut st. osmundsen <bird@innotek.de>
 *
 *
 * This file is part of kBuild.
 *
 * kBuild is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * kBuild is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with kBuild; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "kDep.h"

#ifdef HAVE_FGETC_UNLOCKED
# define FGETC(s)   getc_unlocked(s)
#else
# define FGETC(s)   fgetc(s)
#endif

#ifdef NEED_ISBLANK
# define isblank(ch) ( (unsigned char)(ch) == ' ' || (unsigned char)(ch) == '\t' )
#endif




/**
 * Parses the output from a preprocessor of a C-style language.
 *
 * @returns 0 on success.
 * @returns 1 or other approriate exit code on failure.
 * @param   pInput      Input stream. (probably not seekable)
 */
static int ParseCPrecompiler(FILE *pInput)
{
    enum
    {
        C_DISCOVER = 0,
        C_SKIP_LINE,
        C_PARSE_FILENAME,
        C_EOF
    } enmMode = C_DISCOVER;
    PDEP    pDep = NULL;
    int     ch;
    char    szBuf[8192];

    for (;;)
    {
        switch (enmMode)
        {
            /*
             * Start of line, need to look for '#[[:space]]*line <num> "file"' and '# <num> "file"'.
             */
            case C_DISCOVER:
                /* first find '#' */
                while ((ch = FGETC(pInput)) != EOF)
                    if (!isblank(ch))
                        break;
                if (ch == '#')
                {
                    /* skip spaces */
                    while ((ch = FGETC(pInput)) != EOF)
                        if (!isblank(ch))
                            break;

                    /* check for "line" */
                    if (ch == 'l')
                    {
                        if (    (ch = FGETC(pInput)) == 'i'
                            &&  (ch = FGETC(pInput)) == 'n'
                            &&  (ch = FGETC(pInput)) == 'e')
                        {
                            ch = FGETC(pInput);
                            if (isblank(ch))
                            {
                                /* skip spaces */
                                while ((ch = FGETC(pInput)) != EOF)
                                    if (!isblank(ch))
                                        break;
                            }
                            else
                                ch = 'x';
                        }
                        else
                            ch = 'x';
                    }

                    /* line number */
                    if (ch >= '0' && ch <= '9')
                    {
                        /* skip the number following spaces */
                        while ((ch = FGETC(pInput)) != EOF)
                            if (!isxdigit(ch))
                                break;
                        if (isblank(ch))
                        {
                            while ((ch = FGETC(pInput)) != EOF)
                                if (!isblank(ch))
                                    break;
                            /* quoted filename */
                            if (ch == '"')
                            {
                                enmMode = C_PARSE_FILENAME;
                                break;
                            }
                        }
                    }
                }
                enmMode = C_SKIP_LINE;
                break;

            /*
             * Skip past the end of the current line.
             */
            case C_SKIP_LINE:
                do
                {
                    if (    ch == '\r'
                        ||  ch == '\n')
                        break;
                } while ((ch = FGETC(pInput)) != EOF);
                enmMode = C_DISCOVER;
                break;

            /*
             * Parse the filename.
             */
            case C_PARSE_FILENAME:
            {
                /* retreive and unescape the filename. */
                char   *psz = &szBuf[0];
                while (     (ch = FGETC(pInput)) != EOF
                       &&   psz < &szBuf[sizeof(szBuf) - 1])
                {
                    if (ch == '\\')
                    {
                        ch = FGETC(pInput);
                        switch (ch)
                        {
                            case '\\': ch = '/'; break;
                            case 't':  ch = '\t'; break;
                            case 'r':  ch = '\r'; break;
                            case 'n':  ch = '\n'; break;
                            case 'b':  ch = '\b'; break;
                            default:
                                fprintf(stderr, "warning: unknown escape char '%c'\n", ch);
                                continue;

                        }
                        *psz++ = ch == '\\' ? '/' : ch;
                    }
                    else if (ch != '"')
                        *psz++ = ch;
                    else
                    {
                        size_t cchFilename = psz - &szBuf[0];
                        *psz = '\0';
                        /* compare with current dep, add & switch on mismatch. */
                        if (    !pDep
                            ||  pDep->cchFilename != cchFilename
                            ||  memcmp(pDep->szFilename, szBuf, cchFilename))
                            pDep = depAdd(szBuf, cchFilename);
                        break;
                    }
                }
                enmMode = C_SKIP_LINE;
                break;
            }

            /*
             * Handle EOF.
             */
            case C_EOF:
                if (feof(pInput))
                    return 0;
                enmMode = C_DISCOVER;
                break;
        }
        if (ch == EOF)
            enmMode = C_EOF;
    }

    return 0;
}


static void usage(const char *argv0)
{
    printf("syntax: %s [-l=c] -o <output> -t <target> [-f] [-s] < - | <filename> | -e <cmdline> >\n", argv0);
}


int main(int argc, char *argv[])
{
    int         i;

    /* Arguments. */
    int         iExec = 0;
    FILE       *pOutput = NULL;
    const char *pszOutput = NULL;
    FILE       *pInput = NULL;
    const char *pszTarget = NULL;
    int         fStubs = 0;
    int         fFixCase = 0;
    /* Argument parsing. */
    int         fInput = 0;             /* set when we've found input argument. */

    /*
     * Parse arguments.
     */
    if (argc <= 1)
    {
        usage(argv[0]);
        return 1;
    }
    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
                /*
                 * Output file.
                 */
                case 'o':
                {
                    pszOutput = &argv[i][2];
                    if (pOutput)
                    {
                        fprintf(stderr, "%s: syntax error: only one output file!\n", argv[0]);
                        return 1;
                    }
                    if (!*pszOutput)
                    {
                        if (++i >= argc)
                        {
                            fprintf(stderr, "%s: syntax error: The '-o' argument is missing the filename.\n", argv[0]);
                            return 1;
                        }
                        pszOutput = argv[i];
                    }
                    if (pszOutput[0] == '-' && !pszOutput[1])
                        pOutput = stdout;
                    else
                        pOutput = fopen(pszOutput, "w");
                    if (!pOutput)
                    {
                        fprintf(stderr, "%s: error: Failed to create output file '%s'.\n", argv[0], pszOutput);
                        return 1;
                    }
                    break;
                }

                /*
                 * Language spec.
                 */
                case 'l':
                {
                    const char *psz = &argv[i][2];
                    if (*psz == '=')
                        psz++;
                    if (!strcmp(psz, "c"))
                        ;
                    else
                    {
                        fprintf(stderr, "%s: error: The '%s' language is not supported.\n", argv[0], psz);
                        return 1;
                    }
                    break;
                }

                /*
                 * Target name.
                 */
                case 't':
                {
                    if (pszTarget)
                    {
                        fprintf(stderr, "%s: syntax error: only one target!\n", argv[0]);
                        return 1;
                    }
                    pszTarget = &argv[i][2];
                    if (!*pszTarget)
                    {
                        if (++i >= argc)
                        {
                            fprintf(stderr, "%s: syntax error: The '-t' argument is missing the target name.\n", argv[0]);
                            return 1;
                        }
                        pszTarget = argv[i];
                    }
                    break;
                }

                /*
                 * Exec.
                 */
                case 'e':
                {
                    if (++i >= argc)
                    {
                        fprintf(stderr, "%s: syntax error: The '-e' argument is missing the command.\n", argv[0]);
                        return 1;
                    }
                    iExec = i;
                    i = argc - 1;
                    break;
                }

                /*
                 * Pipe input.
                 */
                case '\0':
                {
                    pInput = stdin;
                    fInput = 1;
                    break;
                }

                /*
                 * Fix case.
                 */
                case 'f':
                {
                    fFixCase = 1;
                    break;
                }

                /*
                 * Generate stubs.
                 */
                case 's':
                {
                    fStubs = 1;
                    break;
                }

                /*
                 * Invalid argument.
                 */
                default:
                    fprintf(stderr, "%s: syntax error: Invalid argument '%s'.\n", argv[0], argv[i]);
                    usage(argv[0]);
                    return 1;
            }
        }
        else
        {
            pInput = fopen(argv[i], "r");
            if (!pInput)
            {
                fprintf(stderr, "%s: error: Failed to open input file '%s'.\n", argv[0], argv[i]);
                return 1;
            }
            fInput = 1;
        }

        /*
         * End of the line?
         */
        if (fInput)
        {
            if (++i < argc)
            {
                fprintf(stderr, "%s: syntax error: No arguments shall follow the input spec.\n", argv[0]);
                return 1;
            }
            break;
        }
    }

    /*
     * Got all we require?
     */
    if (!pInput && iExec <= 0)
    {
        fprintf(stderr, "%s: syntax error: No input!\n", argv[0]);
        return 1;
    }
    if (!pOutput)
    {
        fprintf(stderr, "%s: syntax error: No output!\n", argv[0]);
        return 1;
    }
    if (!pszTarget)
    {
        fprintf(stderr, "%s: syntax error: No target!\n", argv[0]);
        return 1;
    }

    /*
     * Spawn process?
     */
    if (iExec > 0)
    {
        fprintf(stderr, "%s: -e is not yet implemented!\n", argv[0]);
        return 1;
    }

    /*
     * Do the parsing.
     */
    i = ParseCPrecompiler(pInput);

    /*
     * Reap child.
     */
    if (iExec > 0)
    {
        // later
    }

    /*
     * Write the dependecy file.
     */
    if (!i)
    {
        depOptimize(fFixCase);
        fprintf(pOutput, "%s:", pszTarget);
        depPrint(pOutput);
        if (fStubs)
            depPrintStubs(pOutput);
    }

    /*
     * Close the output, delete output on failure.
     */
    if (!i && ferror(pOutput))
    {
        i = 1;
        fprintf(stderr, "%s: error: Error writing to '%s'.\n", argv[0], pszOutput);
    }
    fclose(pOutput);
    if (i)
    {
        if (unlink(pszOutput))
            fprintf(stderr, "%s: warning: failed to remove output file '%s' on failure.\n", argv[0], pszOutput);
    }

    return i;
}

