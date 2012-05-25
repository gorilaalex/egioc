#ifndef TRANSFORMS_H_INCLUDED
#define TRANSFORMS_H_INCLUDED

#include <float.h>
#include <math.h>

#include "pointops.h"
#include "datatypes.h"

/* 3D matrix transformations/initialization region */
extern void
initSHTranslation(SpaceHomoMatrix *, double, double, double);

extern void
initSHIdentity(SpaceHomoMatrix *);

extern void
applySHTranslation(SpaceHomoMatrix *, double, double, double);

extern void
applySHScaling(SpaceHomoMatrix *, double, double, double);

extern void
initSHScale(SpaceHomoMatrix *, double , double, double);

extern void
matrixSHProduct(SpaceHomoMatrix *, SpaceHomoMatrix, SpaceHomoMatrix);

extern void
matrixSHVectorProduct(SpaceHomoCoord *, SpaceHomoMatrix, SpaceHomoCoord);

extern void
initSHHomoVector(SpaceHomoCoord *, float , float, float);

extern void
twoDSHCoord(PlanePoint *, SpaceHomoCoord);

/**/

/* 2D homogenous matrix transformations/initialization region */
extern void
initTranslation(SpaceMatrix *, double, double);

extern void
initIdentity(SpaceMatrix *);

extern void
applyTranslation(SpaceMatrix *, double, double);

extern void
applyScaling(SpaceMatrix *, double, double);

extern void
initScale(SpaceMatrix *, double , double);

extern void
matrixProduct(SpaceMatrix *, SpaceMatrix, SpaceMatrix);

extern void
matrixVectorProduct(PlaneHomoCoord *, SpaceMatrix, PlaneHomoCoord);

extern void
initHomoVector(PlaneHomoCoord *, int , int);

extern void
twoDCoord(PlanePoint *, PlaneHomoCoord);

/**/

extern PlanePoint 
pointToViewport(Region, Region, PlanePoint);

#endif // TRANSFORMS_H_INCLUDED
