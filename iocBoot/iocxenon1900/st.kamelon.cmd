#!../../bin/linux-x86_64/xenon1900

## You may have to change xenon1900 to something else
## everywhere it appears in this file

< envPaths

epicsEnvSet("EPICS_CAS_INTF_ADDR_LIST" "172.20.10.3")

epicsEnvSet("STREAM_PROTOCOL_PATH", ".:${TOP}/db")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/xenon1900.dbd"
xenon1900_registerRecordDeviceDriver pdbbase

drvAsynIPPortConfigure("L0", "127.0.0.1:9999", 0, 0, 0)

# CR, "\r" , 0x0d.
# LF, "\n" , 0x0a

#asynOctetSetInputEos("L0", 0, "\r\n")
#asynOctetSetOutputEos  "L0", 0, "\r"


#dbLoadRecords("db/iocAdminSoft.db", "IOC=ICSLAB:IocStat")
dbLoadRecords("db/xenon1900.db",    "SYSDEV=ICSLAB:,PORT=L0")
dbLoadRecords("db/xenon1900kam.db",    "SYSDEV=ICSLAB:,PORT=L0")

## Set this to see messages from xenonASub.c

var xenonDebug 0

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=jhlee"

dbl > "${TOP}/${IOC}_PVs.list"
