#include "Jira.hh"


JiraProject::JiraProject()
{
  
  fUrl             = "";
  fIssueUrl       = "";
  fCreateUrl       = "";
  fBulkCreateUrl   = "";
  fSearchUrl       = "";
  fUpdateDeleteUrl = "";
  fAttachmentsUrl  = "";

  fProjectName     = "";
  fIssueName       = "";
 
};

JiraProject::~JiraProject()
{
};



JiraProject::JiraProject(string projectUrl, string projectName, string issueName)
{

  SetProjectUrl(projectUrl);
  fProjectName     = projectName;
  fIssueName       = issueName;
  
};


void
JiraProject::SetProjectUrl(const string& url)
{
  fUrl             = url + "/rest/api/2";
  fIssueUrl        = fUrl + "/issue";
  fCreateUrl       = fIssueUrl;
  fBulkCreateUrl   = fIssueUrl + "/bulk";
  fUpdateDeleteUrl = "";
  fAttachmentsUrl  = "";
  fSearchUrl       = url + "/search";
};
