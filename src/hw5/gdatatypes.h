#ifndef _GDATATYPES_H_
#define _GDATATYPES_H_

/* A simple point structure */
typedef struct {
  int x;
  int y;
} Point;

/* A double-linked point list structure */
struct PointNode {
  Point pt;
  struct PointNode *next;
  struct PointNode *prev;
};

typedef enum {UNDEFINED_ELEMENT, BLANK, LINE, POINT, POLY} GElementType;

/* Graphics Element structure containing various primitive elements */
typedef struct{
  union{
    Point point;
    struct PointNode *headPoint;
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
