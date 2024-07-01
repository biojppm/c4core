#!/bin/bash -xe

set -o pipefail

# To run an image:
#
#  docker run -it --network host -v ~/proj/rapidyaml:/rapidyaml --name c4core_ubuntu22 ghcr.io/biojppm/c4core/ubuntu22.04:latest /bin/bash
#
# To delete all containers including its volumes use,
#
#   docker rm -vf $(docker ps -aq)
#
# To delete all the images,
#
#   docker rmi -f $(docker images -aq)
#
# Remember, you should remove all the containers before removing all
# the images from which those containers were created. See
# https://stackoverflow.com/questions/44785585/how-can-i-delete-all-local-docker-images


if [ -z "$CR_USR" ] ; then
    echo "error: \$CR_USR is not defined. Set it to a user name."
    exit 1
fi
if [ -z "$CR_PAT" ] ; then
    echo "error: \$CR_PAT is not defined. Set it to a personal access token."
    exit 1
fi

# see https://docs.github.com/en/packages/working-with-a-github-packages-registry/working-with-the-container-registry#building-container-images
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
