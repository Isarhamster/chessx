This is the ezsmtp package -- an "easy" method for sending email
using Tcl's built-in sockets.  Intended to cross-platform.  Based
on original work by Keith Vetter (see below), currently 
maintained by D. J. Hagberg <dhagberg@millibits.com>.

Newer versions may be made available at:

    http://www.millibits.com/djh/tcl/

The latest version will be kept as:

    http://www.millibits.com/djh/tcl/ezsmtp-current.tar.gz

This library is designed to work with Tcl 8.0 and newer, though
I do most of my testing with 8.0 and 8.2.

To install, run the install.tcl script with either tclsh or wish.  On
Windows, double-clicking install.tcl should run it with wish.  Once 
installs, it may optionally test itself by emailing me.

The documentation in the file:

    ezsmtp.html

is as close to complete as I can make it.

For the (non-detailed) history of changes to this package, see the file:

    ChangeLog

This release has been in daily use by myself and several other folks
for quite some time now, as part of my company's internal CVS repository
notification system and the Notification Services at 
http://www.lightningstorm.com/ (a near-real-time severe weather notification
system).  I am reasonably confident in its quality for those tasks but I
do acknowledge that there may be some latent bugs in areas that I have not
been stressed (non-ASCII character sets for one).  Email me bug reports
(better yet, patches) and I will try to get to them as quickly as I can.

This version is pretty close to RFC-compliance (821, 822, 1521, 1891),
but I would appreciate any suggestions to improve this.


CONTRIBUTORS:

Keith Vetter <keithv@cs.berkeley.edu>
- Wrote the original version from which all this code is derived.

Laurent Duperval <lduperval@sprint.ca>
- Contributed several solutions and pushed me to release this to
  a wider audience.

and lastly,
D. J. Hagberg <dhagberg@millibits.com>
- Your humble maintainer.

- - -

Please see the file license.txt for licensing terms and conditions.
This package follows the same licensing terms as the core Tcl/Tk
distributions.

