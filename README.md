morebin is the most overengineered single binary.

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
[![license](https://img.shields.io/github/license/peterfpeterson/morebin.svg?maxAge=2592000)](LICENSE.txt)
[![Travis-CI Build Status](https://travis-ci.org/peterfpeterson/morebin.svg)](https://travis-ci.org/peterfpeterson/morebin)
[![Drone.io Build Status](https://drone.io/github.com/peterfpeterson/morebin/status.png)](https://drone.io/github.com/peterfpeterson/morebin/latest)
[![Build status](https://ci.appveyor.com/api/projects/status/aah5p1hjasigwes6?svg=true)](https://ci.appveyor.com/project/peterfpeterson/morebin)
[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/peterfpeterson/morebin/trend.png)](https://bitdeli.com/free "Bitdeli Badge")
[![Coverity Scan Build Status](https://scan.coverity.com/projects/2832/badge.svg)](https://scan.coverity.com/projects/2832)
[![Coverage Status](https://coveralls.io/repos/peterfpeterson/morebin/badge.svg?branch=master)](https://coveralls.io/r/peterfpeterson/morebin?branch=master)

[![Pull Request Stats](http://www.issuestats.com/github/peterfpeterson/morebin/badge/pr)](http://www.issuestats.com/github/peterfpeterson/morebin)
[![Issue Stats](http://www.issuestats.com/github/peterfpeterson/morebin/badge/issue)](http://www.issuestats.com/github/peterfpeterson/morebin)
