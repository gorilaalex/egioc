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
