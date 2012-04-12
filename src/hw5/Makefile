# application configuration region
PRJNAME = lineout
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm
OBJS = xpmimage.o xpmps.o gtransforms.o gloader.o gpoly.o

#!!! you shouldn't modify anything beyond this point
all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) main.c -o $(PRJNAME) $(LDFLAGS)
hw4: all
	./$(PRJNAME) -f test2.ps -w 200 -h 200 -t transf.tsf -o out.xpm
hw5: all
	./$(PRJNAME) -f in.pol -w 200 -h 200 -wl 12 -wt 70 -wr 150 -wb 2 -o out.xpm

./%.o: ./%.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) *.xpm $(PRJNAME)
