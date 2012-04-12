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
  b = ptStart.y - ptEnd.x * slope;

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
  struct PointNode *lastLPt = NULL;
  struct PointNode *ptel = (*lhead);
  struct PointNode *newPolyList = NULL;

  ptStart = (*lhead)->prev->pt;
  do {
    ptEnd = ptel->pt;
    printf("ptst {x = %d, y = %d}, pten {x = %d, y = %d}\n", ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
    if(isPtInside(ptEnd, r, watchedSide)) {
      /* point ptEnd is "inside" */
      if(!isPtInside(ptStart, r, watchedSide)) {
	/* ptEnd is "inside" and ptStart is "outside" */
	addPtNodeToList(&newPolyList, &lastLPt, intersectPt(ptEnd, ptStart, r, watchedSide));
      }
      addPtNodeToList(&newPolyList, &lastLPt, ptEnd);
    } else if(isPtInside(ptStart, r, watchedSide)) {
      /* s is "inside" and p is "outside" */
      addPtNodeToList(&newPolyList, &lastLPt, intersectPt(ptStart, ptEnd, r, watchedSide));
    }
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

  clipResult = clipPlaneSH(&lhead, rect, LEFT);
  //partialClipResult = clipPlaneSH(&clipResult, rect, RIGHT);
  //freePointList(&clipResult); clipResult = partialClipResult;
  //clipPlaneSH(&clipResult, &clipResult, rect, TOP);
  //clipPlaneSH(&clipResult, &clipResult, rect, BOTTOM);

  return clipResult;
}

void
clipPolyWA(struct GENode *el) {
}
