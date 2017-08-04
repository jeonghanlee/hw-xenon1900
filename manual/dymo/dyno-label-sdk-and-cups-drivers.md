## Download DYMO Label SDK and CUPS Drivers for Linux


http://www.dymo.com/en-US/dymo-label-sdk-and-cups-drivers-for-linux-dymo-label-sdk-cups-linux-p--1


# Install packages and compile sources, and install source

```
root@kaffee:# aptitude install libcups2-dev libcupsimage2-dev

jhlee@kaffee: dymo-cups-drivers-1.4.0.5$ ./configure
jhlee@kaffee: dymo-cups-drivers-1.4.0.5$ make

root@kaffee:# make install

root@kaffee:# systemctl restart cups
```


# Add LabelWriter 450 DUO Label

http://localhost:631/admin/

1. Printers
[Add Printer]
2. Local Printers
DYMO LabelWriter 450 DUO Label (DYMO LabelWriter 450 DUO Label)
[Continue]
3. Name

[DYMO_LabelWriter_450_DUO_Label]

# Select Printer properly


$ lpstat -p -d
printer DYMO_LabelWriter_450_DUO_Label is idle.  enabled since Thu 03 Aug 2017 05:20:03 PM CEST
        Ready to print.
printer DYMO_LabelWriter_450_DUO_Tape is idle.  enabled since Thu 03 Aug 2017 05:22:44 PM CEST
        Ready to print.

$ lpoptions -p DYMO_LabelWriter_450_DUO_Label -l
PageSize/Media Size: w154h64 w72h154 w72h72 w162h90 w54h144 w118h252 w167h188 w79h252 w167h252 w167h288 w154h198 w82h248 w154h64.1 w79h252.1 w102h252 w154h286 w154h198.1 w54h424 w131h221 w57h248 w54h144.1 *w72h72.1 w72h72.2 w162h90.1 30334_2-1_4_in_x_1-1_4_in w73h86 w72h154.1 w118h252.1 w54h203 w54h180 w36h136 w72h108 w65h90 w167h288.1 w168h252 w144h169 w71h144 w144h252 w80h144 w162h504 30383_PC_Postage_3-Part w167h540 30384_PC_Postage_2-Part w167h756 30387_PC_Postage_EPS w167h188.1 w176h292 w167h288.2 w112h126 w79h252.2 w102h252.1 w154h286.1 w154h286.2 w154h198.2 w63h419 w139h221 w36h144 w108h539 w167h539 w154h7680 w154h792 Custom.WIDTHxHEIGHT
Resolution/Output Resolution: 300dpi *300x600dpi
DymoHalftoning/Halftoning: Default *ErrorDiffusion NLL
DymoPrintDensity/Print Density: Light Medium *Normal Dark
DymoPrintQuality/Print Quality: Text *Graphics


For the Dymo 1x1 inches label (30332)




    -o orientation-requested=3 - portrait orientation (no rotation)
    -o orientation-requested=4 - landscape orientation (90 degrees)
    -o orientation-requested=5 - reverse landscape or seascape orientation (270 degrees)
    -o orientation-requested=6 - reverse portrait or upside-down orientation (180 degrees)




lpr -P  DYMO_LabelWriter_450_DUO_Label -o media=w72h72.1 -o fit-to-page -o orientation-requested=3 -o DymoHalftoning=Default qr_symbol.png 


$ lpoptions -p DYMO_LabelWriter_450_DUO_Tape -l

MediaType/Label Width: 06mm 09mm *12mm 19mm 24mm
PageSize/Media Size: w18h252 w18h4000 w26h252 w26h4000 w35h252 w35h144 *w35h252.1 w35h4000 w55h252 w55h4000 w68h252 w68h4000 w68h252.2 w35h252.2 w35h144.1 Custom.WIDTHxHEIGHT
Resolution/Output Resolution: *180dpi
DymoHalftoning/Halftoning: Default *ErrorDiffusion NLL
DymoCutOptions/Cut Options: *Cut ChainMarks
DymoLabelAlignment/Label Alignment: *Center Left Right
DymoContinuousPaper/Continuous Paper: *0 1
DymoPrintChainMarksAtDocEnd/Print cut marks between labels: *0 1
DymoTapeColor/Label Cassette Color: *0 1 2 3 4 5 6 7 8 9 10 11 12



For 12mm 
lpr -P DYMO_LabelWriter_450_DUO_Tape -o MediaType=12mm -o PageSize=w35h252.1  -o orientation-requested=3 -o DymoCutOptions=Cut -o DymoContinuousPaper=0 -o DymoHalftoning=Default dm_symbol.png 

For 6mm

lpr -P DYMO_LabelWriter_450_DUO_Tape -o MediaType=6mm -o PageSize=w18h252  -o orientation-requested=4 -o DymoCutOptions=Cut -o DymoContinuousPaper=0 -o DymoHalftoning=Default dm_symbol.png
