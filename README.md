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

To make the installer (rpm only for now) instead execute the target

    make package_rpm

There is a [weird bug](http://stackoverflow.com/questions/9948375/cmake-find-package-succeeds-but-returns-wrong-path)
with old boost and new CMake (like on RHEL6) that makes it impossible to link.
To get around this just `cmake . -DBoost_NO_BOOST_CMAKE=ON` and make again.

Extra information
-----------------
[![DOI Badge](https://zenodo.org/badge/doi/10.5281/zenodo.10056.png)](http://dx.doi.org/10.5281/zenodo.10056)
[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/peterfpeterson/morebin/trend.png)](https://bitdeli.com/free "Bitdeli Badge")
[![Build Status](https://travis-ci.org/peterfpeterson/morebin.png)](https://travis-ci.org/peterfpeterson/morebin)
