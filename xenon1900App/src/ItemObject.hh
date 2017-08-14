#ifndef INC_ItemObject_HH
#define INC_ItemObject_HH

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

#include <zint.h>

#include "dbDefs.h"
#include "dbAccess.h"
#include "dbFldTypes.h"
#include "link.h"
#include "dbAddr.h"
#include "registryFunction.h"
#include "aSubRecord.h"
#include "epicsExport.h"
#include "epicsString.h"
#include "epicsTime.h"
#include "stringinRecord.h"
#include "biRecord.h"

#include "BcodeDefine.h"

// typedef struct InvDataType {
//   unsigned int hash;
//   char *serialnumber;
//   char *formfactor;
//   char *vendor;
//   char *location;
//   char *status;
//   char *model_name;
//   unsigned int label;

// } InvDataType;



// using namespace std;
        
#ifdef __cplusplus
extern "C" {
#endif
  

class ItemObject
{

  
public:
  ItemObject();
  ItemObject(const ItemObject& iobj);
  // ItemObject(InvDataType in);
  ItemObject(aSubRecord *pRec);
  
  virtual ~ItemObject();

  void Init();
  
  ItemObject & operator=(const ItemObject &iobj);
  
  // overloaded operator methods

  friend std::ostream& operator<<(std::ostream& os, const ItemObject &itemobj);
// std::ostream& operator<<(std::ostream& o, const SomeType& t);
  bool IsValid();
  
  bool HasParent() { return fHasParent; } ;
  bool HasChild()  { return fHasChild; } ;

  const std::string GetParentID             () const { return fParentID; };
  const std::vector<std::string> GetChildID () const { return fChildID; };
  const epicsUInt32 GetChildNumber          () const { return fChildNumber; };

  const std::string GetLocationStructID()      const { return fLocationStructID; };
  const std::string GetFacilityStructID()      const { return fFacilityStructID; };
  const std::string GetInstallationStrcutID()  const { return fInstallationStructID; };

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
  
  const std::string GetHashID()              { return fHashIdStream.str();};
  const char*       GetCharHashID()          { return fHashIdStream.str().c_str();};
  const std::string GetSerialNumber()        { return fSerialNumber; };
  const std::string GetName()                { return fName; };
  const std::string GetFormfactor()          { return fFormfactor; };
  const std::string GetVendor()              { return fVendor; };
  const std::string GetLocation()            { return fLocation; };
  const std::string GetStatus()              { return fStatus; };
  const std::string GetModel()               { return fName; };
  const std::string GetJiraIssueNumber()     { return fJiraIssueNumber; };
  
  
  // void SetHashID       (const epicsUInt32 hash) {
  //   fHashID       = hash;
  //   fHashIdStream << hash;
  // };
  // void SetSerialNumber (const char* sn)         {std::string s(sn);   fSerialNumber = Split(s);};
  // void SetName         (const char* name)       {std::string n(name); fName = Split(n);};
  // void SetFormfactor   (const char* ff)         {std::string f(ff);   fFormfactor = Split(f);};
  // void SetVendor       (const char* vd)         {std::string v(vd);   fVendor = Split(v);};
  // void SetLocation     (const char* lo)         {std::string l(lo);   fLocation = Split(l);};
  // void SetStatus       (const char* st)         {std::string sta(st); fStatus = Split(sta);};
  // void SetModel        (const char* mo)         { SetName(mo); };


  // void SetJIRAInfo (const std::string& project, const std::string& issuetype, const std::string& desc) {
  //   fJiraProjectName = project;
  //   fJiraIssueName   = issuetype;
  //   fJiraDesc        = desc;
  // }
  
  // void AddParent(ItemObject *parent)    {
  //   fParentID = parent->GetHashID(); 
  //   fHasParent = true;
  //   fLocation  = parent->GetLocation();
  // };
  // void RemoveParent(ItemObject *parent) {
  //   fParentID  = 0;
  //   fHasParent = false;
  //   fLocation  = "";
  // };
  
  // void AddChild(ItemObject *child) {
  //   fChildID.push_back( child->GetHashID() );
  //   fChildNumber = (epicsUInt32) fChildID.size();
  //   fHasChild &= true;
  // };
  
  // void RemoveChildID(ItemObject *child) {
  //   fChildID.erase(remove(fChildID.begin(), fChildID.end(), child->GetHashID() ), fChildID.end());
  //   fChildNumber = (epicsUInt32) fChildID.size();
  //   if ( fChildNumber == 0 ) fHasChild = false;
  // };

  
  bool IsLabel() {  return fLabel;}
  
  void Print ();
    
private:

  bool         fHasParent;
  bool         fHasChild;
  std::string  fParentID;
  
  std::vector<std::string> fChildID;

  epicsUInt32  fChildNumber;

  std::string  fLocationStructID;
  std::string  fFacilityStructID;
  std::string  fInstallationStructID;

  // std::string  fHashID;   // Used for the ICS Label 

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

  bool              fLabel;
  // Return value after curl command from JIRA
  std::string       fJiraIssueNumber; // Used for the ICS Label 

  const std::string RemovePrefix(char * in) {
    std::string str = in;
    size_t found = str.find_last_of(",\\");
    //    cout << " prefix: " << str.substr(0,found) << '\n';
    return str.substr(found+1);
  }
  

  
};

#ifdef __cplusplus
}
#endif


#endif
