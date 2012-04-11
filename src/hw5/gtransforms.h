#ifndef _GTRANSFORMS_H_
#define _GTRANSFORMS_H_

#include "xpmimage.h"
#include "xpmps.h"

#define PI 3.14159265

struct transMatrix
{
  double matrix[3][3];
};

struct transf{
  struct transMatrix tm;
  struct transf *next;
};

struct homoCoord
{
  double coord[3];
};

extern struct transf *
loadTStructure(char *);

extern void
freeTStrcuture(struct transf **);

extern void 
applyTransforms(struct transf *, struct GENode *);

extern void 
initTranslation(struct transMatrix *, double, double);

extern void 
initRotation(struct transMatrix *, double);

extern void
initIdentity(struct transMatrix *);

extern void
applyTranslation(struct transMatrix *, double, double);

extern void
applyRotation(struct transMatrix *, double);

extern void
applyScaling(struct transMatrix *, double, double);

extern void
applyComposedRotation(struct transMatrix *, double, double, double);

extern void
applyComposedScaling(struct transMatrix *, double, double, double, double);

extern void 
initScale(struct transMatrix *, double , double);

extern void 
matrixProduct(struct transMatrix *, struct transMatrix, struct transMatrix);

extern double 
degToRad(double);

extern void 
matrixVectorProduct(struct homoCoord *, struct transMatrix, struct homoCoord);

extern void 
initHomoVector(struct homoCoord *, double , double);

extern void 
twoDCoord(Point *, struct homoCoord);

#endif
