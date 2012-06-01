#ifndef POINTOPS_H_INCLUDED
#define POINTOPS_H_INCLUDED

#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include "datatypes.h"

int
ptEquals(PlanePoint A, PlanePoint B);

void
freePointList(struct PlanePolygon **lhead);

void
addPtNodeToList(struct PlanePolygon **lhead, struct PlanePolygon **llastEl, PlanePoint pt);

int
isPtInside(PlanePoint pt, Region r, int watchedSide);

int
intersectPt(PlanePoint ptStart, PlanePoint ptEnd, Region r, int watchedSide, PlanePoint *ptResult);

void
sortPointsOverX(struct PlanePolygon *ptLst);

#endif // POINTOPS_H_INCLUDED
