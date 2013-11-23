#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// global variables justified by logic.
char PROGNAME[64] = "nap"; // overwritten by main() with argv[0]
char VERSION[64] = "20131117";

struct nruns {
    long long runs;
    struct timespec runlength;
    double final;
};

void usage(int showname) {
    if (showname) {
        printf("%s %s -- sleep with a progress bar\n\n", PROGNAME, VERSION);
    }
    printf("usage: %s seconds\n", PROGNAME);
    printf("       %s time(s|m|h|d)\n", PROGNAME);
    printf("       %s (-h | --help)\n", PROGNAME);
    printf("       %s (-v | --version)\n", PROGNAME);
}

int error(char* errortext, int showusage) {
    printf("error: %s\n", errortext);
    if (showusage) usage(0);
    exit(1);
}

int get_termlength() {
    return 80; // TODO
}

void pbar(double value, double max) {
    int fullwidth = get_termlength();
    int pbarwidth = fullwidth - 8;
    double progress = value / max;

    // calculate percentage
    double perc = progress * 100;
    char percs[5] = "  0%";
    if (value == max) {
        strcpy(percs, "100.0");
    } else {
        sprintf(percs, " %4.1f", perc);
    }

    // calculate things to display
    int now = round(progress * pbarwidth);
    char bar[fullwidth];
    memset(bar, '\0', sizeof(bar));
    memset(bar, ' ', pbarwidth);
    if (now == 0) {
        strcpy(bar, "");
    } else if (progress == 1) {
        memset(bar, '=', now);
    } else {
        memset(bar, '=', now - 1);
        bar[now - 1] = '>';
    }

    printf("[%s]%s%%\n", bar, percs);
}

double evaluate_seconds(double time, char suffix) {
    if (isdigit(suffix)) {
        return time;
    } else {
        switch (suffix) {
            // coreutils' idea, it's quite useful.
            case 's':
                return time;
                break;
            case 'm':
                return time * 60;
                break;
            case 'h':
                return time * 3600;
                break;
            case 'd':
                return time * 86400;
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
        sprintf(buf, "suffix %c not supported (not [smhd ])", suffix);
        error(buf, 0);
    }
    struct timespec out = sec_to_timespec(usertime);
    return out;
}

struct nruns timespec_to_nruns(struct timespec time) {
    struct nruns nanoruns;
    struct timespec runlength;
    double sec = time.tv_sec + (time.tv_nsec / 1.e9);
    if (sec < 0.2) {
        nanoruns.runlength = time;
        nanoruns.runs = 1L;
        nanoruns.final = 0.;
    } else {
        int pbarsize = get_termlength() - 8;
        runlength = sec_to_timespec(sec/pbarsize);
        double runsec = runlength.tv_sec + (runlength.tv_nsec / 1.e9);
        if (runsec < 0.1) {
            runlength.tv_sec = 0;
            runlength.tv_nsec = 1e8;
        }
        double druns = sec/runsec;
        long runs = floor(druns);
        double final = druns - runs;
        nanoruns.runlength = runlength;
        nanoruns.runs = runs;
        nanoruns.final = final;
    }
    return nanoruns;
}

char* print_timespec(struct timespec ts) {
    char* out = malloc(512);
    sprintf(out, "%ld s, %ld ns=%f s", ts.tv_sec, ts.tv_nsec,
            (double)ts.tv_nsec / 1.e9);
    return out;
}

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
        printf("%s %s\n", argv[0], VERSION);
        return 2;
    } else {
        // we do use timespec, but we manage them ourselves
        // (we do not pass tmsec to nanosleep or the like)
        struct timespec tmsec = input_to_timespec(argv[1]);
        struct nruns nr = timespec_to_nruns(tmsec);
#if defined(DEBUG) || defined(DRYMODE)
        printf("abuser wants %s\n", print_timespec(tmsec));
        printf("nruns: %lld runs, %s, final %f\n", nr.runs,
               print_timespec(nr.runlength), nr.final);
#endif

#ifndef DRYMODE
        printf("not dry mode, but doesn't matter just yet\n");
        // TODO REMOVE
        do_nothingts(tmsec);
        do_nothingnr(nr);
#endif
    }

    return 0;
}
