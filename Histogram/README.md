The assignment is complete

It has been tested on the CIS 3110 Docker container, compiled with no warnings with the makefile provided and runs with ```./main <file> <file>``` 

The code may have some difficulties when a signal is sent, since it may cause some memory to be still reachable on the signals process, but it will always free all blocks on regular file processes.

This program was tested with the folder .docker and files compose-dev.yaml, wonderland.txt, wonderland2.txt, makefile and main.c. Due to precaution, I have not removed these files from the zip.