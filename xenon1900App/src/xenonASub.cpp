/*************************************************************************\
|* xenonASub.c
|*
|*      Original Author : Jeong Han Lee
|*      Date            : Tuesday, May 23 17:03:10 CEST 2017
|*
\*************************************************************************/
/**************************************************************************
|*                     COPYRIGHT NOTIFICATION
|**************************************************************************
|*  
|* THE FOLLOWING IS A NOTICE OF COPYRIGHT, AVAILABILITY OF THE CODE,
|* AND DISCLAIMER WHICH MUST BE INCLUDED IN THE PROLOGUE OF THE CODE
|* AND IN ALL SOURCE LISTINGS OF THE CODE.
|*
|**************************************************************************
|*
|* This software is distributed under the EPICS Open License Agreement which
|* can be found in the file, LICENSE, included with this distribution.
|*
\**************************************************************************/

#include <stdio.h>
#include <string.h>

#include "dbDefs.h"
#include "dbAccess.h"
#include "dbFldTypes.h"
#include "link.h"
#include "dbAddr.h"
#include "registryFunction.h"
#include "aSubRecord.h"
#include "epicsExport.h"
#include "epicsString.h"
#include "epicsTime.h"
#include "stringinRecord.h"

typedef struct InvDataType {
  char *serialnumber;
  char *formfactor;
  char *vendor;
  char *location;
  char *status;
  char *model;

} InvDataType;


InvDataType outData;
int       xenonDebug;


/* Predefined PREFIX for Barcode */
const char * const ff="FF"; /* Formfactor */
const char * const vd="VD"; /* Vendor     */
const char * const lo="LO"; /* Location   */
const char * const st="ST"; /* Status     */
const char * const mo="MO"; /* Model      */
  
  
const char * const sv="SV"; /* Save and overwrite the current scanned PVs */
const char * const cl="CL"; /* Clear any scanned PVs                      */
const char * const pd="PD"; /* Push the saved PVs to RDB                  */
const char * const pj="PJ"; /* Push the saved PVs to JIFA                 */
const char * const DJ="DJ"; /* Push the saved PVs to RDB and JIRA         */



static char * getLinkStrVal(DBLINK *dbLink);
static void InitInvDataType();
static int fillInvDataType(aSubRecord *pRecord);
static void printInvDataType(InvDataType iDtype);
static char *timeString(aSubRecord *pRecord);
//static int csvWrite(aSubRecord *pRecord);

/* 
  The following macro was removed from 3.15. 
  Until I find the proper way to do this,
  I will use the macro locally
 

 #define dbGetPdbAddrFromLink(PLNK) \
     ( ( (PLNK)->type != DB_LINK ) \
       ? 0 \
       : ( ( (struct dbAddr *)( (PLNK)->value.pv_link.pvt) ) ) )
*/

static char *getLinkStrVal(DBLINK *dbLink)
{
  if   (dbLink->type != DB_LINK) {
    return epicsStrDup("NO DB_LINK type");
  } else {
    DBADDR *pdbAddr       = (DBADDR*) dbLink->value.pv_link.pvt;
    stringinRecord * pRec = (stringinRecord*) pdbAddr->precord;
    return epicsStrDup(pRec -> val);
  }
}


static void InitInvDataType()
{
  outData.serialnumber = epicsStrDup("");
  outData.formfactor   = epicsStrDup("");
  outData.vendor       = epicsStrDup("");
  outData.location     = epicsStrDup("");
  outData.status       = epicsStrDup("");
  outData.model        = epicsStrDup("");
  return ;
} 

static int fillInvDataType(aSubRecord *pRecord)
{
  outData.serialnumber = getLinkStrVal(&pRecord->outa);
  outData.formfactor   = getLinkStrVal(&pRecord->outb);
  outData.vendor       = getLinkStrVal(&pRecord->outc);
  outData.location     = getLinkStrVal(&pRecord->outd);
  outData.status       = getLinkStrVal(&pRecord->oute);
  outData.model        = getLinkStrVal(&pRecord->outf);
  
  if(xenonDebug) printInvDataType(outData);
    
  return 0;
}

static void printInvDataType(InvDataType iDtype)
{
  printf("SN         is %s\n", iDtype.serialnumber);
  printf("Formfactor is %s\n", iDtype.formfactor);
  printf("Vendor     is %s\n", iDtype.vendor);
  printf("Location   is %s\n", iDtype.location);
  printf("Status     is %s\n", iDtype.status);
  printf("Model      is %s\n", iDtype.model);

  return;
}

/* 
 * TimeStamp has no accurate meaning, we only use this info
 * to create the csv file as the part of its filename. 
 * 
 * Epics time stamp is <undefined> at the first scan time, 
 * becaue the aSub record never be processed before. 
 * 
 * I force to use PINI in DB record, to get time at first. 
 * 
*/
static char *timeString(aSubRecord *pRecord)
{
  epicsTime et = (epicsTimeStamp) pRecord->time;
  char buf[80];
  const char * pFormat = "%Y%m%d_%H%M%S";
  et.strftime(buf, sizeof(buf), pFormat);
  return epicsStrDup(buf);
}	


// static int csvWrite(InvDataType iDtype)
// {
//   iDtype.serialnumber;
//   iDtype.formfactor;
//   iDtype.vendor;
//   iDtype.location;
//   iDtype.status;
//   iDtype.model;

//   return 0;
// }

static long InitXenonASub(aSubRecord *pRecord)
{
  InitInvDataType();
  return 0;
}

static long DistXenonASub(aSubRecord *pRecord)
{

  aSubRecord* prec = (aSubRecord*) pRecord;
    
  char *aval = (char*) prec->a;
  char id[3];
  strncpy(id, aval, 2);
  id[2] = '\0';

  epicsUInt32 id_hash = epicsStrHash(id,0);
  char      * fwd_val = epicsStrDup(aval);

  /*
   * The XENON 1900 Scanner triggers aSub record twice, still unclear why it is so, and how to fix it. 
   * So simply ignore the second trigger with no data
   */
  
  if (id_hash == 0) return 1;

  if(xenonDebug) {
    printf("%s id %s id StrHash %d, and fwr value %s.\n",
	   timeString(prec), id, id_hash, fwd_val);
  }
  
  /* Serial Number should be the last Output value U */
  /* strncpy((char *)prec->valb, aval, strlen(aval)); */
  
  if      ( epicsStrnCaseCmp(ff, aval, 2) == 0 ) prec->valb = fwd_val;
  else if ( epicsStrnCaseCmp(vd, aval, 2) == 0 ) prec->valc = fwd_val;
  else if ( epicsStrnCaseCmp(lo, aval, 2) == 0 ) prec->vald = fwd_val;
  else if ( epicsStrnCaseCmp(st, aval, 2) == 0 ) prec->vale = fwd_val;
  else if ( epicsStrnCaseCmp(mo, aval, 2) == 0 ) prec->valf = fwd_val;
  else if ( epicsStrnCaseCmp(cl, aval, 2) == 0 )
    {
      /* Send the empty string to all data PVs (ff,vd,lo,st,mo,sn) */
      fwd_val = epicsStrDup("");
      prec->valb = fwd_val; prec->valc = fwd_val;
      prec->vald = fwd_val; prec->vale = fwd_val;
      prec->valf = fwd_val; prec->vala = fwd_val;
    }
  else if ( epicsStrnCaseCmp(sv, aval, 2) == 0 )
    {
      fillInvDataType(prec);
      
      epicsString fileName;
      fileName.length=80;
      fileName.pString=epicsStrDup("");

      if( epicsStrCaseCmp("", outData.serialnumber) ) {
	sprintf(fileName.pString, "inv_data_at_%s.csv", timeString(prec));
	if (xenonDebug) printf ("printf %s\n", fileName.pString);
	FILE *ofp;
	ofp = fopen(fileName.pString, "w");
	if (ofp == NULL) {
	  printf("Error opening file!\n");
	} else {
	  fprintf(ofp, "SN,0,%s",  outData.serialnumber);
	  fprintf(ofp, ",%s",      outData.formfactor);
	  fprintf(ofp, ",%s",      outData.vendor);
	  fprintf(ofp, ",%s",      outData.location);
	  fprintf(ofp, ",%s",      outData.status);
	  fprintf(ofp, ",%s\n",    outData.model);
	  fclose(ofp);
	}
      } else {
	printf("SN is mandatory data, please scan SN!\n");
      }
      
    }
  else {
    prec->vala = fwd_val;
  }
  
  return 0;
}

epicsExportAddress(int, xenonDebug);
epicsRegisterFunction(InitXenonASub);
epicsRegisterFunction(DistXenonASub);

