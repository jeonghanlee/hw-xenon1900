Tools for the Barcode input
--------------------------
# Requirement
```
aptitude install qt4-default
```

# Installation

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
```


# Example
```
zint --whitesp=1  --barcode=71 --output=test.png --data="test"
```
# References
* https://zint.github.io/
* https://cloud.github.com/downloads/zint/zint/zint_manual_242.pdf
