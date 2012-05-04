#ifndef _BEZIER_H_
#define _BEZIER_H_

#include <stdio.h>
#include <stdlib.h>
#include "datatypes.h"
#include "xpm.h"

extern PointVector
loadBZEFile(const char *);

extern void
printPtVector(PointVector);

extern void
freePtVector(PointVector);

extern void
drawBezier(XPM *, Point, Point, Point, Point, double);

extern void
bezierCurve(XPM *, PointVector, double);
#endif
