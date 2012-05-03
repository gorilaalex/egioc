#ifndef TRANSFORMS_H_INCLUDED
#define TRANSFORMS_H_INCLUDED

#include <float.h>
#include <math.h>

#include "pointops.h"
#include "datatypes.h"

extern int
intersect2D_Segments(struct Segment, struct Segment, Point*, Point*);

extern void
initTranslation(struct transMatrix *, double, double);

extern void
initIdentity(struct transMatrix *);

extern void
applyTranslation(struct transMatrix *, double, double);

extern void
applyScaling(struct transMatrix *, double, double);

extern void
initScale(struct transMatrix *, double , double);

extern void
matrixProduct(struct transMatrix *, struct transMatrix, struct transMatrix);

extern void
matrixVectorProduct(struct homoCoord *, struct transMatrix, struct homoCoord);

extern void
initHomoVector(struct homoCoord *, int , int);

extern void
twoDCoord(Point *, struct homoCoord);

#endif // TRANSFORMS_H_INCLUDED
