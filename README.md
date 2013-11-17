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
