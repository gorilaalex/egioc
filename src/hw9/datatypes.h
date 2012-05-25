#ifndef DATATYPES_H_INCLUDED
#define DATATYPES_H_INCLUDED

/* Macro definition region */
#define SYMMETRIC 1
#define ASYMMETRIC 0

#define WINDOWLEFT 0
#define WINDOWBOTTOM 0

//#define _DEBUG_

#ifdef _DEBUG_
 #define DBG(...) printf(__VA_ARGS__)
#else
 #define DBG(...) 
#endif

#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y)
#define perp(u,v)  ((u).x * (v).y - (u).y * (v).x)

#define PI 3.14159265

/* 3D data declaration region */
typedef struct {
    float x, y, z;
} SpacePoint;

typedef struct {
  SpacePoint *v;
  int length;
} SpacePointVector;

typedef struct {
  unsigned int fId;
  unsigned int sId;
  unsigned int tId;
} TriangleIdDef;

typedef struct {
  TriangleIdDef *v;
  unsigned int length;
} TriangleIdVector;

typedef struct {
  SpacePointVector pts;
  TriangleIdVector trigs;
} SpaceObjData;

typedef struct {
  SpacePoint VRP;  
  SpacePoint VPN;
  SpacePoint VUP;
  SpacePoint PRP;
  float UVMINx, UVMINy;
  float UVMAXx, UVMAXy;
  int PTYPE;
} SpaceViewSettings;

/* ending of 3D data declaration region */

typedef struct {
   float x,y;
} PlanePoint;

typedef struct {
  PlanePoint *v;
  unsigned int length;
} PlanePointVector;

struct PlaneSegment {
    PlanePoint P0, P1;
};

typedef struct
{
    float x,y;
} PlaneVector;

struct PlanePolygon
{
    PlanePoint P;
    struct PlanePolygon *next;
    struct PlanePolygon *prev;
};

typedef struct {
  float windowTop;
  float windowLeft;
  float windowBottom;
  float windowRight;
} Region;

/* Color structure that holds pixel data */
typedef struct {
	unsigned short red;
	unsigned short green;
	unsigned short blue;
} Color;

/* XPM specific color structure (see Chapter 2 of "XPM Manual", v3.4i) */
typedef struct {
	Color clr;
	char *chars;
	char *key;
} XPMColor;

/* XPM structure that holds a complete XPM image segment */
typedef struct {
  unsigned int width;
  unsigned int height;
  Region displayRegion;
  unsigned short chrperpixel;
  unsigned int ncolors;
  XPMColor *colta;
  unsigned short **data;
} XPM;

typedef struct {
  unsigned int width;
  unsigned int height;
  unsigned short **data;
} ZBuffer;

typedef struct {
  double matrix[4][4];
} SpaceHomoMatrix;

typedef struct {
  float coord[4];
} SpaceHomoCoord;

typedef struct {
  double matrix[3][3];
} SpaceMatrix;

typedef struct {
  float coord[3];
} PlaneHomoCoord;

enum { TOP = 0x1, BOTTOM = 0x2, RIGHT = 0x4, LEFT = 0x8 };
typedef enum {PSFILE_OK, ERROR_ON_OPEN, ERROR_ON_PARSE} procPSFileReturnVal;
typedef enum {PSLINE_OK, UNRECOGNIZED_ELEMENT} procPSLineReturnVal;

#endif // DATATYPES_H_INCLUDED
