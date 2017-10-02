/* $Id$ */
/** @file
 * kmk_touch - Simple touch implementation.
 */

/*
 * Copyright (c) 2017 knut st. osmundsen <bird-kBuild-spamx@anduin.net>
 *
 * This file is part of kBuild.
 *
 * kBuild is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * kBuild is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with kBuild.  If not, see <http://www.gnu.org/licenses/>
 *
 */

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include "make.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#if defined(_MSC_VER)
# include <ctype.h>
# include <io.h>
# include <sys/timeb.h>
#else
# include <unistd.h>
#endif

#include <k/kDefs.h>
#include <k/kTypes.h>
#include "err.h"
#include "kbuild_version.h"
#include "kmkbuiltin.h"

#ifdef _MSC_VER
# include "nt/ntstat.h"
# undef FILE_TIMESTAMP_HI_RES
# define FILE_TIMESTAMP_HI_RES 1
#endif


/*********************************************************************************************************************************
*   Defined Constants And Macros                                                                                                 *
*********************************************************************************************************************************/
/** The program name to use in message. */
#ifdef KMK
# define TOUCH_NAME  "kmk_builtin_touch"
#else
# define TOUCH_NAME  "kmk_touch"
#endif
/** Converts a two digit decimal field to a number. */
#define TWO_CHARS_TO_INT(chHigh, chLow)  ( ((unsigned)(chHigh) - (unsigned)'0') * 10 + ((unsigned)(chLow) - (unsigned)'0') )
/** Checks an alleged digit. */
#define IS_DIGIT(chDigit, uMax)          ( ((unsigned)(chDigit) - (unsigned)'0') <= (unsigned)(uMax) )


/*********************************************************************************************************************************
*   Structures and Typedefs                                                                                                      *
*********************************************************************************************************************************/
typedef enum KMKTOUCHTARGET
{
    kTouchAccessAndModify,
    kTouchAccessOnly,
    kTouchModifyOnly
} KMKTOUCHTARGET;

typedef enum KMKTOUCHACTION
{
    kTouchActionCurrent,
    kTouchActionSet,
    kTouchActionAdjust
} KMKTOUCHACTION;


typedef struct KMKTOUCHOPTS
{
    /** What timestamps to modify on the files. */
    KMKTOUCHTARGET  enmWhatToTouch;
    /** How to update the time. */
    KMKTOUCHACTION  enmAction;
    /** Whether to create files (K_TRUE) or ignore missing (K_FALSE). */
    KBOOL           fCreate;
    /** Whether to dereference files. */
    KBOOL           fDereference;
    /** The new access time value. */
    struct timeval  NewATime;
    /** The new modified time value. */
    struct timeval  NewMTime;

    /** Number of files. */
    int             cFiles;
    /** The specified files.   */
    char          **papszFiles;
} KMKTOUCHOPTS;
typedef KMKTOUCHOPTS *PKMKTOUCHOPTS;



static int touch_error(const char *pszMsg, ...)
{
    va_list va;
    fputs(TOUCH_NAME ": error: ", stderr);
    va_start(va, pszMsg);
    vfprintf(stderr, pszMsg, va);
    va_end(va);
    fputc('\n', stderr);
    return 1;
}

static int touch_syntax(const char *pszMsg, ...)
{
    va_list va;
    fputs(TOUCH_NAME ": syntax error: ", stderr);
    va_start(va, pszMsg);
    vfprintf(stderr, pszMsg, va);
    va_end(va);
    fputc('\n', stderr);
    return 2;
}

static int touch_usage(void)
{
    fputs("Usage: " TOUCH_NAME " [options] [MMDDhhmm[YY]] <file1> [.. [fileN]]\n"
          "\n"
          "Options:\n"
          "  -A [-][[hh]mm]SS, --adjust=[-][[hh]mm]SS\n"
          "    Adjust timestamps by given delta.\n"
          "  -a, --time=atime, --time=access\n"
          "    Only change the accessed timestamp.\n"
          "  -c, --no-create\n"
          "    Ignore missing files and don't create them. (default create empty file)\n"
          "  -d YYYY-MM-DDThh:mm:SS[.frac][tz], --date=YYYY-MM-DDThh:mm:SS[.frac][tz]\n"
          "    Set the timestamps to the given one.\n"
          "  -f\n"
          "    Ignored for compatbility reasons.\n"
          "  -h, --no-dereference\n"
          "    Don't follow links, touch links. (Not applicable to -r.)\n"
          "  -m, --time=mtime, --time=modify\n"
          "    Only changed the modified timestamp.\n"
          "  -r <file>, --reference=<file>\n"
          "    Take the timestamps from <file>.\n"
          "  -t [[CC]YY]MMDDhhmm[.SS]\n"
          "    Set the timestamps to the given one.\n"
          "\n"
          "Note. For compatibility reasons the first file can be taken to be a 8 or 10\n"
          "      character long timestamp if it matches the given pattern and none of\n"
          "      the -A, -d, --date, -r, --reference, or -t options are given.  So, use\n"
          "      absolute or relative paths when specifying more than one file.\n"
          , stdout);
    return 0;
}


#if K_OS == K_OS_SOLARIS
/**
 * Solaris doesn't have lutimes because System V doesn't believe in stuff like file modes on symbolic links.
 */
static int lutimes(const char *pszFile, struct timeval aTimes[2])
{
    struct stat Stat;
    if (stat(pszFile, &Stat) != -1)
    {
        if (!S_ISLNK(Stat.st_mode))
            return utimes(pszFile, aTimes);
        return 0;
    }
    return -1;
}
#endif 


/**
 * Parses adjustment value: [-][[hh]mm]SS
 */
static int touch_parse_adjust(const char *pszValue, int *piAdjustValue)
{
    const char * const  pszInValue = pszValue;
    size_t              cchValue  = strlen(pszValue);
    KBOOL               fNegative = K_FALSE;

    /* Deal with negativity */
    if (pszValue[0] == '-')
    {
        fNegative = K_TRUE;
        pszValue++;
        cchValue--;
    }

    /* Validate and convert. */
    *piAdjustValue = 0;
    switch (cchValue)
    {
        case 6:
            if (   !IS_DIGIT(pszValue[0], 9)
                || !IS_DIGIT(pszValue[0], 9))
                return touch_syntax("Malformed hour part of -A value: %s", pszInValue);
            *piAdjustValue = TWO_CHARS_TO_INT(pszValue[0], pszValue[1]) * 60 * 60;
            /* fall thru */
        case 4:
            if (   !IS_DIGIT(pszValue[cchValue - 4], 9) /* don't bother limit to 60 minutes */
                || !IS_DIGIT(pszValue[cchValue - 3], 9))
                return touch_syntax("Malformed minute part of -A value: %s", pszInValue);
            *piAdjustValue += TWO_CHARS_TO_INT(pszValue[cchValue - 4], pszValue[cchValue - 3]) * 60;
            /* fall thru */
        case 2:
            if (   !IS_DIGIT(pszValue[cchValue - 2], 9) /* don't bother limit to 60 seconds */
                || !IS_DIGIT(pszValue[cchValue - 1], 9))
                return touch_syntax("Malformed second part of -A value: %s", pszInValue);
            *piAdjustValue += TWO_CHARS_TO_INT(pszValue[cchValue - 2], pszValue[cchValue - 1]);
            break;

        default:
            return touch_syntax("Invalid -A value (length): %s", pszInValue);
    }

    /* Apply negativity. */
    if (fNegative)
        *piAdjustValue = -*piAdjustValue;

    return 0;
}


/**
 * Parse -d timestamp: YYYY-MM-DDThh:mm:SS[.frac][tz]
 */
static int touch_parse_d_ts(const char *pszTs, struct timeval *pDst)
{
    const char * const  pszTsIn = pszTs;
    struct tm           ExpTime;

    /*
     * Validate and parse the timestamp into the tm structure.
     */
    memset(&ExpTime, 0, sizeof(ExpTime));

    /* year */
    if (   !IS_DIGIT(pszTs[0], 9)
        || !IS_DIGIT(pszTs[1], 9)
        || !IS_DIGIT(pszTs[2], 9)
        || !IS_DIGIT(pszTs[3], 9)
        || pszTs[4] != '-')
        return touch_error("Malformed timestamp '%s' given to -d: expected to start with 4 digit year followed by a dash",
                           pszTsIn);
    ExpTime.tm_year = TWO_CHARS_TO_INT(pszTs[0], pszTs[1]) * 100
                    + TWO_CHARS_TO_INT(pszTs[2], pszTs[3])
                    - 1900;
    pszTs += 5;

    /* month */
    if (   !IS_DIGIT(pszTs[0], 1)
        || !IS_DIGIT(pszTs[1], 9)
        || pszTs[2] != '-')
        return touch_error("Malformed timestamp '%s' given to -d: expected to two digit month at position 6 followed by a dash",
                           pszTsIn);
    ExpTime.tm_mon = TWO_CHARS_TO_INT(pszTs[0], pszTs[1]) - 1;
    pszTs += 3;

    /* day */
    if (   !IS_DIGIT(pszTs[0], 3)
        || !IS_DIGIT(pszTs[1], 9)
        || (pszTs[2] != 'T' && pszTs[2] != 't' && pszTs[2] != ' ') )
        return touch_error("Malformed timestamp '%s' given to -d: expected to two digit day of month at position 9 followed by 'T' or space",
                           pszTsIn);
    ExpTime.tm_mday = TWO_CHARS_TO_INT(pszTs[0], pszTs[1]);
    pszTs += 3;

    /* hour */
    if (   !IS_DIGIT(pszTs[0], 2)
        || !IS_DIGIT(pszTs[1], 9)
        || pszTs[2] != ':')
        return touch_error("Malformed timestamp '%s' given to -d: expected to two digit hour at position 12 followed by colon",
                           pszTsIn);
    ExpTime.tm_hour = TWO_CHARS_TO_INT(pszTs[0], pszTs[1]);
    pszTs += 3;

    /* minute */
    if (   !IS_DIGIT(pszTs[0], 5)
        || !IS_DIGIT(pszTs[1], 9)
        || pszTs[2] != ':')
        return touch_error("Malformed timestamp '%s' given to -d: expected to two digit minute at position 15 followed by colon",
                           pszTsIn);
    ExpTime.tm_min = TWO_CHARS_TO_INT(pszTs[0], pszTs[1]);
    pszTs += 3;

    /* seconds */
    if (   !IS_DIGIT(pszTs[0], 5)
        || !IS_DIGIT(pszTs[1], 9))
        return touch_error("Malformed timestamp '%s' given to -d: expected to two digit seconds at position 12", pszTsIn);
    ExpTime.tm_sec = TWO_CHARS_TO_INT(pszTs[0], pszTs[1]);
    pszTs += 2;

    /* fraction */
    pDst->tv_usec = 0;
    if (*pszTs == '.' || *pszTs == ',')
    {
        int iFactor;

        pszTs++;
        if (!IS_DIGIT(*pszTs, 9))
            return touch_error("Malformed timestamp '%s' given to -d: empty fraction", pszTsIn);

        iFactor = 100000;
        do
        {
            pDst->tv_usec += ((unsigned)*pszTs - (unsigned)'0') * iFactor;
            iFactor /= 10;
            pszTs++;
        } while (IS_DIGIT(*pszTs, 9));
    }

    /* zulu time indicator */
    ExpTime.tm_isdst = -1;
    if (*pszTs == 'Z' || *pszTs == 'z')
    {
        ExpTime.tm_isdst = 0;
        pszTs++;
        if (*pszTs != '\0')
            return touch_error("Malformed timestamp '%s' given to -d: Unexpected character(s) after zulu time indicator at end of timestamp",
                               pszTsIn);
    }
    else if (*pszTs != '\0')
        return touch_error("Malformed timestamp '%s' given to -d: expected to 'Z' (zulu) or nothing at end of timestamp", pszTsIn);

    /*
     * Convert to UTC seconds using either timegm or mktime.
     */
    ExpTime.tm_yday  = -1;
    ExpTime.tm_wday  = -1;
    if (ExpTime.tm_isdst == 0)
    {
#if K_OS == K_OS_SOLARIS
        pDst->tv_sec = mktime(&ExpTime) - timezone; /* best we can do for now */
#else
        pDst->tv_sec = timegm(&ExpTime);
#endif
        if (pDst->tv_sec == -1)
            return touch_error("timegm failed on '%s': %s", pszTs, strerror(errno));
    }
    else
    {
        pDst->tv_sec = mktime(&ExpTime);
        if (pDst->tv_sec == -1)
            return touch_error("mktime failed on '%s': %s", pszTs, strerror(errno));
    }
    return 0;
}


/**
 * Parse -t timestamp: [[CC]YY]MMDDhhmm[.SS]
 */
static int touch_parse_ts(const char *pszTs, struct timeval *pDst)
{
    size_t const    cchTs = strlen(pszTs);
    size_t          cchTsNoSec;
    struct tm       ExpTime;
    struct tm      *pExpTime;
    struct timeval  Now;
    int rc;

    /*
     * Do some input validations first.
     */
    if ((cchTs & 1) && pszTs[cchTs - 3] != '.')
        return touch_error("Invalid timestamp given to -t: %s", pszTs);
    switch (cchTs)
    {
        case 8:             /*     MMDDhhmm */
        case 8 + 2:         /*   YYMMDDhhmm */
        case 8 + 2 + 2:     /* CCYYMMDDhhmm */
            cchTsNoSec = cchTs;
            break;
        case 8 + 3:         /*     MMDDhhmm.SS */
        case 8 + 3 + 2:     /*   YYMMDDhhmm.SS */
        case 8 + 3 + 2 + 2: /* CCYYMMDDhhmm.SS */
            if (pszTs[cchTs - 3] != '.')
                return touch_error("Invalid timestamp (-t) '%s': missing dot for seconds part", pszTs);
            if (   !IS_DIGIT(pszTs[cchTs - 2], 5)
                || !IS_DIGIT(pszTs[cchTs - 1], 9))
                return touch_error("Invalid timestamp (-t) '%s': malformed seconds part", pszTs);
            cchTsNoSec = cchTs - 3;
            break;
        default:
            return touch_error("Invalid timestamp (-t) '%s': wrong length (%d)", pszTs, (int)cchTs);
    }

    switch (cchTsNoSec)
    {
        case 8 + 2 + 2:     /* CCYYMMDDhhmm */
            if (   !IS_DIGIT(pszTs[cchTsNoSec - 12], 9)
                || !IS_DIGIT(pszTs[cchTsNoSec - 11], 9))
                return touch_error("Invalid timestamp (-t) '%s': malformed CC part", pszTs);
            /* fall thru */
        case 8 + 2:         /*   YYMMDDhhmm */
            if (   !IS_DIGIT(pszTs[cchTsNoSec - 10], 9)
                || !IS_DIGIT(pszTs[cchTsNoSec -  9], 9))
                return touch_error("Invalid timestamp (-t) '%s': malformed YY part", pszTs);
            /* fall thru */
        case 8:             /*     MMDDhhmm */
            if (   !IS_DIGIT(pszTs[cchTsNoSec - 8], 1)
                || !IS_DIGIT(pszTs[cchTsNoSec - 7], 9) )
                return touch_error("Invalid timestamp (-t) '%s': malformed month part", pszTs);
            if (   !IS_DIGIT(pszTs[cchTsNoSec - 6], 3)
                || !IS_DIGIT(pszTs[cchTsNoSec - 5], 9) )
                return touch_error("Invalid timestamp (-t) '%s': malformed day part", pszTs);
            if (   !IS_DIGIT(pszTs[cchTsNoSec - 4], 2)
                || !IS_DIGIT(pszTs[cchTsNoSec - 3], 9) )
                return touch_error("Invalid timestamp (-t) '%s': malformed hour part", pszTs);
            if (   !IS_DIGIT(pszTs[cchTsNoSec - 2], 5)
                || !IS_DIGIT(pszTs[cchTsNoSec - 1], 9) )
                return touch_error("Invalid timestamp (-t) '%s': malformed minute part", pszTs);
            break;
    }

    /*
     * Get the current time and explode it.
     */
    rc = gettimeofday(&Now, NULL);
    if (rc != 0)
        return touch_error("gettimeofday failed: %s", strerror(errno));

    pExpTime = localtime_r(&Now.tv_sec, &ExpTime);
    if (pExpTime == NULL)
        return touch_error("localtime_r failed: %s", strerror(errno));

    /*
     * Do the decoding.
     */
    if (cchTs & 1)
        pExpTime->tm_sec = TWO_CHARS_TO_INT(pszTs[cchTs - 2], pszTs[cchTs - 1]);
    else
        pExpTime->tm_sec = 0;

    if (cchTsNoSec == 8 + 2 + 2) /* CCYY */
        pExpTime->tm_year = TWO_CHARS_TO_INT(pszTs[0], pszTs[1]) * 100
                          + TWO_CHARS_TO_INT(pszTs[2], pszTs[3])
                          - 1900;
    else  if (cchTsNoSec == 8 + 2) /* YY */
    {
        pExpTime->tm_year = TWO_CHARS_TO_INT(pszTs[0], pszTs[1]);
        if (pExpTime->tm_year < 69)
            pExpTime->tm_year += 100;
    }

    pExpTime->tm_mon  = TWO_CHARS_TO_INT(pszTs[cchTsNoSec - 8], pszTs[cchTsNoSec - 7]) - 1;
    pExpTime->tm_mday = TWO_CHARS_TO_INT(pszTs[cchTsNoSec - 6], pszTs[cchTsNoSec - 5]);
    pExpTime->tm_hour = TWO_CHARS_TO_INT(pszTs[cchTsNoSec - 4], pszTs[cchTsNoSec - 3]);
    pExpTime->tm_min  = TWO_CHARS_TO_INT(pszTs[cchTsNoSec - 2], pszTs[cchTsNoSec - 1]);

    /*
     * Use mktime to convert to UTC seconds.
     */
    pExpTime->tm_isdst = -1;
    pExpTime->tm_yday  = -1;
    pExpTime->tm_wday  = -1;
    pDst->tv_usec = 0;
    pDst->tv_sec  = mktime(pExpTime);
    if (pDst->tv_sec != -1)
        return 0;
    return touch_error("mktime failed on '%s': %s", pszTs, strerror(errno));
}


/**
 * Check for old timestamp: MMDDhhmm[YY]
 */
static int touch_parse_old_ts(const char *pszOldTs, time_t Now, struct timeval *pDst)
{
    /*
     * Check if this is a valid timestamp.
     */
    size_t const cchOldTs = strlen(pszOldTs);
    if (   (   cchOldTs == 8
            || cchOldTs == 10)
        && IS_DIGIT(pszOldTs[0], 1)
        && IS_DIGIT(pszOldTs[1], 9)
        && IS_DIGIT(pszOldTs[2], 3)
        && IS_DIGIT(pszOldTs[3], 9)
        && IS_DIGIT(pszOldTs[4], 2)
        && IS_DIGIT(pszOldTs[5], 9)
        && IS_DIGIT(pszOldTs[6], 5)
        && IS_DIGIT(pszOldTs[7], 9)
        && (   cchOldTs == 8
            || (   IS_DIGIT(pszOldTs[8], 9)
                && IS_DIGIT(pszOldTs[9], 9) )) )
    {
        /*
         * Explode the current time as local.
         */
        struct tm   ExpTime;
        struct tm  *pExpTime;
        pExpTime = localtime_r(&Now, &ExpTime);
        if (pExpTime == NULL)
            return touch_error("localtime_r failed: %s", strerror(errno));

        /*
         * Decode the bits we've got.
         */
        pExpTime->tm_mon  = TWO_CHARS_TO_INT(pszOldTs[0], pszOldTs[1]) - 1;
        pExpTime->tm_mday = TWO_CHARS_TO_INT(pszOldTs[2], pszOldTs[3]);
        pExpTime->tm_hour = TWO_CHARS_TO_INT(pszOldTs[4], pszOldTs[5]);
        pExpTime->tm_min  = TWO_CHARS_TO_INT(pszOldTs[6], pszOldTs[7]);
        if (cchOldTs == 10)
        {
            pExpTime->tm_year = TWO_CHARS_TO_INT(pszOldTs[8], pszOldTs[9]);
            if (pExpTime->tm_year <= 38)  /* up to 2038, 32-bit time_t style logic. */
                pExpTime->tm_year += 100;
        }

        /*
         * Use mktime to convert to UTC seconds.
         */
        pExpTime->tm_isdst = -1;
        pExpTime->tm_yday  = -1;
        pExpTime->tm_wday  = -1;
        pDst->tv_usec = 0;
        pDst->tv_sec  = mktime(pExpTime);
        if (pDst->tv_sec != -1)
            return 0;
        return touch_error("mktime failed on '%s': %s", pszOldTs, strerror(errno));
    }

    /* No valid timestamp present. */
    return -1;
}


/**
 * Parses the arguments into pOpts.
 *
 * @returns exit code.
 * @param   pOpts               Options structure to return the parsed info in.
 *                              Caller initalizes this with defaults.
 * @param   cArgs               The number of arguments.
 * @param   papszArgs           The arguments.
 * @param   pfExit              Indicates whether to exit or to start processing
 *                              files.
 */
static int touch_parse_args(PKMKTOUCHOPTS pOpts, int cArgs, char **papszArgs, KBOOL *pfExit)
{
    int iAdjustValue = 0;
    int iArg;
    int rc;

    *pfExit = K_TRUE;
    /*
     * Parse arguments, skipping all files (GNU style).
     */
    for (iArg = 1; iArg < cArgs; iArg++)
    {
        const char *pszArg = papszArgs[iArg];
        if (*pszArg == '-')
        {
            const char *pszLongValue = NULL;
            char ch = *++pszArg;
            pszArg++;

            /*
             * Deal with long options first, preferably translating them into short ones.
             */
            if (ch == '-')
            {
                const char *pszLong = pszArg;
                ch = *pszArg++;
                if (!ch)
                {
                    while (++iArg < cArgs)
                        pOpts->papszFiles[pOpts->cFiles++] = papszArgs[iArg];
                    break; /* '--' */
                }

                /* Translate long options. */
                pszArg = "";
                if (strcmp(pszLong, "adjust") == 0)
                    ch = 'A';
                else if (strncmp(pszLong, "adjust=", sizeof("adjust=") - 1) == 0)
                {
                    ch = 'A';
                    pszLongValue = pszArg = pszLong + sizeof("adjust=") - 1;
                }
                else if (strcmp(pszLong, "no-create") == 0)
                    ch = 'c';
                else if (strcmp(pszLong, "date") == 0)
                    ch = 'd';
                else if (strncmp(pszLong, "date=", sizeof("date=") - 1) == 0)
                {
                    ch = 'd';
                    pszLongValue = pszArg = pszLong + sizeof("date=") - 1;
                }
                else if (strcmp(pszLong, "no-dereference") == 0)
                    ch = 'h';
                else if (strcmp(pszLong, "reference") == 0)
                    ch = 'r';
                else if (strncmp(pszLong, "reference=", sizeof("reference=") - 1) == 0)
                {
                    ch = 'r';
                    pszLongValue = pszArg = pszLong + sizeof("reference=") - 1;
                }
                else if (strcmp(pszLong, "time") == 0)
                    ch = 'T';
                else if (strncmp(pszLong, "time=", sizeof("time=") - 1) == 0)
                {
                    ch = 'T';
                    pszLongValue = pszArg = pszLong + sizeof("time=") - 1;
                }
                else if (strcmp(pszLong, "help") == 0)
                    return touch_usage();
                else if (strcmp(pszLong, "version") == 0)
                    return kbuild_version(papszArgs[0]);
                else
                    return touch_syntax("Unknown option: --%s", pszLong);
            }

            /*
             * Process short options.
             */
            do
            {
                /* Some options takes a value. */
                const char *pszValue;
                switch (ch)
                {
                    case 'A':
                    case 'd':
                    case 'r':
                    case 't':
                    case 'T':
                        if (*pszArg || pszLongValue)
                        {
                            pszValue = pszArg;
                            pszArg = "";
                        }
                        else if (iArg + 1 < cArgs)
                            pszValue = papszArgs[++iArg];
                        else
                            return touch_syntax("Option -%c requires a value", ch);
                        break;

                    default:
                        pszValue = NULL;
                        break;
                }

                switch (ch)
                {
                    /* -A [-][[HH]MM]SS */
                    case 'A':
                        rc = touch_parse_adjust(pszValue, &iAdjustValue);
                        if (rc != 0)
                            return rc;
                        if (pOpts->enmAction != kTouchActionSet)
                        {
                            pOpts->enmAction = kTouchActionAdjust;
                            pOpts->NewATime.tv_sec  = iAdjustValue;
                            pOpts->NewATime.tv_usec = 0;
                            pOpts->NewMTime = pOpts->NewATime;
                        }
                        /* else: applied after parsing everything. */
                        break;

                    case 'a':
                        pOpts->enmWhatToTouch = kTouchAccessOnly;
                        break;

                    case 'c':
                        pOpts->fCreate = K_FALSE;
                        break;

                    case 'd':
                        rc = touch_parse_d_ts(pszValue, &pOpts->NewATime);
                        if (rc != 0)
                            return rc;
                        pOpts->enmAction = kTouchActionSet;
                        pOpts->NewMTime  = pOpts->NewATime;
                        break;

                    case 'f':
                        /* some historical thing, ignored. */
                        break;

                    case 'h':
                        pOpts->fDereference = K_FALSE;
                        break;

                    case 'm':
                        pOpts->enmWhatToTouch = kTouchModifyOnly;
                        break;

                    case 'r':
                    {
                        struct stat St;
                        if (stat(pszValue, &St) != 0)
                            return touch_error("Failed to stat '%s' (-r option): %s", pszValue, strerror(errno));

                        pOpts->enmAction = kTouchActionSet;
                        pOpts->NewATime.tv_sec  = St.st_atime;
                        pOpts->NewMTime.tv_sec  = St.st_mtime;
#if FILE_TIMESTAMP_HI_RES
                        pOpts->NewATime.tv_usec = St.st_atim.tv_nsec / 1000;
                        pOpts->NewMTime.tv_usec = St.st_mtim.tv_nsec / 1000;
#else
                        pOpts->NewATime.tv_usec = 0;
                        pOpts->NewMTime.tv_usec = 0;
#endif
                        break;
                    }

                    case 't':
                        rc = touch_parse_ts(pszValue, &pOpts->NewATime);
                        if (rc != 0)
                            return rc;
                        pOpts->enmAction = kTouchActionSet;
                        pOpts->NewMTime  = pOpts->NewATime;
                        break;

                    case 'T':
                        if (   strcmp(pszValue, "atime") == 0
                            || strcmp(pszValue, "access") == 0)
                            pOpts->enmWhatToTouch = kTouchAccessOnly;
                        else if (   strcmp(pszValue, "mtime") == 0
                                 || strcmp(pszValue, "modify") == 0)
                            pOpts->enmWhatToTouch = kTouchModifyOnly;
                        else
                            return touch_syntax("Unknown --time value: %s", pszValue);
                        break;

                    case 'V':
                        return kbuild_version(papszArgs[0]);

                    default:
                        return touch_syntax("Unknown option: -%c (%c%s)", ch, ch, pszArg);
                }

            } while ((ch = *pszArg++) != '\0');
        }
        else
            pOpts->papszFiles[pOpts->cFiles++] = papszArgs[iArg];
    }

    /*
     * Allow adjusting specified timestamps too like BSD does.
     */
    if (   pOpts->enmAction == kTouchActionSet
        && iAdjustValue != 0)
    {
        if (   pOpts->enmWhatToTouch == kTouchAccessAndModify
            || pOpts->enmWhatToTouch == kTouchAccessOnly)
            pOpts->NewATime.tv_sec += iAdjustValue;
        if (   pOpts->enmWhatToTouch == kTouchAccessAndModify
            || pOpts->enmWhatToTouch == kTouchModifyOnly)
            pOpts->NewMTime.tv_sec += iAdjustValue;
    }

    /*
     * Check for old timestamp: MMDDhhmm[YY]
     */
    if (   pOpts->enmAction == kTouchActionCurrent
        && pOpts->cFiles >= 2)
    {
        struct timeval OldTs;
        rc = touch_parse_old_ts(pOpts->papszFiles[0], pOpts->NewATime.tv_sec, &OldTs);
        if (rc == 0)
        {
            int iFile;

            pOpts->NewATime = OldTs;
            pOpts->NewMTime = OldTs;
            pOpts->enmAction = kTouchActionSet;

            for (iFile = 1; iFile < pOpts->cFiles; iFile++)
                pOpts->papszFiles[iFile - 1] = pOpts->papszFiles[iFile];
            pOpts->cFiles--;
        }
        else if (rc > 0)
            return rc;
    }

    /*
     * Check that we've found at least one file argument.
     */
    if (pOpts->cFiles > 0)
    {
        *pfExit = K_FALSE;
        return 0;
    }
    return touch_syntax("No file specified");
}


/**
 * Touches one file.
 *
 * @returns exit code.
 * @param   pOpts               The options.
 * @param   pszFile             The file to touch.
 */
static int touch_process_file(PKMKTOUCHOPTS pOpts, const char *pszFile)
{
    int             fd;
    int             rc;
    struct stat     St;
    struct timeval  aTimes[2];

    /*
     * Create the file if it doesn't exists.  If the --no-create/-c option is
     * in effect, we silently skip the file if it doesn't already exist.
     */
    if (pOpts->fDereference)
        rc = stat(pszFile, &St);
    else
        rc = lstat(pszFile, &St);
    if (rc != 0)
    {
        if (errno != ENOENT)
            return touch_error("Failed to stat '%s': %s", pszFile, strerror(errno));

        if (!pOpts->fCreate)
            return 0;
        fd = open(pszFile, O_WRONLY | O_CREAT, 0666);
        if (fd == -1)
            return touch_error("Failed to create '%s': %s", pszFile, strerror(errno));

        /* If we're not setting the current time, we may need value stat info
           on the file, so get it thru the file descriptor before closing it. */
        if (pOpts->enmAction == kTouchActionCurrent)
            rc = 0;
        else
            rc = fstat(fd, &St);
        if (close(fd) != 0)
            return touch_error("Failed to close '%s' after creation: %s", pszFile, strerror(errno));
        if (rc != 0)
            return touch_error("Failed to fstat '%s' after creation: %s", pszFile, strerror(errno));

        /* We're done now if we're setting the current time. */
        if (pOpts->enmAction == kTouchActionCurrent)
            return 0;
    }

    /*
     * Create aTimes and call utimes/lutimes.
     */
    aTimes[0].tv_sec  = St.st_atime;
    aTimes[1].tv_sec  = St.st_mtime;
#if FILE_TIMESTAMP_HI_RES
    aTimes[0].tv_usec = St.st_atim.tv_nsec / 1000;
    aTimes[1].tv_usec = St.st_mtim.tv_nsec / 1000;
#else
    aTimes[0].tv_usec = 0;
    aTimes[1].tv_usec = 0;
#endif
    if (   pOpts->enmWhatToTouch == kTouchAccessAndModify
        || pOpts->enmWhatToTouch == kTouchAccessOnly)
    {
        if (   pOpts->enmAction == kTouchActionCurrent
            || pOpts->enmAction == kTouchActionSet)
            aTimes[0] = pOpts->NewATime;
        else
            aTimes[0].tv_sec += pOpts->NewATime.tv_sec;
    }
    if (   pOpts->enmWhatToTouch == kTouchAccessAndModify
        || pOpts->enmWhatToTouch == kTouchModifyOnly)
    {
        if (   pOpts->enmAction == kTouchActionCurrent
            || pOpts->enmAction == kTouchActionSet)
            aTimes[1] = pOpts->NewMTime;
        else
            aTimes[1].tv_sec += pOpts->NewMTime.tv_sec;
    }

    /*
     * Try set the times.  If we're setting current time, fall back on calling
     * [l]utimes with a NULL timeval vector since that has slightly different
     * permissions checks.  (Note that we don't do that by default because it
     * may do more than what we want (st_ctime).)
     */
    if (pOpts->fDereference)
        rc = utimes(pszFile, aTimes);
    else
        rc = lutimes(pszFile, aTimes);
    if (rc != 0)
    {
        if (pOpts->enmAction == kTouchActionCurrent)
        {
            if (pOpts->fDereference)
                rc = utimes(pszFile, NULL);
            else
                rc = lutimes(pszFile, NULL);
        }
        if (rc != 0)
            rc = touch_error("%stimes failed on '%s': %s", pOpts->fDereference ? "" : "l", pszFile, strerror(errno));
    }

    return rc;
}


/**
 * The function that does almost everything here... ugly.
 */
#ifdef KMK
int kmk_builtin_touch(int argc, char **argv, char **envp)
#else
int main(int argc, char **argv, char **envp)
#endif
{
    int             rc;
    KMKTOUCHOPTS    Opts;
    K_NOREF(envp);

    /*
     * Initialize options with defaults and parse them.
     */
    Opts.enmWhatToTouch = kTouchAccessAndModify;
    Opts.enmAction      = kTouchActionCurrent;
    Opts.fCreate        = K_TRUE;
    Opts.fDereference   = K_TRUE;
    Opts.cFiles         = 0;
    Opts.papszFiles     = (char **)calloc(argc, sizeof(char *));
    if (Opts.papszFiles)
    {
        rc = gettimeofday(&Opts.NewATime, NULL);
        if (rc == 0)
        {
            KBOOL fExit;
            Opts.NewMTime = Opts.NewATime;

            rc = touch_parse_args(&Opts, argc, argv, &fExit);
            if (rc == 0 && !fExit)
            {
                /*
                 * Process the files.
                 */
                int iFile;
                for (iFile = 0; iFile < Opts.cFiles; iFile++)
                {
                    int rc2 = touch_process_file(&Opts, Opts.papszFiles[iFile]);
                    if (rc2 != 0 && rc == 0)
                        rc = rc2;
                }
            }
        }
        else
            rc = touch_error("gettimeofday failed: %s", strerror(errno));
        free(Opts.papszFiles);
    }
    else
        rc = touch_error("calloc failed");
    return rc;
}

