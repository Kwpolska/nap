#include <stdio.h>
#include <string.h>

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
    }

    return 0;
}
