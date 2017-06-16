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

  fUserName.clear();
  fDescription.clear();
  fItemObject.Init();
  
  curl_headers = NULL;
};

JiraProject::~JiraProject()
{
};



JiraProject::JiraProject(std::string projectUrl, std::string projectName, std::string issueName)
{

  SetProjectUrl(projectUrl);
  fProjectName     = projectName;
  fIssueName       = issueName;
  fIssueIdOrKey.clear();

  fUserName.clear();
  fDescription.clear();
    
  fItemObject.Init();

  curl_headers = NULL;
};


void
JiraProject::SetProjectUrl(const std::string& url)
{
  fUrl             = url + "/rest/api/2";
  fIssueUrl        = fUrl + "/issue";
  fCreateUrl       = fIssueUrl;
  fBulkCreateUrl   = fIssueUrl + "/bulk";
  fUpdateDeleteUrl.clear();
  fAttachmentsUrl.clear();
  fSearchUrl       = url + "/search";
};



std::string 
JiraProject::CreateIssue(ItemObject& obj)
{
  std::string jira_return_message;

  // Currently, we only consider one obj per a jira submit 
  int obj_count = 0;
  
  this->AddItem(obj);
  this->SetCreateJsonData(obj_count, true);
  
  this->SetCreateCurlData();
  std::cout << jRootJsonData << std::endl;
  std::cout << obj << std::endl;
  obj.Init();
  return jira_return_message;
  
};



std::string 
JiraProject::UpdateIssue(ItemObject& obj)
{
  std::string jira_return_message;
  
  //  this->AddItem(obj);

  return jira_return_message;
  
};



std::string 
JiraProject::DeleteIssue(ItemObject& obj)
{
  std::string jira_return_message;
  
  //  this->AddItem(obj);


  return jira_return_message;
  
};



std::string 
JiraProject::SearchIssue(ItemObject& obj)
{
  std::string jira_return_message;
  
  //  this->AddItem(obj);


  return jira_return_message;
  
};


void
JiraProject::SetCreateJsonData(int no, bool json_style)
{
  Json::Value       item;
  Json::Value       fields;
  Json::Value       labels;

  jRoot.clear();

  jRoot["issueUpdates"]                = Json::Value::null;
  
  fields["project"]["key"]             = fProjectName.c_str();
  fields["issuetype"]["name"]          = fIssueName.c_str();
  
  fields["summary"]                    = fItemObject.GetName();

  /* Location could be empty, in that case, drop this field */
  /* Location also is needed to make a lookup table
   * from code, in csv to jira inventory list...
   * So, I disable it temporary now.
   */
  
  // if ( fItemObject.HasLocation() )
  //   fields["customfield_10502"]["value"] = fItemObject.GetLocation();
  
  fields["customfield_10500"]          = fItemObject.GetSerialNumber();
  fields["customfield_11002"]          = fItemObject.GetCharHashID();

  if ( fItemObject.HasFormfactor() )  {
    labels.append(fItemObject.GetFormfactor());
    fields["labels"]                     = labels;
  }
  
  if (! fUserName.empty() ) 
    fields["assignee"]["name"]           = fUserName.c_str();
  if (! fDescription.empty() ) 
    fields["description"]                = fDescription.c_str();
  
  item["fields"]                       = fields;

  // Only one item object into a jira json file

  jRoot["issueUpdates"][no]              = item;

  if (json_style) jRootJsonData = jStyledWriter.write(jRoot);
  else            jRootJsonData = jFasterWriter.write(jRoot);
  

  return;
}

void
JiraProject::SetupCurlHeaders()
{
  curl_headers = curl_slist_append(curl_headers, "Accept: application/json");
  curl_headers = curl_slist_append(curl_headers, "Content-Type:application/json");
  curl_headers = curl_slist_append(curl_headers, auth_headers.c_str());
  
  return;
}



void
JiraProject::SetCreateCurlData()
{
  curl_obj = curl_easy_init();
  if(curl_obj) {
    SetupCurlHeaders();

    curl_easy_setopt(curl_obj, CURLOPT_URL, fBulkCreateUrl.c_str());
    
    curl_easy_setopt(curl_obj, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl_obj, CURLOPT_HTTPHEADER, curl_headers);
    curl_easy_setopt(curl_obj, CURLOPT_POSTFIELDS, jRootJsonData.c_str());
    
    curl_easy_setopt(curl_obj, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curl_obj, CURLOPT_WRITEFUNCTION, CurlWriteToString);
    curl_easy_setopt(curl_obj, CURLOPT_WRITEDATA, &fCurlResponse);
    
    curl_res = curl_easy_perform(curl_obj); 
    if(curl_res != CURLE_OK)  fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(curl_res));
    
    curl_easy_cleanup(curl_obj);
    curl_slist_free_all(curl_headers);
  }

    
  Json::Value jsonFromString;
  Json::Reader reader;
  bool parsingSuccessful = reader.parse(fCurlResponse, jsonFromString);

  if (parsingSuccessful)
    {
      std::cout << jStyledWriter.write(jsonFromString) << std::endl;
      
      // for (int i = 0; i < count; i++)
      // 	{
      // 	  std::cout << "----" << std::endl;
      // 	  std::cout << jsonFromString["issues"][i]["id"]   << jsonFromString["issues"][i]["id"].asString() << std::endl;
      // 	  std::cout << jsonFromString["issues"][i]["key"]  << jsonFromString["issues"][i]["key"].asString() << std::endl;
      // 	  std::cout << jsonFromString["issues"][i]["self"] << jsonFromString["issues"][i]["self"].asString() << std::endl;
      // 	}
    }
  return;
}


void
JiraProject::SetUpdateCurlData()
{
  curl_obj = curl_easy_init();
  if(curl_obj) {
    
  }
    
  curl_easy_cleanup(curl_obj);
  
  return;
}


void
JiraProject::SetSearchCurlData()
{
  curl_obj = curl_easy_init();
  if(curl_obj) {
    
  }
    
  curl_easy_cleanup(curl_obj);
  
  return;
}
