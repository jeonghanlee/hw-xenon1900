#ifndef INC_ItemObject_HH
#define INC_ItemObject_HH

#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

#include <curl/curl.h>
//#include <curlpp/cURLpp.hpp>
//#include <curlpp/Easy.hpp>
//#include <curlpp/Options.hpp>


#include <zint.h>

#include "aSubRecord.h"


using namespace std;
        
#ifdef __cplusplus
extern "C" {
#endif
  
  
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
  ItemObject(epicsUInt32  hashID, string serial_num, string model_name);
  ItemObject(epicsUInt32  hashID, char* serial_num, char* model_name);
  ItemObject(epicsUInt32  hashID, string serial_num, string model_name, string formfactor, string vendor_name, string ics_location, string status);
  ItemObject(epicsUInt32  hashID, char* serial_num, char* model_name, char* formfactor, char* vendor_name, char* ics_location,  char* status);
	     
  virtual ~ItemObject();


  bool IsValid();
  
  bool HasParent() { return fHasParent; } ;
  bool HasChild()  { return fHasChild; } ;

  const epicsUInt32 GetParentID             () const { return fParentID; };
  const vector<epicsUInt32> GetChildID () const { return fChildID; };
  const epicsUInt32 GetChildNumber          () const { return fChildNumber; };

  const epicsUInt32 GetLocationStructID()      const { return fLocationStructID; };
  const epicsUInt32 GetFacilityStructID()      const { return fFacilityStructID; };
  const epicsUInt32 GetInstallationStrcutID()  const { return fInstallationStructID; };

  const epicsUInt32 GetHashID()                const { return fHashID; };
  const string GetSerialNumber()          const { return fSerialNumber; };
  const string GetName()                  const { return fName; };
  const string GetFormfactor()            const { return fFormfactor; };
  const string GetVendor()                const { return fVendor; };
  const string GetLocation()              const { return fLocation; };
  const string GetStatus()                const { return fStatus; };
  const string GetModel()                 const { return fName; };

  void SetHashID       (const epicsUInt32 hash) {fHashID       = hash;};
  void SetSerialNumber (const char* sn)         {string s(sn);   fSerialNumber = s;};
  void SetName         (const char* name)       {string n(name); fName = n;};
  void SetFormfactor   (const char* ff)         {string f(ff);   fFormfactor = f;};
  void SetVendor       (const char* vd)         {string v(vd);   fVendor = v;};
  void SetLocation     (const char* lo)         {string l(lo);   fLocation = l;};
  void SetStatus       (const char* st)         {string sta(st); fStatus = sta;};
  void SetModel        (const char* mo)         { SetName(mo); };


  void SetJIRAInfo (const string& project, const string& issuetype, const string& desc) {
    fJiraProjectName = project;
    fJiraIssueName   = issuetype;
    fJiraDesc        = desc;
  }
  
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
    fChildID.erase(remove(fChildID.begin(), fChildID.end(), child->GetHashID() ), fChildID.end());
    fChildNumber = (epicsUInt32) fChildID.size();
    if ( fChildNumber == 0 ) fHasChild = false;
  };

  const string GetJiraJSON();
  const string GetJiraCSV();

  void Print ();
    
private:

  bool         fHasParent;
  bool         fHasChild;
  epicsUInt32  fParentID;
  vector<epicsUInt32> fChildID;

  epicsUInt32  fChildNumber;

  epicsUInt32  fLocationStructID;
  epicsUInt32  fFacilityStructID;
  epicsUInt32  fInstallationStructID;

  epicsUInt32  fHashID;
  string  fSerialNumber;
  string  fName;
  string  fFormfactor;
  string  fVendor;
  string  fLocation;
  string  fStatus;

  string  fJiraProjectName;
  string  fJiraIssueName;
  string  fJiraDesc;
  
  
  
  const string Split (const string& str) {
    size_t found = str.find_last_of(",\\");
    //    cout << " prefix: " << str.substr(0,found) << '\n';
    return str.substr(found+1);
  }

  
};

#ifdef __cplusplus
}
#endif


#endif
