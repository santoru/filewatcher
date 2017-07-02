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
 * pipeconfig.c
 *
 * This is the configuration file for the pipe, it initializes the audit pipe
 * with the ioctl methods and set which audit classes to follow.
 *
 */

#include "pipeconfig.h"

FILE* initPipe() {

    char* auditPipe = "/dev/auditpipe";

    // According with /etc/security/audit_class
    u_int dataFlow =
        //0x00000000 | // Invalid Class (no)
        0x00000001 | // File read (fr)
        0x00000002 | // File write (fw)
        0x00000004 | // File attribute access (fa)
        0x00000008 | // File attribute modify (fm)
        0x00000010 | // File create (fc)
        0x00000020 | // File delete (fd)
        0x00000040 ;     // File close (cl)
        //0x00000080 | // Process (pc)
        //0x00000100 | // Network (nt)
        //0x00000200 | // IPC (ip)
        //0x00000400 | // Non attributable (na)
        //0x00000800 | // Administrative (ad)
        //0x00001000 | // Login/Logout (lo)
        //0x00002000 | // Authentication and authorization (aa)
        //0x00004000 | // Application (ap)
        //0x20000000 | // ioctl (io)
        //0x40000000 | // exec (ex)
        //0x80000000 | // Miscellaneous (ot)
        //0xffffffff ; // All flags set (all)

    FILE* auditFile;
    int auditFileDescriptor;

    auditFile = fopen(auditPipe, "r");
    if (auditFile == NULL) {
        fprintf(stderr, "Unable to open audit pipe: %s\n", auditPipe);
        perror("Error ");
        exit(1);
    }
    auditFileDescriptor = fileno(auditFile);


    // Configure the audit pipe

    int ioctlReturn;

    int mode = AUDITPIPE_PRESELECT_MODE_LOCAL;
    ioctlReturn = ioctl(
        auditFileDescriptor,
        AUDITPIPE_SET_PRESELECT_MODE,
        &mode);
    if (ioctlReturn == -1) {
        fprintf(stderr, "Unable to set the audit pipe mode to local.\n");
        perror("Error ");
    }

    int queueLength;
    ioctlReturn = ioctl(
        auditFileDescriptor,
        AUDITPIPE_GET_QLIMIT_MAX,
        &queueLength);
    if (ioctlReturn == -1) {
        fprintf(stderr,
            "Unable to get the maximum queue length of the audit pipe.\n");
        perror("Error ");
    }

    ioctlReturn = ioctl(
        auditFileDescriptor,
        AUDITPIPE_SET_QLIMIT,
        &queueLength);
    if (ioctlReturn == -1) {
        fprintf(stderr,
            "Unable to set the queue length of the audit pipe.\n");
        perror("Error ");
    }

    u_int attributableEventsMask = dataFlow;
    ioctlReturn = ioctl(
        auditFileDescriptor,
        AUDITPIPE_SET_PRESELECT_FLAGS,
        &attributableEventsMask);
    if (ioctlReturn == -1) {
        fprintf(stderr,
            "Unable to set the attributable events preselection mask.\n");
        perror("Error ");
    }

    u_int nonAttributableEventsMask = dataFlow;
    ioctlReturn = ioctl(
        auditFileDescriptor,
        AUDITPIPE_SET_PRESELECT_NAFLAGS,
        &nonAttributableEventsMask);
    if (ioctlReturn == -1) {
        fprintf(stderr,
            "Unable to set the non-attributable events preselection mask.\n");
        perror("Error ");
    }
    return auditFile;
}
