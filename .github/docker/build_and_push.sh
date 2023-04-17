#!/bin/bash -xe

set -o pipefail

if [ -z "$CR_USR" ] ; then
    echo "error: \$CR_USR is not defined. Set it to a user name."
    exit 1
fi
if [ -z "$CR_PAT" ] ; then
    echo "error: \$CR_PAT is not defined. Set it to a personal access token."
    exit 1
fi
echo $CR_PAT | docker login ghcr.io -u $CR_USR --password-stdin

mydir=$(cd $(dirname $0) ; pwd)

time ( \
  cd $mydir/ubuntu18.04 ; \
  img=ghcr.io/biojppm/c4core/ubuntu18.04:latest ; \
  time docker build -t $img . ; \
  time docker push $img \
  )

time ( \
  cd $mydir/ubuntu22.04 ; \
  img=ghcr.io/biojppm/c4core/ubuntu22.04:latest ; \
  docker build -t $img . ; \
  time docker push $img \
  )
