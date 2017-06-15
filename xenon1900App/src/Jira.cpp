#include "Jira.hh"


JiraProject::JiraProject()
{
  
  fUrl.clear();
  fIssueUrl.clear();
  fCreateUrl.clear();
  fBulkCreateUrl.clear();
  fSearchUrl.clear(); 
  fUpdateDeleteUrl.clear();
  fAttachmentsUrl.clear();

  fProjectName.clear(); 
  fIssueName.clear();   
  fIssueIdOrKey.clear();
 
};

JiraProject::~JiraProject()
{
};



JiraProject::JiraProject(string projectUrl, string projectName, string issueName)
{

  SetProjectUrl(projectUrl);
  fProjectName     = projectName;
  fIssueName       = issueName;
  fIssueIdOrKey.clear();
  
};


void
JiraProject::SetProjectUrl(const string& url)
{
  fUrl             = url + "/rest/api/2";
  fIssueUrl        = fUrl + "/issue";
  fCreateUrl       = fIssueUrl;
  fBulkCreateUrl   = fIssueUrl + "/bulk";
  fUpdateDeleteUrl.clear();
  fAttachmentsUrl.clear();
  fSearchUrl       = url + "/search";
};



string 
JiraProject::CreateIssue(const ItemObject& obj)
{
  string jira_return_message;
  
  this->AddItem(obj);


  return jira_return_message;
  
};



string 
JiraProject::UpdateIssue(const ItemObject& obj)
{
  string jira_return_message;
  
  this->AddItem(obj);

  return jira_return_message;
  
};



string 
JiraProject::DeleteIssue(const ItemObject& obj)
{
  string jira_return_message;
  
  this->AddItem(obj);


  return jira_return_message;
  
};



string 
JiraProject::SearchIssue(const ItemObject& obj)
{
  string jira_return_message;
  
  this->AddItem(obj);


  return jira_return_message;
  
};
