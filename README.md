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

[![Build Status](https://travis-ci.org/peterfpeterson/morebin.png)](https://travis-ci.org/peterfpeterson/morebin)
