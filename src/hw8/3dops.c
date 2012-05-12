#include "3dops.h"

#define PT_REALLOC_STEP 5

SpaceObjData
loadOBJFile(const char *fName) {
  SpaceObjData objData = {{NULL, 0}, {NULL, 0}};
  SpacePoint ptNew;
  TriangleIdDef trigInfo;
  FILE *dataFile = fopen(fName, "r");
  char lineBuff[512];
  char op;

  if(NULL != dataFile) {
    objData.pts.v = (SpacePoint *)calloc(PT_REALLOC_STEP, sizeof(SpacePoint));
    objData.trigs.v = (TriangleIdDef *)calloc(PT_REALLOC_STEP, sizeof(TriangleIdDef));

    while(fgets(lineBuff, sizeof(lineBuff), dataFile) != NULL) {
      if(strlen(lineBuff) != 0 && lineBuff[0] != '#') {
	/* We have a valid OBJ data on current line, parse it */
	op = lineBuff[0];
        /* Pull out the 'op' char from the current line */
        strcpy(lineBuff, lineBuff + 2);
	switch(op) {
        case VERTEX_OP:
          sscanf(lineBuff, "%lf %lf %lf", &ptNew.x, &ptNew.y, &ptNew.z);
          objData.pts.v[objData.pts.length++] = ptNew;
          if(objData.pts.length % PT_REALLOC_STEP == 0) {
            objData.pts.v = (SpacePoint *)realloc(objData.pts.v, (objData.pts.length + PT_REALLOC_STEP) * sizeof(SpacePoint));
          }
          break;
        case DEFINE_TRIANGLE:
          sscanf(lineBuff, "%d %d %d", &trigInfo.fId, &trigInfo.sId, &trigInfo.tId);
          objData.trigs.v[objData.trigs.length++] = trigInfo;
          if(objData.trigs.length % PT_REALLOC_STEP == 0) {
            objData.trigs.v = (TriangleIdDef *)realloc(objData.trigs.v, (objData.trigs.length + PT_REALLOC_STEP) * sizeof(TriangleIdDef));
          }
          break;
        }
      }
    }

    fclose(dataFile);
  }

  return objData;
}

void 
freeOBJFile(SpaceObjData oData) {
  free(oData.pts.v);
  free(oData.trigs.v);
}

SpaceViewSettings
loadVWFile(const char *fName) {
  SpaceViewSettings vwSett;
  FILE *viewFile = fopen(fName, "r");
  char dataStruct[20];

  if(NULL != viewFile) {
    while(!feof(viewFile) && fscanf(viewFile, "%s", dataStruct)) {
      if(strcmp(dataStruct, "VRP") == 0)  fscanf(viewFile, "%lf %lf %lf", &vwSett.VRP.x, &vwSett.VRP.y, &vwSett.VRP.z);
      else if(strcmp(dataStruct, "VPN") == 0)  fscanf(viewFile, "%lf %lf %lf", &vwSett.VPN.x, &vwSett.VPN.y, &vwSett.VPN.z);
      else if(strcmp(dataStruct, "VUP") == 0)  fscanf(viewFile, "%lf %lf %lf", &vwSett.VUP.x, &vwSett.VUP.y, &vwSett.VUP.z);
      else if(strcmp(dataStruct, "PRP") == 0)  fscanf(viewFile, "%lf %lf %lf", &vwSett.PRP.x, &vwSett.PRP.y, &vwSett.PRP.z);
      else if(strcmp(dataStruct, "UVMIN") == 0)  fscanf(viewFile, "%lf %lf", &vwSett.UVMINx, &vwSett.UVMINy);
      else if(strcmp(dataStruct, "UVMAX") == 0)  fscanf(viewFile, "%lf %lf", &vwSett.UVMAXx, &vwSett.UVMAXy);
      else if(strcmp(dataStruct, "PTYPE") == 0)  fscanf(viewFile, "%d", &vwSett.PTYPE);
    }

    fclose(viewFile);
  }

  return vwSett;
}
