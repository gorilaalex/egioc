3DVIEW_PATH := ../../../view3D
TESTS_ARGS = -w 200 -h 200

all:
	$(3DVIEW_PATH) $(TESTS_ARGS) -f sphere.obj -viz 0sphere.vw -o 0sphere.xpm
	$(3DVIEW_PATH) $(TESTS_ARGS) -f sphere.obj -viz 1sphere.vw -o 1sphere.xpm

clean:
	rm -f *.xpm
