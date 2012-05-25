#include "transforms.h"

void
initSHTranslation(SpaceHomoMatrix *m, double tx, double ty, double tz)
{
  /*
   Load the following matrix into 'm' :
   |1 0 0 tx|
   |0 1 0 ty|
   |0 0 1 tz|
   |0 0 0  1|
  */
  m->matrix[0][0] = 1; m->matrix[0][1] = 0; m->matrix[0][2] = 0; m->matrix[0][3] = tx;
  m->matrix[1][0] = 0; m->matrix[1][1] = 1; m->matrix[1][2] = 0; m->matrix[1][3] = ty;
  m->matrix[2][0] = 0; m->matrix[2][1] = 0; m->matrix[2][2] = 1; m->matrix[2][3] = tz;
  m->matrix[3][0] = 0; m->matrix[3][1] = 0; m->matrix[3][2] = 0; m->matrix[3][3] =  1;
}

void
initSHIdentity(SpaceHomoMatrix *m){
  /*
   Load the following matrix into 'm' :
   |1 0 0 0|
   |0 1 0 0|
   |0 0 1 0|
   |0 0 0 1|
  */
  initSHTranslation(m, 0.0, 0.0, 0.0);
}

void
applySHTranslation(SpaceHomoMatrix *m, double tx, double ty, double tz){
  /*
   Apply the following transformation to 'm' :
                 |1 0 0 tx|
   [m] = [m]  x  |0 1 0 ty|
                 |0 0 1 tz|
                 |0 0 0  1|
  */
  SpaceHomoMatrix maux;

  initSHTranslation(&maux, tx, ty, tz);
  matrixSHProduct(m, (*m), maux);
}

void
applySHScaling(SpaceHomoMatrix *m, double sx, double sy, double sz){
  /*
   Apply the following transformation to 'm' :
                 |sx  0   0  0|
   [m] = [m]  x  | 0 sy   0  0|
                 | 0  0  sz  0|
                 | 0  0   0  1|
  */
  SpaceHomoMatrix maux;

  initSHScale(&maux, sx, sy, sz);
  matrixSHProduct(m, (*m), maux);
}

void
initSHScale(SpaceHomoMatrix * m, double sx, double sy, double sz)
{
   /*
    Load the following matrix into 'm' :
    |sx  0  0  0|
    | 0 sy  0  0|
    | 0  0 sz  0|
    | 0  0  0  1|
   */
  m->matrix[0][0] = sx; m->matrix[0][1] =  0; m->matrix[0][2] = 0;  m->matrix[0][3] = 0;
  m->matrix[1][0] =  0; m->matrix[1][1] = sy; m->matrix[1][2] = 0;  m->matrix[1][3] = 0;
  m->matrix[2][0] =  0; m->matrix[2][1] =  0; m->matrix[2][2] = sz; m->matrix[2][3] = 0;
  m->matrix[3][0] =  0; m->matrix[3][1] =  0; m->matrix[3][2] = sz; m->matrix[3][3] = 1;
}

void
matrixSHProduct(SpaceHomoMatrix *prod, SpaceHomoMatrix a, SpaceHomoMatrix b) 
{
  /* [prod] = [a] * [b] */
  int lin, col, index;
  double sum;

  for(lin=0; lin<4; lin++)
    for(col=0; col<4; col++) {
	sum = 0;
	for(index=0; index<3; index++)
	  sum+= a.matrix[lin][index]*b.matrix[index][col];
	prod->matrix[lin][col] = sum;
      }
}

void
matrixSHVectorProduct(SpaceHomoCoord *prod, SpaceHomoMatrix m, SpaceHomoCoord vect)
{
  int lin, col;
  double sum = 0;

  for(lin=0; lin<4; lin++)
    {
      sum = 0;
      for(col=0; col<4; col++)
	sum+= m.matrix[lin][col]*vect.coord[col];
      prod->coord[lin] = sum;
    }
}

void
initSHHomoVector(SpaceHomoCoord *point, float x, float y, float z)
{
  point->coord[0] = x;
  point->coord[1] = y;
  point->coord[2] = z;
  point->coord[3] = 1;
}

void
twoDSHCoord(PlanePoint * el, SpaceHomoCoord point)
{
  /* double factor = point.coord[2];
  el->x = point.coord[0]/factor;
  el->y = point.coord[1]/factor; */
  /* TODO: Add correct code to this function to project a 3D homogenous point onto a 2D space */
}

void initTranslation(SpaceMatrix *m, double tx, double ty)
{
  /*
   Load the following matrix into 'm' :
   |1 0 tx|
   |0 1 ty|
   |0 0  1|
  */
  m->matrix[0][0] = 1; m->matrix[0][1] = 0; m->matrix[0][2] = tx;
  m->matrix[1][0] = 0; m->matrix[1][1] = 1; m->matrix[1][2] = ty;
  m->matrix[2][0] = 0; m->matrix[2][1] = 0; m->matrix[2][2] = 1;
}

void
initIdentity(SpaceMatrix *m){
  /*
   Load the following matrix into 'm' :
   |1 0 0|
   |0 1 0|
   |0 0 1|
  */
  initTranslation(m, 0, 0);
}

void
applyTranslation(SpaceMatrix *m, double tx, double ty){
  /*
   Apply the following transformation to 'm' :
                 |1 0 tx|
   [m] = [m]  x  |0 1 ty|
                 |0 0  1|
  */
  SpaceMatrix maux;

  initTranslation(&maux, tx, ty);
  matrixProduct(m, (*m), maux);
}

void
applyScaling(SpaceMatrix *m, double sx, double sy){
  /*
   Apply the following transformation to 'm' :
                 |sx  0  0|
   [m] = [m]  x  | 0 sy  0|
                 | 0  0  1|
  */
  SpaceMatrix maux;

  initScale(&maux, sx, sy);
  matrixProduct(m, (*m), maux);
}

void initScale(SpaceMatrix * m, double sx, double sy)
{
   /*
    Load the following matrix into 'm' :
    |sx  0 0|
    | 0 sy 0|
    | 0  0 1|
   */
  m->matrix[0][0] = sx; m->matrix[0][1] =  0; m->matrix[0][2] = 0;
  m->matrix[1][0] =  0; m->matrix[1][1] = sy; m->matrix[1][2] = 0;
  m->matrix[2][0] =  0; m->matrix[2][1] =  0; m->matrix[2][2] = 1;
}

void matrixProduct(SpaceMatrix *prod, SpaceMatrix a, SpaceMatrix b)
{
  /* [prod] = [a] * [b] */
  int lin, col, index;
  double sum;
  for(lin=0; lin<3; lin++)
    for(col=0; col<3; col++)
      {
	sum = 0;
	for(index=0; index<3; index++)
	  sum+= a.matrix[lin][index]*b.matrix[index][col];
	prod->matrix[lin][col] = sum;
      }
}

void matrixVectorProduct(PlaneHomoCoord *prod, SpaceMatrix m, PlaneHomoCoord vect)
{
  int lin, col;
  double sum = 0;

  for(lin=0; lin<3; lin++)
    {
      sum = 0;
      for(col=0; col<3; col++)
	sum+= m.matrix[lin][col]*vect.coord[col];
      prod->coord[lin] = sum;
    }
}

void initHomoVector(PlaneHomoCoord *point, int x, int y)
{
  point->coord[0] = x;
  point->coord[1] = y;
  point->coord[2] = 1;
}

void twoDCoord(PlanePoint * el, PlaneHomoCoord point)
{
  double factor = point.coord[2];
  el->x = point.coord[0]/factor;
  el->y = point.coord[1]/factor;
}

PlanePoint pointToViewport(Region window, Region viewport, PlanePoint point) {
  PlaneHomoCoord initial, final;
  SpaceMatrix m;

  DBG("'pointToViewport' : viewport {l:%f d:%f t:%f r:%f}\n", viewport.windowLeft, viewport.windowBottom, viewport.windowTop, viewport.windowRight);
  DBG("'pointToViewport' : window {l:%f d:%f t:%f r:%f}\n", window.windowLeft, window.windowBottom, window.windowTop, window.windowRight);

  DBG("'pointToViewport' : scaling factor {%f %f}\n", (viewport.windowRight - viewport.windowLeft)*1.0/(window.windowRight - window.windowLeft), (viewport.windowTop - viewport.windowBottom)*1.0/(window.windowTop - window.windowBottom));

  initIdentity(&m);
  applyTranslation(&m, viewport.windowLeft, viewport.windowBottom);
  applyScaling(&m, (viewport.windowRight - viewport.windowLeft)*1.0/(window.windowRight - window.windowLeft), (viewport.windowTop - viewport.windowBottom)*1.0/(window.windowTop - window.windowBottom));
  applyTranslation(&m, -1.0*window.windowLeft, -1.0*window.windowBottom);

  initHomoVector(&initial, point.x, point.y);
  matrixVectorProduct(&final, m, initial);
  DBG("Point before 'twoDCoord' call : {%f %f}\n", final.coord[0], final.coord[1]);
  twoDCoord(&point, final);

  return point;
}
