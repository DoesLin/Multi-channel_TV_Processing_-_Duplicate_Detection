OS Support : Ubuntu with package ffmpeg-3.4.2

Commands : 
	To build the project : 					make
	To clear the objects : 					make clean
	To clear the log files : 				make cleanFrame
	To test whether there are memory leak : make valgrindTest
	To use the tests we've provided : 		make test'n' (see Makefile)

Folders : 
	include : header files
	obj : released files
	src : source files
	frame : record the serval frames in the current processing stream
	record : log of response time

Usage : 
	./MyVideo.out *.mp4... 

Attention : 
	Even we can choose what the template video is, but for convenience, we choose "Alone30s.mp4" default. You can customize your template in main.cpp.
	All the functionalities we privided are defined as macro in "header.h". You could try all you want.
