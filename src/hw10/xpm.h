#ifndef XPM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <float.h>

#include "datatypes.h"

#define BIT0IS1 1
#define BIT1IS1 2
#define BIT2IS1 4
#define BIT3IS1 8

#define COMPLETELYIN 1
#define COMPLETELYOUT 0

/* Initialize a XPM structure with custom
 * width, height, characters/pixel and number of colors
 */
extern XPM *
newXPM(unsigned int width, unsigned int height, unsigned short cpp, unsigned int ncolors);

/* Frees up the memory used by a XPM heap alocated structure */
extern void
freeXPM(XPM **img);

unsigned char **
newColorTable(unsigned char rmin, unsigned char gmin, unsigned char bmin,
              unsigned char rmax, unsigned char gmax, unsigned char bmax, 
              unsigned char clrsInterval);

void 
freeColorTable(unsigned char ***ctbl, unsigned char ctblCnt);

/* Initialize a ZBuffer matrix sutrcture */
extern ZBuffer *
newZBuffer(unsigned int width, unsigned int height);

/* Free a ZBuffer structure */
extern void 
freeZBuffer(ZBuffer **zbuff);

/* Assigns the display coordonates used in the image clipping process */
extern void
assignXPMdisplayRegion(XPM *img, int wLeft, int wTop, int wRight, int wBottom);

/* Inserts a pixel value specified by it's table index at (x, y) with x mod chrperpixel = 0
 * Returns 1 if succesfull and 0 if not
 */
extern int
putXPMpixel(XPM *img, unsigned int x, unsigned int y, unsigned short colorindex);

/* Assigns a color to a color table entry (colta) in RGB format
 * Returns 1 if succesfull and 0 if not
 */
extern int
setXPMColor(XPM *img, unsigned int index, XPMColor pixdata);

/* Saves the XPM image to a disk file located at 'filepath'
 * Returns 1 if succesfull and 0 if not
 */
extern int
saveXPMtofile(XPM *img, char *filepath);

/* Assigns a color matrix of nx3 values to the desired XPM structure */
void
assignXPMColorTable(XPM *img, unsigned char **vColors, int clrCnt);

/* Draws a line onto the XPM image using Bresenham' algorithm */
extern void
drawBresenhamLine(XPM *canvas, PlanePoint pStart, PlanePoint pEnd, unsigned short colorIndex);

extern void
drawSimplePoly(XPM *canvas, struct PlanePolygon *poly, unsigned short colorIndex);

extern void
renderLine(XPM *canvas, PlanePoint ptStart, PlanePoint ptEnd, unsigned short colorIndex);

#endif // XPM_H_INCLUDED
