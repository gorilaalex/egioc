#include "3dops.h"

float vectorLength(SpacePoint p)
{
    return (float)sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
}

SpacePoint crossProduct(SpacePoint u, SpacePoint w)
{
        SpacePoint product;
        product.x = u.y * w.z - u.z * w.y;
        product.y = u.z * w.x - u.x * w.z;
        product.z = u.x * w.y - u.y * w.x;
        return product;
}

SpacePoint normalized(SpacePoint u)
{
    float length = vectorLength(u);
    u.x /= length;
    u.y /= length;
    u.z /= length;
    return u;
}

SpacePoint pointToUVN(SpaceViewSettings *space, SpacePoint point) {
  SpacePoint n, u, v, prod;

  n = normalized(space->VPN);
  DBG("'pointToUVN' : n after normalizing VPN {%f %f %f}\n", n.x, n.y, n.z);
  prod = crossProduct(space->VUP, space->VPN);
  u = normalized(prod);
  prod = crossProduct(n, u);
  v = normalized(prod);
  float matrix[4][4]={{u.x, u.y, u.z, -1.0*(u.x*space->VRP.x + u.y*space->VRP.y + u.z*space->VRP.z)},
                      {v.x, v.y, v.z, -1.0*(v.x*space->VRP.x + v.y*space->VRP.y + v.z*space->VRP.z)},
                      {n.x, n.y, n.z, -1.0*(n.x*space->VRP.x + n.y*space->VRP.y + n.z*space->VRP.z)},
                      {0, 0, 0, 1}
                     };

       
  point.x = matrix[0][0]*point.x + matrix[0][1]*point.y + matrix[0][2]*point.z + matrix[0][3];
  point.y = matrix[1][0]*point.x + matrix[1][1]*point.y + matrix[1][2]*point.z + matrix[1][3];
  point.z = matrix[2][0]*point.x + matrix[2][1]*point.y + matrix[2][2]*point.z + matrix[2][3];  

  return point;
}

void worldToUVN(SpaceViewSettings *space) 
{
  SpacePoint VRP, VPN, VUP;
  VRP = pointToUVN(space, space->VRP);
  VPN = pointToUVN(space, space->VPN);
  VUP = pointToUVN(space, space->VUP);
  space->VRP = VRP;
  space->VPN = VPN;
  space->VUP = VUP;
}

PlanePoint perspectiveProjection(SpacePoint point, SpacePoint center)
{
    PlanePoint projection;
    DBG("Center{%-4.2f %-4.2f %-4.2f}, Pt{%-4.2f %-4.2f %-4.2f} ", center.x, center.y, center.z,
                                                    point.x, point.y, point.z);
    projection.x = (point.x*center.z - point.z*center.x)/(center.z - point.z);
    projection.y = (point.y*center.z - point.z*center.y)/(center.z - point.z); 

    DBG("Proj{%f %f}\n", projection.x, projection.y);

    return projection;
}

SpacePoint projectionDirection(SpaceViewSettings space)
{
    SpacePoint direction, viewCenter;
    viewCenter.x = (space.UVMAXx - space.UVMINx)/2.0;
    viewCenter.y = (space.UVMAXy - space.UVMINy)/2.0;
    viewCenter.z = 0.0;
    direction.x = viewCenter.x - space.PRP.x;
    direction.y = viewCenter.y - space.PRP.y;
    direction.z = viewCenter.z - space.PRP.z;
    return direction;
}

PlanePoint parallelProjection(SpacePoint point, SpacePoint direction)
{
    PlanePoint projection;
    projection.x = point.x - direction.x*point.z/direction.z;
    projection.y = point.y - direction.y*point.z/direction.z;
    return projection;
}

int isInWindow(Region window, PlanePoint point)
{
    return(point.x<=window.windowRight && point.x>=window.windowLeft && point.y>=window.windowBottom && point.y<=window.windowTop);
}

void project(XPM *img, SpaceViewSettings space, SpaceObjData data, Region window, Region viewport)
{
    worldToUVN(&space);
    int index;
    SpacePoint vertex1, vertex2, vertex3;
    PlanePoint point1, point2, point3;

    for(index = 0; index<data.trigs.length; index++)
        {
	    vertex1 = data.pts.v[(data.trigs.v[index].fId) -1];
            vertex2 = data.pts.v[(data.trigs.v[index].sId) -1];
            vertex3 = data.pts.v[(data.trigs.v[index].tId) -1];
            if(space.PTYPE == 0) //perspective projection
            {
                point1 = perspectiveProjection(vertex1, space.PRP);
                point2 = perspectiveProjection(vertex2, space.PRP);
                point3 = perspectiveProjection(vertex3, space.PRP);

            } 
            else if(space.PTYPE == 1) //parallel projection
            {
                SpacePoint direction = projectionDirection(space);
                point1 = parallelProjection(vertex1, direction);
                point2 = parallelProjection(vertex2, direction);
                point3 = parallelProjection(vertex3, direction);
            }
            DBG("Calculated triangle {%f %f}, {%f %f}, {%f %f}\n", point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
         
           //if(isInWindow(window, point1))
             //{
               point1 = pointToViewport(window, viewport, point1);
              // if(isInWindow(window, point2))
                // {
                  point2 = pointToViewport(window, viewport, point2);
                  //if(isInWindow(window, point3))
                    // {
                      point3 = pointToViewport(window, viewport, point3);
                      renderLine(img, point1, point2, 1);
                      renderLine(img, point2, point3, 1);
                      renderLine(img, point3, point1, 1);
                    //}
                  //else renderLine(img, point1, point2, 1);
                 //}
             // }
           // getchar();
      }

}

SpaceObjData
loadOBJFile(const char *fName) {
  SpaceObjData objData = {{NULL, 0}, {NULL, 0}};
  SpacePoint ptNew;
  TriangleIdDef trigInfo;
  FILE *dataFile = fopen(fName, "r");
  char lineBuff[512];
  char *pStr;
  char op;

  if(NULL != dataFile) {
    objData.pts.v = (SpacePoint *)calloc(PT_REALLOC_STEP, sizeof(SpacePoint));
    objData.trigs.v = (TriangleIdDef *)calloc(PT_REALLOC_STEP, sizeof(TriangleIdDef));

    while(fgets(lineBuff, sizeof(lineBuff), dataFile) != NULL) {
      if(strlen(lineBuff) != 0 && lineBuff[0] != '#') {
	/* We have a valid OBJ data on current line, parse it */
	op = lineBuff[0];
        pStr = lineBuff + 2 * sizeof(char);
	switch(op) {
        case VERTEX_OP:
          sscanf(pStr, "%f %f %f", &ptNew.x, &ptNew.y, &ptNew.z);
          //printf("%f %f %f\n", ptNew.x, ptNew.y, ptNew.z); 
          objData.pts.v[objData.pts.length++] = ptNew;
          if(objData.pts.length % PT_REALLOC_STEP == 0) {
            objData.pts.v = (SpacePoint *)realloc(objData.pts.v, (objData.pts.length + PT_REALLOC_STEP) * sizeof(SpacePoint));
          }
          break;
        case DEFINE_TRIANGLE:
          sscanf(pStr, "%d %d %d", &trigInfo.fId, &trigInfo.sId, &trigInfo.tId);
          objData.trigs.v[objData.trigs.length++] = trigInfo;
          if(objData.trigs.length % PT_REALLOC_STEP == 0) {
            objData.trigs.v = (TriangleIdDef *)realloc(objData.trigs.v, (objData.trigs.length + PT_REALLOC_STEP) * sizeof(TriangleIdDef));
          }
          break;
        }
      }
    }

    fclose(dataFile);
  }
  return objData;
}

void
freeOBJFile(SpaceObjData oData) {
  free(oData.pts.v);
  free(oData.trigs.v);
}

SpaceViewSettings
loadVWFile(const char *fName) {
  SpaceViewSettings vwSett;
  FILE *viewFile = fopen(fName, "r");
  char dataStruct[20];
  float minx, miny, maxx, maxy;

  if(NULL != viewFile) {
    while(!feof(viewFile) && fscanf(viewFile, "%s", dataStruct)) {
      if(strcmp(dataStruct, "VRP") == 0)  fscanf(viewFile, "%f %f %f", &vwSett.VRP.x, &vwSett.VRP.y, &vwSett.VRP.z);
      else if(strcmp(dataStruct, "VPN") == 0)  fscanf(viewFile, "%f %f %f", &vwSett.VPN.x, &vwSett.VPN.y, &vwSett.VPN.z);
      else if(strcmp(dataStruct, "VUP") == 0)  fscanf(viewFile, "%f %f %f", &vwSett.VUP.x, &vwSett.VUP.y, &vwSett.VUP.z);
      else if(strcmp(dataStruct, "PRP") == 0)  fscanf(viewFile, "%f %f %f", &vwSett.PRP.x, &vwSett.PRP.y, &vwSett.PRP.z);
      else if(strcmp(dataStruct, "UVMIN") == 0)  fscanf(viewFile, "%f %f", &minx, &miny);
      else if(strcmp(dataStruct, "UVMAX") == 0)  fscanf(viewFile, "%f %f", &maxx, &maxy);
      else if(strcmp(dataStruct, "PTYPE") == 0)  fscanf(viewFile, "%d", &vwSett.PTYPE);
    }
   // printf("%f %f %f %f\n", minx,  miny,  maxx,  maxy);
    if(minx<maxx)
       {
        vwSett.UVMINx = minx;
        vwSett.UVMAXx = maxx;
       }
    else {
          vwSett.UVMINx = maxx;
          vwSett.UVMAXx = minx;
         }
    if(miny<maxy)
       {
        vwSett.UVMINy = miny;
        vwSett.UVMAXy = maxy;
       }
    else {
          vwSett.UVMINy = maxy;
          vwSett.UVMAXy = miny;
         }
    fclose(viewFile);
    printf("%f %f %f %f\n", vwSett.UVMINx,  vwSett.UVMINy,  vwSett.UVMAXx,  vwSett.UVMAXy);
  }

  return vwSett;
}

static int 
isPtInTriangle(PlanePoint pt, PlanePoint ptTr1, PlanePoint ptTr2, PlanePoint ptTr3) {
  /* Applied the theory from here : http://mathworld.wolfram.com/TriangleInterior.html
     where : v = pt, v0 = ptTr1, v1 = ptTr2, v2 = ptTr3 and det(U, V) = Ux * Vy - Uy * Vx */

  double a =  ((pt.x * ptTr3.y - pt.y * ptTr3.x) - (ptTr1.x * ptTr3.y - ptTr1.y * ptTr3.x)) / 
               (ptTr2.x * ptTr3.y - ptTr2.y * ptTr3.x); 
  double b = -((pt.x * ptTr2.y - pt.y * ptTr2.x) - (ptTr1.x * ptTr2.y - ptTr1.y * ptTr2.x)) /
               (ptTr2.x * ptTr3.y - ptTr2.y * ptTr3.x); 

  if(a > 0 && b > 0 && (a + b) < 1) return 1;

  return 0;
}

static double 
computez(PlanePoint pt, PlaneCoefficients pcoeffs) {
  double z = 0.0;

  if (pcoeffs.B > 1e-7) z = (-pcoeffs.A * pt.x - pcoeffs.C * pt.y - pcoeffs.D) / pcoeffs.B;

  return z;
}

static PlaneCoefficients
computeCoeffOfPlane(SpacePoint pt1, SpacePoint pt2, SpacePoint pt3) {
  /* Followed this judgement for finding the plane's coefficients : http://en.wikipedia.org/wiki/Plane_%28geometry%29#Method_3 */
  PlaneCoefficients ccoef;

  ccoef.A = (pt2.z - pt3.z) * (pt1.y - pt2.y) - (pt1.z - pt2.z) * (pt2.y - pt3.y);
  ccoef.B = (pt2.x - pt3.x) * (pt1.z - pt2.z) - (pt1.x - pt2.x) * (pt2.z - pt3.z);
  ccoef.C = (pt2.y - pt3.y) * (pt1.x - pt2.x) - (pt1.y - pt2.y) * (pt2.x - pt3.x);
  ccoef.D = - pt1.x * (pt2.y * pt3.z - pt2.z * pt3.y) + pt1.y * (pt2.x * pt3.z - pt2.z * pt3.x) - pt1.z * (pt2.x * pt3.y - pt2.y * pt3.x);
  
  return ccoef; 
}

void
computeZBuffer(XPM *canvas, ZBuffer *zb, SpaceViewSettings space, SpaceObjData data, Region window, Region viewport) {
  int id;
  int xmin, xmax;
  int ymin, ymax;
  PlaneCoefficients currentPlCoeffs;
  SpacePoint trPt1, trPt2, trPt3;
  PlanePoint trPlanePt1, trPlanePt2, trPlanePt3;
  PlanePoint pt;
  int xrect, yrect;
  int zval;

  for(id = 0; id < data.trigs.length; ++id) {
    /* get the 3D triangle points */
    trPt1 = data.pts.v[data.trigs.v[id].fId - 1];
    trPt2 = data.pts.v[data.trigs.v[id].sId - 1];
    trPt3 = data.pts.v[data.trigs.v[id].tId - 1];

    /* compute the projection on the 2D surface given by SpaceViewSettings */
    trPlanePt1 = project2DPoint(trPt1, space);
    trPlanePt2 = project2DPoint(trPt2, space);
    trPlanePt3 = project2DPoint(trPt3, space);

    /* blit the point to viewport */
    trPlanePt1 = pointToViewport(window, viewport, trPlanePt1);
    trPlanePt2 = pointToViewport(window, viewport, trPlanePt2);
    trPlanePt3 = pointToViewport(window, viewport, trPlanePt3);

    /* compute the minimum required holding rectangle for the current triangle */
    xmax = ymax = INT_MIN;
    xmin = ymin = INT_MAX;
    /* compute xmax */
    if(trPlanePt1.x > xmax) xmax = trPlanePt1.x;
    if(trPlanePt2.x > xmax) xmax = trPlanePt2.x;
    if(trPlanePt3.x > xmax) xmax = trPlanePt3.x;
    /* compute xmin */
    if(trPlanePt1.x < xmin) xmin = trPlanePt1.x;
    if(trPlanePt2.x < xmin) xmin = trPlanePt2.x;
    if(trPlanePt3.x < xmin) xmin = trPlanePt3.x;
    /* compute ymax */
    if(trPlanePt1.y > ymax) ymax = trPlanePt1.y;
    if(trPlanePt2.y > ymax) ymax = trPlanePt2.y;
    if(trPlanePt3.y > ymax) ymax = trPlanePt3.y;
    /* compute ymin */
    if(trPlanePt1.y < ymin) ymin = trPlanePt1.y;
    if(trPlanePt2.y < ymin) ymin = trPlanePt2.y;
    if(trPlanePt3.y < ymin) ymin = trPlanePt3.y;

    /* compute and fill zbuffer */
    currentPlCoeffs = computeCoeffOfPlane(trPt1, trPt2, trPt3);
    for(xrect = xmin; xrect < xmax; ++xrect) {
      for(yrect = ymin; yrect < ymax; ++yrect) {
        pt.x = xrect;
        pt.y = yrect;
        if(isPtInTriangle(pt, trPlanePt1, trPlanePt2, trPlanePt3) != 0) {
          zval = computez(pt, currentPlCoeffs);
          if(zval > zb->data[yrect][xrect]) {
            zb->data[yrect][xrect] = zval;
            if(zval > zb->maxz) zb->maxz = zval;
            else if(zval < zb->minz) zb->minz = zval;
            //putXPMpixel(canvas, xrect, yrect, 1);
          }
        }
      }
    }
  }
}

void 
projectZBuffer(XPM *canvas, ZBuffer *zb, unsigned short clrBase, unsigned short clrShadeCnt) {
  assert(clrShadeCnt != 0);

  double absMinz = fabs(zb->minz);
  int clrIncrement = (int)((absMinz + fabs(zb->maxz))/clrShadeCnt);
  double absZData;
  int xpix, ypix;
  int clrOffset;

  for(ypix = 0; ypix < zb->height; ++ypix) {
    for(xpix = 0; xpix < zb->width; ++xpix) {
      absZData = fabs(zb->data[ypix][xpix]);
      if(absZData - absMinz > 1e-7) {
        clrOffset = (int)(absZData + absMinz) / clrIncrement;
        putXPMpixel(canvas, xpix, ypix, clrBase + clrOffset);
      } 
    }
  }
}
