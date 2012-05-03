#ifndef XPM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "datatypes.h"
#include "pointops.h"
#include "transforms.h"

/* Initialize a XPM structure with custom
 * width, height, characters/pixel and number of colors
 */
extern XPM *
newXPM(unsigned int width, unsigned int height, unsigned short cpp, unsigned int ncolors);

/* Frees up the memory used by a XPM heap alocated structure */
extern void
freeXPM(XPM **img);

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
assignXPMColorTable(XPM *img, unsigned char vColors[][3], int clrCnt);

/* Draws a line onto the XPM image using Bresenham' algorithm */
extern void
drawBresenhamLine(XPM *canvas, Point pStart, Point pEnd, unsigned short colorIndex);

void
drawSimplePoly(XPM *canvas, struct Polygon *poly, unsigned short colorIndex);

#endif // XPM_H_INCLUDED
