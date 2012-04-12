#ifndef _GPOLY_H_
#define _GPOLY_H_

#include <math.h>
#include <float.h>
#include <string.h>

#include "gdatatypes.h"
#include "gloader.h"
#include "xpmimage.h"

struct PointNode *
clipPolySH(DispRegion r, struct PointNode *lhead);

void
clipPolyWA(struct GENode *el);

#endif
