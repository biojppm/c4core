#!/bin/bash

export SRC_DIR=$(pwd)
export CODECOV_TOKEN=d64e8387-b35e-4ff9-96f4-5c5707071331
export COVERALLS_REPO_TOKEN=CA38CKJfGL2juulJSwK5LeeTFy3KTaJZF

function submit_coverage()
{
    coveralls --include src --root $SRC_DIR --build-root . --gcov-options '\-lp'
    curl -s https://codecov.io/bash > submit_codecov.bash
    chmod a+x submit_codecov.bash
    ./submit_codecov.bash -t $CODECOV_TOKEN -g test -G src -p $SRC_DIR -a '\-lp'
}

if [ "${BT}" == "Coverage" ] ; then
    ( cd build/64 && make c4core-coverage && submit_coverage )
    ( cd build/32 && make c4core-coverage && submit_coverage )
fi
