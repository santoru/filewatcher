/**
 * filewatcher - a simple auditing utility for macOS
 * Copyright (C) 2017 m3liot
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
 * along with filewatcher. If not, see <http://www.gnu.org/licenses/>.
 *
 * common.c
 *
 * This file contains the definitions of common used functions and variables.
 *
 */

 #include "common.h"

/*
 * Function: isRoot
 * ----------------
 * Checks if the program run with root privileges
 * and return a boolean.
 *
 */
bool isRoot() {
    uid_t euid = geteuid();
    if (euid != 0) {
        return false;
    }
    return true;
}

/*
 * Function: getPathArg
 * ----------------
 * Split a path in pieces and return the element asked from "value".
 * value = 0 return the last token.
 *
 */
char* getPathArg(char * path, int value){
    char * pathDelimiter = "/";
    char * segment;
    // I want last token, no need to use strtok()
    if (value == 0) {
        segment = strrchr(path, '/');
        segment = segment+1;
    }
    else {
        // I want paramenter number value, using strtok()
        segment = strtok(path, pathDelimiter);
        value--;

        while(value > 0) {
            segment = strtok(0, pathDelimiter);
            value--;
        }
    }
    if (segment != NULL){
        return segment;
    }
    return "";
}

/*
 * Function: getProcFromPid
 * ----------------
 * Return the effective process path from its pid
 */
char* getProcFromPid (pid_t pid) {
    int ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
    if ( ret <= 0 ) {
        fprintf(error, "[!] PID %d: proc_pidpath ();\n", pid);
        fprintf(error, "    %s\n", strerror(errno));
    } else {
        return pathbuf;
    }
    return "";
}

char* getCurrentTimestamp() {
    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(timebuff, 26, "[%Y-%m-%d %H:%M:%S]", tm_info);
    return timebuff;
}

char* getCurrentTime() {
    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(hourbuff, 26, "[%H:%M:%S]", tm_info);
    return hourbuff;
}

void banner() {
    printf("\n");
    printf("filewatcher - a simple auditing utility for macOS\n");
    printf("\n");
}

void shutDown(){
    fclose(auditFile);
    fprintf(output, "\nExiting...\n");
    exit(0);
}
