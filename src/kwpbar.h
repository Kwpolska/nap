#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/ioctl.h>
#include <unistd.h>

/*
 * KwPBar for C, v0.2.0
 * Copyright © 2013-2021, Chris Warrick.
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

int get_termlength() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

int pbar(double value, double max) {
    int fullwidth = get_termlength();
    int pbarwidth = fullwidth - 9;
    double progress;
    if (max == 0) {
        fprintf(stderr, "ERROR: invalid progressbar maximum (0)\n");
        return 1;
    } else {
        progress = value / max;
    }

    if (progress < 0 || progress > 1) {
        fprintf(stderr, "ERROR: invalid progressbar value (not in range [0, 1])\n");
        return 2;
    }

    // calculate percentage
    double perc = progress * 100;
    char percs[10];
    sprintf(percs, " %4.1f", perc);

    // calculate things to display
    int now = round(progress * pbarwidth);

    char bar[fullwidth];
    memset(bar, '\0', sizeof(bar));
    memset(bar, ' ', pbarwidth);
    if (progress == 1) {
        memset(bar, '=', now);
    } else if (progress != 0) {
        memset(bar, '=', now - 1);
        bar[now - 1] = '>';
    }

    fprintf(stderr, "\r[%s]%s%%", bar, percs);
    return 0;
}

void erase_pbar() {
    int fullwidth = get_termlength();
    char bar[fullwidth];
    memset(bar, '\0', sizeof(bar));
    memset(bar, ' ', fullwidth);
    fprintf(stderr, "\r%s\r", bar);
}
