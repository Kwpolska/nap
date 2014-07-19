#include <stdio.h>
#include <math.h>
#include "src/nap.h"

int main(int argc, char* argv[]) {
    fprintf(stderr, "nap %s -- test suite\n", VERSION);
    int TESTCOUNT = 0;
    int TESTOUT[TESTCOUNT];
    int SUBTESTCOUNT[] = {11};
    // TEST 0: evaluate seconds
    double expected[11] = {5, 55, 60, 120, 90, 3600, 7200, 1800, 86400, 172800, 43200};
    double actual[11];
    actual[0] = evaluate_seconds(5, '5');
    actual[1] = evaluate_seconds(55, '5');
    actual[2] = evaluate_seconds(1, 'm');
    actual[3] = evaluate_seconds(2, 'm');
    actual[4] = evaluate_seconds(1.5, 'm');
    actual[5] = evaluate_seconds(1, 'h');
    actual[6] = evaluate_seconds(2, 'h');
    actual[7] = evaluate_seconds(0.5, 'h');
    actual[8] = evaluate_seconds(1, 'd');
    actual[9] = evaluate_seconds(2, 'd');
    actual[10] = evaluate_seconds(0.5, 'd');
    TESTOUT[0] = 0;
    for (int i = 0; i < 11; i++) {
        if (actual[i] != expected[i]) {
            TESTOUT[0] += (int)pow(2., i);
        }
    }

    // END OF TESTS
    // summary
    int PASSED = 0;
    int FAILED = 0;
    for (int i = 0; i <= TESTCOUNT; i++) {
        if (TESTOUT[i] == 0) {
            fprintf(stderr, ".");
            PASSED++;
        } else {
            fprintf(stderr, "F");
            FAILED++;
        }
    }
    if (FAILED != 0) fprintf(stderr, "\n---\n");
    for (int i = 0; i <= TESTCOUNT; i++) {
        if (TESTOUT[i] != 0) {
            fprintf(stderr, "Failed test %d\n", i);
            for (int j = 0; j <= SUBTESTCOUNT[i]; j++) {
                if ((TESTOUT[i] & (int)pow(2., j)) != 0) {
                    fprintf(stderr, "    case %d\n", j);
                }
            }
        }
    }
    fprintf(stderr, "\n\n");
    fprintf(stderr, "%d passed, %d failed\n", PASSED, FAILED);
    return 0;
}
