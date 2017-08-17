#ifndef INC_JIRA_HH
#define INC_JIRA_HH

#include <iostream>
#include <sstream>
#include <cstdio>
#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <zint.h>

#include <cups/cups.h>
#include <cups/ppd.h>


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
#include "Jira_ess.hh"


#ifdef __cplusplus
extern "C" {
#endif

  

class JiraProject
{
  
public:
  JiraProject();
  
  virtual ~JiraProject();


  std::string CreateIssue();
  std::string UpdateIssue();
  std::string DeleteIssue();
   
  //  void DefineChild();
  //  void DefineParent();

  const std::string GetUrl()             const { return fUrl; };
  const std::string GetCreateUrl()       const { return fCreateUrl; };
  const std::string GetBulkCreateUrl()   const { return fBulkCreateUrl; };
  const std::string GetSearchUrl()       const { return fSearchUrl; };

  const std::string GetUpdateDeleteUrl() {
    fUpdateDeleteUrl.append(fIssueUrl);
    fUpdateDeleteUrl.append("/");
    fUpdateDeleteUrl.append(fIssueIdOrKey);
    return fUpdateDeleteUrl;
  };


  const std::string GetUpdateDeleteUrl(const std::string& issueIdOrKey) {
    fUpdateDeleteUrl.append(fIssueUrl);
    fUpdateDeleteUrl.append("/");
    fUpdateDeleteUrl.append(issueIdOrKey);
    return fUpdateDeleteUrl;
  };
  

  void SetIssueIdOrKey(const std::string& id);   
  const std::string GetIssueIdOrKey() { return fIssueIdOrKey;};
  void ClearIssueIdOrKey()                      {fIssueIdOrKey.clear();};

  const std::string GetKey()  const { return jKey;  };
  const std::string GetSelf() const { return jSelf; };
  const std::string GetHash() const { return jHash; };

  void AddObj(ItemObject &in);
  void Print();
  
private:

  ItemObject        fItemObject;
     
  std::string       fUrl;
  std::string       fIssueUrl;
  std::string       fCreateUrl;
  std::string       fBulkCreateUrl;
  std::string       fSearchUrl;
  
  // Dynamically change according to Issue ID or Issue Key

  std::string       fIssueIdOrKey_url;
  std::string       fUpdateDeleteUrl;
  std::string       fAttachmentsUrl;


  
  std::string       fProjectName;
  std::string       fIssueName;

  std::string       fIssueIdOrKey;
  std::string       fUserName;
  std::string       fDescription;

  
  Json::StyledWriter jStyledWriter;
  Json::FastWriter   jFasterWriter;
  Json::Value        jRoot;
  std::string        jRootJsonData;

  CURL               *curl_obj;      // Cleanup after executing curl in any function
  CURLcode           curl_res;        
  struct curl_slist  *curl_headers;  // free it after executing curl in any function
  std::string        fCurlResponse;  // Clear it after the main action. 


  Json::Value        jResponse;
  Json::Value        jIssues;
  Json::Value        jErrors;
  bool               jParsingSuccess;
  bool               jErrorsStatus;

  std::string        jKey;
  std::string        jSelf;
  std::string        jHash;

  std::string        jIssueDoesNotExist;
  
  // Overwrite files


   
  std::string qr_name;
  std::string dm_name;
  std::string file_suffix;

  std::string qr_file; 
  std::string dm_file;


  void SetProjectUrl  (const std::string& url);
  void SetProjectName (const std::string& proj_name)   { fProjectName = proj_name; };
  void SetIssueName   (const std::string& issue_name)  { fIssueName   = issue_name; };

  
  void ClearActionResults();
  
  void SetupDefaultHeaders();
  
  void SetCreateJsonData(bool json_style);
  bool SetCreateCurlData();
  bool GetCreateCurlResponse();
  
  void SetUpdateJsonData(bool json_style);
  bool SetUpdateCurlData();
  std::string GetUpdateCurlResponse();

  bool SetDeleteCurlData();
  std::string GetDeleteCurlResponse();

  void SetIssueIdOrKeyUrl(std::string url) { fIssueIdOrKey_url = url;};
  
  void CreateBarcodes();
  void CreateUpdateBarcodes(std::string issue_id, std::string hash_id);
  bool AddBarcodesJira();
  void PrintBarcodes();
  
  void cups_jobs_status(const char* printer_name, int job_id);
  bool cups_printer_status(const char* printer_name);
  
  
  static size_t CurlWriteToString(void *ptr, size_t size, size_t count, void *stream)
  {
    ((std::string*)stream)->append((char*)ptr, 0, size*count);
    return size*count;
  }

};

  
#ifdef __cplusplus
}
#endif




#endif
