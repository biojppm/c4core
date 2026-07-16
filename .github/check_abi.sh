#!/bin/bash

set -xeuo pipefail

rootdir=`git rev-parse --show-toplevel`
branch=`git rev-parse --abbrev-ref HEAD`

libname=$1 ; shift
oldver=$1  ; shift
if [ -v 1 ] ; then
    newver=$1 ; shift
else
    newver=$branch
fi
flags="-g -Og"

function gitcheckout()
{
    id=$1
    git checkout $id
    if [ -f $rootdir/.gitmodules ] ; then
        git submodule foreach --recursive git submodule update --recursive
    fi
    git status
}

# https://lvc.github.io/abi-compliance-checker/
function dumpbuild()
{
    id=$1
    gitcheckout $id
    bdir=$rootdir/build/abicheck-$id
    mkdir -p $bdir
    cmake -S $rootdir -B $bdir \
          -D CMAKE_BUILD_TYPE=Debug \
          -D CMAKE_CXX_FLAGS="$flags" \
          -D CMAKE_C_FLAGS="$flags" \
          -D BUILD_SHARED_LIBS=ON
    cmake --build $bdir -j
    abi-dumper $bdir/lib$libname.so \
               -o $bdir/abi.dump \
               -lver $id
}

dumpbuild $newver ; newabi=$bdir/abi.dump
dumpbuild $oldver ; oldabi=$bdir/abi.dump
gitcheckout $branch

stat=0
cd $rootdir/build/
abi-compliance-checker -l $libname \
                       -old $oldabi \
                       -new $newabi \
    | sed '/Report: compat_/d' \
    || stat=1

set +x
echo "REPORT:"
echo "    $rootdir/build/compat_reports/c4core/${oldver}_to_${newver}/compat_report.html"
echo "    status=$stat"

exit $stat
