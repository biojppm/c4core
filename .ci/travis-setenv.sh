#!/usr/bin/env bash

set -e
set -x

pwd
C4CORE_DIR=$(pwd)

export CC_=$(echo "$CXX_" | sed 's:clang++:clang:g' | sed 's:g++:gcc:g')
$CXX_ --version
$CC_ --version
cmake --version


# add cmake flags
function addcmflags()
{
    CMFLAGS="$CMFLAGS $*"
}

case "$LINT" in
    all       ) addcmflags -DC4CORE_LINT=ON -DC4CORE_LINT_TESTS=ON -DC4CORE_LINT_CLANG_TIDY=ON  -DC4CORE_LINT_PVS_STUDIO=ON ;;
    clang-tidy) addcmflags -DC4CORE_LINT=ON -DC4CORE_LINT_TESTS=ON -DC4CORE_LINT_CLANG_TIDY=ON  -DC4CORE_LINT_PVS_STUDIO=OFF ;;
    pvs-studio) addcmflags -DC4CORE_LINT=ON -DC4CORE_LINT_TESTS=ON -DC4CORE_LINT_CLANG_TIDY=OFF -DC4CORE_LINT_PVS_STUDIO=ON ;;
    *         ) addcmflags -DC4CORE_LINT=OFF ;;
esac

case "$SAN" in
    ALL) addcmflags -DC4CORE_SANITIZE=ON ;;
    A  ) addcmflags -DC4CORE_SANITIZE=ON -DC4CORE_ASAN=ON  -DC4CORE_TSAN=OFF -DC4CORE_MSAN=OFF -DC4CORE_UBSAN=OFF ;;
    T  ) addcmflags -DC4CORE_SANITIZE=ON -DC4CORE_ASAN=OFF -DC4CORE_TSAN=ON  -DC4CORE_MSAN=OFF -DC4CORE_UBSAN=OFF ;;
    M  ) addcmflags -DC4CORE_SANITIZE=ON -DC4CORE_ASAN=OFF -DC4CORE_TSAN=OFF -DC4CORE_MSAN=ON  -DC4CORE_UBSAN=OFF ;;
    UB ) addcmflags -DC4CORE_SANITIZE=ON -DC4CORE_ASAN=OFF -DC4CORE_TSAN=OFF -DC4CORE_MSAN=OFF -DC4CORE_UBSAN=ON ;;
    *  ) addcmflags -DC4CORE_SANITIZE=OFF ;;
esac

case "$SAN_ONLY" in
    ON) addcmflags -DC4CORE_SANITIZE_ONLY=ON ;;
    * ) addcmflags -DC4CORE_SANITIZE_ONLY=OFF ;;
esac

case "$VG" in
    ON) addcmflags -DC4CORE_VALGRIND=ON -DC4CORE_VALGRIND_SGCHECK=OFF ;; # FIXME SGCHECK should be ON
    * ) addcmflags -DC4CORE_VALGRIND=OFF -DC4CORE_VALGRIND_SGCHECK=OFF ;;
esac

case "$BM" in
    ON) addcmflags -DC4CORE_BUILD_BENCHMARKS=ON ;;
    * ) addcmflags -DC4CORE_BUILD_BENCHMARKS=OFF ;;
esac

if [ "$STD" != "" ] ; then
    addcmflags -DC4_CXX_STANDARD=$STD -DC4CORE_CXX_STANDARD=$STD
fi

if [ "$BT" == "Coverage" ] ; then
    addcmflags -DC4CORE_COVERAGE=ON
    addcmflags -DC4CORE_COVERAGE_CODECOV=ON
    addcmflags -DC4CORE_COVERAGE_COVERALLS=ON
fi

echo "building with additional cmake flags: $CMFLAGS"

# the coverage repo tokens are set in the travis environment:
# export CODECOV_REPO_TOKEN=.......
# export COVERALLS_REPO_TOKEN=.......
export C4_EXTERN_DIR=`pwd`/build/extern
mkdir -p $C4_EXTERN_DIR

function c4core_run_test()
{
    bits=$1
    linktype=$2
    #
    build=`pwd`/build/$bits
    install=`pwd`/install/$bits
    mkdir -p $build
    mkdir -p $install
    case "$linktype" in
        static) linktype="-DBUILD_SHARED_LIBS=OFF" ;;
        dynamic) linktype="-DBUILD_SHARED_LIBS=ON" ;;
    esac
    cd $build
    cmake -DCMAKE_C_COMPILER=$CC_ -DCMAKE_C_FLAGS="-std=c99 -m$bits" \
          -DCMAKE_CXX_COMPILER=$CXX_ -DCMAKE_CXX_FLAGS="-m$bits" \
          -DCMAKE_INSTALL_PREFIX="$install" \
          -DCMAKE_BUILD_TYPE=$BT \
          -DC4CORE_DEV=ON \
          $CMFLAGS \
          $linktype \
          $C4CORE_DIR
    make help | sed 1d | sort
    make CTEST_OUTPUT_ON_FAILURE=1 test
    cd -
}


function c4core_submit_coverage()
{
    if [ "$BT" == "Coverage" ] ; then
        build_dir=$1
        coverage_service=$2
        echo "Submiting coverage data: $build_dir --> $coverage_service"
        cmake --build $build_dir --target c4core-coverage-submit-$coverage_service
    fi
}
