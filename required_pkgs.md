
## Debian Linux 
```
aptitude install  qt4-default cmake  devscripts cdbs  build-essential  libcups2-dev libcupsimage2-dev  printer-driver-dymo   libcurl4-gnutls-dev libjsoncpp-dev libcups2-dev libcupsimage2-dev texlive java-package
```
## Zint

git clone https://github.com/zint/zint

# Installation from Debian pkg
```
$ git clone https://github.com/zint/zint
$ cd zint
$ dpkg-buildpackage -us -uc
$ cd ../
# dpkg -i *.deb
```

In case of Debian 9, Edit debian/control file to replace libpng12 with libpng as follows:

```
-Build-Depends: cdbs, debhelper (>= 7), cmake, libpng12-dev, libqt4-dev
+Build-Depends: cdbs, debhelper (>= 7), cmake, libpng-dev, libqt4-dev
 
-Depends: libpng12-dev
+Depends: libpng-dev
```


### Installation from source

```
$ git clone https://github.com/zint/zint
$ cd zint
$ mkdir build
$ cd build
$ cmake ..
-- Looking for Q_WS_X11
-- Looking for Q_WS_X11 - found
-- Looking for Q_WS_WIN
-- Looking for Q_WS_WIN - not found
-- Looking for Q_WS_QWS
-- Looking for Q_WS_QWS - not found
-- Looking for Q_WS_MAC
-- Looking for Q_WS_MAC - not found
-- Found Qt4: /usr/bin/qmake (found version "4.8.6") 
-- Configuring done
-- Generating done
-- Build files have been written to: /home/jhlee/gitsrc/zint/build
$ make
$ su

# make install
[ 57%] Built target zint
[ 59%] Built target zint_frontend
[ 61%] Built target QZint
[100%] Built target zint-qt
Install the project...
-- Install configuration: "RelWithDebInfo"
-- Installing: /usr/share/cmake-3.0/Modules/FindZint.cmake
-- Installing: /usr/local/lib/libzint.so.2.4.2
-- Installing: /usr/local/lib/libzint.so.2.4
-- Installing: /usr/local/lib/libzint.so
-- Installing: /usr/local/include/zint.h
-- Installing: /usr/local/bin/zint
-- Installing: /usr/local/lib/libQZint.so.2.4.2
-- Installing: /usr/local/lib/libQZint.so.2.4
-- Installing: /usr/local/lib/libQZint.so
-- Installing: /usr/local/include/qzint.h
-- Installing: /usr/local/bin/zint-qt

# make uninstall
-- Uninstalling "/usr/share/cmake-3.0/Modules/FindZint.cmake"
-- Uninstalling "/usr/local/lib/libzint.so.2.4.2"
-- Uninstalling "/usr/local/lib/libzint.so.2.4"
-- Uninstalling "/usr/local/lib/libzint.so"
-- Uninstalling "/usr/local/include/zint.h"
-- Uninstalling "/usr/local/bin/zint"
-- Uninstalling "/usr/local/lib/libQZint.so.2.4.2"
-- Uninstalling "/usr/local/lib/libQZint.so.2.4"
-- Uninstalling "/usr/local/lib/libQZint.so"
-- Uninstalling "/usr/local/include/qzint.h"
-- Uninstalling "/usr/local/bin/zint-qt"
Built target uninstall

```

## DYMO printers

Don't need to install anythings from source if one wants to use Debian

Install  printer-driver-dymo

If CentOS is used... 



http://www.dymo.com/en-US/dymo-label-sdk-and-cups-drivers-for-linux-dymo-label-sdk-cups-linux-p--1

```
tar xvzf dymo-cups-drivers-1.4.0.tar.gz
cd dymo-cups-drivers-1.4.0.5/
./configure 
make
sudo make install
sudo systemctl restart cups
```

