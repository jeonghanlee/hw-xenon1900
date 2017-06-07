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
		       std::string serial_num,
		       std::string model_name)
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
		       std::string serial_num,
		       std::string model_name,
		       std::string formfactor,
		       std::string vendor_name,
		       std::string ics_location,
		       std::string status)
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
  printf("Object Name   %s\n", fName.c_str());
  printf("Hash ID       %u\n", fHashID);
  printf("Serial Number %s\n", fSerialNumber.c_str());
  printf("Formfactor    %s\n", fFormfactor.c_str());
  printf("Vendor        %s\n", fVendor.c_str());
  printf("Location      %s\n", fLocation.c_str());
  printf("Status        %s\n", fStatus.c_str());
}
