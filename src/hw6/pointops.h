#ifndef POINTOPS_H_INCLUDED
#define POINTOPS_H_INCLUDED

#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include "datatypes.h"

int
ptEquals(Point A, Point B);

void
freePointList(struct Polygon **lhead);

void
addPtNodeToList(struct Polygon **lhead, struct Polygon **llastEl, Point pt);

int
isPtInside(Point pt, Region r, int watchedSide);

int
intersectPt(Point ptStart, Point ptEnd, Region r, int watchedSide, Point *ptResult);

void
sortPointsOverX(struct Polygon *ptLst);

#endif // POINTOPS_H_INCLUDED
