#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// global variable is justified by logic.
char VERSION[] = "20131117";

int print_usage(char* name) {
    printf("%s %s -- sleep with a progress bar\n\n", name, VERSION);
    printf("usage: %s seconds\n", name);
    printf("       %s time(s|m|h|d)\n", name);
    printf("       %s (-h | --help)\n", name);
    printf("       %s (-v | --version)\n", name);
    return 1;
}

unsigned int convert_to_seconds(char* userinput) {
    if (isdigit(userinput[strlen(userinput)-1]) ) {
        return atoi(userinput);
    } else {
        return -1;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2 ||
        strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0) {
        return print_usage(argv[0]);
    } else if (strcmp(argv[1], "-v") == 0 ||
               strcmp(argv[1], "--version") == 0) {
        printf("%s %s\n", argv[0], VERSION);
        return 1;
    } else {
        printf("not implemented\nwhat did you expect?\n");
        unsigned int t = convert_to_seconds(argv[1]);
        printf("want %d seconds", t);
    }

    return 0;
}
