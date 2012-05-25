# application configuration region
PRJNAME = view3D
CC = gcc
DIFLAGS =
CFLAGS = -Wall -g $(DIFLAGS)
LDFLAGS = -lm
OBJS = xpm.o 3dops.o transforms.o pointops.o

TESTS_ARGS = -w 600 -h 600

#!!! you shouldn't modify anything beyond this point
all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) main.c -o $(PRJNAME) $(LDFLAGS)

test: all
	cd tests/input; make


./%.o: ./%.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	cd tests/input; make clean
	rm -f $(OBJS) *.xpm $(PRJNAME)
