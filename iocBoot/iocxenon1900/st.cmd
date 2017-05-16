#!../../bin/linux-x86_64/xenon1900

## You may have to change xenon1900 to something else
## everywhere it appears in this file

< envPaths

epicsEnvSet("STREAM_PROTOCOL_PATH", ".:${TOP}/db")
epicsEnvSet("TTY","$(TTY=/dev/ttyAMA0)")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/xenon1900.dbd"
xenon1900_registerRecordDeviceDriver pdbbase


drvAsynSerialPortConfigure("XENON", "$(TTY)", 0,0,0)

asynSetOption("XENON", -1, "baud", "9600")
asynSetOption("XENON", -1, "bits", "8")
asynSetOption("XENON", -1, "parity", "none")
asynSetOption("XENON", -1, "stop", "1")
asynSetOption("XENON", -1, "clocal", "Y")
asynSetOption("XENON", -1, "crtscts", "N")

dbLoadRecords("db/iocAdminSoft.db", "IOC=ICSLAB:IocStat")
dbLoadRecords("db/xenon1900.db",    "SYSDEV=ICSLAB:,HWUNIT=XENON")


cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=jhlee"
