3DVIEW_PATH := ../../../view3D
TESTS_ARGS = -w 200 -h 200

all:
	$(3DVIEW_PATH) $(TESTS_ARGS) -f cow.obj -viz 0cow.vw -o 0cow.xpm -rmin 0 -gmin 0 -bmin 0 -rmax 255 -gmax 0 -bmax 0
	$(3DVIEW_PATH) $(TESTS_ARGS) -f cow.obj -viz 0cow2.vw -o 0cow2.xpm -rmin 0 -gmin 0 -bmin 0 -rmax 255 -gmax 0 -bmax 0
	$(3DVIEW_PATH) $(TESTS_ARGS) -f cow.obj -viz 1cow.vw -o 1cow.xpm -rmin 0 -gmin 0 -bmin 0 -rmax 255 -gmax 0 -bmax 0

clean:
	rm -f *.xpm
