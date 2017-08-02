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

  jRoot.clear();
  jResponse.clear();


  jKey.clear();
  jSelf.clear();
  jHash.clear();
  qr_name = "qr_symbol";
  dm_name = "dm_symbol";
  file_suffix = ".png";
  qr_file = qr_name + file_suffix;
  dm_file = dm_name + file_suffix;
   
  
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

  jRoot.clear();
  jResponse.clear();
  
  jKey.clear();
  jSelf.clear();
  jHash.clear();
  qr_name = "qr_symbol";
  dm_name = "dm_symbol";
  file_suffix = ".png";
  qr_file = qr_name + file_suffix;
  dm_file = dm_name + file_suffix;
  
  
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
  //
  int obj_count = 0;
  
  this->AddItem(obj);
  this->SetCreateJsonData(obj_count, true);
  this->SetCreateCurlData();
  this->GetCurlResponse();
  this->CreateBarcodes();
  this->AddBarcodesJira();
  
  // std::cout << jRootJsonData << std::endl;
  // std::cout << obj << std::endl;
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
  /* Jira Location should be redefined in order to match the Barcode
   * which we will use for..
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
JiraProject::SetupDefaultHeaders()
{
  curl_headers = curl_slist_append(curl_headers, "Accept: application/json");
  curl_headers = curl_slist_append(curl_headers, "Content-Type:application/json");
  curl_headers = curl_slist_append(curl_headers, auth_headers.c_str());
  
  return;
}


// true  : jira returns through curl jira returns issues and errors 
// false : curl cannot be executed due to init fail or curl_easy_perform

bool 
JiraProject::SetCreateCurlData()
{
  //  curl_global_init(CURL_GLOBAL_ALL);
  
  curl_obj = curl_easy_init();
  
  if(curl_obj) {
    SetupDefaultHeaders();

    curl_easy_setopt(curl_obj, CURLOPT_URL, fBulkCreateUrl.c_str());
    
    curl_easy_setopt(curl_obj, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl_obj, CURLOPT_HTTPHEADER, curl_headers);
    curl_easy_setopt(curl_obj, CURLOPT_POSTFIELDS, jRootJsonData.c_str());
    
    //    curl_easy_setopt(curl_obj, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curl_obj, CURLOPT_WRITEFUNCTION, CurlWriteToString);
    curl_easy_setopt(curl_obj, CURLOPT_WRITEDATA, &fCurlResponse);
    
    curl_res = curl_easy_perform(curl_obj); 
    if(curl_res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(curl_res));
    }
    
    curl_easy_cleanup(curl_obj);
    curl_slist_free_all(curl_headers);
    if(curl_res != CURLE_OK) return false;
    
  }
  else {
    return false;
  }

  return true;
}

// bool
// JiraProject::GetCurlResponse()
// {
//   Json::Reader reader;
//   unsigned int i;
  
//   jParsingSuccess = reader.parse(fCurlResponse, jResponse);
//   // it returns an empty array, so default value never show up
//   jErrors = jResponse.get("errors", "no show");
//   jIssues = jResponse.get("issues", "no show");
  
//   if (jErrors.size() == 0 ) jErrorsStatus = true;
//   else                      jErrorsStatus = false;

//   if (! jErrorsStatus) {
//     for(i=0; i < jIssues.size(); i++) {
//       jKeyList.push_back(jIssues[i].get("key", "").asString());
//       jSelfList.push_back(jIssues[i].get("self", "").asString());
//     }
//   }
  
//   return jParsingSuccess;
// }

// Currently, we only consider one obj per a jira submit
//

bool
JiraProject::GetCurlResponse()
{
  Json::Reader reader;
  
  jParsingSuccess = reader.parse(fCurlResponse, jResponse);
  // it returns an empty array, so default value never show up
  jErrors = jResponse.get("errors", "no show");
  jIssues = jResponse.get("issues", "no show");
  
  if (jErrors.size() == 0 ) jErrorsStatus = false;
  else                      jErrorsStatus = true;

  if (! jErrorsStatus) {
    jKey  = jIssues[0].get("key", "").asString();
    jSelf = jIssues[0].get("self", "").asString();
    jHash = fItemObject.GetCharHashID();
  }
  return jParsingSuccess;
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


void
JiraProject::CreateBarcodes()
{
  struct zint_symbol *qr_symbol;
  struct zint_symbol *dm_symbol;

  std::string keyWithoutTAG = jKey;  
  keyWithoutTAG.erase(jKey.find("TAG-"), 4);

  const char* hash = jHash.c_str();
  const char* key  = keyWithoutTAG.c_str();

  char qr_filename[FILENAME_MAX];
  char dm_filename[FILENAME_MAX];

  sprintf(qr_filename, "%s", qr_file.c_str());
  sprintf(dm_filename, "%s", dm_file.c_str());
  
  // QR Code
  qr_symbol = ZBarcode_Create();
  qr_symbol -> symbology        = BARCODE_QRCODE;
  qr_symbol -> height           = 48;
  qr_symbol -> whitespace_width = 6;
  qr_symbol -> border_width     = 2;
  qr_symbol -> input_mode = DATA_MODE;
  memcpy(qr_symbol->outfile, qr_filename, sizeof(qr_filename));
  memcpy(qr_symbol->text,    (uint8_t*) key, sizeof(key));
  ZBarcode_Encode_and_Print(qr_symbol, (uint8_t *) hash, 0 , 0);
  ZBarcode_Delete(qr_symbol);

  // DataMatrix 
  dm_symbol = ZBarcode_Create();
  dm_symbol -> symbology        = BARCODE_DATAMATRIX; 
  //  dm_symbol -> height           = 48;
  dm_symbol -> whitespace_width = 6;
  dm_symbol -> border_width     = 0;
  dm_symbol -> input_mode       = DATA_MODE;
  // can handle only 7 length char  1234567
  dm_symbol -> option_3         = DM_SQUARE;
  memcpy(dm_symbol->outfile, dm_filename, sizeof(dm_filename));
  memcpy(dm_symbol->text, (uint8_t*) key, sizeof(key));
  ZBarcode_Encode_and_Print(dm_symbol, (uint8_t *) hash, 0 , 0);
  ZBarcode_Delete(dm_symbol);
  
  return;
}


bool
JiraProject::AddBarcodesJira()
{

  std::cout << "\nAddBarcodesJira : \n" << std::endl;


  fAttachmentsUrl = jSelf + "/attachments";

  std::cout << fAttachmentsUrl << std::endl;
  
  struct curl_httppost* post = NULL;
  struct curl_httppost* last = NULL;


  //  curl_global_init(CURL_GLOBAL_ALL);

  curl_formadd(&post, &last,
	       CURLFORM_COPYNAME, "file",
	       CURLFORM_FILE, qr_file.c_str(),
	       CURLFORM_FILE, dm_file.c_str(),
	       CURLFORM_END);
  curl_formadd(&post, &last, 
	       CURLFORM_COPYNAME, "name",
	       CURLFORM_COPYCONTENTS, qr_name.c_str(),
	       CURLFORM_COPYCONTENTS, dm_name.c_str(),
	       CURLFORM_END);
  
  curl_obj = curl_easy_init();
  
  if(curl_obj) {

    curl_headers = NULL;
    
    curl_headers = curl_slist_append(curl_headers, "X-Atlassian-Token: no-check");
    curl_headers = curl_slist_append(curl_headers, auth_headers.c_str());
    
    curl_easy_setopt(curl_obj, CURLOPT_URL, fAttachmentsUrl.c_str());
    curl_easy_setopt(curl_obj, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl_obj, CURLOPT_HTTPHEADER, curl_headers);
        
    curl_easy_setopt(curl_obj, CURLOPT_HTTPPOST, post);

    curl_easy_setopt(curl_obj, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl_obj, CURLOPT_WRITEFUNCTION, CurlWriteToString);
    //
    // save the returned json file in fCurlResponse, however, don't process it
    curl_easy_setopt(curl_obj, CURLOPT_WRITEDATA, &fCurlResponse);


       
    curl_res = curl_easy_perform(curl_obj); 
    if(curl_res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(curl_res));
    }
    

    // Clean up all curl objects 
    curl_easy_cleanup(curl_obj);
    curl_formfree(post);
    curl_slist_free_all(curl_headers);
    
    if(curl_res != CURLE_OK) return false;
  }
  else {
    return false;
  }
  
  return true;
  
};
