/*
 * SNOOPY LOGGER
 *
 * File: snoopy/output/devlogoutput.c
 *
 * Copyright (c) 2014-2015 Bostjan Skufca (bostjan _A_T_ a2o.si)
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
 * SNOOPY OUTPUT: devlogoutput
 *
 * Description:
 *     Sends given message directly to /dev/log socket, without using syslog() syscall
 *
 * Params:
 *     message: message to send
 *
 * Return:
 *     void
 */
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "snoopy.h"
#include "configuration.h"
#include "output/socketoutput.h"



int snoopy_output_devlogoutput (char *logMessage, int errorOrMessage, char *arg)
{
    char  *logMessageWithPrefix = NULL;
    int    retVal;

    /* Generate final message - add prefix which is otherwise added by syslog() syscall */
    logMessageWithPrefix    = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE + 100);   // +100 ought to be enough
    logMessageWithPrefix[0] = '\0';
    sprintf(logMessageWithPrefix, "<%d>snoopy[%d]: %s",
        snoopy_configuration.syslog_facility | snoopy_configuration.syslog_level,
        getpid(),
        logMessage
    );

    /* Pass execution to another output provider */
    retVal = snoopy_output_socketoutput(logMessageWithPrefix, errorOrMessage, "/dev/log");

    /* Housekeeping */
    free(logMessageWithPrefix);
    return retVal;
}
