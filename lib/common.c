/**
 * filewatcher - a simple auditing utility for macOS
 * Copyright (C) 2018 santoru
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
 * Function: getProcFromPid
 * ----------------
 * Return the effective process path from its pid
 */
char* getProcFromPid (pid_t pid) {
#ifdef __APPLE__
    int ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
    if ( ret <= 0 ) {
        fprintf(error, "[!] PID %d: proc_pidpath ();\n", pid);
        fprintf(error, "    %s\n", strerror(errno));
    } else {
        return pathbuf;
    }
#elif defined(__FreeBSD__)
    struct kinfo_proc *proc = kinfo_getproc(pid);
    if ( proc == NULL ) {
        fprintf(error, "[!] PID %d: kinfo_getproc ();\n", pid);
        fprintf(error, "    %s\n", strerror(errno));
    } else {
        memcpy(pathbuf, proc->ki_comm, sizeof(pathbuf));
        free(proc);
        return pathbuf;
    }
#endif
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
    fprintf(output, "\n");
    fprintf(output, "filewatcher - a simple auditing utility for macOS\n");
    fprintf(output, "\n");
}

void shutDown(){
    fclose(auditFile);
    fprintf(output, "\nExiting...\n");
    exit(0);
}

bool startsWith(const char *pre, const char *str) {
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

char* getEventString(int event){
    char ev[15];
    sprintf(ev, "%d", event);
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(AUDIT_EVENT_FILE, "r");
    if (fp == NULL)
        return "N/A";

    while ((read = getline(&line, &len, fp)) != -1) {
        //Getting the line of the event.
        if (startsWith(ev, line)) {
            fprintf(debug,
                "%s - (Event: %i) Found line : %s\n",
                getCurrentTimestamp(), event, line);
            //Parsing the line to return only the event human-readable.
            //TODO: Make this ugly code here better!
            char* segment = strtok(line, ":");
            segment = strtok(0, ":");
            segment = strtok(0, ":");
            //Eliminating the content after "(" to return a nicer output
            char *p = strchr(segment, '(');
            if (!p){
                fclose(fp);
                return segment;
            }
            *p = 0;
            fclose(fp);
            return segment;
        }
    }
    return "N/A";
}
