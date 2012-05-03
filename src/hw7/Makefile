# application configuration region
PRJNAME = bezier
CC = gcc
DIFLAGS =
CFLAGS = -Wall -g $(DIFLAGS)
LDFLAGS = -lm
OBJS = bezier.o transforms.o xpm.o pointops.o

#!!! you shouldn't modify anything beyond this point
all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) main.c -o $(PRJNAME) $(LDFLAGS)

test: all
	./$(PRJNAME) -f in.pol -w 200 -h 200 -r 255 -g 125 -b 0 -o out.xpm


./%.o: ./%.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) *.xpm $(PRJNAME)
