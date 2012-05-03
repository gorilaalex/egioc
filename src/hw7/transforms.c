#include "transforms.h"

static int inSegment(Point P, struct Segment S)
{
    if (S.P0.x != S.P1.x)
    {
        if (S.P0.x <= P.x && P.x <= S.P1.x) return 1;
        if (S.P0.x >= P.x && P.x >= S.P1.x) return 1;
    }
    else
    {
        if (S.P0.y <= P.y && P.y <= S.P1.y) return 1;
        if (S.P0.y >= P.y && P.y >= S.P1.y) return 1;
    }
    return 0;
}

static Point pointVectorAdd(Point P, Vector v)
{
    P.x += v.x;
    P.y += v.y;
    return P;
}

static Vector scalarVectorMul(float s, Vector v)
{
    v.x *=s;
    v.y *=s;
    return v;
}

static Vector difference(Point A, Point B)
{
    Vector v;
    v.x = A.x - B.x;
    v.y = A.y - B.y;
    return v;
}

int intersect2D_Segments(struct Segment S1, struct Segment S2, Point *I0, Point *I1 )
{
    Vector    u = difference(S1.P1, S1.P0);
    Vector    v = difference(S2.P1, S2.P0);
    Vector    w = difference(S1.P0, S2.P0);
    float     D = perp(u,v);

    if (fabs(D) < DBL_MIN)
    {
        if (perp(u,w) != 0 || perp(v,w) != 0) return 0;
        float du = dot(u,u);
        float dv = dot(v,v);
        if ((du < 1e-7) && (dv < 1e-7))
        {
            if (!ptEquals(S1.P0, S2.P0))
                return 0;
            *I0 = S1.P0;
            return 1;
        }
        if (du < 1e-7)
        {
            if (inSegment(S1.P0, S2) == 0)
                return 0;
            *I0 = S1.P0;
            return 1;
        }
        if (dv < 1e-7)
        {
            if (inSegment(S2.P0, S1) == 0)
                return 0;
            *I0 = S2.P0;
            return 1;
        }

        float t0, t1;
        Vector w2 = difference(S1.P1, S2.P0);
        if (v.x > 1e-7)
        {
            t0 = w.x / v.x;
            t1 = w2.x / v.x;
        }
        else
        {
            t0 = w.y / v.y;
            t1 = w2.y / v.y;
        }
        if (fabs(t0 - t1) < 1e-7)
        {
            float t=t0;
            t0=t1;
            t1=t;
        }
        if (t0 > 1 || t1 < 0) return 0;

        t0 = t0<0? 0.0 : t0;
        t1 = t1>1? 1.0 : t1;
        if (fabs(t0 - t1) < 1e-7)
        {
            *I0 = pointVectorAdd(S2.P0, scalarVectorMul(t0, v));
            return 1;
        }
        *I0 = pointVectorAdd(S2.P0, scalarVectorMul(t0, v));
        *I1 = pointVectorAdd(S2.P0, scalarVectorMul(t1, v));
        return 2;
    }
    float     sI = perp(v,w) / D;
    if (sI < 0 || sI > 1) return 0;
    float     tI = perp(u,w) / D;
    if (tI < 0 || tI > 1) return 0;
    *I0 = pointVectorAdd(S1.P0, scalarVectorMul(sI, u));
    return 1;
}

double degToRad(double angle)
{
  return angle * PI/180;
}

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

void twoDCoord(Point * el, struct homoCoord point)
{
  double factor = point.coord[2];
  el->x = point.coord[0]/factor;
  el->y = point.coord[1]/factor;
}
