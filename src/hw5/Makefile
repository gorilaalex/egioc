# application configuration region
PRJNAME = lineout
CC = gcc
DIFLAGS =
CFLAGS = -Wall -g $(DIFLAGS)
LDFLAGS = -lm
OBJS = xpmimage.o xpmps.o gtransforms.o gloader.o gpoly.o

#!!! you shouldn't modify anything beyond this point
all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) main.c -o $(PRJNAME) $(LDFLAGS)
hw4: all
	./$(PRJNAME) -f test2.ps -w 200 -h 200 -t transf.tsf -o out.xpm

hw5_sh: DIFLAGS = -DPOLY_SH_CLIP
hw5_sh: all

hw5_wa: DIFLAGS = -DPOLY_WA_CLIP
hw5_wa: all

run_hw5_sh: all
	./$(PRJNAME) -f insh.pol -w 200 -h 200 -wl 12 -wt 70 -wr 150 -wb 2 -o out.xpm
run_hw5_wa: all
	./$(PRJNAME) -f inwa.pol -w 200 -h 200 -wl 20 -wr 180 -wt 150 -wb 50 -o out.xpm

./%.o: ./%.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) *.xpm $(PRJNAME)
