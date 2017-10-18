# A Simple and Naive Inventory System with EPICS IOC, JIRA, and Dymo LabelWriter 450 Duo

There are infinite ways to develop and maintain an inventory system. However, this inventory is the unique and **temporary** solution for the ICS Lab, especially, where the author works at. This inventory system is designed to minimize our time when we register an item to the existent JIRA ICS HW\&I group inventory task.  Therefore, it does **NOT** provide any fancy and beautiful ways to interact with users, **BUT** provide the minimal tool to monitor and track any equipment in ICS, ESS, and any IK partner. And the system only provide the following three features :

* Do stock an item to existent JIRA structure, and do Add-and-Print its barcodes.
* Do add-and-print the barcodes of the item, which was registered in JIRA before.
* Delete an item from JIRA

## Requirements

* For Barcode generation : zint
* For JIRA communication : libcurl, jsoncpp
* For Dymo LabelWriter 450 Duo : cups 

```
$aptitude install  libcurl4-gnutls-dev libjsoncpp-dev libcups2-dev libcupsimage2-dev  qt4-default cmake devscripts cdbs  build-essential

$ yum install jsoncpp-devel cups-devel libcurl-devel cmake devscripts 

```


## Working Environment or Requirements

* EPICS BASE 
* ASYN
* STREAM

### Scan Bar Codes from User Guide

## Commands and Dir Structure

```
.
├── [jhlee    4.0K]  configure
├── [jhlee    4.0K]  iocBoot
├── [jhlee    4.0K]  manual
├── [jhlee    4.0K]  script
└── [jhlee    4.0K]  xenon1900App
```

### EPICS IOC

### caget script
```
 watch -n 1 bash script/caget_pvs.bash iocxenon1900_PVs.list
```
## License
EPICS Open License

http://www.aps.anl.gov/epics/license/open.php


# Reference
## Barcode scanner
* Xenon 1900, and others User's Guide <br />
  https://www.honeywellaidc.com/products/barcode-scanners/general-duty/xenon-1900g-1902g
* Xenon 1900 Date Sheet <br />
  https://www.honeywellaidc.com/-/media/en/files-public/data-sheets/xenon-1900-general-duty-scanner-data-sheet-en.pdf

## curl : communication with jira
* http://www.curlpp.org/
* https://curl.haxx.se/libcurl/

## jsoncpp

* https://github.com/open-source-parsers/jsoncpp


## zint : Barcode Generator 
* https://zint.github.io/

## CUPS
* https://www.cups.org/doc/options.html


