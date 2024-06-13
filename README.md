RSC Sundae
==========

RSC Sundae is a simple, single-threaded RS Classic server emulator
written in C99.  It is based on clean-room reverse engineering and
contains no copyrighted source code.  For content scripting, it
features an implementation of the original RuneScript API in Lua
based on public documents.

It was inspired by the RSC-C client's lack of a native server.
Since RSC-C runs on old and low-power mobile devices, RSC Sundae
should target similar devices.

An initial target for RSC Sundae should be to accurately replicate
RS Classic on the 10th December 2001, since lots of preserved data
exists for this date. This should be accomplished using the
original game's unmodified free data files. By using the same API and
data sources as the original game, the hope is that it becomes
more difficult to implement content inaccurately.

Currently RSC Sundae requires a copy of mudclient110. 204 protocol
support for RSC-C is a TODO item.

RSC Sundae will never support:

* Multiple threads
* Protocol versions beyond 204
* Protocol versions with the "spook cipher"
* Custom private server protocols or data formats
* Integration with proprietary services like Discord

Building
--------

RSC Sundae is developed on NetBSD. I occasionally test it on
Mac OS X 10.4 and Alpine Linux. If I break something on another
UNIX-like platform, please send a patch.

Dependencies:

* BZip2 (for decompressing Jagex Archives to load the original game data)
* Lua 5.4 (for running content scripts)
* libev (for the event loop; optional - mostly for macOS and Windows)

On Debian Linux and derivatives, install the following:

```
build-essential pkgconf libbz2-dev liblua5.4-dev
```

On Alpine Linux, install the following:

```
build-base pkgconf bzip2-dev lua5.4-dev
```

Run `./configure && make -j2` to build the server.

Resources
---------

* https://github.com/2003scape/rsc-c
* https://web.archive.org/web/20011122124112/http://www.jagex.com/manual/index.html
* https://web.archive.org/web/20080907105147/http://gowerarchive.googlepages.com/documents#Runescript
* https://classic.runescape.wiki/w/User:Stormykins/135_Protocol
