/* $Id$
 *
 * Documentation generator.
 *
 * Copyright (c) 2002 knut st. osmundsen <bird-kBuild-spam@anduin.net>
 *
 *
 * This file is part of kBuild.
 *
 * kBuild is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * kBuild is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with kBuild; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


/*******************************************************************************
*   Structures and Typedefs                                                    *
*******************************************************************************/
typedef struct _Section
{
    struct _Section    *pNext;          /* Next Section. */
    int                 iLevel;         /* 0 is @design, 1 is @subsection and so on. */
    char               *pszHeader;
    char               *pszText;        /* Content of the section. */
    int                 cchText;
} SECTION, *PSECTION;


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
PSECTION    pSections = NULL;
PSECTION    pSectionsTail = NULL;


/**
 * Reads the file parsing out the @design and @sub[..]section
 * parts putting them into the pSections list.
 */
int ParseFile(const char *pszFilename)
{
    int     rc = 0;
    FILE *  phFile;

    /*
     * Open the file.
     */
    phFile = fopen(pszFilename, "r");
    if (phFile)
    {
        static char szLine[0x10000];
        enum {enmUnknown, enmSection, enmSectionClosing}
                    enmState = enmUnknown;
        PSECTION    pCurSection = NULL;

        /*
         * Read the file line by line. looking for @design and @sub[..]section.
         */
        while (fgets(szLine, sizeof(szLine), phFile))
        {
            int     fNew = 0;
            int     iLevel = 0;
            char   *psz = szLine;
            char   *pszEnd = &szLine[strlen(szLine) - 1];
            /*
             * Strip off any ' ', '\t', '\n', '\r', '\*\/' and '*' from end
             * Strip off any ' ', '\t', '\/\*', '*' and '//' from start.
             * Need to check for closing comment too.
             */
            while (     pszEnd >= psz
                   && (   *pszEnd == '*'
                       || *pszEnd == ' '
                       || *pszEnd == '\t'
                       || *pszEnd == '\r'
                       || *pszEnd == '\n'
                        )
                    )
            {
                *pszEnd-- = '\0';
            }
            if (pszEnd > psz && !strcmp(&pszEnd[-1], "*/") && enmState == enmSection)
                enmState = enmSectionClosing;
            while (     pszEnd >= psz
                   && (   *pszEnd == '*'
                       || *pszEnd == ' '
                       || *pszEnd == '\t'
                       || *pszEnd == '\n'
                       || *pszEnd == '\r'
                       || (*pszEnd == '/' && pszEnd > psz && pszEnd[-1] == '*')
                        )
                    )
            {
                if (*pszEnd == '/')
                    *pszEnd-- = '\0';
                *pszEnd-- = '\0';
            }

            while (   *psz == '*'
                   || *psz == ' '
                   || *psz == '\t'
                   || (*psz == '/' && (psz[1] == '*' || psz[1] == '/')))
            {
                if (*psz++ == '/')
                    psz++;
            }


            /*
             * Look for tag.
             */
            if (!strncmp(psz, "@design", 7))
            {
                fNew = 1;
                iLevel = 0;
            }
            else if (!strncmp(psz, "@sub", 4))
            {
                char *psz2 = psz + 4;
                fNew = 1;
                iLevel = 1;
                while (!strncmp(psz2, "sub", 3))
                {
                    psz2 += 3;
                    iLevel++;
                }
            }

            /*
             * Action
             */
            if (fNew)
            {
                char *psz2;
                /*
                 * New Section.
                 *  Change state.
                 *  Allocate new section struct, init it and link it into the list.
                 *  Get section header.
                 */
                if (enmState != enmSectionClosing)
                    enmState = enmSection;

                pCurSection = malloc(sizeof(*pCurSection));
                memset(pCurSection, 0, sizeof(*pCurSection));
                pCurSection->iLevel = iLevel;
                if (pSectionsTail)
                    pSectionsTail = pSectionsTail->pNext = pCurSection;
                else
                    pSections = pSectionsTail = pCurSection;

                psz2 = strpbrk(psz, " \t");
                if (psz2)
                {
                    while (*psz2 == ' ' || *psz == '\t')
                        psz2++;
                    if (*psz)
                        pCurSection->pszHeader = strdup(psz2);
                }
            }
            else if (enmState == enmSection || enmState == enmSectionClosing)
            {
                /*
                 * Add text to current section
                 */
                int cch = strlen(psz);
                if (!cch && pCurSection->cchText)
                {
                    psz = "<p>";
                    cch = strlen(psz);
                }

                if (cch)
                {
                    pCurSection->pszText = realloc(pCurSection->pszText, pCurSection->cchText + cch + 2);
                    pCurSection->pszText[pCurSection->cchText++] = '\n';
                    strcpy(&pCurSection->pszText[pCurSection->cchText], psz);
                    pCurSection->cchText += cch;
                }
            }

            /*
             * State transition.
             */
            if (enmState == enmSectionClosing)
                enmState = enmUnknown;

        } /* while fgets */

        fclose(phFile);
    }
    else
    {
        fprintf(stderr, "error: failed to open %s. errno=%d\n", pszFilename, errno);
        rc = errno;
    }

    return rc;
}


/**
 * Checks if psz is point to a tag we pass thru.
 * @returns length of tag if pass thru tag.
 * @returns 0 if not.
 * @param   psz     Pointer to text string.
 */
int isTag(const char *psz)
{
    int     i;
    static char *  apszTags[] =
    {
        "<b>",  "</b>",
        "<i>",  "</i>",
        "<ul>",  "</ul>",
        "<ol>",  "</ol>",
        "<pre>", "</pre>",
        "<h1>",  "</h1>",
        "<h2>",  "</h2>",
        "<h3>",  "</h3>",
        "<h4>",  "</h4>",
        "<h5>",  "</h5>",
        "<h6>",  "</h6>",
        "<li>",
        "<p>",
        "<br>"
    };

    if (*psz == '<')
    {
        for (i = 0; i < sizeof(apszTags) / sizeof(apszTags[0]); i++)
        {
            int cch = strlen(apszTags[i]);
            if (!strnicmp(apszTags[i], psz, cch))
                return cch;
        }
    }

    return 0;
}


/**
 * HTMLify text and print it.
 * @param   pszText     Text in question.
 */
void PutHtmlText(const char *pszText)
{
    while (*pszText)
    {
        char ch = *pszText;
        char sz[256];
        sz[0] = '\0';
        switch (ch)
        {
            case '<':
            {
                int cch = isTag(pszText);
                if (cch)
                {
                    strncat(sz, pszText, cch);
                    pszText += cch - 1;
                }
                else
                    strcpy(sz, "&lt;");
                break;
            }

            case '>':
                strcpy(sz, "&gt;");
                break;

            case '&':
                strcpy(sz, "&amp;");
                break;

            default:
                sz[0] = ch;
                sz[1] = '\0';
        }
        printf("%s", sz);
        pszText++;
    }
}


/**
 * Keep track and formats section level.
 */
void SectionNumber(int iLevel, int *paiSections, char *pszSection)
{
    int i;

    paiSections[iLevel]++;
    for (i = iLevel + 1; i < 100; i++)
        paiSections[i] = 0;

    sprintf(pszSection, "%d", paiSections[0]);
    if (iLevel == 0)
        strcat(pszSection, ".0");
    else
    {
        for (i = 1; i <= iLevel; i++)
            sprintf(&pszSection[strlen(pszSection)], ".%d", paiSections[i]);
    }
}


/**
 * Outputs the section stuff to stdout as HTML.
 */
int MakeHTML(void)
{
    int         aiSections[100];
    char        szSection[1024];
    PSECTION    pCurSection;

    #if 0
    /* debug */
    for (pCurSection = pSections; pCurSection; pCurSection = pCurSection->pNext)
        fprintf(stderr, "debug: level=%d  cchText=%-4d header=%s \n",
                pCurSection->iLevel, pCurSection->cchText, pCurSection->pszHeader);
    #endif

    /*
     * Header
     */
    printf("<!-- Generate by docdesign -->\n"
           "<head>\n"
           "<title>Design Document</title>\n"
           "\n"
           "<body>\n"
           );

    /*
     * Content
     */
    printf("<a name=content><h2>Content</h2></a>\n"
           "<ul>\n"
           );
    memset(&aiSections[0], 0, sizeof(aiSections));
    for (pCurSection = pSections; pCurSection; pCurSection = pCurSection->pNext)
    {
        SectionNumber(pCurSection->iLevel, &aiSections[0], szSection);
        printf("  <li><a href=\"#%s\">%s %s</a>\n",
               szSection, szSection, pCurSection->pszHeader);
    }
    printf("</ul>\n"
           "\n");

    /*
     * Sections.
     */
    memset(&aiSections[0], 0, sizeof(aiSections));
    for (pCurSection = pSections; pCurSection; pCurSection = pCurSection->pNext)
    {
        int iHNumber = min(pCurSection->iLevel + 1, 5);
        SectionNumber(pCurSection->iLevel, &aiSections[0], szSection);
        printf("<p><br><p>\n"
               "<a href=#content><a name=%s><h%d>%s %s</h%d></a></a>\n"
               "\n",
               szSection, iHNumber, szSection, pCurSection->pszHeader, iHNumber);
        if (pCurSection->pszText)
            PutHtmlText(pCurSection->pszText);
    }
    printf("</ul>\n"
           "\n");


    /* footer */
    printf("</body>\n"
           "</head>\n");

    return -1;
}


int main(int argc, char **argv)
{
    int rc;
    int argi;

    /*
     * Parse arguments.
     */
    for (argi = 1, rc = 0; !rc && argi < argc; argi++)
    {
        if (argv[argi][0] == '@')
        {
            FILE *phFile = fopen(&argv[argi][1], "r");
            if (phFile)
            {
                char szFilename[1024];
                while (!rc && fgets(szFilename, sizeof(szFilename), phFile))
                {
                    char *psz = szFilename;
                    char *pszEnd = &psz[strlen(psz)] - 1;
                    while (*psz == '\t' || *psz == ' ') psz++;
                    while (pszEnd >= psz && (*pszEnd == '\t' || *pszEnd == ' ' || *pszEnd == '\n' || *pszEnd == '\r'))
                        *pszEnd-- = '\0';
                    rc = ParseFile(psz);
                }
            }
        }
        else
            rc = ParseFile(argv[argi]);
    }

    if (!rc)
        rc = MakeHTML();

    return rc;
}
