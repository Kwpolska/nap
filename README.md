nap
===

The `sleep` Unix utility, complete with a progressbar.

A work in progress.

Goals
=====

1. write it in C, based on [GNU coreutils][shit] and [FreeBSD][goodness]
2. Have a nice, width-dependent progressbar that disappears when time is up.
3. Update in 0.1, 0.5, 1 or 10-second increments, depending on the amount of
   time requested, like this:

   | Time requested [s] | Update every [s] |
   | ------------------:| ----------------:|
   | 1                  | 0.1              |
   | 2                  | 0.5              |
   | 10                 | 1                |
   | 300 *              | 10               |

\* may be altered depending on terminal size

[shit]: http://git.savannah.gnu.org/gitweb/?p=coreutils.git;a=blob;f=src/sleep.c;h=b9163bc4c43337aac4a74c1b51e0355ad4f89304;hb=HEAD
[goodness]: https://github.com/freebsd/freebsd/blob/master/bin/sleep/sleep.c

License
=======

Copyright © 2013, Chris “Kwpolska” Warrick.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions, and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions, and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. Neither the name of the author of this software nor the names of
   contributors to this software may be used to endorse or promote
   products derived from this software without specific prior written
   consent.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
