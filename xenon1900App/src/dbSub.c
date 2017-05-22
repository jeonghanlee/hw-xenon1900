#include <stdio.h>

#include <dbDefs.h>
#include <registryFunction.h>
#include <aSubRecord.h>
#include <epicsExport.h>

int mySubDebug;

const char ff[2]="FF";
const char vd[2]="VD";
const char lo[2]="LO";
const char st[2]="ST";
const char mo[2]="MO";


static long myAsubInit(aSubRecord *precord)
{
  if (mySubDebug)
    printf("Record %s called myAsubInit(%p)\n",
	   precord->name, (void*) precord);
  return 0;
}

static long myAsubProcess(aSubRecord *precord)
{
  if (mySubDebug)
    printf("Record %s called myAsubProcess(%p)\n", precord->name, (void*) precord);
  printf ("print out %s\n", (char *)precord->a);
  
  char inString[39] = "";
  strncpy(inString, (char *)precord->a, 39);
  
  if (strncmp(inString, ff, 2) == 0 ) {
    strncpy((char *)precord->vala,  inString, 39);
  }
  else if ( strncmp(inString, vd, 2) == 0 ) {
    strncpy((char *) precord->valb, inString, 39);
  }
  else {
    printf ("inString %s\n", inString);
  }
      
  return 0;
}

  epicsExportAddress(int, mySubDebug);
  epicsRegisterFunction(myAsubInit);
  epicsRegisterFunction(myAsubProcess);
