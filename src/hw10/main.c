#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "datatypes.h"
#include "xpm.h"
#include "3dops.h"

#define CLR_TABLE_COUNT 130

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
         "-rmin <0-255 number> : minimum red channel color\n"
         "-gmin <0-255 number> : minimum green channel color\n"
         "-bmin <0-255 number> : minimum blue channel color\n"
         "-rmax <0-255 number> : maximum red channel color\n"
         "-gmax <0-255 number> : maximum green channel color\n"
         "-bmax <0-255 number> : maximum blue channel color\n"
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

  unsigned char **clrTable;

  char *xpmOut = NULL;
  int optWidth = 200;
  int optHeight = 200;
  Region viewport;
  Region window;

  char *fileOBJInput = NULL;
  char *fileVWInput = NULL;
  unsigned char col_rmin, col_gmin, col_bmin;
  unsigned char col_rmax, col_gmax, col_bmax;
  XPM *img = NULL;
  ZBuffer *izb = NULL;
  SpaceObjData fileInputData;
  SpaceViewSettings vwSett;

  struct option options[] = {
    {"-f",  1, 0, 0},
    {"-viz",  1, 0, 0},
    {"-w",  1, 0, 0},
    {"-h",  1, 0, 0},
    {"-rmin",  1, 0, 0},
    {"-gmin",  1, 0, 0},
    {"-bmin",  1, 0, 0},
    {"-rmax",  1, 0, 0},
    {"-gmax",  1, 0, 0},
    {"-bmax",  1, 0, 0},
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
	} else if(strcmp(argv[argIndex], "-rmax") == 0){
	  col_rmax = atoi(argv[argIndex + 1]);
	} else if(strcmp(argv[argIndex], "-gmax") == 0){
	  col_gmax = atoi(argv[argIndex + 1]);
	} else if(strcmp(argv[argIndex], "-bmax") == 0){
	  col_bmax = atoi(argv[argIndex + 1]);
	} else if(strcmp(argv[argIndex], "-rmin") == 0){
	  col_rmin = atoi(argv[argIndex + 1]);
	} else if(strcmp(argv[argIndex], "-gmin") == 0){
	  col_gmin = atoi(argv[argIndex + 1]);
	} else if(strcmp(argv[argIndex], "-bmin") == 0){
	  col_bmin = atoi(argv[argIndex + 1]);
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

  viewport.windowTop = optHeight;
  viewport.windowBottom = 0;
  viewport.windowLeft = 0;
  viewport.windowRight = optWidth;
  
  DBG("Initializing XPM and the ZBuffer structure...\n");
  clrTable = newColorTable(col_rmin, col_gmin, col_bmin, col_rmax, col_gmax, col_bmax, CLR_TABLE_COUNT);
  img = newXPM(optWidth, optHeight, 1, CLR_TABLE_COUNT);
  izb = newZBuffer(optWidth, optHeight);
  assignXPMdisplayRegion(img, viewport.windowLeft, viewport.windowTop, viewport.windowRight,   viewport.windowBottom);
  assignXPMColorTable(img, clrTable, CLR_TABLE_COUNT);

  DBG("Loading OBJ and VW data ...\n");
  fileInputData = loadOBJFile(fileOBJInput);
  vwSett = loadVWFile(fileVWInput);
  window.windowTop = vwSett.UVMAXy;
  window.windowBottom = vwSett.UVMINy;
  window.windowLeft = vwSett.UVMINx;
  window.windowRight = vwSett.UVMAXx;
  
  DBG("Computing the ZBuffer and projecting the image ...\n");
  computeZBuffer(img, izb, vwSett, fileInputData, window, viewport);
  projectZBuffer(img, izb, 1, CLR_TABLE_COUNT - 1);

  DBG("Outputting data to XPM file...\n");
  saveXPMtofile(img, xpmOut);

  DBG("Free memory structures...\n");
  freeOBJFile(fileInputData);
  freeXPM(&img);
  freeColorTable(&clrTable, CLR_TABLE_COUNT);
  freeZBuffer(&izb);

  return 0;
}
