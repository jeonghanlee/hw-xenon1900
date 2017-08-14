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

#include "ItemObject.hh"
#include "Jira.hh"

static InvDataType outData;
static int       xenonDebug;
//typedef enum { ENABLED, DISABLED} PrintStatus;
typedef enum { DISABLED, ENABLED} PrintStatus;
static PrintStatus enabled = ENABLED;
static PrintStatus disabled = DISABLED;
/* Predefined PREFIX for Barcode */
static const char * const ff="FF"; /* Formfactor */
static const char * const vd="VD"; /* Vendor     */
static const char * const lo="LO"; /* Location   */
static const char * const st="ST"; /* Status     */
static const char * const mo="MO"; /* Model      */
static const char * const oo="00"; /* Place holder */

static const char * const hs="HS"; /* Hash number per each SN                    */
static const char * const cl="CL"; /* Clear all scanned PVs                         */

static const char * const le="LE"; /* Enable  Label Printing after JIRA action (JC) */
static const char * const ld="LD"; /* Disable Label Printing after JIRA action (JC) */

static const char * const jc="JC"; /* Create an JIRA issue                          */
static const char * const ju="JU"; /* Update an JIRA issue (Scan Hash ID and other fields first) */
static const char * const jd="JD"; /* Delete an JIRA issue (Scan Hash ID and other fields first) */
static const char * const js="JS"; /* Search an JIRA issue (Scan Hash ID and other fields first) */

static const char * const ji="JI"; /* Define the Child (Scan Hash ID later)         */
static const char * const jp="JP"; /* Define the Parent (Scan Hash ID later)        */

static const char * const pj="PJ"; /*Save and append each scanned PV to CSV file which JIRA can import  (per day) */
static const char * const dj="DJ"; /* Push the saved PVs to RDB and JIRA         */
static const char * const pd="PD"; /* Push the saved PVs to RDB                  */


static std::string jira_return_msg;

static char * getLinkStrVal(DBLINK *dbLink);
static void InitInvDataType();
static int fillInvDataType(aSubRecord *pRecord);
// static void printInvDataType(InvDataType iDtype);
static char *timeString(aSubRecord *pRecord, const char *pFormat);
// static char *timeStringDay(aSubRecord *pRecord);
static char *timeStringSecond(aSubRecord *pRecord);


/* 
   The following macro was removed from 3.15. 
   Until I find the proper way to do this,
   I will use the macro locally
 

   #define dbGetPdbAddrFromLink(PLNK) \
   ( ( (PLNK)->type != DB_LINK ) \
   ? 0 \
   : ( ( (struct dbAddr *)( (PLNK)->value.pv_link.pvt) ) ) )
*/


/* We get all values as string */
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


static epicsEnum16 getLinkVal(DBLINK *dbLink)
{
  if   (dbLink->type != DB_LINK) {
    return disabled;
  } else {
    DBADDR *pdbAddr = (DBADDR*) dbLink->value.pv_link.pvt;
    biRecord * pRec = (biRecord*) pdbAddr->precord;
    return pRec -> val;
  }
}



static void InitInvDataType()
{
  outData.hash         = 0;
  outData.serialnumber = epicsStrDup("");
  outData.formfactor   = epicsStrDup("");
  outData.vendor       = epicsStrDup("");
  outData.location     = epicsStrDup("");
  outData.status       = epicsStrDup("");
  outData.model_name   = epicsStrDup("");
  outData.label        = enabled;
  return ;
} 

static int fillInvDataType(aSubRecord *pRecord)
{

  std::string tmp_str;
  
  outData.serialnumber = getLinkStrVal(&pRecord->outa);
  outData.formfactor   = getLinkStrVal(&pRecord->outb);
  outData.vendor       = getLinkStrVal(&pRecord->outc);
  outData.location     = getLinkStrVal(&pRecord->outd);
  outData.status       = getLinkStrVal(&pRecord->oute);
  outData.model_name   = getLinkStrVal(&pRecord->outf);
  /* Likely have the chance to have the same serial number */
  tmp_str = outData.serialnumber;
  tmp_str += outData.model_name;
  outData.hash         = epicsStrHash(tmp_str.c_str(),0);
  outData.label        = getLinkVal(&pRecord->outu);
  
  // printInvDataType(outData);
    
  return 0;
}

// static void printInvDataType(InvDataType iDtype)
// {
//   printf("Hash       is %u\n", iDtype.hash);
//   printf("SN         is %s\n", iDtype.serialnumber);
//   printf("Formfactor is %s\n", iDtype.formfactor);
//   printf("Vendor     is %s\n", iDtype.vendor);
//   printf("Location   is %s\n", iDtype.location);
//   printf("Status     is %s\n", iDtype.status);
//   printf("Model      is %s\n", iDtype.model_name);

//   return;
// }

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

// static char *timeStringDay(aSubRecord *pRecord)
// {
//   const char * pFormat = "%Y%m%d";
//   return timeString(pRecord, pFormat);
// }	


static char *timeStringSecond(aSubRecord *pRecord)
{
  const char * pFormat = "%Y%m%d_%H%M%S";
  return timeString(pRecord, pFormat);
}	



// static char *checkStr(char *in)
// {
//   if ( epicsStrCaseCmp("", in) ) {
//     return in;
//   }
//   else {
//     return epicsStrDup(",,");
//   }
// }




static long InitXenonASub(aSubRecord *pRecord)
{
  InitInvDataType();
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
      fwd_val = epicsStrDup("");
      prec->valb = fwd_val; prec->valc = fwd_val;
      prec->vald = fwd_val; prec->vale = fwd_val;
      prec->valf = fwd_val; prec->vala = fwd_val;
      prec->valu = &disabled;
    }
  else if ( epicsStrnCaseCmp(jc, aval, 2) == 0 )  /* Create */
    {
      jira_return_msg.clear();
      
      fillInvDataType(prec);
      ItemObject item (outData);
      if( item.IsValid() ) {
       	JiraProject jira(url, project, issue, prec);
	jira_return_msg = jira.CreateIssue(item);
      }
      else {
	jira_return_msg = "SN and NAME are mandatory data!";
	prec->valg = (void*)jira_return_msg.c_str();
      }
	
    }
  else if ( epicsStrnCaseCmp(ju, aval, 2) == 0 ) /* Update */ 
    {
      jira_return_msg.clear();

      if( issue_id_status ) {
	fillInvDataType(prec);
	ItemObject item (outData);
	JiraProject jira(url, project, issue);
	jira.SetIssueIdOrKey(issue_id);
	std::cout << jira.GetIssueIdOrKey() << std::endl;
	jira_return_msg = jira.UpdateIssue(item);
	std::cout << jira.GetUpdateDeleteUrl() << std::endl;
	//      std::cout << jira_return_msg  << std::endl;
      }
      else {
	jira_return_msg = "Define Issue Number for Update!";
	prec->valg = (void*)jira_return_msg.c_str();
      }

    }
  else if ( epicsStrnCaseCmp(jd, aval, 2) == 0 ) /* Delete */
    {
      jira_return_msg.clear();
      fillInvDataType(prec);
      ItemObject item (outData);
      JiraProject jira(url, project, issue);
      jira_return_msg = jira.DeleteIssue(item);
      std::cout << jira.GetUpdateDeleteUrl("TAG-334") << std::endl;
      std::cout << jira_return_msg  << std::endl;

    }
  else if ( epicsStrnCaseCmp(js, aval, 2) == 0 ) /* Search */ 
    {
      jira_return_msg.clear();
      fillInvDataType(prec);
      ItemObject item (outData);
      JiraProject jira(url, project, issue);
      jira_return_msg = jira.SearchIssue(item);
            std::cout << jira.GetSearchUrl() << std::endl;
      std::cout << jira_return_msg  << std::endl;
	    
    }
  else if ( epicsStrnCaseCmp(ji, aval, 2) == 0 )
    {
      //     fillInvDataType(prec);
      //     // ItemObject item (outData);
      //     JiraProject jira(url, project, issue);
      //     std::cout << jira.GetCreateUrl() << std::endl;
    }
  else if ( epicsStrnCaseCmp(jp, aval, 2) == 0 )
    {
      //     fillInvDataType(prec);
      //     // ItemObject item (outData);
      //     JiraProject jira(url, project, issue);
      //     std::cout << jira.GetCreateUrl() << std::endl;
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

