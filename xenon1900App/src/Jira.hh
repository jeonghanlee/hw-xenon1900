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

using namespace std;
        
#ifdef __cplusplus
extern "C" {
#endif

  

class JiraProject
{
  
public:
  JiraProject();
  JiraProject(string projectUrl, string projectName, string issueName);
  virtual ~JiraProject();

  string CreateIssue(const ItemObject& obj);
  string UpdateIssue(const ItemObject& obj);
  string DeleteIssue(const ItemObject& obj);
  string SearchIssue(const ItemObject& obj);

  //  void DefineChild();
  //  void DefineParent();
  
  const string GetUrl()             const { return fUrl; };
  const string GetCreateUrl()       const { return fCreateUrl; };
  const string GetBulkCreateUrl()   const { return fBulkCreateUrl; };
  const string GetSearchUrl()       const { return fSearchUrl; };

  const string GetUpdateDeleteUrl() {
    fUpdateDeleteUrl.append(fIssueUrl);
    fUpdateDeleteUrl.append("/");
    fUpdateDeleteUrl.append(fIssueIdOrKey);
    return fUpdateDeleteUrl;
  };

  const string GetAttachmentsUrl() {
    fAttachmentsUrl.append(fIssueUrl);
    fAttachmentsUrl.append("/");
    fAttachmentsUrl.append(fIssueIdOrKey);
    fAttachmentsUrl.append("/attachements");
    return fAttachmentsUrl;
  };
  
  const string GetUpdateDeleteUrl(const string& issueIdOrKey) {
    fUpdateDeleteUrl.append(fIssueUrl);
    fUpdateDeleteUrl.append("/");
    fUpdateDeleteUrl.append(issueIdOrKey);
    return fUpdateDeleteUrl;
  };

  const string GetAttachmentsUrl(const string& issueIdOrKey) {
    fAttachmentsUrl.append(fIssueUrl);
    fAttachmentsUrl.append("/");
    fAttachmentsUrl.append(issueIdOrKey);
    fAttachmentsUrl.append("/attachements");
    return fAttachmentsUrl;
  };
  
  void SetProjectUrl(const string& url);
  void SetIssueIdOrKey(const string& id)   {fIssueIdOrKey = id;} ;
  void ClearIssueIdOrKey()                 {fIssueIdOrKey.clear();};


  
private:
  
  string       fUrl;
  string       fIssueUrl;
  string       fCreateUrl;
  string       fBulkCreateUrl;
  string       fSearchUrl;

  // Dynamically change according to Issue ID or Issue Key
  string       fUpdateDeleteUrl;
  string       fAttachmentsUrl;

  string       fProjectName;
  string       fIssueName;
  string       fIssueIdOrKey;


  ItemObject  fItemObject;
  
  void SetUpdateJsonData();
  void SetCreateJsonData();
  void SetSearchJsonData();
  void AddItem(const ItemObject& obj) { fItemObject = obj; }; 

};

  
#ifdef __cplusplus
}
#endif


#endif
