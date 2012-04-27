#include "pointops.h"

int
ptEquals(Point A, Point B)
{
    return (A.x == B.x && A.y == B.y);
}

void
freePointList(struct Polygon **lhead) {
  struct Polygon *cNode = (*lhead);
  struct Polygon *nextNode = NULL;

  do{
    nextNode = cNode->next;
    free(cNode);
    cNode = nextNode;
  }while(cNode != (*lhead));
}

void
addPtNodeToList(struct Polygon **lhead, struct Polygon **llastEl, Point pt) {
  struct Polygon *newPtNode = (struct Polygon *)malloc(sizeof(struct Polygon));
  newPtNode->P = pt;
  newPtNode->prev = NULL;
  newPtNode->next = NULL;

  if(NULL == (*lhead)){
    (*lhead) = (*llastEl) = newPtNode;
    (*llastEl)->next = newPtNode;
    (*llastEl)->prev = newPtNode;
  } else {
    newPtNode->prev = (*llastEl);
    newPtNode->next = (*lhead);
    (*lhead)->prev = newPtNode;
    (*llastEl)->next = newPtNode;
    (*llastEl) = newPtNode;
  }
}

int
isPtInside(Point pt, Region r, int watchedSide) {
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

int
intersectPt(Point ptStart, Point ptEnd, Region r, int watchedSide, Point *ptResult)
{
  double slope, b;

  /* find slope and intercept of segment ptStart-ptEnd */
  if(ptEnd.x != ptStart.x)
    slope = (ptEnd.y - ptStart.y) * 1.0/ (ptEnd.x - ptStart.x);
  else
    slope = DBL_MAX;
  b = ptStart.y - ptStart.x * slope;

  if(slope < 1e-7) return 0; // horizontal lines
  switch(watchedSide) {
  case LEFT:
    ptResult->x = r.windowLeft;
    ptResult->y = ptResult->x * slope + b;
    break;
  case RIGHT:
    ptResult->x = r.windowRight;
    ptResult->y = ptResult->x * slope + b;
    break;
  case TOP:
    ptResult->y = r.windowTop;
    if(slope != DBL_MAX)
      ptResult->x = (ptResult->y - b) / slope;
    else
      ptResult->x = ptStart.x;
    break;
  case BOTTOM:
    ptResult->y = r.windowBottom;
    if(slope != DBL_MAX)
      ptResult->x = (ptResult->y - b) / slope;
    else
      ptResult->x = ptEnd.x;
    break;
  }

  return 1;
}

void
sortPointsOverX(struct Polygon *ptLst){
  struct Polygon *pointA = ptLst;
  struct Polygon *pointB = NULL;
  Point aux;

  do {
      pointB = pointA->next;
      do{
        if(pointA->P.x > pointB->P.x) {
            aux = pointA->P;
            pointA->P = pointB->P;
            pointB->P = aux;
        }
        pointB = pointB->next;
      }while(pointB != pointA->next);
      pointA = pointA->next;
  } while(pointA != ptLst);

}
