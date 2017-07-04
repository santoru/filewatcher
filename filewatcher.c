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
 * along with filewatcher.  If not, see <http://www.gnu.org/licenses/>.
 *
 * filewatcher.c
 *
 * This is the main file of the program.
 * It contains (obviously) the main function :)
 *
 */

#include "lib/common.h"
#include "lib/optparse.h"
#include "lib/whitelist.h"
#include "lib/pipeconfig.h"
#include "lib/auditing.h"
#include "lib/tokens.h"

extern char* fileFilter;
extern char* processFilter;
extern bool verbose;

int main(int argc, char** argv) {
    signal(SIGINT, shutDown);

    output = OUT;
    error = ERR;
    debug = OFF;
    banner();

    if(!isRoot()) {
        fprintf(output, "This software must be run as root.\n");
        exit(1);
    }

    parseArguments(argc, argv);

    auditFile = initPipe();


    int i = 0;
    while (true) {
        i++;
        struct auditEvent currentEvent;

        currentEvent = getEvent(auditFile);

        // Printing output
        printEvent(currentEvent);
    }

    fclose(auditFile);
    fprintf(output, "Exiting..\n");
}
