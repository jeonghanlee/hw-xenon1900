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



void
JiraProject::ClearActionResults()
{
  curl_headers = NULL;
  fCurlResponse.clear();

  fUpdateDeleteUrl.clear();
  fAttachmentsUrl.clear();
  
  jRoot.clear();
  jResponse.clear();
  jIssues.clear();
  jErrors.clear();
  jParsingSuccess = false;
  jErrorsStatus   = false;  
  jKey.clear();
  jSelf.clear();
  jHash.clear();
  
};


void
JiraProject::AddObj(ItemObject &in)
{
  
  fItemObject = in;

  this->SetProjectUrl(fItemObject.GetJiraUrl());
  this->SetProjectName(fItemObject.GetJiraProject());
  this->SetIssueName(fItemObject.GetJiraIssue());

  return;
}


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

  return;
};



std::string 
JiraProject::CreateIssue()
{
  std::string jira_return_message;

  // Currently, we only consider one obj per a jira submit

  int obj_count = 0;
  this->SetCreateJsonData(obj_count, true);

  std::cout << jRootJsonData << std::endl;

  this->SetCreateCurlData();
  this->GetCurlResponse();
  this->CreateBarcodes();
  this->AddBarcodesJira();
  if( fItemObject.IsLabel())  PrintBarcodes();
  this->ClearActionResults();

  fItemObject.Init();
  
  return jira_return_message;
};



std::string 
JiraProject::UpdateIssue()
{
  std::string jira_return_message;
  
  // this->SetUpdateJsonData(true);
  // std::cout << jRootJsonData << std::endl;
  // //  this->SetCreateCurlData();
  // //  this->GetCurlResponse();
  // this->CreateUpdateBarcodes(fIssueIdOrKey, obj.GetHashID());
  // //  this->AddBarcodesJira();
  // if( obj.IsLabel())  PrintBarcodes();
  // this->ClearActionResults();

  // obj.Init();
  return jira_return_message;
  
};



std::string 
JiraProject::DeleteIssue()
{
  std::string jira_return_message;
  
  //  this->AddItem(obj);


  return jira_return_message;
  
};



std::string 
JiraProject::SearchIssue()
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
  
  fields["summary"]                    = fItemObject.GetCharName();

  /* Location could be empty, in that case, drop this field */
  /* Location also is needed to make a lookup table
   * from code, in csv to jira inventory list...
   * So, I disable it temporary now.
   */
  /* Jira Location should be redefined in order to match the Barcode
   * which we will use for..
   */
  
  // if ( fItemObject.HasLocation() )
  //   fields["customfield_10502"]["value"] = fItemObject.GetCharLocation();
  
  fields["customfield_10500"]          = fItemObject.GetCharSerialNumber();
  fields["customfield_11002"]          = fItemObject.GetCharHashID();

  if ( fItemObject.HasFormfactor() )  {
    labels.append(fItemObject.GetCharFormfactor());
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


void
JiraProject::SetUpdateJsonData(bool json_style)
{

  int no = 0;
  // 01 
  Json::Value j01_update;
  // 012
  Json::Value j012_summary;
  Json::Value j012_cfield10500;
  Json::Value j012_cfield11002;
  Json::Value j012_desc;

  // 012
  Json::Value j012_cfield10502;
  Json::Value j012_assignee;
  
  // 0123
  Json::Value j0123_name;
  Json::Value j0123_value;
  
  jRoot.clear();

  // Data > 0123
  j0123_name             ["name"] = "ics_inventory";
  if ( fItemObject.HasLocation() ) 
    j0123_value          ["value"] = fItemObject.GetLocation();

  // 0123 > 012
  j012_assignee       [no]["set"] = j0123_name;
  j012_cfield10502    [no]["set"] = j0123_value;
  // Data > 012
  j012_summary        [no]["set"] = fItemObject.GetName();
  j012_cfield10500    [no]["set"] = fItemObject.GetSerialNumber();
  j012_cfield11002    [no]["set"] = fItemObject.GetCharHashID();
  j012_desc           [no]["set"] = fDescription.c_str();
  
  // 012 > 01
  j01_update["summary"]           = j012_summary;
  j01_update["description"]       = j012_desc;
  j01_update["assignee"]          = j012_assignee;
  j01_update["customfield_10500"] = j012_cfield10500;
  j01_update["customfield_10502"] = j012_cfield10502;
  j01_update["customfield_11002"] = j012_cfield11002;
  
  // 01 > ROOT
  jRoot["update"] = j01_update;

  if (json_style) jRootJsonData = jStyledWriter.write(jRoot);
  else            jRootJsonData = jFasterWriter.write(jRoot);
  
  return;
}


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
  
  /* QR Code */
  qr_symbol = ZBarcode_Create();
  qr_symbol -> symbology        = BARCODE_QRCODE;
  qr_symbol -> height           = 48;
  qr_symbol -> whitespace_width = 6;
  qr_symbol -> border_width     = 2;
  qr_symbol -> input_mode       = DATA_MODE;
  qr_symbol -> option_1         = 3;            /* ECC Level L1/M2/Q3/H4 */
  memcpy(qr_symbol->outfile, qr_filename, sizeof(qr_filename));
  memcpy(qr_symbol->text,    (uint8_t*) key, sizeof(key));
  ZBarcode_Encode_and_Print(qr_symbol, (uint8_t *) hash, 0 , 0);
  ZBarcode_Delete(qr_symbol);

  /* DataMatrix Code */
  dm_symbol = ZBarcode_Create();
  dm_symbol -> symbology        = BARCODE_DATAMATRIX; 
  dm_symbol -> height           = 48;
  dm_symbol -> whitespace_width = 6;
  dm_symbol -> border_width     = 0;
  dm_symbol -> input_mode       = DATA_MODE;
  /* The fixed size symbol is selected for  6mm Dyno Tape 
   * DM Symbol is used for the small equipment, e.g., MTCA EVR 300
   */ 
  dm_symbol -> option_2         = 26; /* Symbol Size 8x32 */

  memcpy(dm_symbol->outfile, dm_filename, sizeof(dm_filename));
  memcpy(dm_symbol->text, (uint8_t*) key, sizeof(key));
  ZBarcode_Encode_and_Print(dm_symbol, (uint8_t *) hash, 0 , 0);
  ZBarcode_Delete(dm_symbol);
  
  return;
}



void
JiraProject::CreateUpdateBarcodes(std::string issue_id, std::string hash_id)
{
  struct zint_symbol *qr_symbol;
  struct zint_symbol *dm_symbol;

  std::string keyWithoutTAG = issue_id;  
  keyWithoutTAG.erase(jKey.find("TAG-"), 4);

  const char* hash = hash_id.c_str();
  const char* key  = keyWithoutTAG.c_str();

  char qr_filename[FILENAME_MAX];
  char dm_filename[FILENAME_MAX];

  sprintf(qr_filename, "%s", qr_file.c_str());
  sprintf(dm_filename, "%s", dm_file.c_str());
  
  /* QR Code */
  qr_symbol = ZBarcode_Create();
  qr_symbol -> symbology        = BARCODE_QRCODE;
  qr_symbol -> height           = 48;
  qr_symbol -> whitespace_width = 6;
  qr_symbol -> border_width     = 2;
  qr_symbol -> input_mode       = DATA_MODE;
  qr_symbol -> option_1         = 3;            /* ECC Level L1/M2/Q3/H4 */
  memcpy(qr_symbol->outfile, qr_filename, sizeof(qr_filename));
  memcpy(qr_symbol->text,    (uint8_t*) key, sizeof(key));
  ZBarcode_Encode_and_Print(qr_symbol, (uint8_t *) hash, 0 , 0);
  ZBarcode_Delete(qr_symbol);

  /* DataMatrix Code */
  dm_symbol = ZBarcode_Create();
  dm_symbol -> symbology        = BARCODE_DATAMATRIX; 
  dm_symbol -> height           = 48;
  dm_symbol -> whitespace_width = 6;
  dm_symbol -> border_width     = 0;
  dm_symbol -> input_mode       = DATA_MODE;
  /* The fixed size symbol is selected for  6mm Dyno Tape 
   * DM Symbol is used for the small equipment, e.g., MTCA EVR 300
   */ 
  dm_symbol -> option_2         = 26; /* Symbol Size 8x32 */

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

void
JiraProject::cups_jobs_status(const char* printer_name, int job_id)
{
  int             i;
  int             num_jobs;
  cups_job_t      *jobs;
  ipp_jstate_t    job_state = IPP_JOB_PENDING;

  std::ostringstream oss;

  oss << printer_name << " : Job " << job_id << " is ";
  
  while (job_state < IPP_JOB_STOPPED) {
    /* Get my jobs (1) with any state (-1) */
    num_jobs = cupsGetJobs(&jobs, printer_name, 1, -1);
    
    /* Loop to find my job */
    job_state = IPP_JOB_COMPLETED;
    
    for (i = 0; i < num_jobs; i ++)
      if (jobs[i].id == job_id)
	{
	  job_state = jobs[i].state;
	  break;
	}
    
    /* Free the job array */
    cupsFreeJobs(num_jobs, jobs);

    
    /* Show the current state */
    switch (job_state)
      {
      case IPP_JOB_PENDING :
	oss << "pending. ";
	std::cout << oss.str().c_str() << std::endl;
	//	prec->valg = oss.str().c_str();
	break;
      case IPP_JOB_HELD :
	oss << "held. ";
	std::cout << oss.str().c_str() << std::endl;
	//	prec->valg = oss.str().c_str();
	break;
      case IPP_JOB_PROCESSING :
	oss << "processing. ";
	std::cout << oss.str().c_str() << std::endl;
	// prec->valg = epicsStrDup("processing");
	break;
      case IPP_JOB_STOPPED :
	oss << "stopped. ";
	std::cout << oss.str().c_str() << std::endl;
	//	prec->valg = oss.str().c_str();
	break;
      case IPP_JOB_CANCELED :
	oss << "canceled. ";
	std::cout << oss.str().c_str() << std::endl;
	//	prec->valg = oss.str().c_str();
	break;
      case IPP_JOB_ABORTED :
	oss << "aborted. ";
	std::cout << oss.str().c_str() << std::endl;
	//	prec->valg = (void*) oss.str().c_str();
	break;
      case IPP_JOB_COMPLETED :
	oss << "completed. ";
	std::cout << oss.str().c_str() << std::endl;
	//	prec->valg = oss.str().c_str();
	break;
      }
    
    /* Sleep if the job is not finished */
    if (job_state < IPP_JOB_STOPPED)
      sleep(5);
  }

  return;
}


bool
JiraProject::cups_printer_status(const char* printer_name)
{

  int         i;
  cups_dest_t *dests;
  cups_dest_t *dest;
  int         num_dests = cupsGetDests(&dests);
  bool        printer_exist = false;
  
  for (i = num_dests, dest = dests; i > 0; i --, dest ++) {
    if (dest->instance == NULL) {
      //	value = cupsGetOption("printer-info", dest->num_options, dest->options);
      
      if (strcmp(printer_name, dest->name) != 0) printer_exist = true;
      else                                       printer_exist = false;
    }
  }

  cupsFreeDests(num_dests, dests);
  return printer_exist;
  
}


void
JiraProject::PrintBarcodes()
{

  const char* label_printer_name = "DYMO_LabelWriter_450_DUO_Label";
  const char* tape_printer_name  = "DYMO_LabelWriter_450_DUO_Tape";

  int             num_options;
  cups_option_t*  options;
  int             job_id;
  
  if ( this-> cups_printer_status(label_printer_name) ) {
    num_options = 0;
    options = NULL;
    job_id = -1;
    num_options = cupsAddOption("PageSize",              "w72h72.1", num_options, &options);
    num_options = cupsAddOption("scaling",               "100",      num_options, &options);
    num_options = cupsAddOption("orientation-requested", "3",        num_options, &options);
    num_options = cupsAddOption("DymoHalftoning",        "Default",  num_options, &options);
    job_id = cupsPrintFile(label_printer_name, qr_file.c_str(), "print qr code", num_options, options);
    this->cups_jobs_status(label_printer_name, job_id);
    cupsFreeOptions(num_options, options);
  }


  if ( this->cups_printer_status(tape_printer_name) ) {
    num_options = 0;
    options = NULL;
    job_id = -1;
    
    num_options = cupsAddOption("MediaType",             "6mm",     num_options, &options);
    num_options = cupsAddOption("PageSize",              "w18h252", num_options, &options);
    num_options = cupsAddOption("orientation-requested", "4",       num_options, &options);
    num_options = cupsAddOption("DymoCutOptions",        "Cut",     num_options, &options);
    num_options = cupsAddOption("DymoContinuousPaper",   "0",       num_options, &options);
    num_options = cupsAddOption("DymoHalftoning",        "Default", num_options, &options);
    
    job_id = cupsPrintFile(tape_printer_name, dm_file.c_str(), "print dm code", num_options, options);
    this->cups_jobs_status(tape_printer_name, job_id);
    cupsFreeOptions(num_options, options);
  }
    
  return;
}



void
JiraProject::Print()
{

  int width = 30;
  std::cout << std::setiosflags(std::ios::right);
  std::cout << "\n Object  -----------  -------------------------------------- ";
  std::cout << fItemObject;
  std::cout << "\n -------------------  -------------------------------------- ";
  std::cout << "\n Issue Url        : ";
  std::cout << std::setw(width) << fIssueUrl;
  std::cout << "\n Create Url       : ";
  std::cout << std::setw(width) << fCreateUrl;
  std::cout << "\n Bulk Create Url  : ";
  std::cout << std::setw(width) << fBulkCreateUrl;
  std::cout << "\n Search      Url  : ";
  std::cout << std::setw(width) << fSearchUrl;
  std::cout << "\n UpdateDelete Url : ";
  std::cout << std::setw(width) << fUpdateDeleteUrl;
  std::cout << "\n Attchement   Url : ";
  std::cout << std::setw(width) << fAttachmentsUrl;
  std::cout << "\n IssueIdOrKey     : ";
  std::cout << std::setw(width) << fIssueIdOrKey;
  std::cout << "\n User Name        : ";
  std::cout << std::setw(width) << fUserName;
  std::cout << "\n Description      : ";
  std::cout << std::setw(width) << fDescription;
  std::cout << "\n Json Root Data   : ";
  std::cout << std::setw(width) << jRootJsonData;
  std::cout << "\n JKey             : ";
  std::cout << std::setw(width) << jKey;
  std::cout << "\n JSelf            : ";
  std::cout << std::setw(width) << jSelf;
  std::cout << "\n JHash            : ";
  std::cout << std::setw(width) << jHash;
  std::cout << "\n";
  return;
}

