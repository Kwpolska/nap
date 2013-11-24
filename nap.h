#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// global variables justified by logic.
char PROGNAME[64] = "nap"; // overwritten by main() with argv[0]
char VERSION[16] = "20131117";

struct nruns {
    long long runs;
    struct timespec runlength;
    double final;
};

void usage(int showname) {
    if (showname) {
        fprintf(stderr, "%s %s -- sleep with a progress bar\n\n", PROGNAME, VERSION);
    }
    fprintf(stderr, "usage: %s seconds\n", PROGNAME);
    fprintf(stderr, "       %s time(s|m|h|d)\n", PROGNAME);
    fprintf(stderr, "       %s (-h | --help)\n", PROGNAME);
    fprintf(stderr, "       %s (-v | --version)\n", PROGNAME);
}

int error(char* errortext, int showusage) {
    fprintf(stderr, "error: %s\n", errortext);
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
    if (progress == 1) {
        memset(bar, '=', now);
    } else if (progress < 0 || progress > 1) {
        fprintf(stderr, "ERROR: invalid progressbar value (not in range [0, 1])");
        exit(1);
    } else if (progress != 0) {
        memset(bar, '=', now - 1);
        bar[now - 1] = '>';
    }

    fprintf(stderr, "\r[%s]%s%%", bar, percs);
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
    char* out = malloc(128);
    sprintf(out, "%ld s, %ld ns=%f s", (long)ts.tv_sec, ts.tv_nsec,
            (double)ts.tv_nsec / 1.e9);
    return out;
}


