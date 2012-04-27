#include "clipping.h"

void
printPoly(const char *preMsg, struct Polygon *lhead) {
  struct Polygon *nextNode = lhead;

  printf("%s", preMsg);
  do {
    printf("{%d, %d} ", nextNode->P.x, nextNode->P.y);
    nextNode = nextNode->next;
  } while(nextNode != lhead);
  printf("\n");
}

void
formClippedPolygon(struct Polygon **toBeClipped, struct Polygon **toBeClippedLast, char *fileName)
{
  FILE *f = fopen(fileName, "r");
  Point P;

  if(NULL != f) {
    while(fscanf(f, "%d %d", &P.x, &P.y) != EOF)
      addPtNodeToList(toBeClipped, toBeClippedLast, P);

    fclose(f);
  } else {
    fprintf(stderr, "Could not open input file '%s'!\n", fileName);
  }
}

static struct Polygon *
clipPlaneSH(struct Polygon **lhead, Region r, int watchedSide) {
  Point ptStart;
  Point ptEnd;
  Point ptIntersect;
  struct Polygon *lastLPt = NULL;
  struct Polygon *ptel = (*lhead);
  struct Polygon *newPolyList = NULL;

  ptStart = (*lhead)->prev->P;
  do {
    ptEnd = ptel->P;
    printf("cliPlaneSH : ptst {x = %d, y = %d}, pten {x = %d, y = %d} added : ", ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
    if(isPtInside(ptEnd, r, watchedSide)) {
      /* point ptEnd is "inside" */
      if(!isPtInside(ptStart, r, watchedSide)) {
	/* ptEnd is "inside" and ptStart is "outside" */
	if(intersectPt(ptEnd, ptStart, r, watchedSide, &ptIntersect))
        if(!ptEquals(ptEnd, ptIntersect))
            {
                addPtNodeToList(&newPolyList, &lastLPt, ptIntersect);
                printf("pt {x = %d, y = %d}, ", ptIntersect.x, ptIntersect.y);
            }
      }
      addPtNodeToList(&newPolyList, &lastLPt, ptEnd);
      printf("pt {x = %d, y = %d}\n", ptEnd.x, ptEnd.y);
    } else if(isPtInside(ptStart, r, watchedSide))
    {
      /* ptStart is "inside" and ptEnd is "outside" */
      if(intersectPt(ptStart, ptEnd, r, watchedSide, &ptIntersect))
        {addPtNodeToList(&newPolyList, &lastLPt, ptIntersect);
         printf("pt {x = %d, y = %d}\n", ptIntersect.x, ptIntersect.y);
        }
    }
    ptStart = ptEnd;
    ptel = ptel->next;
  } while(ptel != (*lhead));

  /* set return value */
  return newPolyList;
}

struct Polygon *
clipPolySH(Region rect, struct Polygon *lhead) {
  struct Polygon *clipResult = NULL;
  struct Polygon *partialClipResult = NULL;

  printf("*** Clipping poly against the left margin %d\n", rect.windowLeft);
  clipResult = clipPlaneSH(&lhead, rect, LEFT);

  printf("*** Clipping poly against the right margin %d\n", rect.windowRight);
  partialClipResult = clipPlaneSH(&clipResult, rect, RIGHT);
  if(NULL != clipResult) freePointList(&clipResult);
  clipResult = partialClipResult;

  printf("*** Clipping poly against the top margin %d\n", rect.windowTop);
  partialClipResult = clipPlaneSH(&clipResult, rect, TOP);
  if(NULL != clipResult) freePointList(&clipResult);
  clipResult = partialClipResult;

  printf("*** Clipping poly against the bottom margin %d\n", rect.windowBottom);
  partialClipResult = clipPlaneSH(&clipResult, rect, BOTTOM);
  if(NULL != clipResult) freePointList(&clipResult);
  clipResult = partialClipResult;

  return clipResult;
}
