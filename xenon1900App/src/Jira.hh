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


#include "ItemObject.hh"
#include "Jira_ess.hh"

//using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

  

class JiraProject
{
  
public:
  JiraProject();
  JiraProject(std::string projectUrl, std::string projectName, std::string issueName);
  virtual ~JiraProject();

  std::string CreateIssue(ItemObject& obj);
  std::string UpdateIssue(ItemObject& obj);
  std::string DeleteIssue(ItemObject& obj);
  std::string SearchIssue(ItemObject& obj);
  
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
  
  void SetProjectUrl(const std::string& url);
  void SetIssueIdOrKey(const std::string& id)   {fIssueIdOrKey = id;} ;
  void ClearIssueIdOrKey()                      {fIssueIdOrKey.clear();};

  const std::string GetKey()  const { return jKey;  };
  const std::string GetSelf() const { return jSelf; };
  const std::string GetHash() const { return jHash; };

  ItemObject  fItemObject;

  
private:
  
  std::string       fUrl;
  std::string       fIssueUrl;
  std::string       fCreateUrl;
  std::string       fBulkCreateUrl;
  std::string       fSearchUrl;


  
  // Dynamically change according to Issue ID or Issue Key
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

  CURL               *curl_obj;
  CURLcode           curl_res;
  struct curl_slist  *curl_headers;
  std::string        fCurlResponse;

  Json::Value        jResponse;
  bool               jParsingSuccess;
  Json::Value        jIssues;
  Json::Value        jErrors;
  bool               jErrorsStatus;

  std::string        jKey;
  std::string        jSelf;
  std::string        jHash;
  
  // Overwrite files


   
  std::string qr_name;
  std::string dm_name;
  std::string file_suffix;

  std::string qr_file; 
  std::string dm_file;

  //  std::vector<std::string> jKeyList;
  //  std::vector<std::string> jSelfList;
  
  void AddItem(ItemObject &in) { fItemObject = in; }; 

  void SetupDefaultHeaders();
  
  void SetCreateJsonData(int no, bool json_style);
  void SetUpdateJsonData();
  void SetSearchJsonData();

  bool SetCreateCurlData();
  bool GetCurlResponse();
  void SetUpdateCurlData();
  void SetSearchCurlData();

  void CreateBarcodes();
  bool AddBarcodesJira();

  
  // const std::string AttachmentsUrl() {
  //   if (jParsingSuccess) {
  //     fAttachmentsUrl = jSelf;
  //   }
  //   else {
  //     fAttachmentsUrl.append(fIssueUrl);
  //     fAttachmentsUrl.append("/");
  //     fAttachmentsUrl.append(fIssueIdOrKey);
  //   }
  //   fAttachmentsUrl.append("/attachements");
  //   return fAttachmentsUrl;
  // };
  
  
  // const std::string AttachmentsUrl(const std::string& issueIdOrKey) {
  //   if (jParsingSuccess) {
  //     fAttachmentsUrl = jSelf;
  //   }
  //   else {
  //     fAttachmentsUrl.append(fIssueUrl);
  //     fAttachmentsUrl.append("/");
  //     fAttachmentsUrl.append(issueIdOrKey);
  //   }
  //   fAttachmentsUrl.append("/attachements");
  //   return fAttachmentsUrl;
  // };

  
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
