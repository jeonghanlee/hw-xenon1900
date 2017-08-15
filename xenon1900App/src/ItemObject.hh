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

// using namespace std;
        
#ifdef __cplusplus
extern "C" {
#endif
  

class ItemObject
{

  
public:
  ItemObject();
  ItemObject(const ItemObject& iobj);
  ItemObject(aSubRecord *pRec, const std::string& url, const std::string& project, const std::string& issuetype);
  
  virtual ~ItemObject();

  friend std::ostream& operator<<(std::ostream& os, const ItemObject &itemobj);
    
  void Init();
  
  ItemObject & operator=(const ItemObject &iobj);

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
  
  const std::string GetJiraProject()         { return fJiraProjectName; };
  const std::string GetJiraIssue()           { return fJiraIssueName; };
  const std::string GetJiraUrl()             { return fJiraUrl; };
  
  
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

  epicsUInt32       fChildNumber;

  std::string       fLocationStructID;
  std::string       fFacilityStructID;
  std::string       fInstallationStructID;

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
  std::string       fJiraUrl;

  bool              fLabel;
  // Return value after curl command from JIRA
  std::string       fJiraIssueNumber; // Used for the ICS Label 

  const std::string RemovePrefix(char * in) {
    std::string str = in;
    size_t found = str.find_last_of(",\\");
    //    cout << " prefix: " << str.substr(0,found) << '\n';
    return str.substr(found+1);
  }
  

  void SetJIRAInfo (const std::string& url, const std::string& project, const std::string& issuetype) {
    fJiraProjectName = project;
    fJiraIssueName   = issuetype;
    fJiraUrl         = url;
  }
  
};

#ifdef __cplusplus
}
#endif


#endif
