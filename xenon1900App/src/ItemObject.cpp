#include "ItemObject.hh"


ItemObject::ItemObject()
{
  Init();
  
};



ItemObject::ItemObject(epicsUInt32  hashID,
		       std::string serial_num,
		       std::string model_name)
{
  Init();
  SetHashID(hashID);
  fSerialNumber = serial_num;
  fName         = model_name;
};




ItemObject::ItemObject(epicsUInt32  hashID,
		       char* serial_num,
		       char* model_name)
{
  Init();
  SetHashID(hashID);
  SetSerialNumber(serial_num);
  SetName(model_name);
};




ItemObject::ItemObject(epicsUInt32  hashID,
		       std::string serial_num,
		       std::string model_name,
		       std::string formfactor,
		       std::string vendor_name,
		       std::string ics_location,
		       std::string status)
{
  Init();
  SetHashID(hashID);
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
  Init();
  SetHashID(hashID);
  SetSerialNumber(serial_num);
  SetName(model_name);
  SetFormfactor(formfactor);
  SetVendor(vendor_name);
  SetLocation(ics_location);
  SetStatus(status);
};





ItemObject::ItemObject(InvDataType in)
{
  Init();
  
  SetHashID(in.hash);
  SetSerialNumber(in.serialnumber);
  SetName(in.model_name);
  SetFormfactor(in.formfactor);
  SetVendor(in.vendor);
  SetLocation(in.location);
  SetStatus(in.status);
  fLabel = in.label;
};

ItemObject::ItemObject(const ItemObject &iobj)
{
  fHasParent            = iobj.fHasParent;
  fHasChild             = iobj.fHasChild;
  fParentID             = iobj.fParentID;
  fChildID              = iobj.fChildID;
  
  fChildNumber          = iobj.fChildNumber;
  fLocationStructID     = iobj.fLocationStructID;
  fFacilityStructID     = iobj.fFacilityStructID;
  fInstallationStructID = iobj.fInstallationStructID;

  fHashID               = iobj.fHashID;
  fHashIdStream         << iobj.fHashIdStream.str();

  fSerialNumber         = iobj.fSerialNumber;
  fName                 = iobj.fName;
  fVendor               = iobj.fVendor;
  fLocation             = iobj.fLocation;
  fStatus               = iobj.fStatus;

  fJiraProjectName      = iobj.fJiraProjectName;
  fJiraIssueName        = iobj.fJiraIssueName;
  fJiraDesc             = iobj.fJiraDesc;

  fLabel                = iobj.fLabel;
};


ItemObject::~ItemObject()
{
};


void 
ItemObject::Init()
{
  fHasParent = false;
  fHasChild  = false;
  fParentID  = 0;
  fChildID.clear();

  
  fChildNumber          = 0;
  fLocationStructID     = 0;
  fFacilityStructID     = 0;
  fInstallationStructID = 0;

  fHashID       = 0;
  fHashIdStream.clear();
  
  fSerialNumber.clear();
  fName.clear();
  fFormfactor.clear();
  fVendor.clear();
  fLocation.clear();
  fStatus.clear();

  fJiraProjectName.clear();
  fJiraIssueName.clear();
  fJiraDesc.clear();

  fLabel = true;
};



ItemObject & ItemObject::operator=(const ItemObject &iobj)
{
  if (this == &iobj)
    return *this;

  fHasParent            = iobj.fHasParent;
  fHasChild             = iobj.fHasChild;
  fParentID             = iobj.fParentID;
  fChildID              = iobj.fChildID;
  
  fChildNumber          = iobj.fChildNumber;
  fLocationStructID     = iobj.fLocationStructID;
  fFacilityStructID     = iobj.fFacilityStructID;
  fInstallationStructID = iobj.fInstallationStructID;

  fHashID               = iobj.fHashID;
  fHashIdStream         << iobj.fHashIdStream.str();

  fSerialNumber         = iobj.fSerialNumber;
  fName                 = iobj.fName;
  fVendor               = iobj.fVendor;
  fLocation             = iobj.fLocation;
  fStatus               = iobj.fStatus;

  fJiraProjectName      = iobj.fJiraProjectName;
  fJiraIssueName        = iobj.fJiraIssueName;
  fJiraDesc             = iobj.fJiraDesc;

  fLabel                = iobj.fLabel;
  return *this;
  
};

void
ItemObject::Print()
{
  // std::string out;
  // std::string hash; 

  // out = "Object Name " + this->GetName();
  // out += "\n";
  // out += "Hash ID ";
  // out += to_std::string(this-> GetHashID()) ;   
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


const std::string
ItemObject::GetJiraCSV()
{
  std::stringstream out;
  out << fName
      << ","
      << fSerialNumber
      << ","
      << fHashID
      << ","
      << fFormfactor
      << ","
      << fVendor
      << ","
      << fLocation
      << "\n";
  return out.str();
};


std::ostream& operator<<(std::ostream& os, const ItemObject &itemobj)
{
  int width = 20;
  os << std::setiosflags(std::ios::right);
  os << "Jira Project : ";
  os << std::setw(width) << itemobj.fJiraProjectName;
  os << "\n Hash ID       : ";
  os << std::setw(width) << itemobj.fHashID;
  os << "\n Serial Number : ";
  os << std::setw(width) << itemobj.fSerialNumber;
  os << "\n Model Name    : ";
  os << std::setw(width) << itemobj.fName;
  if ( itemobj.HasFormfactor() ) {
    os << "\n Formfactor    : ";
    os << std::setw(width) << itemobj.fFormfactor;
  }
  if ( itemobj.HasVendor() ) {
    os << "\n Vendor        : ";
    os << std::setw(width) << itemobj.fVendor;
  }
  if ( itemobj.HasLocation() ) {
    os << "\n Location      : ";
    os << std::setw(width) << itemobj.fLocation;
  }
  if ( itemobj.HasStatus() ) {
    os << "\n Status        : ";
    os << std::setw(width) << itemobj.fStatus;
  }

  return os;
  
}




