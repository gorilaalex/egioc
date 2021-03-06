#include "xpm.h"

XPM *
newXPM(unsigned int width, unsigned int height, unsigned short cpp, unsigned int ncolors){
  XPM *toRet = NULL;

  toRet = (XPM *)malloc(sizeof(XPM));
  toRet->width = width;
  toRet->height = height;
  toRet->displayRegion.windowLeft = 0;
  toRet->displayRegion.windowBottom = 0;
  toRet->displayRegion.windowTop = height;
  toRet->displayRegion.windowRight = width;
  toRet->chrperpixel = cpp;
  toRet->ncolors = ncolors;
  toRet->colta = (XPMColor *)calloc(ncolors, sizeof(XPMColor));
  toRet->data = (unsigned short **)malloc(height * sizeof(unsigned short *));

  int cnt;
  for(cnt = 0; cnt < height; ++cnt){
    toRet->data[cnt] = (unsigned short *)calloc(width, sizeof(unsigned short));
  }

  return toRet;
}

void
freeXPM(XPM **img){
  int cnt;

  for(cnt = 0; cnt < (*img)->height; ++cnt){
    free((*img)->data[cnt]);
  }
  free((*img)->data);

  for(cnt = 0; cnt < (*img)->ncolors; ++cnt){
    free((*img)->colta[cnt].key);
    free((*img)->colta[cnt].chars);
  }
  free((*img)->colta);

  free((*img));
}

unsigned char **
newColorTable(unsigned char rmin, unsigned char gmin, unsigned char bmin,
              unsigned char rmax, unsigned char gmax, unsigned char bmax, 
              unsigned char clrsInterval) {
  assert(clrsInterval < 255);
  assert(rmin <= rmax);
  assert(gmin <= gmax);
  assert(bmin <= bmax);
  unsigned char **toRet = NULL;
  unsigned char id;

  toRet = (unsigned char **)malloc(sizeof(unsigned char *) * (clrsInterval + 2));
  toRet[0] = (unsigned char *)malloc(sizeof(unsigned char) * 3);
  toRet[0][0] = toRet[0][1] = toRet[0][2] = 255; /* set background color to white */
  for(id = 1; id <= clrsInterval - 1; ++id) {
    toRet[id] = (unsigned char *)malloc(sizeof(unsigned char) * 3);
    toRet[id][0] = rmin + id * (rmax - rmin)/clrsInterval; /* red */
    toRet[id][1] = gmin + id * (gmax - gmin)/clrsInterval; /* green */
    toRet[id][2] = bmin + id * (bmax - bmin)/clrsInterval; /* blue */
  }
  toRet[clrsInterval + 1] = (unsigned char *)malloc(sizeof(unsigned char) * 3);
  toRet[clrsInterval + 1][0] = toRet[clrsInterval + 1][1] = toRet[clrsInterval + 1][2] = 0; /* last color in table is black */

  return toRet;
}

void 
freeColorTable(unsigned char ***ctbl, unsigned char ctblCnt) {
  unsigned char id;

  for(id = 0; id < ctblCnt + 2; ++id) {
    free((*ctbl)[id]);
  }
  free(*ctbl);
}

ZBuffer *
newZBuffer(unsigned int width, unsigned int height) {
  ZBuffer *zb = NULL;

  zb = (ZBuffer *)malloc(sizeof(ZBuffer));
  zb->height = height;
  zb->width = width;
  zb->minz = DBL_MAX;
  zb->maxz = DBL_MIN;
  zb->data = (int **)malloc(zb->height * sizeof(int *));

  int rowId;
  int colId;
  for(rowId = 0; rowId < zb->height; ++rowId){
    zb->data[rowId] = (int *)calloc(zb->width, sizeof(int));
    for(colId = 0; colId < zb->width; ++colId){
      zb->data[rowId][colId] = INT_MIN;
    }
  }

  return zb;
}

void 
freeZBuffer(ZBuffer **zbuff) {
  int cnt;
  for(cnt = 0; cnt < (*zbuff)->height; ++cnt){
    free((*zbuff)->data[cnt]);
  }
  free((*zbuff)->data);
  free((*zbuff));
}

void
assignXPMdisplayRegion(XPM *img, int wLeft, int wTop, int wRight, int wBottom){
  img->displayRegion.windowLeft = wLeft;
  img->displayRegion.windowBottom = wBottom;
  img->displayRegion.windowTop = wTop;
  img->displayRegion.windowRight = wRight;
}

int
putXPMpixel(XPM *img, unsigned int x, unsigned int y, unsigned short colorindex){
  int fctStatus = 1;

  if(colorindex > img->ncolors ||
     x > img->width ||
     y > img->height)
    fctStatus = 0;
  else{
    img->data[y][x] = colorindex;
  }

  return fctStatus;
}

int
setXPMColor(XPM *img, unsigned int index, XPMColor pixdata){
  int fctStatus = 1;

  if(index > img->ncolors)	fctStatus = 0;
  else{
    if(img->colta[index].chars != NULL)	free(img->colta[index].chars);
    if(img->colta[index].key != NULL)	free(img->colta[index].key);

    img->colta[index] = pixdata;
  }

  return fctStatus;
}

int
saveXPMtofile(XPM *img, char *filepath){
  FILE *fXPM = fopen(filepath, "w");
  int fctStatus = 1;

  if(fXPM == NULL)	fctStatus = 0;
  else{
    /* write XPM header */
    fprintf(fXPM, "/* XPM */\n"
	    "static char *egc[] = {\n");

    /* dump image properties */
    fprintf(fXPM, "\" %d %d %d %d \",\n", img->width, img->height, img->ncolors, img->chrperpixel);

    /* dump color table */
    int id = 0;
    for(id = 0; id < img->ncolors; ++id){
      fprintf(fXPM, "\"%s %s #%02X%02X%02X\",\n", img->colta[id].chars,
	      img->colta[id].key,
	      img->colta[id].clr.red,
	      img->colta[id].clr.green,
	      img->colta[id].clr.blue);
    }

    /* dump image pixels */
    int xpix = 0;
    int ypix = 0;
    for(ypix = 0; ypix < img->height; ++ypix){
      fprintf(fXPM,"\"");
      for(xpix = 0; xpix < img->width; ++xpix){
	fprintf(fXPM, "%s", img->colta[img->data[ypix][xpix]].chars);
      }
      fprintf(fXPM,"\"%c\n", (ypix == img->height - 1 ? ' ' : ','));
    }

    fprintf(fXPM, "};\n");
    fclose(fXPM);
  }

  return fctStatus;
}

void
assignXPMColorTable(XPM *img, unsigned char **vColors, int clrCnt){
  XPMColor clrData;
  int clrIndex = 0;
  char charsFormat[10];

  if(clrCnt > img->ncolors ||
     clrCnt > pow(2, img->chrperpixel * 8) - 1) return;

  for(clrIndex = 1; clrIndex <= clrCnt; clrIndex ++){
    /* load RGB values */
    clrData.clr.red = vColors[clrIndex - 1][0];
    clrData.clr.green = vColors[clrIndex - 1][1];
    clrData.clr.blue = vColors[clrIndex - 1][2];

    /* load dynamic data */
    clrData.key = (char *)malloc(2 * sizeof(char));
    strcpy(clrData.key, "c");
    clrData.chars = (char *)malloc((1 + img->chrperpixel) * sizeof(char));

    /* load formated pixel chars */
    switch(img->chrperpixel){
    case 1:
      if(clrCnt <= 0x0F) sprintf(clrData.chars, "%1X", clrIndex);
      else sprintf(clrData.chars, "%c", clrIndex);
      break;
    default:
      sprintf(charsFormat, "%%0%dX", img->chrperpixel);
      sprintf(clrData.chars, charsFormat, clrIndex);
      break;
    }

    setXPMColor(img, clrIndex - 1, clrData);
  }
}

static void
bresenhamX (XPM *canvas, PlanePoint q, PlanePoint r, int symmetry, unsigned short clrIndex)
{
    int dx, dy, D, x, y;
    dx = r.x - q.x;
    if (symmetry == ASYMMETRIC)
        dy = r.y - q.y;
    else dy = q.y - r.y;
    D = 2*dy - dx;
    y = q.y;
    for (x = q.x; x <= r.x; x++)
    {
      putXPMpixel(canvas, x, canvas->height - y - 1, clrIndex);
        if (D <= 0) D += 2 * dy;
        else
        {
            D += 2*(dy - dx);
            if(symmetry == ASYMMETRIC)
                y++;
            else y--;
        }
    }
}

static void
bresenhamY (XPM *canvas, PlanePoint q, PlanePoint r, int symmetry, unsigned short clrIndex)
{
    int dx, dy, D, x, y;
    if(symmetry == ASYMMETRIC)
        dx = r.x - q.x;
    else dx = q.x - r.x;
    dy = r.y - q.y;
    D = 2*dx - dy;
    x = q.x;
    for (y = q.y; y <= r.y; y++)
    {
        putXPMpixel(canvas, x, canvas->height - y - 1, clrIndex);
        if (D <= 0) D += 2 * dx;
        else
        {
            D += 2*(dx - dy);
            if(symmetry == ASYMMETRIC)
                x++;
            else x--;
        }
    }
}

void
drawBresenhamLine(XPM *canvas, PlanePoint pStart, PlanePoint pEnd, unsigned short colorIndex)
{
    if(pEnd.x > pStart.x &&
       pEnd.y >= pStart.y &&
       (pEnd.x - pStart.x) >= (pEnd.y - pStart.y))
        //octant 1
    {
        bresenhamX(canvas,pStart,pEnd,ASYMMETRIC,colorIndex);
    }
    else if(pEnd.x >= pStart.x &&
	    pEnd.y > pStart.y &&
	    (pEnd.x - pStart.x) < (pEnd.y - pStart.y))
        //octant 2
    {
        bresenhamY(canvas,pStart,pEnd,ASYMMETRIC,colorIndex);
    }
    else if(pEnd.x < pStart.x &&
	    pEnd.y > pStart.y &&
	    (pStart.x - pEnd.x) < (pEnd.y - pStart.y))
        //octant 3
    {
        bresenhamY(canvas,pStart,pEnd,SYMMETRIC,colorIndex);
    }
    else if(pEnd.x < pStart.x &&
	    pEnd.y >= pStart.y &&
	    (pStart.x - pEnd.x) >= (pEnd.y - pStart.y))
        //octant 4
    {
        bresenhamX(canvas,pEnd,pStart,SYMMETRIC,colorIndex);
    }
    else if (pEnd.x < pStart.x &&
	     pEnd.y < pStart.y &&
	     (pStart.x - pEnd.x) >= (pStart.y - pEnd.y))
        //octant 5
    {
        bresenhamX(canvas,pEnd,pStart,ASYMMETRIC,colorIndex);
    }
    else if (pEnd.x <= pStart.x &&
	     pEnd.y < pStart.y &&
	     (pStart.x - pEnd.x) < (pStart.y - pEnd.y))
        //octant 6
    {
        bresenhamY(canvas,pEnd,pStart,ASYMMETRIC,colorIndex);
    }
    else if(pEnd.x > pStart.x &&
	    pEnd.y < pStart.y &&
	    (pEnd.x - pStart.x) <= (pStart.y - pEnd.y))
        //octant 7
    {
        bresenhamY(canvas,pEnd,pStart,SYMMETRIC,colorIndex);
    }
    else //octant 8
    {
        bresenhamX(canvas,pStart,pEnd,SYMMETRIC,colorIndex);
    }
}

void
drawSimplePoly(XPM *canvas, struct PlanePolygon *poly, unsigned short colorIndex) {
  struct PlanePolygon *ptn = NULL;

  if(NULL != poly) {
	ptn = poly;
  do {
    renderLine(canvas, ptn->P, ptn->next->P, colorIndex);
    ptn = ptn->next;
  } while(poly != ptn);
  }
}

static unsigned char
getRegionCode(XPM *canvas, PlanePoint *pt)
{
    char code = 0;
    
    if(pt->y > canvas->displayRegion.windowTop) code |= BIT3IS1;
    if(pt->y < canvas->displayRegion.windowBottom) code |= BIT2IS1;
    if(pt->x > canvas->displayRegion.windowRight) code |= BIT1IS1;
    if(pt->x < canvas->displayRegion.windowLeft) code |= BIT0IS1;
    
    return code;
}

static void
trimLineToRegion(XPM *canvas, PlanePoint *start, PlanePoint *end, unsigned char codeStart, unsigned char codeEnd)
{
    int code, newX, newY;

    if(codeStart == 0) code = codeEnd;
    else code = codeStart;

    if(code & BIT0IS1)
      {
	newY = (*start).y + ((*end).y - (*start).y) * (canvas->displayRegion.windowLeft - (*start).x) / ((*end).x - (*start).x);
	newX = canvas->displayRegion.windowLeft;
      }
    else if(code & BIT1IS1)
      {
	newY = (*start).y + ((*end).y - (*start).y) * (canvas->displayRegion.windowRight - (*start).x) / ((*end).x - (*start).x);
	newX = canvas->displayRegion.windowRight;
      }
    else if(code & BIT2IS1)
      {
	newX = (*start).x + ((*end).x - (*start).x) * (canvas->displayRegion.windowBottom - (*start).y) / ((*end).y - (*start).y);
	newY = canvas->displayRegion.windowBottom;
      }
    else if(code & BIT3IS1)
      {
        newX = (*start).x + ((*end).x - (*start).x) * (canvas->displayRegion.windowTop - (*start).y) / ((*end).y - (*start).y);
        newY = canvas->displayRegion.windowTop;
      }
    if (code == codeStart)
      {
        (*start).x = newX;
        (*start).y = newY;
      }
    else
      {
        (*end).x = newX;
        (*end).y = newY;
      }
}


static int 
cohenSutherlandFrameLine(XPM *canvas, PlanePoint *pStart, PlanePoint *pEnd)
{
  unsigned char codeStart = getRegionCode(canvas, pStart);
  unsigned char codeEnd = getRegionCode(canvas,pEnd);

    if((codeStart | codeEnd) == 0) return COMPLETELYIN;
    if((codeStart & codeEnd) != 0) return COMPLETELYOUT;
    trimLineToRegion(canvas, pStart, pEnd, codeStart, codeEnd);

    return cohenSutherlandFrameLine(canvas, pStart, pEnd);
}

void
renderLine(XPM *canvas, PlanePoint ptStart, PlanePoint ptEnd, unsigned short colorIndex) {
  DBG("Original Line := start{x:%4f y:%4f}, end{x:%4f y:%4f}\n", ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
  if(cohenSutherlandFrameLine(canvas, &ptStart, &ptEnd) == 1){
    DBG("Trimed Line := start{x:%4f y:%4f}, end{x:%4f y:%4f}\n", ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
    drawBresenhamLine(canvas, ptStart, ptEnd, colorIndex);
  }
  else{
  	DBG("*** Line was totally outside the canvas.\n");
  }
}
