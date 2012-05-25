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
