/*
 * SNOOPY LOGGER
 *
 * File: snoopy/filter/only_uid.c
 *
 * Copyright (c) 2014-2015 Bostjan Skufca <bostjan@a2o.si>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */



/*
 * SNOOPY FILTER: only_uid
 *
 * Description:
 *     Excludes all log messages not comming from specific UID
 *
 * Params:
 *     logMessage: pointer to string that contains formatted log message (may be manipulated)
 *     arg:        Comma-separated list of UIDs for which log message is passed on, dropped for all others
 *
 * Return:
 *     SNOOPY_FILTER_PASS or SNOOPY_FILTER_DROP
 */
#include "snoopy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>



int snoopy_filter_only_uid (char *msg, char *arg)
{
    uid_t  curUid;     // Actual UID of running process
    int    j;
    char  *str1;

    // Do not assign null to it explicitly, as you get "Explicit null dereference" Coverity error.
    // If you do not assign it, Coverity complains with "Uninitialized pointer read".
    char  *saveptr1 = "";


    /* Get uid of current process */
    curUid = getuid();

    /* Loop through all UIDs passed to the filter as argument */
    for (j=1, str1=arg;  ; j++, str1=NULL) {
        char  *argCurUidStr;   // Literal UID
        uid_t  argCurUid;      // Actual UID to be used for comparison

        // Get next literal UID
        argCurUidStr = strtok_r(str1, ",", &saveptr1);
        if (NULL == argCurUidStr) {
            break;
        }

        // Convert literal UID to numeric type
        argCurUid = atol(argCurUidStr);

        // Compare and return PASS if matches
        if (argCurUid == curUid) {
            return SNOOPY_FILTER_PASS;
        }
    }

    // None of the UIDs matched so far, therefore we are dropping this
    return SNOOPY_FILTER_DROP;
}
