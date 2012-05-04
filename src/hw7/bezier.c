#include "bezier.h"
#include <math.h>

#define ALLOC_INCREMENT 5
#define BEZIER_MATRIX_ORD 4

PointVector
loadBZEFile(const char *fName) {
  char dataType[20];
  PointVector ptVector = {NULL, 0};
  Point newPt = {0, 0};
  FILE *fHandle = fopen(fName, "r");

  if(NULL != fHandle) {
    ptVector.v = (Point *)calloc(ALLOC_INCREMENT, sizeof(Point));
    while(fscanf(fHandle, "%d %d %s", &newPt.x, &newPt.y, dataType) == 3) {
      ptVector.v[ptVector.length++] = newPt;
      if((ptVector.length % ALLOC_INCREMENT) == 0) {
        ptVector.v = (Point *)realloc(ptVector.v, (ptVector.length + ALLOC_INCREMENT) * sizeof(Point));
      }
    }
    fclose(fHandle);
  }

  return ptVector;
}

void
printPtVector(PointVector ptVector) {
  unsigned int id = 0;

  for(id = 0; id < ptVector.length; ++id) {
    printf("%d %d\n", ptVector.v[id].x, ptVector.v[id].y);
  }
}

void
freePtVector(PointVector ptVector) {
  if(NULL != ptVector.v)
   free(ptVector.v);
}

void firstBezierProduct(double **prod, double *a, int b[BEZIER_MATRIX_ORD][BEZIER_MATRIX_ORD], int dimension)
{
  /* [prod] = [a] * [b] */
  int col, index;
  double sum;
    for(col=0; col<dimension; col++)
      {
        sum = 0;
        for(index=0; index<dimension; index++)
            sum+= a[index]*b[index][col];
        (*prod)[col] = sum;
      }
}

Point pointScalarProduct(double scalar, Point p)
{
    p.x *= scalar;
    p.y *= scalar;
    return p;
}

Point pointSum(Point a, Point b)
{
    Point sum;
    sum.x = a.x + b.x;
    sum.y = a.y + b.y;
    return sum;
}

Point secondBezierProduct(double *a, Point *b, int dimension)
{
  /* [prod] = [a] * [b] */
  Point prod;
  prod.x = 0;
  prod.y = 0;
  int index;
  for(index  = 0; index < dimension; index++)
      prod = pointSum(prod, pointScalarProduct(a[index], b[index]));
  return prod;
}

void drawBezier(XPM *img, Point P1, Point P2, Point P3, Point P4, double prec)
{
    int len = 1+1.01/prec, index;
    double step;
    PointVector curve = {NULL, len};
    curve.v = (Point *)calloc(len, sizeof(Point));

    len = 0;
    double *leftTerm = (double*) malloc(BEZIER_MATRIX_ORD*sizeof(double));
    double *leftProd = (double*) malloc(BEZIER_MATRIX_ORD*sizeof(double));

    Point rightTerm[4] = {P1, P2, P3, P4};

    int bezierMatrix[BEZIER_MATRIX_ORD][BEZIER_MATRIX_ORD]={{-1, 3, -3, 1}, {3, -6, 3, 0}, {-3, 3, 0, 0}, {1, 0, 0, 0}};
    for(step = 0.0; step<=1.0; step += prec)
        {
            for(index = 0; index < BEZIER_MATRIX_ORD; index ++)
                leftTerm[index] = pow(step, BEZIER_MATRIX_ORD - index -1);
            firstBezierProduct(&leftProd, leftTerm, bezierMatrix, BEZIER_MATRIX_ORD);
            curve.v[len++] = secondBezierProduct(leftProd, rightTerm, BEZIER_MATRIX_ORD);
        }
    //printPtVector(curve);
    for(index = 0; index < curve.length - 1; index ++)
        drawBresenhamLine(img, curve.v[index], curve.v[index+1], 1);
    free(leftProd);
    free(leftTerm);
}

 void bezierCurve(XPM *img, PointVector bzPts, double prec)
 {
    int index;
    for(index=0; index<=bzPts.length - BEZIER_MATRIX_ORD; index +=3)
        drawBezier(img, bzPts.v[index],bzPts.v[index+1],bzPts.v[index+2],bzPts.v[index+3], prec);
 }
