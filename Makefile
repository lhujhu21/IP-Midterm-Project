# Morgan Wu, mwu69
# Lucy Hu, lhu15
# Midterm Project - Solution Makefile

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

## TODO: Write your own Makefile commands to link and compile your project as well as the cleanup

project: project.o img_processing.o ppm_io.o
	$(CC) $(CFLAGS) -o project project.o img_processing.o ppm_io.o 

project.o: project.c img_processing.h ppm_io.h
	$(CC) $(CFLAGS) -c project.c 

img_processing.o: img_processing.c img_processing.h ppm_io.h
	$(CC) $(CFLAGS) -c img_processing.c

ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

clean:
	rm -f *.o project
