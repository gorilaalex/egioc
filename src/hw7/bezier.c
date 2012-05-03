#include "bezier.h"

#define ALLOC_INCREMENT 5

PointVector
loadBZEFile(const char *fName) {
  char dataType[20];
  PointVector ptVector = {NULL, 0};
  Point newPt = {0, 0};
  FILE *fHandle = fopen(fName, "r");

  if(NULL != fHandle) {
    ptVector.v = (Point *)calloc(ALLOC_INCREMENT, sizeof(Point));
    while(fscanf(fHandle, "%d %d %s", &newPt.x, &newPt.y, dataType) == 3) {
      ptVector.v[ptVector.length++] = newPt;
      if((ptVector.length % ALLOC_INCREMENT) == 0) {
        ptVector.v = (Point *)realloc(ptVector.v, (ptVector.length + ALLOC_INCREMENT) * sizeof(Point));
      }
    }
    fclose(fHandle);
  }

  return ptVector;
}

void 
printPtVector(PointVector ptVector) {
  unsigned int id = 0;

  for(id = 0; id < ptVector.length; ++id) {
    printf("%d %d\n", ptVector.v[id].x, ptVector.v[id].y);
  }
}

void
freePtVector(PointVector ptVector) {
  if(NULL != ptVector.v)
   free(ptVector.v);
}
