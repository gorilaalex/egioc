#include "gloader.h"

void
loadFromFile(struct GENode **ellist, const char *file) {
  if(NULL != file) {
    if(strstr(file, ".ps") == (char *)(file + strlen(file) - 3))
      loadPS(ellist, file);
    else if(strstr(file, ".pol") == (char *)(file + strlen(file) - 4))
      loadPOL(ellist, file);
  }
}

void
loadPS(struct GENode **ellist, const char *file) {
  char currentLine[255];
  FILE *fhandle = fopen(file, "r");
  struct GENode *plastNode = NULL;
  struct GENode *newNode = NULL;

  if(NULL != fhandle){
    /* file got opened ok, go past the header */
    do{
      /* search for the beginning of relevant PS data structures */
      fgets(currentLine, sizeof(currentLine), fhandle);
    }while(!feof(fhandle) && strcmp(currentLine, PS_BEGIN_MARK) != 0);

    /* search for the last element of the GElementList and continue adding from there */
    if(NULL != (*ellist)) {
      plastNode = (*ellist);
      while(NULL != plastNode->next) {
	plastNode = plastNode->next;
      }
    }

    /* process the file */
    fgets(currentLine, sizeof(currentLine), fhandle);
    while(!feof(fhandle) && strcmp(currentLine, PS_END_MARK) != 0){
      if(strlen(currentLine) != 1){
	newNode = (struct GENode *)malloc(sizeof(struct GENode));
	newNode->next = NULL;
	parsePSLine(currentLine, &newNode->el);
	
	if(NULL == (*ellist)){
	  (*ellist) = plastNode = newNode;
	} else {
	  plastNode->next = newNode;
	  plastNode = newNode;
	}
      }
      fgets(currentLine, sizeof(currentLine), fhandle);
    }

    fclose(fhandle);
  }
}
  
void
loadPOL(struct GENode **ellist, const char *file) {
  char currentLine[255];
  FILE *fhandle = fopen(file, "r");
  Point readpt;
  struct GENode *plastGNode = NULL;
  struct PointNode *plastPtNode = NULL; 
  struct GENode *newGNode = NULL;
  struct PointNode *newPtNode = NULL;

  if(NULL != fhandle){

    /* search for the last element of the GElementList and continue adding from there */
    if(NULL != (*ellist)) {
      plastGNode = (*ellist);
      while(NULL != plastGNode->next) {
	plastGNode = plastGNode->next;
      }
    }

    /* allocate a poly structure after the last node */
    newGNode = (struct GENode *)malloc(sizeof(struct GENode));
    newGNode->el.type = POLY;
    newGNode->el.data.headPoint = NULL;
    if(NULL == (*ellist)) {
      (*ellist) = newGNode;
      plastGNode = (*ellist);
    } else {
      plastGNode->next = newGNode;
      plastGNode = newGNode;
    }

    /* process the file */
    fgets(currentLine, sizeof(currentLine), fhandle);
    while(!feof(fhandle) && strlen(currentLine) != 0){
      if(sscanf(currentLine, "%d %d", &readpt.x, &readpt.y) != 0){
	newPtNode = (struct PointNode *)malloc(sizeof(struct PointNode));
	newPtNode->pt = readpt;
	newPtNode->next = NULL;
	newPtNode->prev = NULL;

	if(NULL == plastGNode->el.data.headPoint){
	  plastGNode->el.data.headPoint = plastPtNode = newPtNode;
	} else {
	  plastPtNode->next = newPtNode;
	  newPtNode->prev = plastPtNode;
	  plastPtNode = newPtNode;
	}
      }
      fgets(currentLine, sizeof(currentLine), fhandle);
    }

    fclose(fhandle);
  }
}
  
void
freeGList(struct GENode **glist) {
  struct GENode *pge = (*glist);
  struct GENode *nextpge;

  while(NULL != pge){
    nextpge = pge->next;
    free(pge);
    pge = nextpge;
  }
}
