#!/bin/bash -xe

set -o pipefail

mydir=$(cd $(dirname $0) ; pwd)

time ( \
  cd $mydir/ubuntu18.04 ; \
  img=ghcr.io/biojppm/c4core/ubuntu18.04:latest ; \
  time docker build -t $img . ; \
  time echo # docker push $img \
  )

time ( \
  cd $mydir/ubuntu22.04 ; \
  img=ghcr.io/biojppm/c4core/ubuntu22.04:latest ; \
  docker build -t $img . ; \
  time echo # docker push $img \
  )
