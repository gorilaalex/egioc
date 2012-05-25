3DVIEW_PATH := ../../../view3D
TESTS_ARGS = -w 200 -h 200

all:
	$(3DVIEW_PATH) $(TESTS_ARGS) -f cow.obj -viz 0cow.vw -o 0cow.xpm
	$(3DVIEW_PATH) $(TESTS_ARGS) -f cow.obj -viz 0cow2.vw -o 0cow2.xpm
	$(3DVIEW_PATH) $(TESTS_ARGS) -f cow.obj -viz 1cow.vw -o 1cow.xpm

clean:
	rm -f *.xpm
