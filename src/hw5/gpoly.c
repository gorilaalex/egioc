#include "gpoly.h"

enum { TOP = 0x1, BOTTOM = 0x2, RIGHT = 0x4, LEFT = 0x8 };

int isPtInside(Point pt, DispRegion r, int watchedSide) {
  switch(watchedSide) {
  case LEFT:
    return pt.x >= r.windowLeft;
  case RIGHT:
    return pt.x <= r.windowRight;
  case TOP:
    return pt.y <= r.windowTop;
  case BOTTOM:
    return pt.y >= r.windowBottom;
  default:
    break;
  }

  return 0;
}

Point intersectPt( Point ptStart, Point ptEnd, DispRegion r, int watchedSide )  {
  Point ptResult;
  double slope, b;
  
  /* find slope and intercept of segment ptStart-ptEnd */
  if(ptEnd.x != ptStart.x)
    slope = (ptEnd.y - ptStart.y) / (ptEnd.x - ptStart.x);
  else
    slope = DBL_MAX;
  b = ptStart.y - ptStart.x * slope;

  switch(watchedSide) {
  case LEFT:
    ptResult.x = r.windowLeft;
    ptResult.y = ptResult.x * slope + b;
    break;
  case RIGHT:
    ptResult.x = r.windowRight;
    ptResult.y = ptResult.x * slope + b;
    break;
  case TOP:
    ptResult.y = r.windowTop;
    if(slope != DBL_MAX)
      ptResult.x = (ptResult.y - b) / slope;
    else
      ptResult.x = ptStart.x;
    break;
  case BOTTOM:
    ptResult.y = r.windowBottom;
    if(slope != DBL_MAX)
      ptResult.x = (ptResult.y - b) / slope;
    else
      ptResult.x = ptEnd.x;
    break;
  }

  return ptResult;
}

struct PointNode *
clipPlaneSH(struct PointNode **lhead, DispRegion r, int watchedSide) {
  Point ptStart;
  Point ptEnd;
  Point ptIntersect;
  struct PointNode *lastLPt = NULL;
  struct PointNode *ptel = (*lhead);
  struct PointNode *newPolyList = NULL;
  
  ptStart = (*lhead)->prev->pt;
  do {
    ptEnd = ptel->pt;
    printf("clipPlaneSH : ptst {x = %d, y = %d}, pten {x = %d, y = %d} added : ", ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
    if(isPtInside(ptEnd, r, watchedSide)) {
      /* point ptEnd is "inside" */
      if(!isPtInside(ptStart, r, watchedSide)) {
	/* ptEnd is "inside" and ptStart is "outside" */
	ptIntersect = intersectPt(ptEnd, ptStart, r, watchedSide);
	addPtNodeToList(&newPolyList, &lastLPt, ptIntersect);
	printf("pt {x = %d, y = %d}, ", ptIntersect.x, ptIntersect.y);
      }
      addPtNodeToList(&newPolyList, &lastLPt, ptEnd);
      printf("pt {x = %d, y = %d}\n", ptEnd.x, ptEnd.y);
    } else if(isPtInside(ptStart, r, watchedSide)) {
      /* ptStart is "inside" and ptEnd is "outside" */
      ptIntersect = intersectPt(ptStart, ptEnd, r, watchedSide);
      addPtNodeToList(&newPolyList, &lastLPt, ptIntersect);
      printf("pt {x = %d, y = %d}\n", ptIntersect.x, ptIntersect.y);
    }
    else
      printf("NO POINTS\n");
    ptStart = ptEnd;
    ptel = ptel->next;
  } while(ptel != (*lhead));

  /* set return value */
  return newPolyList;
}


struct PointNode *
clipPolySH(DispRegion rect, struct PointNode *lhead) {
  struct PointNode *clipResult = NULL;
  struct PointNode *partialClipResult = NULL;

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

void
clipPolyWA(struct GENode *el) {
}
