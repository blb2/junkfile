junkfile
========
A Windows utility for creating junk files.

Motivation
----------
I made this utility for myself as a way to create dummy files of certain sizes or to easily fill up a drive.  With a nearly full drive, I could test I/O conditions surrounding when the drive actually fills.  In Windows, it's roughly equivalent to running the command `fsutil file createnew JUNK N`, where `N` is the number of bytes to allocate for a file called `JUNK`.

The current project page is located here: <https://github.com/bbrice/junkfile>

Building
--------
At the moment, Visual Studio 2013.4 is used for this project.

Usage
-----
This application takes only one argument: file size in MB.

To create a 100 MB file:

	junkfile 100

To fill up the current drive, leaving only 100 MB free:

	junkfile -100

License
-------
junkfile is licensed under the BSD license. See [LICENSE.txt](LICENSE.txt) for more info.
