RSCSundae
=========

RSCSundae is a simple, single-threaded RS Classic server emulator
written in C99.  It is based on clean-room reverse engineering.

It was inspired by the RSC-C client's lack of a native server.
Since RSC-C runs on old and low-power mobile devices, RSCSundae
should target similar devices.

An initial target for RSCSundae should be to accurately replicate
RS Classic on the 19th December 2001, since lots of preserved data
exists for this date. This should be accomplished using the
original game's data files and an implementation of the RuneScript
API in a modern scripting language.

Currently RSCSundae requires a copy of mudclient110. 204 protocol
support for RSC-C is a TODO item.

RSC Sundae will never support:

* Multiple threads
* Protocol versions beyond 204
* Protocol versions with the "spook cipher"
* Custom private server protocols or data formats
* Integration with proprietary services like Discord

Resources:

* https://github.com/2003scape/rsc-c
* https://classic.runescape.wiki/w/User:Stormykins/135_Protocol
* https://web.archive.org/web/20080907105147/http://gowerarchive.googlepages.com/documents#Runescript