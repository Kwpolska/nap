#include <stdio.h>
#include "nap.h"

// TODO REMOVE
void do_nothingts(struct timespec ts) {}
void do_nothingnr(struct nruns nr) {}

int main(int argc, char* argv[]) {
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
        fprintf(stderr, "nruns: %lld runs, %s, final %f\n", nr.runs,
               print_timespec(nr.runlength), nr.final);
#endif

#ifndef DRYMODE
        fprintf(stderr, "not dry mode, but doesn't matter just yet\n");
        // TODO REMOVE
        do_nothingts(tmsec);
        do_nothingnr(nr);
#endif
    }

    return 0;
}
