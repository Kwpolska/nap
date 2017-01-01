#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "kwpbar.h"

/*
 * nap v0.2.5
 * sleep with a progressbar
 * Copyright © 2013-2017, Chris Warrick.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions, and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the author of this software nor the names of
 *    contributors to this software may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    consent.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

char PROGNAME[64] = "nap"; // overwritten by main() with argv[0]
char VERSION[16] = "0.2.5";

struct nruns {
    long long runs;
    struct timespec runlength;
    struct timespec final;
};

void usage(int showname) {
    if (showname) {
        fprintf(stderr, "%s %s -- sleep with a progress bar\n\n", PROGNAME, VERSION);
    }
    fprintf(stderr, "usage: %s [-s | --stay] SECONDS\n", PROGNAME);
    fprintf(stderr, "       %s [-s | --stay] TIME(s|m|h|d)\n", PROGNAME);
    fprintf(stderr, "       %s (-h | --help)\n", PROGNAME);
    fprintf(stderr, "       %s (-v | --version)\n", PROGNAME);
    fprintf(stderr, "\nIf -s, --stay is specified, the progressbar stays on the screen.\n");
    fprintf(stderr, "Otherwise, it is erased after execution.\n");
}

int error(char* errortext, int showusage) {
    fprintf(stderr, "error: %s\n", errortext);
    if (showusage) usage(0);
    exit(1);
}

double timemul(double time, double num) {
    // I do this only to display an error easily without repeating myself.
    double t = time * num;
    // usually INT_MAX, just without a wasteful import of limits.h.
    // Besides, I hard-coded the human representation below.
    // 16-bit systems are not supported by design, because you could only
    // represent 9 (signed) or 18 (unsigned) hours.  And time_t being
    // 9 or 18 hours is illogical.
    // On the other hand, certain systems could get this value bigger.
    // However, nobody will use this for 68 years (68y 19d 3:14:08) thus
    // it makes no sense to support anything bigger.
    // Actually, I could’ve chosen a smaller arbitrary number, like
    // a year (~31.5M).  This is a quite generous choice.
    double MAX = 2147483647.;
    if (time > MAX) {
        error("value is too big (over (2^31) - 1 = 68 years)", 0);
    }
    return t;
}

double evaluate_seconds(double time, char suffix) {
    if (isdigit(suffix)) {
        return time;
    } else {
        switch (suffix) {
            // coreutils' idea, it's quite useful.
            case 's':
                return timemul(time, 1);
                break;
            case 'm':
                return timemul(time, 60);
                break;
            case 'h':
                return timemul(time, 3600);
                break;
            case 'd':
                return timemul(time, 86400);
                break;
            default:
                return -1;
        }
    }
}

struct timespec sec_to_timespec(double usertime) {
    time_t usersec = (time_t)floor(usertime);
    long usernsec = (long)floor(1e9 * (usertime - usersec));
    struct timespec sleeptime;
    if (usernsec >= 1e9) {
        error("nsec over or equal 1e9 -- something is wrong", 0);
    }
    sleeptime.tv_sec = usersec;
    sleeptime.tv_nsec = usernsec;
    return sleeptime;
}

struct timespec input_to_timespec(char* userinput) {
    if (userinput[0] == '-') {
        error("not a time machine", 0);
    }
    char suffix = userinput[strlen(userinput) - 1];
    double usertime = evaluate_seconds(atof(userinput), suffix);

    if (usertime == -1) {
        char* buf = NULL;
        sprintf(buf, "suffix %c not supported (not [ smhd])", suffix);
        error(buf, 0);
    }
    struct timespec out = sec_to_timespec(usertime);
    return out;
}

struct nruns timespec_to_nruns(struct timespec time) {
    struct nruns nanoruns;
    struct timespec runlength;
    double sec = time.tv_sec + (time.tv_nsec / 1.e9);
        int pbarsize = get_termlength() - 8;
        runlength = sec_to_timespec(sec/pbarsize);
        double runsec = runlength.tv_sec + (runlength.tv_nsec / 1.e9);
        double druns = sec/runsec;
        long runs = floor(druns);
        double final = druns - runs;
        nanoruns.runlength = runlength;
        nanoruns.runs = runs;
        nanoruns.final = sec_to_timespec(final);
    return nanoruns;
}

char* print_timespec(struct timespec ts) {
    char* out = malloc(128);
    sprintf(out, "%ld s, %ld ns=%f s", (long)ts.tv_sec, ts.tv_nsec,
            (double)ts.tv_nsec / 1.e9);
    return out;
}


