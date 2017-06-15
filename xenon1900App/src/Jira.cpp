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

  // Currently, we only consider one obj per a jira submit 
  int obj_count = 0;
  
  this->AddItem(obj);
  this->SetCreateJsonData(obj_count, true);

  cout << jRootJsonData << endl;


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


void
JiraProject::SetCreateJsonData(int no, bool json_style)
{
  Json::Value       item;
  Json::Value       fields;
  Json::Value       labels;

  jRoot.clear();

  JRoot["issueUpdates"]                = Json::Value::null;
  
  fields["project"]["key"]             = fProjectName.c_str();
  fields["issuetype"]["name"]          = fIssueName.c_str();
  
  fields["summary"]                    = fItemObject.GetName();
  fields["customfield_10502"]["value"] = fItemObject.GetLocation();
  fields["customfield_10500"]          = fItemObject.GetSerialNumber();
  fields["customfield_11002"]          = fHashID.c_str();
  
  labels.append(fFormfactor.c_str());
  
  fields["labels"]                     = labels;
  fields["assignee"]["name"]           = fUserName.c_str();
  fields["description"]                = fJiraDesc.c_str();
  
  item["fields"]                       = fields;

  // Only one item object into a jira json file

  jRoot["issueUpdates"][no]              = item;

  if (json_style) jRootJsonData = jStyledWriter.write(jRoot);
  else            jRootJsonData = jFasterWriter.write(jRoo);
  

  return;
}
 
