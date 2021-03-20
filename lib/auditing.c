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
 * auditing.h
 *
 * This file manage the auditing of events. For every event, it fetch tokens
 * from the audit pipe and parse the token structs to return an auditEvent
 * struct with all informations about the single event.
 *
 */

 #include "auditing.h"

u_char* buffer;
int recordLength;
int recordBalance;
int processedLength;
int tokenCount;
int fetchToken;
tokenstr_t token;


struct auditEvent getEvent(FILE* auditFile) {
    struct auditEvent curr;
    memset(&curr, 0, sizeof(curr));

    recordLength = au_read_rec(auditFile, &buffer);
    if (recordLength == -1) {
        return curr;
    }

    recordBalance = recordLength;
    processedLength = 0;
    tokenCount = 0;
    int i = 1;


    while (recordBalance) {
        // Extract a token from the record
        fetchToken = au_fetch_tok(&token,
            buffer + processedLength,
            recordBalance);

            if (fetchToken == -1) {
                fprintf(error, "Error fetching token.\n");
                break;
            }
            switch (token.id) {
                case AUT_HEADER32:
                curr.eventType = token.tt.hdr32.e_type;
                break;
                case AUT_HEADER32_EX:
                fprintf(debug, "AUT_HEADER32_EX\n");
                break;
                case AUT_HEADER64:
                fprintf(debug, "AUT_HEADER64\n");
                break;
                case AUT_HEADER64_EX:
                fprintf(debug, "AUT_HEADER64_EX\n");
                break;
                case AUT_SUBJECT32:
                curr.processPid = (long)token.tt.subj32.pid;
                curr.processPath = getProcFromPid(token.tt.subj32.pid);
                struct passwd *user = getpwuid(token.tt.subj32.ruid);
                curr.username = user->pw_name;
                break;
                case AUT_SUBJECT64:
                fprintf(debug, "AUT_SUBJECT64\n");
                break;
                case AUT_SUBJECT32_EX:
                fprintf(debug, "AUT_SUBJECT32_EX\n");
                break;
                case AUT_RETURN32:
                //fprintf(debug, "AUT_RETURN32\n");
                break;
                case AUT_RETURN64:
                fprintf(debug, "AUT_RETURN64\n");
                break;
                case AUT_ATTR:
                case AUT_ATTR32:
                //fprintf(debug, "AUT_ATTR\n");
                break;
                case AUT_PATH:
                curr.filePath = token.tt.path.path;
                break;
            }
            tokenCount++;
            processedLength += token.len;
            recordBalance -= token.len;
            i++;
        }
        free(buffer);
        return curr;
    }
