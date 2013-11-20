#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// global variables justified by logic.
char PROGNAME[64] = "nap"; // overwritten by main() with argv[0]
char VERSION[64] = "20131117";

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

void pbar(double value, double max) {
    int fullwidth = 80; // TODO
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
    char nowdraw[fullwidth];
    memset(nowdraw, '\0', sizeof(nowdraw));
    if (now == 0) {
        strcpy(nowdraw, "");
    } else if (progress == 1) {
        memset(nowdraw, '=', now);
    } else {
        memset(nowdraw, '=', now - 1);
        nowdraw[now - 1] = '>';
    }

    char padding[fullwidth];
    memset(padding, '\0', sizeof(padding));
    if (now != pbarwidth) {
        memset(padding, ' ', pbarwidth - now);
    }
    printf("[%s%s]%s%%\n", nowdraw, padding, percs);
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
        printf("abuser wants %ld s, %ld ns=%f s\n", tmsec.tv_sec, tmsec.tv_nsec, (double)tmsec.tv_nsec / 1.e9);
    }

    return 0;
}
