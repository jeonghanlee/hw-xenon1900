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

  string CreateIssue();
  string UpdateIssue();
  string DeleteIssue();
  string SearchIssue();

  //  void DefineChild();
  //  void DefineParent();
  
  const string GetUrl()             const { return fUrl; };
  const string GetCreateUrl()       const { return fCreateUrl; };
  const string GetBulkCreateUrl()   const { return fBulkCreateUrl; };
  const string GetSearchUrl()       const { return fSearchUrl; };

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

  
};

  
#ifdef __cplusplus
}
#endif


#endif
