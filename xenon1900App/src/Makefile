TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
USR_CPPFLAGS += -DUSE_TYPED_RSET
#=============================
# Build the IOC support library

LIBRARY_IOC += xenon1900Support

# # xxxRecord.h will be created from xxxRecord.dbd
# DBDINC += xxxRecord
# # Install devXxxSoft.dbd into <top>/dbd
# DBD += xxxSupport.dbd

# Compile and add the code to the support library
xenon1900Support_SRCS += xenonASub.cpp
xenon1900Support_SRCS += ItemObject.cpp
xenon1900Support_SRCS += Jira.cpp

xenon1900Support_LIBS += $(EPICS_BASE_IOC_LIBS)


#=============================
# Build the IOC application

PROD_IOC = xenon1900
# xenon1900.dbd will be created and installed
DBD += xenon1900.dbd

# xenon1900.dbd will be made up from these files:
xenon1900_DBD += base.dbd

# Include dbd files from all support applications:
#xenon1900_DBD += xxx.dbd
xenon1900_DBD += devIocStats.dbd
xenon1900_DBD += stream.dbd
xenon1900_DBD += asyn.dbd
xenon1900_DBD += drvAsynSerialPort.dbd
xenon1900_DBD += drvAsynIPPort.dbd
xenon1900_DBD += xenonASub.dbd

# Add all the support libraries needed by this IOC
#xenon1900_LIBS += xxx
xenon1900_LIBS += stream
xenon1900_LIBS += asyn
xenon1900_LIBS += devIocStats


# xenon1900_registerRecordDeviceDriver.cpp derives from xenon1900.dbd
xenon1900_SRCS += xenon1900_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
xenon1900_SRCS_DEFAULT += xenon1900Main.cpp
xenon1900_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#xenon1900_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

xenon1900_LIBS += xenon1900Support
# Finally link to the EPICS Base libraries
xenon1900_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

