# application configuration region
PRJNAME = bezier
CC = gcc
DIFLAGS =
CFLAGS = -Wall -g $(DIFLAGS)
LDFLAGS = -lm
OBJS = bezier.o transforms.o xpm.o

#!!! you shouldn't modify anything beyond this point
all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) main.c -o $(PRJNAME) $(LDFLAGS)

test: all
	./$(PRJNAME) -i in.bze -w 200 -h 200 -o out.xpm -p 0.2


./%.o: ./%.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) *.xpm $(PRJNAME)
