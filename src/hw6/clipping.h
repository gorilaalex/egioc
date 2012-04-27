#ifndef CLIPPING_H_INCLUDED
#define CLIPPING_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include "datatypes.h"
#include "pointops.h"
#include "transforms.h"

void
formClippedPolygon(struct Polygon **toBeClipped, struct Polygon **toBeClippedLast, char *fileName);

void
printPoly(const char *preMsg, struct Polygon *lhead);

struct Polygon *
clipPolySH(Region rect, struct Polygon *lhead);

#endif // CLIPPING_H_INCLUDED
