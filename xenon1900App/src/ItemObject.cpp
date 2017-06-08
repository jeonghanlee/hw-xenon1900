#include "ItemObject.hh"


ItemObject::ItemObject()
{
  fHasParent = false;
  fHasChild  = false;
  fParentID  = 0;

  
  fChildNumber          = 0;
  fLocationStructID     = 0;
  fFacilityStructID     = 0;
  fInstallationStructID = 0;

  fHashID       = 0;
  fSerialNumber = "";
  fName         = "";
  fFormfactor   = "";
  fVendor       = "";
  fLocation     = "";
  fStatus       = "";
};



ItemObject::ItemObject(epicsUInt32  hashID,
		       string serial_num,
		       string model_name)
{
  fHasParent = false;
  fHasChild  = false;
  fParentID  = 0;

  fChildNumber          = 0;
  fLocationStructID     = 0;
  fFacilityStructID     = 0;
  fInstallationStructID = 0;

  fHashID       = hashID;
  fSerialNumber = serial_num;
  fName         = model_name;
  fFormfactor   = "";
  fVendor       = "";
  fLocation     = "";
  fStatus       = "";
};




ItemObject::ItemObject(epicsUInt32  hashID,
		       char* serial_num,
		       char* model_name)
{
  fHasParent = false;
  fHasChild  = false;
  fParentID  = 0;

  fChildNumber          = 0;
  fLocationStructID     = 0;
  fFacilityStructID     = 0;
  fInstallationStructID = 0;

  fHashID       = hashID;
  SetSerialNumber(serial_num);
  SetName(model_name);
  fFormfactor   = "";
  fVendor       = "";
  fLocation     = "";
  fStatus       = "";
  
};




ItemObject::ItemObject(epicsUInt32  hashID,
		       string serial_num,
		       string model_name,
		       string formfactor,
		       string vendor_name,
		       string ics_location,
		       string status)
{
  fHasParent = false;
  fHasChild  = false;
  fParentID  = 0;

  fChildNumber          = 0;
  fLocationStructID     = 0;
  fFacilityStructID     = 0;
  fInstallationStructID = 0;

  fHashID       = hashID;
  fSerialNumber = serial_num;
  fName         = model_name;
  fFormfactor   = formfactor;
  fVendor       = vendor_name;
  fLocation     = ics_location;
  fStatus       = status;
};




ItemObject::ItemObject(epicsUInt32  hashID,
		       char* serial_num,
		       char* model_name,
		       char* formfactor,
		       char* vendor_name,
		       char* ics_location,
		       char* status)
{
  fHasParent = false;
  fHasChild  = false;
  fParentID  = 0;

  fChildNumber          = 0;
  fLocationStructID     = 0;
  fFacilityStructID     = 0;
  fInstallationStructID = 0;

  fHashID       = hashID;
  SetSerialNumber(serial_num);
  SetName(model_name);
  SetFormfactor(formfactor);
  SetVendor(vendor_name);
  SetLocation(ics_location);
  SetStatus(status);
};





ItemObject::ItemObject(InvDataType in)
{
  fHasParent = false;
  fHasChild  = false;
  fParentID  = 0;

  fChildNumber          = 0;
  fLocationStructID     = 0;
  fFacilityStructID     = 0;
  fInstallationStructID = 0;

  fHashID       = in.hash;
  SetSerialNumber(in.serialnumber);
  SetName(in.model);
  SetFormfactor(in.formfactor);
  SetVendor(in.vendor);
  SetLocation(in.location);
  SetStatus(in.status);
};



ItemObject::~ItemObject()
{
};


void
ItemObject::Print()
{
  // string out;
  // string hash; 

  // out = "Object Name " + this->GetName();
  // out += "\n";
  // out += "Hash ID ";
  // out += to_string(this-> GetHashID()) ;   
  // out += " ";

  // cout << out << endl;
  
  printf("Object Name   %s\n", fName.c_str());
  printf("Hash ID       %u\n", fHashID);
  printf("Serial Number %s\n", fSerialNumber.c_str());
  printf("Formfactor    %s\n", fFormfactor.c_str());
  printf("Vendor        %s\n", fVendor.c_str());
  printf("Location      %s\n", fLocation.c_str());
  printf("Status        %s\n", fStatus.c_str());
}



bool
ItemObject::IsValid()
{
  if (fName != "" && fSerialNumber != "" ) return true;
  else                                     return false;
}


const string
ItemObject::GetJiraCSV()
{
  stringstream out;
  out << Split(fName)
      << ","
      << fSerialNumber
      << ","
      << fHashID
      << ","
      << Split(fFormfactor)
      << ","
      << Split(fVendor)
      << ","
      << Split(fLocation)
      << "\n";
  return out.str();
};

const string
ItemObject::GetJiraJSON()
{
  stringstream out;
  out << "{\n"
      << "  \"fields\": { \n"
      << "       \"project\": { \"key\": \"" << fJiraProjectName << "\" } \n"
      << "     , \"issuetype\": { \"name\": \"" << fJiraIssueName << "\" } \n"
      << "     , \"summary\": \""<< Split(fName) << "\"\n"
      << "     , \"customfield_10502\": { \"value\": \"" << Split(fLocation) << "\" } \n"
      << "     , \"customfield_10500\": \""<< fSerialNumber << "\"\n"
      << "     , \"customfield_11002\": \""<< fHashID << "\"\n"
      << "     , \"labels\" : [ \"" <<  Split(fFormfactor) << "\"]\n"
    //      << ", \"assignee\" : {\"name\" : \"" >> potential_username << "\"}"
      << "     , \"description\": \"" << fJiraDesc << "\"\n"
      << "  }\n"
      << "}\n";
  
  return out.str();

   //  "fields": {
   //     "project": { "key": "TAG" }
   //     , "issuetype": { "name": "Hardware"}	
   //     , "summary": "MO, Model"
   //     , "customfield_10502" : {"value": "ICS Lab"}
   //     , "customfield_10500" : "SN,SERIALNUMBER"
   //     , "customfield_11002" : "HASH ID"
   //     , "labels" : [ "FORMFACTOR"]
   //     , "assignee" : {"name" : "hanlee"}
   //     , "description": "Description.... Creating of an issue using ids for projects and issue types using the REST API"
   // }
};

