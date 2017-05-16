Default configuration, I plugin 

```
jhlee@hadron: ~$ udevadm monitor
monitor will print the received events for:
UDEV - the event which udev sends out after rule processing
KERNEL - the kernel uevent

KERNEL[141815.099344] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3 (usb)
KERNEL[141815.102230] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0 (usb)
KERNEL[141815.105301] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009 (hid)
KERNEL[141815.105414] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009/input/input22 (input)
KERNEL[141815.105460] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009/input/input22/event19 (input)
KERNEL[141815.105505] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009/hidraw/hidraw2 (hidraw)
KERNEL[141815.106271] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1 (usb)
KERNEL[141815.110288] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1/0003:0C2E:0901.000A (hid)
KERNEL[141815.112226] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1/usbmisc/hiddev1 (usbmisc)
KERNEL[141815.112297] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1/0003:0C2E:0901.000A/hidraw/hidraw3 (hidraw)
UDEV  [141815.119706] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3 (usb)
UDEV  [141815.122045] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0 (usb)
UDEV  [141815.122524] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1 (usb)
UDEV  [141815.122542] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009 (hid)
UDEV  [141815.123732] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1/0003:0C2E:0901.000A (hid)
UDEV  [141815.123745] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1/0003:0C2E:0901.000A/hidraw/hidraw3 (hidraw)
UDEV  [141815.124669] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009/input/input22 (input)
UDEV  [141815.125762] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009/hidraw/hidraw2 (hidraw)
UDEV  [141815.125805] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009/input/input22/event19 (input)
UDEV  [141815.128422] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1/usbmisc/hiddev1 (usbmisc)
KERNEL[141828.010925] change   /devices/pci0000:00/0000:00:1c.1/0000:03:00.0/ieee80211/phy0/rfkill1 (rfkill)
KERNEL[141828.011185] change   /devices/platform/regulatory.0 (platform)
UDEV  [141828.012950] change   /devices/pci0000:00/0000:00:1c.1/0000:03:00.0/ieee80211/phy0/rfkill1 (rfkill)
UDEV  [141828.016546] change   /devices/platform/regulatory.0 (platform)
KERNEL[141851.259607] change   /devices/pci0000:00/0000:00:1c.1/0000:03:00.0/ieee80211/phy0/rfkill1 (rfkill)
UDEV  [141851.261318] change   /devices/pci0000:00/0000:00:1c.1/0000:03:00.0/ieee80211/phy0/rfkill1 (rfkill)
KERNEL[141855.074003] change   /devices/platform/regulatory.0 (platform)
UDEV  [141855.077205] change   /devices/platform/regulatory.0 (platform)
KERNEL[141863.581782] change   /devices/platform/regulatory.0 (platform)
UDEV  [141863.584953] change   /devices/platform/regulatory.0 (platform)
KERNEL[141866.373965] change   /devices/platform/regulatory.0 (platform)
UDEV  [141866.375981] change   /devices/platform/regulatory.0 (platform)
KERNEL[141889.788349] change   /devices/platform/regulatory.0 (platform)
UDEV  [141889.813128] change   /devices/platform/regulatory.0 (platform)
KERNEL[141893.446408] change   /devices/platform/regulatory.0 (platform)
UDEV  [141893.448982] change   /devices/platform/regulatory.0 (platform)
```

Scan the barcode in USB serial,

```
KERNEL[141921.348717] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009/input/input22/event19 (input)
UDEV  [141921.349608] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009/input/input22/event19 (input)
KERNEL[141921.372104] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009/input/input22 (input)
KERNEL[141921.372122] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009/hidraw/hidraw2 (hidraw)
KERNEL[141921.372144] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009 (hid)
KERNEL[141921.372164] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0 (usb)
KERNEL[141921.372234] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1/usbmisc/hiddev1 (usbmisc)
KERNEL[141921.372256] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1/0003:0C2E:0901.000A/hidraw/hidraw3 (hidraw)
KERNEL[141921.372268] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1/0003:0C2E:0901.000A (hid)
KERNEL[141921.372285] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1 (usb)
UDEV  [141921.372524] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009/input/input22 (input)
UDEV  [141921.372716] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009/hidraw/hidraw2 (hidraw)
UDEV  [141921.372957] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1/usbmisc/hiddev1 (usbmisc)
UDEV  [141921.373117] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1/0003:0C2E:0901.000A/hidraw/hidraw3 (hidraw)
UDEV  [141921.373136] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/0003:0C2E:0901.0009 (hid)
UDEV  [141921.373295] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1/0003:0C2E:0901.000A (hid)
UDEV  [141921.373426] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0 (usb)
UDEV  [141921.373518] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1 (usb)
KERNEL[141921.384177] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3 (usb)
UDEV  [141921.386440] remove   /devices/pci0000:00/0000:00:14.0/usb2/2-3 (usb)
KERNEL[141921.815438] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3 (usb)
KERNEL[141921.818257] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0 (usb)
KERNEL[141921.819249] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1 (usb)
UDEV  [141921.830579] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3 (usb)
UDEV  [141921.831247] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.1 (usb)
KERNEL[141921.834667] add      /module/cdc_acm (module)
UDEV  [141921.834810] add      /module/cdc_acm (module)
KERNEL[141921.835744] add      /devices/pci0000:00/0000:00:14.0/usb2/2-3/2-3:1.0/tty/ttyACM0 (tty)
```

```
screen -S scanner /dev/ttyACM2 
```

It shows the bar code data in the screen.


OK, the proper procedure for the correct RS232 configuration

* Scan USB Serial bar code
  The scanned data is showing in screen as follows:
```
  57058312603685705831260368570583126036857058312603685705831260368570583126036857058312603685705831260368
```

* Scan RS232 Serial Port bar code
  After scanning RS232 Serial Port bar code,
```
57058312603685705831260368570583126036857058312603685705831260368570583126036857058312603685705831260368
5705831260368
5705831260368
5705831260368
5705831260368
5705831260368
5705831260368
5705831260368
5705831260368
5705831260368
5705831260368
  ```

## Default RS232 configuration
* Baud Rate 115200 bps
* 8 Data bits
* No Parity bit
* 1 stop bit



