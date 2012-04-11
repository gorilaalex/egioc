#include "xpmps.h"

void
renderGElements(XPM *canvas, struct GENode *glist){
  struct GENode *pgl = glist;
  GElement *pge = NULL;

  while(NULL != pgl){
    pge = &pgl->el;
    switch(pge->type){
    case LINE:
      printf("Original Line := start{x:%4d y:%4d}, end{x:%4d y:%4d}\n", pge->data.line.st.x, pge->data.line.st.y, pge->data.line.en.x, pge->data.line.en.y);
      if(cohenSutherlandFrameLine(canvas, &pge->data.line.st, &pge->data.line.en) == 1){
	printf("Trimed Line := start{x:%4d y:%4d}, end{x:%4d y:%4d}\n", pge->data.line.st.x, pge->data.line.st.y, pge->data.line.en.x, pge->data.line.en.y);
	drawBresenhamLine(canvas, pge->data.line.st, pge->data.line.en, 1);
      }
      else{
	fprintf(stderr, "*** Line was totally outside the canvas.\n");
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
