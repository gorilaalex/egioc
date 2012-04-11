#include "gtransforms.h"

struct transf *
loadTStructure(char *tsFile){
  FILE *fhandle = fopen(tsFile, "r");
  struct transf *tsList = NULL;
  double tparams[6];
  char transType;

  if(NULL != fhandle){
    /* init the transformation list with an identity matrix */
    tsList = (struct transf*)malloc(sizeof(struct transf));
    initIdentity(&tsList->tm);
    tsList->next = NULL;

    /* process the file */
    fscanf(fhandle, "%c", &transType);
    while(!feof(fhandle)){
      switch(transType){
      case 't':
	/* tparams[0] = tx; tparams[1] = ty */
	fscanf(fhandle, "%lf %lf", &tparams[0], &tparams[1]);
	applyTranslation(&tsList->tm, tparams[0], tparams[1]);
	break;
      case 's':
	/* tparams[0] = px; tparams[1] = py; tparams[2] = sx; tparams[3] = sy */
	fscanf(fhandle, "%lf %lf %lf %lf", &tparams[0], &tparams[1], &tparams[2], &tparams[3]); 
	applyComposedScaling(&tsList->tm, tparams[0], tparams[1], tparams[2], tparams[3]);
	break;
      case 'r':
	/* tparams[0] = px; tparams[1] = py; tparams[2] = u */
	fscanf(fhandle, "%lf %lf %lf", &tparams[0], &tparams[1], &tparams[2]);
	applyComposedRotation(&tsList->tm, tparams[0], tparams[1], tparams[2]);
	break;
      }

      fgetc(fhandle);
      fscanf(fhandle, "%c", &transType);
    }
  }
  fclose(fhandle);
  
  return tsList;
}

void
freeTStrcuture(struct transf **tlist){
  struct transf *ptl = (*tlist);
  struct transf *nextptl = NULL;

  while(NULL != ptl){
    nextptl = ptl->next;
    free(ptl);
    ptl = nextptl;
  }
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
applyRotation(struct transMatrix *m, double u){
  /*
   Apply the following transformation to 'm' :
                 |cos(u)  -sin(u)   0|
   [m] = [m]  x  |sin(u)   cos(u)   0|
                 |    0        0    1|
  */
  struct transMatrix maux;

  initRotation(&maux, u);
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

void
applyComposedRotation(struct transMatrix *m, double px, double py, double u){
  /*
   Apply the following transformation to 'm' :
                 |1 0 px|     |cos(u)  -sin(u)   0|     |1 0 -px|
   [m] = [m]  x  |0 1 py|  x  |sin(u)   cos(u)   0|  x  |0 1 -py|
                 |0 0  1|     |    0        0    1|     |0 0   1|
  */

  applyTranslation(m, px, py);
  applyRotation(m, u);
  applyTranslation(m, -px, -py);
}

void
applyComposedScaling(struct transMatrix *m, double px, double py, double sx, double sy){
  /*
   Apply the following transformation to 'm' :
                 |1 0 px|     |sx  0 0|     |1 0 -px|
   [m] = [m]  x  |0 1 py|  x  | 0 sy 0|  x  |0 1 -py|
                 |0 0  1|     | 0  0 1|     |0 0   1|
  */
  
  applyTranslation(m, px, py);
  applyScaling(m, sx, sy);
  applyTranslation(m, -px, -py);
}

void initRotation(struct transMatrix * m, double angle)
{
  double radAngle = degToRad(angle);
  /*
    Load the following matrix into 'm' :
    |cos(t) -sin(t)  0|
    |sin(t)  cos(t)  0|
    |  0       0     1|
   */
  m->matrix[0][0] = cos(radAngle); m->matrix[0][1] = -1*sin(radAngle); m->matrix[0][2] = 0;
  m->matrix[1][0] = sin(radAngle); m->matrix[1][1] =    cos(radAngle); m->matrix[1][2] = 0;
  m->matrix[2][0] =             0; m->matrix[2][1] =                0; m->matrix[2][2] = 1;
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

void initHomoVector(struct homoCoord *point, double x, double y)
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

void 
applyTransforms(struct transf *tlist, struct GENode *glist){
  struct transf *ptl = tlist;
  struct GENode *pgl = NULL;

  struct homoCoord start;
  struct homoCoord end;

  while(NULL != ptl){
    pgl = glist;
    while(NULL != pgl){
      switch(pgl->el.type)
      {case LINE: initHomoVector(&start, pgl->el.data.line.st.x, pgl->el.data.line.st.y);
                  initHomoVector(&end, pgl->el.data.line.en.x, pgl->el.data.line.en.y);
                  break;
       default: break;
      }
      /*switch(ptl->tType){
      case TRANSLATION: initTranslation(&m, ptl->data.t.tx, ptl->data.t.ty);
	                break;
      case SCALING: initScale(&m, ptl->data.s.sx, ptl->data.s.sy);
                    if (!(ptl->data.s.px == 0 && ptl->data.s.py == 0))
                        {
                          initTranslation(&aux, -1*ptl->data.s.px, -1*ptl->data.s.py);
                          matrixProduct(&m, m, aux);
                          initTranslation(&aux, ptl->data.s.px, ptl->data.s.py);
                          matrixProduct(&m, m, aux);
                        }
	             break;
      case ROTATION: initRotation(&m, ptl->data.r.u);
                     if (!(ptl->data.s.px == 0 && ptl->data.s.py == 0))
                        {
                          initTranslation(&aux, -1*ptl->data.r.px, -1*ptl->data.r.py);
                          matrixProduct(&m, m, aux);
                          initTranslation(&aux, ptl->data.s.px, ptl->data.s.py);
                          matrixProduct(&m, m, aux);
                        }
	             break;
      */
      matrixVectorProduct(&start, ptl->tm, start);
      matrixVectorProduct(&end, ptl->tm, end);
      twoDCoord(&pgl->el.data.line.st, start);
      twoDCoord(&pgl->el.data.line.en, end);     
      pgl = pgl->next;
    }
    ptl = ptl->next;
  }
}


