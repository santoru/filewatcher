/**
 * filewatcher - a simple auditing utility for macOS
 * Copyright (C) 2018 meliot
 *
 * This file is part of filewatcher.
 *
 * filewatcher is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * filewatcher is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with filewatcher.  If not, see <http://www.gnu.org/licenses/>.
 *
 * whitelist.c
 *
 * This file manage the whitelists (process and files).
 * It also check filters from user to "decide" if an alert must be printed or
 * if it can be ignored.
 *
 */

#include "whitelist.h"

bool raiseAlertForFile(char* filePath) {
    if (filePath == NULL) {
        filePath = "";
    }
    if (fileFilter != NULL) {
        if (!strcasestr(filePath, fileFilter)) {
            return false;
        }
    }
    return true;
    // FIXME:
    // Raise alert only if a process touch files in current user HOMEDIR
    if (strncmp(filePath, HOMEDIR, strlen(HOMEDIR)) == 0) {
        fprintf(debug, "[D] %s is in %s\n", filePath, HOMEDIR);
        return true;
    }
    return false;
}

bool raiseAlertForProcess(char * processPath) {
    if (processFilter != NULL) {
        if (!strcasestr(processPath, processFilter)) {
            return false;
        }
    }
    return true;
    // FIXME:
    // We need to check some white list and rules
    if (isWhiteList(processPath)) {
        return false;
    }
    else if (isAllowedRule(processPath)) {
        return false;
    }
    return true;
}



bool isWhiteList(char* path) {
    char * processName = basename(strdup(path));
    /* TODO: Design a better white list :D */
    char * whiteListProcess [] = {
/*
        "mdworker",
        "mds",
        "mds_stores",
        "locationd",
        "opendirectoryd",
        "netbiosd",
        "storeaccountd",
        "cfprefsd",
        "quicklookd",
        "secinitd",
        "sharedfilelistd",
        "XprotectService",
        "syspolicyd",
*/
    };

    int len = sizeof(whiteListProcess) / sizeof(char*);

    for (int i = 0; i < len; i++) {
        if(!strcmp(whiteListProcess[i], processName)) {
            fprintf(debug, "[D] Process %s is in white list\n", processName);
            return true;
        }
    }
    return false;
}

bool isAllowedRule(char* path){
    // Here we have the list of the rules that are allowed
    return false;
}
