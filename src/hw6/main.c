#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "datatypes.h"
#include "pointops.h"
#include "clipping.h"
#include "xpm.h"

struct option{
  char *name;
  int expectedArgCount;
  int isOptional;
  int visitCount;
};

void
printUsage(void){
  printf("Usage: fillpoly <options>\n"
	 "Where <options> is a combination of the following values (order doesn't matter):\n"
	 "-f <pol_input_file> : POLygon point file\n"
	 "-w <output_width>  : desired output width of image\n"
	 "-h <output_height> : desired output height of image\n"
	 "-o <output_xpm>    : XPM desired output file\n"
	 "ALL of the above options are mandatory!\n\n"
	 "Besides mandatory options, there are the optional ones:\n"
	 "-wl <window_left_margin>  : left margin clipping value\n"
	 "-wr <window_right_margin> : right margin clipping value\n"
	 "-wb <window_bottom_margin>: bottom margin clipping value\n"
	 "-wt <window_top_margin>   : top margin clipping value\n"
	 "-vl <viewport_left_margin>  : left margin viewport value\n"
	 "-vr <viewport_right_margin> : right margin viewport value\n"
	 "-vb <viewport_bottom_margin>: bottom margin viewport value\n"
	 "-vt <viewport_top_margin>   : top margin viewport value\n"
         "-r <red_color_part>  : custom red color value\n"
	 "-g <green_color_part>: custom green color value\n"
	 "-b <blue_color_part> : custom blue color value\n"
	 "Please try again ...\n");
}

int main(int argc, char *argv[])
{
  if(argc == 1){
    printUsage();
    exit(1);
  }

  unsigned char clrTable[][3] = {
    {255, 255, 255}, /* colorIndex = 0 : WHITE */
    {0, 0, 0},       /* colorIndex = 1 : BLACK */
    {0, 0, 0}        /* custom fill color */
  };
  int isClipSet = 0;
  int isViewPortSet = 0;
  char *xpmOut = NULL;
  int optWidth = 200;
  int optHeight = 200;
  
  Region window = {200, 0, 0, 200};
  /*window.windowTop = 200;
  window.windowBottom = 0;
  window.windowLeft = 0;
  window.windowRight = 200;*/

  Region viewport = {200, 0, 0, 200};
  /*viewport.windowTop = 100;
  viewport.windowBottom = 10;
  viewport.windowLeft = 10;
  viewport.windowRight = 100;*/

  char *fileInput = NULL;
  struct Polygon *toBeClipped = NULL;
  struct Polygon *toBeClippedLast = NULL;
  struct Polygon *clipResult = NULL;
  struct Polygon *viewportPoygon = NULL;
  XPM *img = NULL;
  
  struct option options[] = {
    {"-wt", 1, 1, 0},
    {"-wl", 1, 1, 0},
    {"-wr", 1, 1, 0},
    {"-wb", 1, 1, 0},
    {"-vt", 1, 1, 0},
    {"-vl", 1, 1, 0},
    {"-vr", 1, 1, 0},
    {"-vb", 1, 1, 0},
    {"-r",  1, 1, 0},
    {"-g",  1, 1, 0},
    {"-b",  1, 1, 0},
    {"-f",  1, 0, 0},
    {"-w",  1, 0, 0},
    {"-h",  1, 0, 0},
    {"-o",  1, 0, 0},
    {0,    0,  0, 0}
  };

  int argIndex = 1;
  int optIndex = 0;
  for(argIndex = 1; argIndex < argc; ++argIndex){
    for(optIndex = 0; options[optIndex].name != NULL; ++optIndex){
      if(strcmp(argv[argIndex], options[optIndex].name) == 0){
	if(strcmp(argv[argIndex], "-wt") == 0){
	  /* set clip top margin */
	  window.windowTop = atoi(argv[argIndex + 1]);
	  isClipSet = 1;
	} else if(strcmp(argv[argIndex], "-wl") == 0){
	  /* set clip left margin */
	  window.windowLeft = atoi(argv[argIndex + 1]);
	  isClipSet = 1;
	} else if(strcmp(argv[argIndex], "-wr") == 0){
	  /* set clip right margin */
	  window.windowRight = atoi(argv[argIndex + 1]);
	  isClipSet = 1;
	} else if(strcmp(argv[argIndex], "-wb") == 0){
          /* set clip bottom margin */
	  window.windowBottom = atoi(argv[argIndex + 1]);
	  isClipSet = 1;
	} else if(strcmp(argv[argIndex], "-vt") == 0){
	  /* set viewport top margin */
	  viewport.windowTop = atoi(argv[argIndex + 1]);
	  isViewPortSet = 1;
	} else if(strcmp(argv[argIndex], "-vl") == 0){
	  /* set viewport left margin */
	  viewport.windowLeft = atoi(argv[argIndex + 1]);
	  isViewPortSet = 1;
	} else if(strcmp(argv[argIndex], "-vr") == 0){
	  /* set viewport right margin */
	  viewport.windowRight = atoi(argv[argIndex + 1]);
	  isViewPortSet = 1;
	} else if(strcmp(argv[argIndex], "-vb") == 0){
          /* set viewport bottom margin */
	  viewport.windowBottom = atoi(argv[argIndex + 1]);
	  isViewPortSet = 1;
	} else if(strcmp(argv[argIndex], "-r") == 0){
	  /* set custom red fill color */
	  clrTable[2][0] = atoi(argv[argIndex + 1]);
	} else if(strcmp(argv[argIndex], "-g") == 0){
	  /* set custom green fill color */
	  clrTable[2][1] = atoi(argv[argIndex + 1]);
	} else if(strcmp(argv[argIndex], "-b") == 0){
          /* set custom blue fill color */
	  clrTable[2][2] = atoi(argv[argIndex + 1]);
	} else if(strcmp(argv[argIndex], "-f") == 0){
	  fileInput = argv[argIndex + 1];
	} else if(strcmp(argv[argIndex], "-w") == 0){
	  optWidth = atoi(argv[argIndex + 1]);
	} else if(strcmp(argv[argIndex], "-h") == 0){
	  optHeight = atoi(argv[argIndex + 1]);
	} else if(strcmp(argv[argIndex], "-o") == 0){
	  xpmOut = argv[argIndex + 1];
	} else {
	  fprintf(stderr, "Option not handled yet!\n");
	}
	argIndex += options[optIndex].expectedArgCount;
	break;
      }
    }
  }

  if(0 == isViewPortSet) {
    viewport.windowTop = optHeight;
    viewport.windowBottom = 0;
    viewport.windowLeft = 0;
    viewport.windowRight = optWidth;
  }

  if(0 == isClipSet) {
    window.windowTop = optHeight;
    window.windowBottom = 0;
    window.windowLeft = 0;
    window.windowRight = optWidth;
  }

  printf("Initializing XPM structure...\n");
  img = newXPM(optWidth, optHeight, 1, sizeof(clrTable)/(sizeof(unsigned char) * 3));
  assignXPMdisplayRegion(img, window.windowLeft, window.windowTop, window.windowRight, window.windowBottom);
  assignXPMColorTable(img, clrTable, sizeof(clrTable)/(sizeof(unsigned char) * 3));

  printf("Clipping and adjusting viewport to client...\n");
  formClippedPolygon(&toBeClipped, &toBeClippedLast, fileInput);
  clipResult = clipPolySH(window, toBeClipped);
  viewportPoygon = polyWindowToViewport(window, viewport, clipResult);
  printf("Filling poly with desired color...\n");
  drawFilledPoly(img, viewportPoygon, 2);
  printf("Outputting data to XPM file...\n");
  saveXPMtofile(img, xpmOut);

  printf("Free memory structures...\n");
  freePointList(&clipResult);
  freePointList(&toBeClipped);
  freePointList(&viewportPoygon);
  freeXPM(&img);
  return 0;
}
