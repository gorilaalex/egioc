#include "xpmps.h"

void
renderLine(XPM *canvas, Point ptStart, Point ptEnd) {
  printf("Original Line := start{x:%4d y:%4d}, end{x:%4d y:%4d}\n", ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
  if(cohenSutherlandFrameLine(canvas, &ptStart, &ptEnd) == 1){
    printf("Trimed Line := start{x:%4d y:%4d}, end{x:%4d y:%4d}\n", ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
    drawBresenhamLine(canvas, ptStart, ptEnd, 1);
  }
  else{
    fprintf(stderr, "*** Line was totally outside the canvas.\n");
  }
}

void
renderGElements(XPM *canvas, struct GENode *glist){
  struct GENode *pgl = glist;
  struct PointNode *ptn = NULL;
  struct PointNode *clippedPtList = NULL;
  GElement *pge = NULL;

  while(NULL != pgl){
    pge = &pgl->el;
    switch(pge->type){
    case LINE:
      renderLine(canvas, pge->data.line.st, pge->data.line.en);
      break;
    case POLY:
      clippedPtList = clipPolySH(canvas->displayRegion, pge->data.headPoint);
      if(NULL != clippedPtList) {
	ptn = clippedPtList;
	do {
	  drawBresenhamLine(canvas, ptn->pt, ptn->next->pt, 1);
	  ptn = ptn->next;
	} while(clippedPtList != ptn);
	freePointList(&clippedPtList);
      }
      break;
    default:
      fprintf(stderr, "Unhandled type (%d) !\n", pge->type);
      break;
    }
    
    pgl = pgl->next;
  }
}

procPSLineReturnVal
parsePSLine(const char *line, GElement *dest){
  procPSLineReturnVal returnCode = PSLINE_OK;

  if(sscanf(line, "%d %d %d %d line", &dest->data.line.st.x, &dest->data.line.st.y, &dest->data.line.en.x, &dest->data.line.en.y) == 4){
    dest->type = LINE;
  }

  return returnCode;
}
