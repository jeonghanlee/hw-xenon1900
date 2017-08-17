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
#include <fcntl.h>


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
#include "biRecord.h"

#include "Jira.hh"
#include "BcodeDefine.h"

//static InvDataType outData;
static int       xenonDebug;


typedef enum {DISABLED, ENABLED} PrintStatus;

static PrintStatus enabled = ENABLED;
static PrintStatus disabled = DISABLED;




static std::string jira_return_msg;

static char *timeString(aSubRecord *pRecord, const char *pFormat);
static char *timeStringSecond(aSubRecord *pRecord);

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
static char *timeString(aSubRecord *pRecord, const char *pFormat)
{
  epicsTime et = (epicsTimeStamp) pRecord->time;
  char buf[80];
  et.strftime(buf, sizeof(buf), pFormat);
  return epicsStrDup(buf);
}	

static char *timeStringSecond(aSubRecord *pRecord)
{
  const char * pFormat = "%Y%m%d_%H%M%S";
  return timeString(pRecord, pFormat);
}	

static long InitXenonASub(aSubRecord *pRecord)
{
  aSubRecord* prec = (aSubRecord*) pRecord;
  prec->valu = &enabled;
  return 0;
}

static long DistXenonASub(aSubRecord *pRecord)
{

  bool issue_id_status = false;
  
  aSubRecord* prec = (aSubRecord*) pRecord;
    
  char *aval     = (char*) prec->a;
  char *issue_id = (char*) prec->b;

  if ( issue_id && issue_id[0] ) issue_id_status = true;
    
  char id[3];
  strncpy(id, aval, 2);
  id[2] = '\0';

  epicsUInt32 id_hash      = epicsStrHash(id,0);
  char      * fwd_val      = epicsStrDup(aval);

  /*
   * The XENON 1900 Scanner triggers aSub record twice, still unclear why it is so, and how to fix it. 
   * So simply ignore the second trigger with no data. Return value is selected 0.
   * Return value has some dependency upon one of output (OUTU, now)
   */
  
  if (id_hash == 0) return 0;

  if(xenonDebug) {
    printf("%s id %s id StrHash %d, and scanned value %s.\n",
	   timeStringSecond(prec), id, id_hash, fwd_val);
  }
  
  /* Serial Number should be the last Output value U */
  /* strncpy((char *)prec->valb, aval, strlen(aval)); */
  if      ( epicsStrnCaseCmp(oo, aval, 2) == 0 ) ; /* ignore */
  else if ( epicsStrnCaseCmp(ff, aval, 2) == 0 ) prec->valb = fwd_val;
  else if ( epicsStrnCaseCmp(vd, aval, 2) == 0 ) prec->valc = fwd_val;
  else if ( epicsStrnCaseCmp(lo, aval, 2) == 0 ) prec->vald = fwd_val;
  else if ( epicsStrnCaseCmp(st, aval, 2) == 0 ) prec->vale = fwd_val;
  else if ( epicsStrnCaseCmp(mo, aval, 2) == 0 ) prec->valf = fwd_val;
  else if ( epicsStrnCaseCmp(le, aval, 2) == 0 ) prec->valu = &enabled;
  else if ( epicsStrnCaseCmp(ld, aval, 2) == 0 ) prec->valu = &disabled;
  else if ( epicsStrnCaseCmp(cl, aval, 2) == 0 )
    {
      /* Send the empty string to all data PVs (ff,vd,lo,st,mo,sn) */
      fwd_val    = epicsStrDup("");
      prec->valb = fwd_val; prec->valc = fwd_val;
      prec->vald = fwd_val; prec->vale = fwd_val;
      prec->valf = fwd_val; prec->vala = fwd_val;
      prec->valu = &disabled;
    }
  else if ( epicsStrnCaseCmp(jc, aval, 2) == 0 )  /* Create */
    {
      jira_return_msg.clear();
      
      ItemObject item(prec, url, project, issue);
 
      if( item.IsValid() ) {
	JiraProject jira;
	jira.AddObj(item);
	jira.Print();
	jira_return_msg = jira.CreateIssue();
	
      }
      else {
	jira_return_msg = "SN and NAME are mandatory data!";
      }
      prec->valg = (void*)jira_return_msg.c_str();
    }
  else if ( epicsStrnCaseCmp(ju, aval, 2) == 0 ) /* Update */ 
    {
      jira_return_msg.clear();

      if( issue_id_status ) {
	ItemObject item(prec, url, project, issue);
	if( item.IsValid() ) {
	  JiraProject jira;
	  jira.AddObj(item);
	  jira.SetIssueIdOrKey(issue_id);
	  jira_return_msg = jira.UpdateIssue();
	}
	else{
	  jira_return_msg = "SN and NAME are mandatory data!";
	}
      }
      else {
	jira_return_msg = "Define Issue Number for UPDATE!";
      }
      prec->valg = (void*)jira_return_msg.c_str();
    }
  else if ( epicsStrnCaseCmp(jd, aval, 2) == 0 ) /* Delete */
    {
      jira_return_msg.clear();

      if( issue_id_status ) {
	ItemObject item(prec, url, project, issue);
	JiraProject jira;
	jira.AddObj(item);
	jira.SetIssueIdOrKey(issue_id);
	jira_return_msg = jira.DeleteIssue();
      }
      else {
	jira_return_msg = "Define Issue Number for DELETE!";
      }
      prec->valg = (void*)jira_return_msg.c_str();
    }
  else
    {
      prec->vala = fwd_val;
    }

  return 0;

}

epicsExportAddress(int, xenonDebug);
epicsRegisterFunction(InitXenonASub);
epicsRegisterFunction(DistXenonASub);

