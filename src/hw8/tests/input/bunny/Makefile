3DVIEW_PATH := ../../../view3D
TESTS_ARGS = -w 200 -h 200

all:
	$(3DVIEW_PATH) $(TESTS_ARGS) -f bunny.obj -viz 0bunny.vw -o 0bunny.xpm
	$(3DVIEW_PATH) $(TESTS_ARGS) -f bunny.obj -viz 0bunny2.vw -o 0bunny2.xpm
	$(3DVIEW_PATH) $(TESTS_ARGS) -f bunny.obj -viz 1bunny.vw -o 1bunny.xpm

clean:
	rm -f *.xpm
