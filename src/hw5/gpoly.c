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

Point intersectPt(Point ptStart, Point ptEnd, DispRegion r, int watchedSide) {
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

int lineSegmentIntersect(Point ptStartSegA, Point ptEndSegA,
			 Point ptStartSegB, Point ptEndSegB,
			 Point *intersectPt) {

  double  distAB, theCos, theSin, newX, ABpos ;

  //  Fail if either line segment is zero-length.
  if ((ptStartSegA.x==ptEndSegA.x && ptStartSegA.y==ptEndSegA.y) || 
      (ptStartSegB.x==ptEndSegB.x && ptStartSegB.y==ptEndSegB.y)) 
    return 0;

  //  Fail if the segments share an end-point.
  if ((ptStartSegA.x==ptStartSegB.x && ptStartSegA.y==ptStartSegB.y) || 
      (ptEndSegA.x==ptStartSegB.x && ptEndSegA.y==ptStartSegB.y) ||
      (ptStartSegA.x==ptEndSegB.x && ptStartSegA.y==ptEndSegB.y) ||
      (ptEndSegA.x==ptEndSegB.x && ptEndSegA.y==ptEndSegB.y))
    return 0;

  //  (1) Translate the system so that point A is on the origin.
  ptEndSegA.x-=ptStartSegA.x; ptEndSegA.y-=ptStartSegA.y;
  ptStartSegB.x-=ptStartSegA.x; ptStartSegB.y-=ptStartSegA.y;
  ptEndSegB.x-=ptStartSegA.x; ptEndSegB.y-=ptStartSegA.y;

  //  Discover the length of segment A-B.
  distAB=sqrt(ptEndSegA.x * ptEndSegA.x + ptEndSegA.y * ptEndSegA.y);

  //  (2) Rotate the system so that point B is on the positive X axis.
  theCos=ptEndSegA.x/distAB;
  theSin=ptEndSegA.y/distAB;
  newX=ptStartSegB.x*theCos+ptStartSegB.y*theSin;
  ptStartSegB.y  =ptStartSegB.y*theCos-ptStartSegB.x*theSin; ptStartSegB.x=newX;
  newX=ptEndSegB.x*theCos+ptEndSegB.y*theSin;
  ptEndSegB.y  =ptEndSegB.y*theCos-ptEndSegB.x*theSin; ptEndSegB.x=newX;

  //  Fail if segment C-D doesn't cross line A-B.
  if ((ptStartSegB.y<0.0 && ptEndSegB.y<0.0) || (ptStartSegB.y>=0.0 && ptEndSegB.y>=0.0)) return 0;

  //  (3) Discover the position of the intersection point along line A-B.
  ABpos=ptEndSegB.x+(ptStartSegB.x-ptEndSegB.x)*ptEndSegB.y/(ptEndSegB.y-ptStartSegB.y);

  //  Fail if segment C-D crosses line A-B outside of segment A-B.
  if (ABpos<0.0 || ABpos>distAB) return 0;

  //  (4) Apply the discovered position to line A-B in the original coordinate system.
  intersectPt->x = ptStartSegA.x+ABpos*theCos;
  intersectPt->y = ptStartSegA.y+ABpos*theSin;

  //  Success.
  return 1; 
}

struct GENode *
clipPolyWA(DispRegion rect, struct PointNode *lhead) {
  struct GENode *processedPolies = NULL;
  struct GENode *newPoly = NULL;
  struct GENode *lastAddedPoly = NULL;
  struct PointNode *newPolyList = NULL;
  struct PointNode *lastPolyPtNode = NULL;
  struct PointNode *clipPoly = NULL;
  struct PointNode *lastNClipPoly = NULL;
  struct PointNode *ptStart = NULL;
  struct PointNode *ptEnd = NULL;
  struct PointNode *ptStartClip = NULL;
  struct PointNode *ptEndClip = NULL;
  struct PointNode *newPtNode = NULL;
  struct PointNode *ptClipIntersectParent = NULL;
  struct PointNode *entryPtList = NULL;
  struct PointNode *lastEntryPtNode = NULL;
  struct PointNode *exitPtList = NULL;
  struct PointNode *lastExitPtNode = NULL;
  struct PointNode *ptStartValidRegion = NULL;

  int entryOpType = 0;
  Point auxpt;
  Point intersectPt;
  Point closestIntersectPt;
  int intersectFound = 0;

  /* build clip poly using DisplayRegion to provide conter-clockwise coordonates */
  printf("Clip created: {%d %d}, {%d %d}, {%d %d}, {%d %d}\n", 
	 rect.windowLeft, rect.windowTop, 
	 rect.windowLeft, rect.windowBottom,
	 rect.windowRight, rect.windowBottom,
	 rect.windowRight, rect.windowTop);
  auxpt.x = rect.windowLeft; auxpt.y = rect.windowTop;
  addPtNodeToList(&clipPoly, &lastNClipPoly, auxpt);
  auxpt.x = rect.windowLeft; auxpt.y = rect.windowBottom;
  addPtNodeToList(&clipPoly, &lastNClipPoly, auxpt);
  auxpt.x = rect.windowRight; auxpt.y = rect.windowBottom;
  addPtNodeToList(&clipPoly, &lastNClipPoly, auxpt);
  auxpt.x = rect.windowRight; auxpt.y = rect.windowTop;
  addPtNodeToList(&clipPoly, &lastNClipPoly, auxpt);

  ptStart = lhead;
  do {
    ptEnd = ptStart->next;
    printf("[POLY] Start {x = %d, y = %d}, End {x = %d, y = %d}\n", ptStart->pt.x, ptStart->pt.y, ptEnd->pt.x, ptEnd->pt.y);

    /* check for intersects with clip poly and pick the closest point to poly start point */
    auxpt = ptStart->pt;
    closestIntersectPt = ptEnd->pt;
    ptStartClip = clipPoly;
    intersectFound = 0;
    do {
      ptEndClip = ptStartClip->next;
      printf("[CLIP] Start {x = %d, y = %d} End {x = %d, y = %d}\n", ptStartClip->pt.x, ptStartClip->pt.y, ptEndClip->pt.x, ptEndClip->pt.y);
      if(lineSegmentIntersect(auxpt, ptEnd->pt, ptStartClip->pt, ptEndClip->pt, &intersectPt) == 1) {
	if(sqrt(pow(auxpt.x - intersectPt.x, 2) + pow(auxpt.y - intersectPt.y, 2)) <
	   sqrt(pow(auxpt.x - closestIntersectPt.x, 2) + pow(auxpt.y - closestIntersectPt.y, 2))) {	  
	  intersectFound = 1;
	  printf("intersect {x = %d, y = %d}, CIntersect {x = %d, y = %d}\n", intersectPt.x, intersectPt.y, closestIntersectPt.x, closestIntersectPt.y);
	  closestIntersectPt = intersectPt;
	  ptClipIntersectParent = ptStartClip;
	}
      }
      ptStartClip = ptEndClip;
    } while(ptStartClip != clipPoly);
    
    if(intersectFound == 1) {
      /* a closer intersect point to current poly start point segment is available */
      
      /* add it to poly list */
      if(findPtInList(lhead, closestIntersectPt) == NULL) {
	newPtNode = (struct PointNode *)malloc(sizeof(struct PointNode));
	newPtNode->pt = closestIntersectPt;
	newPtNode->clip = newPtNode->poly = NULL;
	newPtNode->prev = ptStart;
	newPtNode->next = ptStart->next;
	ptStart->next = newPtNode;
      }
      
      if(findPtInList(clipPoly, closestIntersectPt) == NULL){
	/* add it to clip list */
	newPtNode = (struct PointNode *)malloc(sizeof(struct PointNode));
	newPtNode->pt = closestIntersectPt;
	newPtNode->clip = newPtNode->poly = NULL;
	newPtNode->prev = ptClipIntersectParent;
	newPtNode->next = ptClipIntersectParent->next;
	ptClipIntersectParent->next = newPtNode;
      }
      
      ptStart->next->clip = ptClipIntersectParent->next;
      ptClipIntersectParent->next->poly = ptStart->next;
      
      /* add it to appropiate entry/exit list */
	if(0 == entryOpType) {
	  /* entry */
	  addPtNodeToList(&entryPtList, &lastEntryPtNode, closestIntersectPt);
	  lastEntryPtNode->clip = ptClipIntersectParent->next;
	  lastEntryPtNode->poly = ptStart->next;
	  entryOpType = 1;
	} else {
	  /* exit */
	  addPtNodeToList(&exitPtList, &lastExitPtNode, closestIntersectPt);
	  lastExitPtNode->clip = ptClipIntersectParent->next;
	  lastExitPtNode->poly = ptStart->next;
	  entryOpType = 0;
	}
	
	auxpt = closestIntersectPt;
    }
    printf("auxpt {x = %d, y = %d} intersectFound = %d\n", auxpt.x, auxpt.y, intersectFound);
    
    /* advance to next poly segment */
    ptStart = ptStart->next;//ptEnd;
  } while(ptStart != lhead);

  /* dump the lists */
  printPtList("Final poly point list contains : ", lhead);
  printPtList("Final clip point list contains : ", clipPoly);
  printPtList("Final entry list : ", entryPtList);
  printPtList("Final exit list : ", exitPtList);

  /* reaching this point we are guranteed to have an exit/entry list and an annoted poly/clip */
  if(NULL != entryPtList) {
    printf("\nAssembling polies: \n");
    ptStart = entryPtList->poly;
    ptStartValidRegion = ptStart;
    do {
      /* aquire a valid clip */
      printf("In region : ");
      do {
	ptEnd = ptStart->next;
	printf("{%d %d} ", ptStart->pt.x, ptStart->pt.y);
	addPtNodeToList(&newPolyList, &lastPolyPtNode, ptStart->pt);
 
	if((lastEntryPtNode = findPtInList(entryPtList, ptStart->pt)) != NULL) {
	  lastEntryPtNode->prev->next = lastEntryPtNode->next;
	  lastEntryPtNode->next->prev = lastEntryPtNode->prev;
	  if(lastEntryPtNode->next != lastEntryPtNode) {
	    entryPtList = lastEntryPtNode->next;
	  } else {
	    entryPtList = NULL;
	  }
	  free(lastEntryPtNode);
	}
	ptStart = ptEnd;
      } while(ptStart != entryPtList && ptStart->clip == NULL);
      ptStart = ptStart->clip;
      printf("{%d %d}\n", ptStart->pt.x, ptStart->pt.y);
      addPtNodeToList(&newPolyList, &lastPolyPtNode, ptStart->pt);

      /* get rid of external vertexes */
      printf("Outside region : ");
      do {
	ptEnd = ptStart->next;
	printf("{%d %d} ", ptStart->pt.x, ptStart->pt.y);
	ptStart = ptEnd;
      } while(ptStart != entryPtList && ptStart->poly == NULL);
      printf("{%d %d}\n", ptStart->pt.x, ptStart->pt.y);
      
      if(ptStart->poly != NULL && 
	 ptStart->pt.x == ptStartValidRegion->pt.x &&
	 ptStart->pt.y == ptStartValidRegion->pt.y) {
	/* we have a assembled a complete new valid region */
	/* add it to GENode structure */
	newPoly = (struct GENode *)malloc(sizeof(struct GENode));
	newPoly->el.type = POLY;
	newPoly->el.data.headPoint = newPolyList;
	if(NULL == processedPolies)
	  lastAddedPoly = processedPolies = newPoly;
	else {
	  lastAddedPoly->next = newPoly;
	  lastAddedPoly = newPoly;
	}
	newPolyList = lastPolyPtNode = NULL;
	
	/* advance to new start region */
	if(NULL != entryPtList) {
	  ptStartValidRegion = entryPtList->poly;
	  printf("New start region initialized at {%d %d}\n", ptStartValidRegion->pt.x, ptStartValidRegion->pt.y);
	} else {
	  ptStartValidRegion = NULL;
	  printf("No more points in entry list!\n");
	}
      }
      ptStart = ptStart->poly;
    } while(ptStartValidRegion != NULL);
  } else {
    /* no clipping done, preserve the point list */
    processedPolies = (struct GENode *)malloc(sizeof(struct GENode));
    processedPolies->el.type = POLY;
    processedPolies->el.data.headPoint = lhead;
    processedPolies->next = NULL;
  }
 
  return processedPolies;
}
