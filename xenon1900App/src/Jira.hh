#ifndef INC_JIRA_HH
#define INC_JIRA_HH

#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <zint.h>

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

  const std::string GetAttachmentsUrl() {
    fAttachmentsUrl.append(fIssueUrl);
    fAttachmentsUrl.append("/");
    fAttachmentsUrl.append(fIssueIdOrKey);
    fAttachmentsUrl.append("/attachements");
    return fAttachmentsUrl;
  };
  
  const std::string GetUpdateDeleteUrl(const std::string& issueIdOrKey) {
    fUpdateDeleteUrl.append(fIssueUrl);
    fUpdateDeleteUrl.append("/");
    fUpdateDeleteUrl.append(issueIdOrKey);
    return fUpdateDeleteUrl;
  };

  const std::string GetAttachmentsUrl(const std::string& issueIdOrKey) {
    fAttachmentsUrl.append(fIssueUrl);
    fAttachmentsUrl.append("/");
    fAttachmentsUrl.append(issueIdOrKey);
    fAttachmentsUrl.append("/attachements");
    return fAttachmentsUrl;
  };
  
  void SetProjectUrl(const std::string& url);
  void SetIssueIdOrKey(const std::string& id)   {fIssueIdOrKey = id;} ;
  void ClearIssueIdOrKey()                 {fIssueIdOrKey.clear();};

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

  void AddItem(ItemObject &in) { fItemObject = in; }; 

  void SetupCurlHeaders();
  void SetCreateJsonData(int no, bool json_style);
  void SetUpdateJsonData();
  void SetSearchJsonData();

  void SetCreateCurlData();
  void SetUpdateCurlData();
  void SetSearchCurlData();

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
