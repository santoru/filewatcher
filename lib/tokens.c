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
 * tokens.c
 *
 * This file parse the tokens created from the application and print it using
 * whitelists and filters from the user.
 *
 */

#include "tokens.h"

void printEvent(struct auditEvent currentEvent) {
    if (
        raiseAlertForFile(currentEvent.filePath) &&
        raiseAlertForProcess(currentEvent.processPath)) {
            char* event = getEventType(currentEvent.eventType);

            //FIXME: Parse more event and manage this better.
            if (strcmp(event, "") == 0) {
                fprintf(
                    debug,
                    "%s - tokens.c - Event = \"\"\n",
                    getCurrentTimestamp()
                );
                char ev[100];
                sprintf(ev, C_RED"%d"C_RESET, currentEvent.eventType);
                event = ev;
            }
            if (strcmp(event, "hide") == 0) {
                return;
            }

            //TODO: find a better way to use these colors :/
            fprintf(
                output,
                "%s ["C_CYAN"%s"C_RESET"] Detected "C_YELLOW"%s"C_RESET" event from "C_GREEN"%s"C_RESET" -> %s\n",
                getCurrentTime(),
                currentEvent.username,
                event,
                getPathArg(strdup(currentEvent.processPath), 0),
                currentEvent.filePath
            );

            fprintf(
                debug,
                "%s - tokens.c - [%s] Detected %s event from %s -> %s - ",
                getCurrentTimestamp(),
                currentEvent.username,
                event,
                getPathArg(strdup(currentEvent.processPath), 0),
                currentEvent.filePath
            );

            fprintf(
                debug,
                "EventType: %i - Pid: %i - ProcessPath: %s\n",
                currentEvent.eventType,
                currentEvent.processPid,
                currentEvent.processPath
            );
    }
}

char* getEventType(int event) {
    switch (event) {
        case 1:
            return "exit";
        case 14:
            return "access";
        case 22:
            return "readlink";
        case 72:
            return "open/read";
        case 112:
            return "close";
    }
    if (showAll) {
        return getEventString(event);
    }
    return "hide";
}
