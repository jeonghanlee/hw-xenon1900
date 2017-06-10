# hw-xenon1900
This repository will contain the EPICS IOC for the Honeywell Xenon 1900 bar code scanner. 


## Requirements

* libcurl

```
aptitude install  libcurl4-gnutls-dev

```

* zint
Please look at docs/zint directory


## Working Environment or Requirements

* EPICS BASE 
* ASYN
* STREAM
* devIocStats

### Scan Bar Codes from User Guide

Scan the bar codes in the Xenon User Guide books according to the following information :
* USB Serial bar code (P19)
* RS232 Serial Port bar code (P16)

### Default RS232 configuration
* Baud Rate 115200 bps
* 8 Data bits
* No Parity bit
* 1 stop bit

### Enable Symbologies (Select some..)
[TT] : default ON  | current ON
[FT] : default OFF | current ON
[FF] :
[TF] :

* Codabar [TT]
* Code 39 [TT]
* Interleaved 2 of 5 [TT]
* NEC 2 of 5 [TT]
* Code 93 [TT]
* Straight 2 of 5 Industrial (p189) [FF]
* Matrix 2 of 5 [FF]
* Code 11 [FF]
* Code 128 [TT]
* GS1-128 [TT]
* Telepen [FF]
* UPC-A   [TT]
* UPC-E0  [TT]
* EAN/JAN-13 [TT]
* EAN/JAN-8 [TT]
* MSI       [FF] (P209)
* PDF417    [TT]
* MacroPDF-417 [TT]
* MicroPDF-417 [FT] (p216)
* QR Code  [TT]
* Data Matrix [TT]
* MaxiCode [FT] (p222)
* Aztec Code [TT] 

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

## zint : Barcode Generator 
* https://zint.github.io/

## CUPS
* https://www.cups.org/doc/options.html

# Acknowledgement

