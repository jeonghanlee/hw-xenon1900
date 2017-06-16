#ifndef INC_ItemObject_HH
#define INC_ItemObject_HH

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

//#include <curl/curl.h>
//#include <jsoncpp/json/json.h>
#include <zint.h>

#include "aSubRecord.h"


// using namespace std;
        
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
  char *model_name;

} InvDataType;



class ItemObject
{

  
public:
  ItemObject();
  ItemObject(const ItemObject& iobj);
  ItemObject(InvDataType in);
  ItemObject(epicsUInt32  hashID, std::string serial_num, std::string model_name);
  ItemObject(epicsUInt32  hashID, char* serial_num, char* model_name);
  ItemObject(epicsUInt32  hashID, std::string serial_num, std::string model_name, std::string formfactor, std::string vendor_name, std::string ics_location, std::string status);
  ItemObject(epicsUInt32  hashID, char* serial_num, char* model_name, char* formfactor, char* vendor_name, char* ics_location,  char* status);
	     
  virtual ~ItemObject();

  void Init();
  ItemObject & operator=(const ItemObject &iobj);
  
  // overloaded operator methods

  friend std::ostream& operator<<(std::ostream& os, const ItemObject &itemobj);
// std::ostream& operator<<(std::ostream& o, const SomeType& t);
  bool IsValid();
  
  bool HasParent() { return fHasParent; } ;
  bool HasChild()  { return fHasChild; } ;

  const epicsUInt32 GetParentID             () const { return fParentID; };
  const std::vector<epicsUInt32> GetChildID () const { return fChildID; };
  const epicsUInt32 GetChildNumber          () const { return fChildNumber; };

  const epicsUInt32 GetLocationStructID()      const { return fLocationStructID; };
  const epicsUInt32 GetFacilityStructID()      const { return fFacilityStructID; };
  const epicsUInt32 GetInstallationStrcutID()  const { return fInstallationStructID; };

  epicsUInt32 GetHashID()                 { return fHashID; };

  bool HasFormfactor() const {
    if ( fFormfactor.empty() || fFormfactor == "") return false;
    else                                           return true;
  };
  
  bool HasVendor() const {
    if ( fVendor.empty() || fVendor == "" ) return false;
    else                                    return true;
  };
  
  bool              HasLocation()      const      {
    if ( fLocation.empty() || fLocation == "" ) return false;
    else                     return true;
  };
  
  bool              HasStatus()       const       {
    if ( fStatus.empty() || fStatus == "" ) return false;
    else                   return true;
  };
  
  const char*       GetCharHashID()          { return fHashIdStream.str().c_str();};
  const char*       GetSerialNumber()        { return fSerialNumber.c_str(); };
  const char*       GetName()                { return fName.c_str(); };
  const char*       GetFormfactor()          { return fFormfactor.c_str(); };
  const char*       GetVendor()              { return fVendor.c_str(); };
  const char*       GetLocation()            { return fLocation.c_str(); };
  const char*       GetStatus()              { return fStatus.c_str(); };
  const char*       GetModel()               { return fName.c_str(); };
  
  const epicsUInt32 GetJiraIssueNumber()       const { return fJiraIssueNumber; };


  
  void SetHashID       (const epicsUInt32 hash) {
    fHashID       = hash;
    fHashIdStream << hash;
  };
  void SetSerialNumber (const char* sn)         {std::string s(sn);   fSerialNumber = Split(s);};
  void SetName         (const char* name)       {std::string n(name); fName = Split(n);};
  void SetFormfactor   (const char* ff)         {std::string f(ff);   fFormfactor = Split(f);};
  void SetVendor       (const char* vd)         {std::string v(vd);   fVendor = Split(v);};
  void SetLocation     (const char* lo)         {std::string l(lo);   fLocation = Split(l);};
  void SetStatus       (const char* st)         {std::string sta(st); fStatus = Split(sta);};
  void SetModel        (const char* mo)         { SetName(mo); };


  void SetJIRAInfo (const std::string& project, const std::string& issuetype, const std::string& desc) {
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

  // // maybe we will move the following functions to private 
  // const std::string GetJiraJSON();
  const std::string GetJiraCSV();

  

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

  epicsUInt32  fHashID;   // Used for the ICS Label 

  std::ostringstream fHashIdStream;
  
  std::string       fSerialNumber;
  std::string       fName;
  
  std::string       fFormfactor;
  std::string       fVendor;
  std::string       fLocation;
  std::string       fStatus;

  std::string       fJiraProjectName;
  std::string       fJiraIssueName;
  std::string       fJiraDesc;

  // Return value after curl command from JIRA
  epicsUInt32  fJiraIssueNumber; // Used for the ICS Label 
  
  const std::string Split (const std::string str) {
    size_t found = str.find_last_of(",\\");
    //    cout << " prefix: " << str.substr(0,found) << '\n';
    return str.substr(found+1);
  }

  
};

#ifdef __cplusplus
}
#endif


#endif
