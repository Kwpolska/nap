#include <stdio.h>
#include "nap.h"

/*
 * nap v0.2.1
 * sleep with a progressbar
 * Copyright © 2013–2014, Chris “Kwpolska” Warrick.
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

int main(int argc, char* argv[]) {
    int i = 0;
    char time_arg = 1;
    char expected_argc = 2;
    char stay_mode = 0;
    strcpy(PROGNAME, argv[0]);
    if (argc == 1 ||
        strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0) {
        usage(1);
        return 2;
    } else if (strcmp(argv[1], "-v") == 0 ||
               strcmp(argv[1], "--version") == 0) {
        fprintf(stderr, "%s %s\n", argv[0], VERSION);
        return 2;
    }

    if (strcmp(argv[1], "-s") == 0 ||
        strcmp(argv[1], "--stay") == 0) {
        stay_mode = 1;
        time_arg = 2;
        expected_argc = 3;
    }

    if (argc != expected_argc) {
        usage(1);
        return 2;
    }

    // we do use timespec, but we manage them ourselves
    // (we do not pass tmsec to nanosleep or the like)
    struct timespec tmsec = input_to_timespec(argv[time_arg]);
    struct nruns nr = timespec_to_nruns(tmsec);
#if defined(DEBUG) || defined(DRYMODE)
    fprintf(stderr, "abuser wants %s\n", print_timespec(tmsec));
    fprintf(stderr, "nruns: %lld runs, %s, final %s\n", nr.runs,
            print_timespec(nr.runlength), print_timespec(nr.final));
#endif

#ifndef DRYMODE
    pbar(0, 1);
    for (i = 0; i < nr.runs; i++) {
        nanosleep(&nr.runlength, &nr.runlength);
        pbar(i, nr.runs + 1);
    }
    nanosleep(&nr.final, &nr.final);
    pbar(1, 1);
    if (stay_mode) {
        // stay mode, progressbar stays on the screen
        printf("\n");
    } else {
        // progressbar disappears at the end
        erase_bar();
    }
#endif

    return 0;
}
