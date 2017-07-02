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
 * optparse.c
 *
 * This file is responsible to parsing and setting options. Main function call
 * the parseArguments function to parse every options set by the user and to
 * configure filters and settings.
 *
 */

#include "optparse.h"
#include <getopt.h>
#include <unistd.h>

int c;
const char* short_opt = "f:p:dah";
struct option long_opt[] = {
   {"file",     required_argument,  NULL,   'f'},
   {"process",  required_argument,  NULL,   'p'},
   {"debug",    no_argument,        NULL,   'd'},
   {"all",      no_argument,        NULL,   'a'}
};

void parseArguments(int argc, char** argv){
    if (argc < 2){
        usage(argv[0]);
        exit(1);
    }
    while ((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
        switch (c) {
            case 'f':
                fileFilter = optarg;
                fprintf(output, "Filtering by file.. ("C_CYAN"%s"C_RESET")\n", fileFilter);
                break;
            case 'p':
                processFilter = optarg;
                fprintf(output, "Filtering by process.. ("C_CYAN"%s"C_RESET")\n", processFilter);
                break;
            case 'd':
                verbose = 1;
                fprintf(output, "Debug logging is "C_GREEN"enabled"C_RESET", you can find debug information under log directory.\n");
                debug = DBG;
                break;
            case 'a':
                showAll = 1;
                fprintf(output, "You choose to show "C_RED"all events"C_RESET" according to the filter you set. Be aware that this could dispay a huge amount of events.\n");
                break;
            case 'h':
                usage(argv[0]);
                exit(1);
            case '?':
                if (optopt == 'f') {
                    fprintf(output, "You must specify a filename to filter\n");
                    exit(1);
                }
                else if (optopt == 'p') {
                    fprintf(output, "You must specify a process to filter\n");
                    exit(1);
                }
                else {
                    fprintf(output, "Operation not recognize, use -h to display help\n");
                    exit(1);
                }
                return;
            default:
                abort();
        }
    }
}

void usage(char* arg) {
    printf("\n");
    printf("Usage: %s [OPTIONS]\n", arg);
    printf("  -f, --file                 Set a file to filter\n");
    printf("  -h, --process              Set a process name to filter\n");
    printf("  -a, --all                  Display all events (By default only basic events like open/read/write are displayed)\n");
    printf("  -d, --debug                Enable debugging messages to be saved into a file\n");
    printf("  -h, --help                 Print this help and exit\n");
    printf("\n");
    return;
}
