# Package
Making Package for same distro

## Archlinux Distributions 

archlinux, manjaro ...

My packaging configuration is done on Manjaro KDE, which provides two options for Qmake and CMake. The version number is more casual, after all, not officially released. For different scenarios, simply run Makepkg-p pkgbuild.qmake or makepkg-p pkgbuild.cmake

## Debian Distributions

debian, ubuntu, KDE neon ...

My packaging configuration is done on KDE neon User Edition, using the default configuration CMake build package, first create an initial configuration through dh_make, then modify debian/control, and finally package with dpkg-buildpackage-rfakeroot.