3DVIEW_PATH := ../../../view3D
TESTS_ARGS = -w 200 -h 200

all:
	$(3DVIEW_PATH) $(TESTS_ARGS) -f cube.obj -viz 0cube.vw -o 0cube.xpm
	$(3DVIEW_PATH) $(TESTS_ARGS) -f cube.obj -viz 0cube2.vw -o 0cube2.xpm
	$(3DVIEW_PATH) $(TESTS_ARGS) -f cube.obj -viz 1cube.vw -o 1cube.xpm

clean:
	rm -f *.xpm
