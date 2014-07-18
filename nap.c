#include <stdio.h>
#include "nap.h"

int main(int argc, char* argv[]) {
    int i = 0;
    strcpy(PROGNAME, argv[0]);
    if (argc != 2 ||
        strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0) {
        usage(1);
        return 2;
    } else if (strcmp(argv[1], "-v") == 0 ||
               strcmp(argv[1], "--version") == 0) {
        fprintf(stderr, "%s %s\n", argv[0], VERSION);
        return 2;
    } else {
        // we do use timespec, but we manage them ourselves
        // (we do not pass tmsec to nanosleep or the like)
        struct timespec tmsec = input_to_timespec(argv[1]);
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
        // the progressbar disappears at the end
        printf("\r");

#endif
    }

    return 0;
}
