#ifndef _3DOPS_H_
#define _3DOPS_H_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "datatypes.h"
#include "xpm.h"
#include "pointops.h"
#include "transforms.h"

#define VERTEX_OP 'v'
#define DEFINE_TRIANGLE 'f'
#define PT_REALLOC_STEP 5

SpaceObjData
loadOBJFile(const char *);

void
freeOBJFile(SpaceObjData );

SpaceViewSettings
loadVWFile(const char *);

void
project(XPM *, SpaceViewSettings, SpaceObjData, Region, Region);
#endif
