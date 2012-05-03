#include "pointops.h"

int
ptEquals(Point A, Point B)
{
    return (A.x == B.x && A.y == B.y);
}

void
freePointVector(Point **v) {
  if(NULL != (*v))
   free((*v));
}
