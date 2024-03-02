# junkfile
A Windows utility for creating junk files.

The current project page is located here: <https://github.com/blb2/junkfile>


## Motivation
I made this utility for myself as a way to create dummy files of certain sizes
or to easily fill up a drive. With a nearly full drive, I could test I/O
conditions surrounding when the drive actually fills. In Windows, it's roughly
equivalent to running the command `fsutil file createnew JUNK N`, where `N` is
the number of bytes to allocate for a file called `JUNK`.


## Building
At the moment, Visual Studio 2022 is used for this project.


## Usage
This application takes only one argument: file size and unit.

To create a 100 MB file:

    junkfile 100M

To fill up the current drive, leaving only 100 MB free:

    junkfile -100M

Accepted units (not case sensitive):

    K, M, G, or T


## License
`junkfile` is licensed under version 3 of the GPL or later. See [LICENSE.txt][1] for more info.


[1]: LICENSE.txt
