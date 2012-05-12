#ifndef _3DOPS_H_
#define _3DOPS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "datatypes.h"

#define VERTEX_OP 'v'
#define DEFINE_TRIANGLE 'f'

SpaceObjData 
loadOBJFile(const char *fName);

void 
freeOBJFile(SpaceObjData oData);

SpaceViewSettings
loadVWFile(const char *fName);

#endif
