#ifndef _XPMPS_H_
#define _XPMPS_H_

#include <math.h>
#include <string.h>

#include "gloader.h"
#include "xpmimage.h"
#include "gdatatypes.h"

#define PS_BEGIN_MARK "%%%BEGIN\n"
#define PS_END_MARK "%%%END\n"

/* Renders a 'GENode *' list to a XPM canvas */
extern void
renderGElements(XPM *canvas, struct GENode *glist);

/* Extracts a GElement from a simple PS text line */
extern procPSLineReturnVal
parsePSLine(const char *line, GElement *dest);

#endif
