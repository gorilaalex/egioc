#ifndef _BEZIER_H_
#define _BEZIER_H_

#include <stdio.h>
#include <stdlib.h>
#include "datatypes.h"

extern PointVector
loadBZEFile(const char *);

extern void 
printPtVector(PointVector);

extern void
freePtVector(PointVector);

#endif
