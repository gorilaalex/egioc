#include "transforms.h"

void initTranslation(struct transMatrix * m, double tx, double ty)
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
initIdentity(struct transMatrix *m){
  /*
   Load the following matrix into 'm' :
   |1 0 0|
   |0 1 0|
   |0 0 1|
  */
  initTranslation(m, 0, 0);
}

void
applyTranslation(struct transMatrix *m, double tx, double ty){
  /*
   Apply the following transformation to 'm' :
                 |1 0 tx|
   [m] = [m]  x  |0 1 ty|
                 |0 0  1|
  */
  struct transMatrix maux;

  initTranslation(&maux, tx, ty);
  matrixProduct(m, (*m), maux);
}

void
applyScaling(struct transMatrix *m, double sx, double sy){
  /*
   Apply the following transformation to 'm' :
                 |sx  0  0|
   [m] = [m]  x  | 0 sy  0|
                 | 0  0  1|
  */
  struct transMatrix maux;

  initScale(&maux, sx, sy);
  matrixProduct(m, (*m), maux);
}

void initScale(struct transMatrix * m, double sx, double sy)
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

void matrixProduct(struct transMatrix *prod, struct transMatrix a, struct transMatrix b)
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

void matrixVectorProduct(struct homoCoord *prod, struct transMatrix m, struct homoCoord vect)
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

void initHomoVector(struct homoCoord *point, int x, int y)
{
  point->coord[0] = x;
  point->coord[1] = y;
  point->coord[2] = 1;
}

void twoDCoord(PlanePoint * el, struct homoCoord point)
{
  double factor = point.coord[2];
  el->x = point.coord[0]/factor;
  el->y = point.coord[1]/factor;
}

PlanePoint pointToViewport(Region window, Region viewport, PlanePoint point) {
  struct homoCoord initial, final;
  struct transMatrix m;

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
