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


static const char * const sv="SV"; /* Save and overwrite each scanned PV in each csv file (per second)   */
static const char * const sj="SJ"; /* Save and overwrite each scanned PV in each json file (per second)  */

static const std::string project = "TAG";
static const std::string issue = "Hardware";
static const std::string url = "https://jira.esss.lu.se";
static std::string jira_return_msg;

static char * getLinkStrVal(DBLINK *dbLink);
static void InitInvDataType();
static int fillInvDataType(aSubRecord *pRecord);
static void printInvDataType(InvDataType iDtype);
static char *timeString(aSubRecord *pRecord, const char *pFormat);
static char *timeStringDay(aSubRecord *pRecord);
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
  outData.hash         = 0;
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
  outData.hash         = epicsStrHash(outData.serialnumber,0);
  
  if(xenonDebug) printInvDataType(outData);
    
  return 0;
}

static void printInvDataType(InvDataType iDtype)
{
  printf("Hash       is %u\n", iDtype.hash);
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
static char *timeString(aSubRecord *pRecord, const char *pFormat)
{
  epicsTime et = (epicsTimeStamp) pRecord->time;
  char buf[80];
  et.strftime(buf, sizeof(buf), pFormat);
  return epicsStrDup(buf);
}	

static char *timeStringDay(aSubRecord *pRecord)
{
  const char * pFormat = "%Y%m%d";
  return timeString(pRecord, pFormat);
}	


static char *timeStringSecond(aSubRecord *pRecord)
{
  const char * pFormat = "%Y%m%d_%H%M%S";
  return timeString(pRecord, pFormat);
}	



static char *checkStr(char *in)
{
  if ( epicsStrCaseCmp("", in) ) {
    return in;
  }
  else {
    return epicsStrDup(",,");
  }
}




static long InitXenonASub(aSubRecord *pRecord)
{
  InitInvDataType();
  aSubRecord* prec = (aSubRecord*) pRecord;
  prec->valu = &enabled;
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
  else if ( epicsStrnCaseCmp(sv, aval, 2) == 0 )
    {
      fillInvDataType(prec);

      ItemObject item (outData);
      if (xenonDebug) item.Print();
      item.GetJiraCSV();
      
      epicsString fileName;
      fileName.length=80;
      fileName.pString=epicsStrDup("");

      if( item.IsValid() ) {
	sprintf(fileName.pString, "inv_data_at_%s.csv", timeStringSecond(prec));
	if (xenonDebug) printf ("printf %s\n", fileName.pString);
	
	//	printf("%s\n", epicsMemHash(outData.serialnumber,10,0));
	
	FILE *ofp;
	ofp = fopen(fileName.pString, "w");
	if (ofp == NULL) {
	  printf("Error opening file!\n");
	} else {
	  /* Since each data have own prefix,number,string
	   * so, I introduce an additional seperatation as 
	   * ,-, in order to distingush them
	   */
	  const char * pHolder = ",--";
	  fprintf(ofp, "%s,0,%u",    hs, outData.hash);
	  fprintf(ofp, "%s,SN,0,%s", pHolder, outData.serialnumber);
	  fprintf(ofp, "%s,%s",      pHolder, checkStr(outData.formfactor));
	  fprintf(ofp, "%s,%s",      pHolder, checkStr(outData.vendor));
	  fprintf(ofp, "%s,%s",      pHolder, checkStr(outData.location));
	  fprintf(ofp, "%s,%s",      pHolder, checkStr(outData.status));
	  fprintf(ofp, "%s,%s\n",    pHolder, checkStr(outData.model));
	  fclose(ofp);
	}
      } else {
	printf("SN is mandatory data, please scan SN!\n");
      }
      
    }
  else if ( epicsStrnCaseCmp(pj, aval, 2) == 0 )
    {
      fillInvDataType(prec);
      ItemObject item (outData);
      if (xenonDebug) item.Print();
      
      epicsString fileName;
      fileName.length=80;
      fileName.pString=epicsStrDup("");

      /* To make the system work quicly and dirty, I decided to create the csv file, which
       * can be imported within the existent ICS Inventory system. Since I am not the admin
       * on this jira, I cannot update them. However, to create them through csv file
       * is the simple way to save time.... 
       * jhlee, Monday, May 29 18:02:20 CEST 2017
       */
      if( item.IsValid() ) {
	sprintf(fileName.pString, "create_issue_at_TAG_%s.csv", timeStringDay(prec));

	
	int file_exist = 0;

	
	if (fopen(fileName.pString, "r") == NULL) {
	  file_exist = 0;
	  if (xenonDebug) printf ("creating the non-existent %s\n", fileName.pString);
	}
	else {
	  file_exist = 1;
	  if (xenonDebug) printf ("opening the existent %s\n", fileName.pString);
	}

	FILE *ofp;

	const char * header = "Summary,Serial Number,Label ID,Labels,Supplier,Where";
	
	if(file_exist) {
	  ofp = fopen(fileName.pString, "a+");
	  fprintf(ofp, "%s", item.GetJiraCSV().c_str());
	  fclose(ofp);
	}
	else {
	  ofp = fopen(fileName.pString, "w");
	  fprintf(ofp, "%s\n", header);
	  fprintf(ofp, "%s", item.GetJiraCSV().c_str());
	  fclose(ofp);
	}
	
      } else {
	printf("SN and MO are mandatory data, please scan them!\n");
      }
      
    }
  else if ( epicsStrnCaseCmp(sj, aval, 2) == 0 )
    {
      fillInvDataType(prec);
      ItemObject item (outData);
      std::string desc="";
      
      desc = "Created at ";
      desc += timeStringSecond(prec);
      desc += " using EPICS Xenon IOC";
      
      item.SetJIRAInfo(project, issue, desc);
      if (xenonDebug) std::cout << item.GetJiraJSON().c_str() << std::endl;
      if( item.IsValid() ) {
	printf("is valid\n");
      }
      else {
	printf("SN and MO are mandatory data, please scan them!\n");
      }
    }
  else if ( epicsStrnCaseCmp(jc, aval, 2) == 0 )  /* Create */
    {
      jira_return_msg.clear();
      fillInvDataType(prec);
      ItemObject item (outData);
      JiraProject jira(url, project, issue);
      jira_return_msg = jira.CreateIssue(item);
      std::cout << jira.GetBulkCreateUrl() << std::endl;
      std::cout << jira_return_msg  << std::endl;
    }
  else if ( epicsStrnCaseCmp(ju, aval, 2) == 0 ) /* Update */ 
    {
      jira_return_msg.clear();
      fillInvDataType(prec);
      ItemObject item (outData);
      JiraProject jira(url, project, issue);
      jira.SetIssueIdOrKey("TAG-333");
      jira_return_msg = jira.UpdateIssue(item);
      std::cout << jira.GetUpdateDeleteUrl() << std::endl;
      std::cout << jira_return_msg  << std::endl;

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

