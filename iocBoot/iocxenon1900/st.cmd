#!../../bin/linux-x86_64/xenon1900

## You may have to change xenon1900 to something else
## everywhere it appears in this file

< envPaths

epicsEnvSet("EPICS_CAS_INTF_ADDR_LIST" "10.0.7.177")

epicsEnvSet("STREAM_PROTOCOL_PATH", ".:${TOP}/db")
epicsEnvSet("TTY","$(TTY=/dev/ttyACM4)")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/xenon1900.dbd"
xenon1900_registerRecordDeviceDriver pdbbase

drvAsynSerialPortConfigure("L0", "$(TTY)", 0, 0, 0)

# CR, "\r" , 0x0d.
# LF, "\n" , 0x0a

asynOctetSetInputEos("L0", 0, "\r\n")
#asynOctetSetOutputEos  "L0", 0, "\r"
asynSetOption("L0", -1, "baud", "115200")
asynSetOption("L0", -1, "bits", "8")
asynSetOption("L0", -1, "parity", "none")
asynSetOption("L0", -1, "stop", "1")
asynSetOption("L0", -1, "clocal", "Y")
asynSetOption("L0", -1, "crtscts", "N")

#dbLoadRecords("db/iocAdminSoft.db", "IOC=ICSLAB:IocStat")
dbLoadRecords("db/xenon1900.db",    "SYSDEV=ICSLAB:,PORT=L0")

## Set this to see messages from xenonASub.c

var xenonDebug 1


cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=jhlee"

dbl > "${TOP}/${IOC}_PVs.list"
