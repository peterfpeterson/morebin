Building from source
====================

Required packages and libraries
-------------------------------
* cmake
* boost-program-options
* boost-date-time

The three step dance
--------------------
    cmake .
    make
    sudo make install

To make the installer (rpm only for now) is just the extra

    make package

There is a [weird bug](http://stackoverflow.com/questions/9948375/cmake-find-package-succeeds-but-returns-wrong-path)
with old boost and new CMake (like on RHEL6) that makes it impossible to link.
To get around this just `cmake . -DBoost_NO_BOOST_CMAKE=ON` and make again.

[![Build Status](https://travis-ci.org/peterfpeterson/morebin.png)](https://travis-ci.org/peterfpeterson/morebin)
