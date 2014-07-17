Building from source
====================

Required packages and libraries
-------------------------------
* cmake
* boost-program-options
* boost-date-time

Installing
----------
You can grab binary installer:
* The [github releases](https://github.com/peterfpeterson/morebin/releases) page
* rpms available at [copr](https://copr.fedoraproject.org/coprs/peterfpeterson/morebin/)

Building from source
--------------------
Simply perform the normal 3-step dance (style points if you build with [ninja](https://github.com/martine/ninja))

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
[MIT License](http://opensource.org/licenses/MIT)
[![Build Status](https://travis-ci.org/peterfpeterson/morebin.png)](https://travis-ci.org/peterfpeterson/morebin)
[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/peterfpeterson/morebin/trend.png)](https://bitdeli.com/free "Bitdeli Badge")
