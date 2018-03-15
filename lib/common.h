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
 * along with filewatcher. If not, see <http://www.gnu.org/licenses/>.
 *
 * common.h
 *
 * This file contains the definitions of common used functions and variables.
 *
 */

#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
//#include <arpa/inet.h>
//#include <bsm/audit.h>
#include <bsm/libbsm.h>
//#include <bsm/audit_kevents.h>
#include <sys/ioctl.h>
//#include <sys/types.h>
#include <security/audit/audit_ioctl.h>
//#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <libproc.h>
#include <pwd.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>


#define OUT stdout
#define ERR fopen("log/error.log", "a+")
#define OFF fopen("/dev/null", "w");
#define DBG fopen("log/debug.log", "a+");

//Put some colors on these outputs :D
#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define C_RESET   "\x1b[0m"


#define HOMEDIR getenv("HOME")

char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
char timebuff[26];
char hourbuff[26];

FILE* output;
FILE* error;
FILE* debug;
FILE * auditFile;

char* fileFilter;
char* processFilter;
bool verbose;
bool showAll;

struct auditEvent {
    char* filePath;
    char* processPath;
    pid_t processPid;
    char* username;
    int eventType;
};

bool isRoot();
char* getPathArg(char * processPath, int value);
char* getProcFromPid (pid_t pid);
char* getCurrentTimestamp();
char* getCurrentTime();
char* getEventString(int event);

void banner();
void shutDown();

#endif
