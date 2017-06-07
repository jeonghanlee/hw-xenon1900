#ifndef INC_ItemObject_HH
#define INC_ItemObject_HH

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

extern "C" {
#include "aSubRecord.h"
  #
}

  
typedef struct InvDataType {
  unsigned int hash;
  char *serialnumber;
  char *formfactor;
  char *vendor;
  char *location;
  char *status;
  char *model;

} InvDataType;




class ItemObject
{
public:
  ItemObject();
  ItemObject(InvDataType in);
  ItemObject(epicsUInt32  hashID, std::string serial_num, std::string model_name);
  ItemObject(epicsUInt32  hashID, char* serial_num, char* model_name);
  ItemObject(epicsUInt32  hashID, std::string serial_num, std::string model_name, std::string formfactor, std::string vendor_name, std::string ics_location, std::string status);
  ItemObject(epicsUInt32  hashID, char* serial_num, char* model_name, char* formfactor, char* vendor_name, char* ics_location,  char* status);
	     
  virtual ~ItemObject();

  bool HasParent() { return fHasParent; } ;
  bool HasChild()  { return fHasChild; } ;

  const epicsUInt32 GetParentID             () const { return fParentID; };
  const std::vector<epicsUInt32> GetChildID () const { return fChildID; };
  const epicsUInt32 GetChildNumber          () const { return fChildNumber; };

  const epicsUInt32 GetLocationStructID()      const { return fLocationStructID; };
  const epicsUInt32 GetFacilityStructID()      const { return fFacilityStructID; };
  const epicsUInt32 GetInstallationStrcutID()  const { return fInstallationStructID; };

  const epicsUInt32 GetHashID()                const { return fHashID; };
  const std::string GetSerialNumber()          const { return fSerialNumber; };
  const std::string GetName()                  const { return fName; };
  const std::string GetFormfactor()            const { return fFormfactor; };
  const std::string GetVendor()                const { return fVendor; };
  const std::string GetLocation()              const { return fLocation; };
  const std::string GetStatus()                const { return fStatus; };
  const std::string GetModel()                 const { return fName; };

  void SetHashID       (const epicsUInt32 hash) {fHashID       = hash;};
  void SetSerialNumber (const char* sn)         {std::string s(sn);   fSerialNumber = s;};
  void SetName         (const char* name)       {std::string n(name); fName = n;};
  void SetFormfactor   (const char* ff)         {std::string f(ff);   fFormfactor = f;};
  void SetVendor       (const char* vd)         {std::string v(vd);   fVendor = v;};
  void SetLocation     (const char* lo)         {std::string l(lo);   fLocation = l;};
  void SetStatus       (const char* st)         {std::string sta(st); fStatus = sta;};
  void SetModel        (const char* mo)         { SetName(mo); };
  
  
  void AddParent(ItemObject *parent)    {
    fParentID = parent->GetHashID(); 
    fHasParent = true;
    fLocation  = parent->GetLocation();
  };
  void RemoveParent(ItemObject *parent) {
    fParentID  = 0;
    fHasParent = false;
    fLocation  = "";
  };
  
  void AddChild(ItemObject *child) {
    fChildID.push_back( child->GetHashID() );
    fChildNumber = (epicsUInt32) fChildID.size();
    fHasChild &= true;
  };
  
  void RemoveChildID(ItemObject *child) {
    fChildID.erase(std::remove(fChildID.begin(), fChildID.end(), child->GetHashID() ), fChildID.end());
    fChildNumber = (epicsUInt32) fChildID.size();
    if ( fChildNumber == 0 ) fHasChild = false;
  };


  void Print ();
    
private:

  bool         fHasParent;
  bool         fHasChild;
  epicsUInt32  fParentID;
  std::vector<epicsUInt32> fChildID;

  epicsUInt32  fChildNumber;

  epicsUInt32  fLocationStructID;
  epicsUInt32  fFacilityStructID;
  epicsUInt32  fInstallationStructID;

  epicsUInt32  fHashID;
  std::string  fSerialNumber;
  std::string  fName;
  std::string  fFormfactor;
  std::string  fVendor;
  std::string  fLocation;
  std::string  fStatus;
  
};

#endif
