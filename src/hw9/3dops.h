#ifndef _3DOPS_H_
#define _3DOPS_H_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "datatypes.h"
#include "xpm.h"
#include "pointops.h"
#include "transforms.h"

#define VERTEX_OP 'v'
#define DEFINE_TRIANGLE 'f'
#define PT_REALLOC_STEP 5

extern SpaceObjData
loadOBJFile(const char *);

extern void
freeOBJFile(SpaceObjData );

extern SpaceViewSettings
loadVWFile(const char *);

extern void
project(XPM *, SpaceViewSettings, SpaceObjData, Region, Region);

extern void
computeZBuffer(XPM *, ZBuffer *, SpaceViewSettings, SpaceObjData, Region, Region);

extern void 
projectZBuffer(XPM *, ZBuffer *, unsigned short, unsigned short);

#endif
