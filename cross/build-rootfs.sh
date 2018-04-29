#!/usr/bin/env bash

usage()
{
    echo "Usage: $0 [BuildArch] [LinuxCodeName] [--skipunmount]"
    echo "BuildArch can be: armv7l"
    echo "LinuxCodeName - optional, Code name for Linux, can be: xenial(default)."
    echo "--skipunmount - optional, will skip the unmount of rootfs folder."
    exit 1
}

__LinuxCodeName=xenial
__CrossDir=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
__InitialDir=$PWD
__BuildArch=armv7l
__UbuntuArch=armhf
__UbuntuRepo="http://ports.ubuntu.com/"
__SkipUnmount=0

# base development support
__UbuntuPackages="build-essential"

# symlinks fixer
__UbuntuPackages+=" symlinks"

# libraries required
__UbuntuPackages+=" libsdl2-dev libsdl2-ttf-dev liblua5.2-dev"
__UbuntuPackages+=" libboost-system-dev libboost-filesystem-dev"

__UnprocessedBuildArgs=
for i in "$@" ; do
    lowerI="$(echo $i | awk '{print tolower($0)}')"
    case $lowerI in
        -?|-h|--help)
            usage
            exit 1
            ;;
        armv7l)
            __BuildArch=armv7l
            __UbuntuArch=armhf
            ;;
        xenial)
            if [ "$__LinuxCodeName" != "jessie" ]; then
                __LinuxCodeName=xenial
            fi
            ;;
        --skipunmount)
            __SkipUnmount=1
            ;;
        *)
            __UnprocessedBuildArgs="$__UnprocessedBuildArgs $i"
            ;;
    esac
done

__RootfsDir="$__CrossDir/rootfs/$__BuildArch"

if [[ -n "$ROOTFS_DIR" ]]; then
    __RootfsDir=$ROOTFS_DIR
fi

if [ -d "$__RootfsDir" ]; then
    if [ $__SkipUnmount == 0 ]; then
        umount $__RootfsDir/*
    fi
    rm -rf $__RootfsDir
fi

qemu-debootstrap --arch $__UbuntuArch $__LinuxCodeName $__RootfsDir $__UbuntuRepo
cp $__CrossDir/$__BuildArch/sources.list.$__LinuxCodeName $__RootfsDir/etc/apt/sources.list
chroot $__RootfsDir apt-get update
chroot $__RootfsDir apt-get -f -y install
chroot $__RootfsDir apt-get -y install $__UbuntuPackages
chroot $__RootfsDir symlinks -cr /usr

if [ $__SkipUnmount == 0 ]; then
    umount $__RootfsDir/*
fi
