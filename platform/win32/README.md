RSC Sundae on Win32
===================

Currently only cross-compilation from a Unix-like system with mingw-w64 has
been tested.  Use `prepare.sh` to set up the environment.  Set the `CC`
variable in Makefile to the path of your toolchain.  The script automates
some of the downloads.

Download DLL binaries
---------------------

- Lua: https://luabinaries.sourceforge.net/download.html
- OpenSSL: https://kb.firedaemon.com/support/solutions/articles/4000121705#Download-OpenSSL

Download library sources
------------------------

- Argon2: https://github.com/P-H-C/phc-winner-argon2/archive/refs/tags/20190702.tar.gz
- SQLite: https://sqlite.org/2024/sqlite-amalgamation-3460100.zip
- BZip2: https://sourceware.org/pub/bzip2/bzip2-1.0.8.tar.gz
- libev: http://dist.schmorp.de/libev/libev-4.33.tar.gz
