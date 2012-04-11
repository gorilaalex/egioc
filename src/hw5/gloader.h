#ifndef _GLOADER_H_
#define _GLOADER_H_

#include <stdio.h>
#include <string.h>

#include "gdatatypes.h"
#include "xpmps.h"

void
loadFromFile(struct GENode **ellist, const char *file);

/* Loads and parses a list of 'struct GENode *' from a simple PS file */
void
loadPS(struct GENode **ellist, const char *file);

void
loadPOL(struct GENode **ellist, const char *file);

/* Frees the previously loaded 'struct GENode *' */
void
freeGList(struct GENode **glist);

#endif
