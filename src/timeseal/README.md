## [zseal](https://github.com/fbergo/zseal)
#### open source client for FICS Timeseal v2 for Linux/Unix systems

:copyright:  2016 Felipe Bergo (Pulga @ FICS), fbergo at gmail_dot_com
https://github.com/fbergo/zseal

Timeseal is a protocol for network lag reporting and compensation used by the FICS chess server (freechess.org). The version 2 of the protocol provides better delay stats as the server actively pings the client periodically. FICS's FTP repository was taken down a few years ago in one of the server host changes or hard disk crashes (I forgot which) and was never put back on. Since then, there has been nowhere to download a trustable timeseal client for Linux systems.

The protocol was deduced from Marcello Mamino's openseal (an implementation of timeseal v1) and the java timeseal v2 implementation in Raptor by cday 
https://github.com/Raptor-Fics-Interface/Raptor

`zseal` is released under the [MIT license](mit.md); you can use this software as you please; the authors are not liable for anything the software does to you or your stuff; derived works must retain the copyright notice.

#### To run from the command line

`$zseal [host [port]]`

- If host is omitted, the default "freechess.org" is used
- If host is given but port is omitted, the default 5000 is used

`$ zseal -v` 
or,
`$ zseal --version` 
prints the version and exit.

See: http://www.bergo.eng.br/eboard/doc.php?d=3 for further information
