#include "ItemObject.hh"


/* 
   The following macro was removed from 3.15. 
   Until I find the proper way to do this,
   I will use the macro locally
 

   #define dbGetPdbAddrFromLink(PLNK) \
   ( ( (PLNK)->type != DB_LINK ) \
   ? 0 \
   : ( ( (struct dbAddr *)( (PLNK)->value.pv_link.pvt) ) ) )
*/


/* We get all values as string */
static char *getLinkStrVal(DBLINK *dbLink)
{
  if   (dbLink->type != DB_LINK) {
    return epicsStrDup("NO DB_LINK type");
  } else {
    DBADDR *pdbAddr       = (DBADDR*) dbLink->value.pv_link.pvt;
    stringinRecord * pRec = (stringinRecord*) pdbAddr->precord;
    return epicsStrDup(pRec -> val);
  }
}


static epicsEnum16 getLinkVal(DBLINK *dbLink)
{
  if   (dbLink->type != DB_LINK) {
    return 0;
  } else {
    DBADDR *pdbAddr = (DBADDR*) dbLink->value.pv_link.pvt;
    biRecord * pRec = (biRecord*) pdbAddr->precord;
    return pRec -> val;
  }
}

  // void SetSerialNumber (const char* sn)         {std::string s(sn);   fSerialNumber = Split(s);};
  // void SetName         (const char* name)       {std::string n(name); fName = Split(n);};
  // void SetFormfactor   (const char* ff)         {std::string f(ff);   fFormfactor = Split(f);};
  // void SetVendor       (const char* vd)         {std::string v(vd);   fVendor = Split(v);};
  // void SetLocation     (const char* lo)         {std::string l(lo);   fLocation = Split(l);};
  // void SetStatus       (const char* st)         {std::string sta(st); fStatus = Split(sta);};
  // void SetModel        (const char* mo)         { SetName(mo); };



ItemObject::ItemObject()
{
  Init();
  
};

ItemObject::ItemObject(aSubRecord *pRec)
{

  Init();

  std::string tmp_str;
  
  fSerialNumber = RemovePrefix( getLinkStrVal(&pRec->outa) );
  fFormfactor   = RemovePrefix( getLinkStrVal(&pRec->outb) );
  fVendor       = RemovePrefix( getLinkStrVal(&pRec->outc) );
  fLocation     = RemovePrefix( getLinkStrVal(&pRec->outd) );
  fStatus       = RemovePrefix( getLinkStrVal(&pRec->oute) );
  fName         = RemovePrefix( getLinkStrVal(&pRec->outf) );

  /* Likely have the chance to have the same serial number in the different product */
  tmp_str = fSerialNumber + fName;
  fHashIdStream << hs;
  fHashIdStream << ",";
  fHashIdStream << epicsStrHash(tmp_str.c_str(),0);

  fLabel = getLinkVal(&pRec->outu);

}


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

  //  fHashID               = iobj.fHashID;
  fHashIdStream         << iobj.fHashIdStream.str();

  fSerialNumber         = iobj.fSerialNumber;
  fName                 = iobj.fName;
  
  fFormfactor           = iobj.fFormfactor;
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
  
  fParentID.clear();
  fChildID.clear();
 
  fChildNumber          = 0;
  
  fLocationStructID.clear();
  fFacilityStructID.clear();
  fInstallationStructID.clear();

  //  fHashID.clear();
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

  fJiraIssueNumber.clear();
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

  //  fHashID               = iobj.fHashID;
  fHashIdStream         << iobj.fHashIdStream.str();

  fSerialNumber         = iobj.fSerialNumber;
  fName                 = iobj.fName;

  fFormfactor           = iobj.fFormfactor;
  fVendor               = iobj.fVendor;
  fLocation             = iobj.fLocation;
  fStatus               = iobj.fStatus;

  fJiraProjectName      = iobj.fJiraProjectName;
  fJiraIssueName        = iobj.fJiraIssueName;
  fJiraDesc             = iobj.fJiraDesc;

  fLabel                = iobj.fLabel;
  fJiraIssueNumber      = iobj.fJiraIssueNumber;
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
  printf("Hash ID       %s\n", fHashIdStream.str().c_str());
  printf("Serial Number %s\n", fSerialNumber.c_str());
  printf("Formfactor    %s\n", fFormfactor.c_str());
  printf("Vendor        %s\n", fVendor.c_str());
  printf("Location      %s\n", fLocation.c_str());
  printf("Status        %s\n", fStatus.c_str());
}



bool
ItemObject::IsValid()
{
  if ( !fName.empty() && !fSerialNumber.empty() ) return true;
  else                                            return false;
}


std::ostream& operator<<(std::ostream& os, const ItemObject &itemobj)
{
  int width = 20;
  os << std::setiosflags(std::ios::right);
  os << "Jira Project : ";
  os << std::setw(width) << itemobj.fJiraProjectName;
  os << "\n Hash ID       : ";
  os << std::setw(width) << itemobj.fHashIdStream.str();
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




