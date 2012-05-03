#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "datatypes.h"
#include "pointops.h"
#include "bezier.h"
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
	 "-i <bze_input_file> : BeZiEr point file\n"
	 "-w <output_width>  : desired output width of image\n"
	 "-h <output_height> : desired output height of image\n"
	 "-o <output_xpm>    : XPM desired output file\n"
	 "ALL of the above options are mandatory!\n\n"
	 "Besides mandatory options, there are the optional ones:\n"
	 "-wl <window_left_margin>  : left margin clipping value\n"
	 "-wr <window_right_margin> : right margin clipping value\n"
	 "-wb <window_bottom_margin>: bottom margin clipping value\n"
	 "-wt <window_top_margin>   : top margin clipping value\n"
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
    {0, 0, 0}       /* colorIndex = 1 : BLACK */
  };
  int isClipSet = 0;
  char *xpmOut = NULL;
  int optWidth = 200;
  int optHeight = 200;
  Region window = {200, 0, 0, 200};

  char *fileInput = NULL;
  Point *bzPts = NULL;
  unsigned int bzPtsCnt = 0;
  XPM *img = NULL;
  
  struct option options[] = {
    {"-wt", 1, 1, 0},
    {"-wl", 1, 1, 0},
    {"-wr", 1, 1, 0},
    {"-wb", 1, 1, 0},
    {"-i",  1, 0, 0},
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
	} else if(strcmp(argv[argIndex], "-i") == 0){
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

  printf("Loading, processing and rendering the Bezier curve...\n");
  bzPtsCnt = loadBZEFile(fileInput, &bzPts);
  printf("Outputting data to XPM file...\n");
  saveXPMtofile(img, xpmOut);

  printf("Free memory structures...\n");
  freeXPM(&img);
  return 0;
}
