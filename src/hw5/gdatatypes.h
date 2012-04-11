#ifndef _GDATATYPES_H_
#define _GDATATYPES_H_

/* A simple point structure */
typedef struct {
  int x;
  int y;
} Point;

/* A simple-linked point list structure */
struct SPointList {
  Point pt;
  struct SPointList *next;
};

typedef enum {UNDEFINED_ELEMENT, BLANK, LINE, POINT, POLY} GElementType;

/* Graphics Element structure containing various primitive elements */
typedef struct{
  union{
    Point point;
    struct SPointList *poly;
    struct{
      Point st;
      Point en;
    } line;
  } data;
  GElementType type;
}GElement;

struct GENode{
  GElement el;
  struct GENode *next;
};

#endif
