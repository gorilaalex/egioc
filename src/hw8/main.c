#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "datatypes.h"
#include "xpm.h"

struct option{
  char *name;
  int expectedArgCount;
  int isOptional;
  int visitCount;
};

void
printUsage(void){
  printf("Usage: view3D <options>\n"
	 "Where <options> is a combination of the following values (order doesn't matter):\n"
	 "-f <obj_input_file> : Wavefront OBJ file\n"
         "-viz <vw_input_file> : 3D view parameters\n"
	 "-w <output_width>  : desired output width of image\n"
	 "-h <output_height> : desired output height of image\n"
	 "-o <output_xpm>    : XPM desired output file\n"
	 "ALL of the above options are mandatory!\n\n"
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

  char *fileOBJInput = NULL;
  char *fileVWInput = NULL;
  XPM *img = NULL;

  struct option options[] = {
    {"-f",  1, 0, 0},
    {"-viz",  1, 0, 0},
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
	if(strcmp(argv[argIndex], "-f") == 0){
	  fileOBJInput = argv[argIndex + 1];
	} else if(strcmp(argv[argIndex], "-viz") == 0){
	  fileVWInput = argv[argIndex + 1];
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

  printf("Loading OBJ and VW data ..., processing and rendering the Bezier curve...\n");
  //bzPts = loadBZEFile(fileInput);
  //bezierCurve(img, bzPts, bzStep);

  printf("Outputting data to XPM file...\n");
  saveXPMtofile(img, xpmOut);

  printf("Free memory structures...\n");
  //freePtVector(bzPts);
  freeXPM(&img);
  return 0;
}
