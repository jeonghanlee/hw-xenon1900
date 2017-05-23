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

#include <dbDefs.h>
#include <registryFunction.h>
#include <aSubRecord.h>
#include <epicsExport.h>
#include <epicsString.h>
#include <epicsTypes.h>

int xenonDebug;

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



static long InitXenonASub(aSubRecord *pRecord)
{
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

  if(xenonDebug) printf("id %s id StrHash %d, and fwr value %s.\n", id, id_hash, fwd_val);
  
 

  /* Serial Number should be the last Output value U */
  /* strncpy((char *)prec->valb, aval, strlen(aval)); */
  
  if      ( epicsStrnCaseCmp(ff, aval, 2) == 0 ) prec->valb = fwd_val;
  else if ( epicsStrnCaseCmp(vd, aval, 2) == 0 ) prec->valc = fwd_val;
  else if ( epicsStrnCaseCmp(lo, aval, 2) == 0 ) prec->vald = fwd_val;
  else if ( epicsStrnCaseCmp(st, aval, 2) == 0 ) prec->vale = fwd_val;
  else if ( epicsStrnCaseCmp(mo, aval, 2) == 0 ) prec->valf = fwd_val;
  else                                           prec->vala = fwd_val;
    
  return 0;
}

epicsExportAddress(int, xenonDebug);
epicsRegisterFunction(InitXenonASub);
epicsRegisterFunction(DistXenonASub);
