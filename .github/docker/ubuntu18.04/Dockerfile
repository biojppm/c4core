FROM ubuntu:18.04

# to build, use the script build_and_push.sh, located above

LABEL org.opencontainers.image.source=https://github.com/biojppm/c4core
LABEL org.opencontainers.image.description="C++ build testing: gcc4.8-gcc11, clang3.9-clang16, arm-eabi-none, swig"
LABEL org.opencontainers.image.licenses=MIT
LABEL maintainer=dev@jpmag.me
LABEL version=1.2
SHELL ["/bin/bash", "-c"]

RUN apt-get update \
 && apt-get install -y \
      apt-utils \
      ca-certificates \
      lsb-core \
      gnupg \
      software-properties-common \
      wget \
      curl \
      sudo \
      build-essential \
      swig \
      musl-dev \
    && \
    echo "basics done"

# git version must be >= 2.18
RUN add-apt-repository --yes --no-update ppa:git-core/ppa \
 && apt-get update \
 && apt-get install -y \
      git \
 && echo "git done"

# cmake
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add - \
 && apt-add-repository --yes --no-update "deb https://apt.kitware.com/ubuntu/ bionic main" \
 && apt-get update \
 && apt-get install -y \
      cmake \
      ninja-build \
 && echo "cmake done"

# 32 bit requirements
RUN dpkg --add-architecture i386 \
 && apt-get update \
 && apt-get install -y \
      linux-libc-dev:i386 \
      libc6:i386 \
      libc6-dev:i386 \
      libc6-dbg:i386 \
      g++-multilib \
 && echo "32bit done"

# libc++
RUN apt-get install -y \
      clang \
      libc++1 \
      libc++abi-dev \
      libc++-dev \
 && echo "libc++ done"

# coverage
RUN apt-get install -y \
      lcov \
      libffi-dev \
      libssl-dev \
 && echo "coverage done"

# python
RUN apt-get install -y \
      python3 \
      python3-setuptools \
      python3-pip \
 && pip3 install setuptools-rust \
 && pip3 install --upgrade pip \
 && pip3 install \
      requests[security] \
      pyopenssl \
      ndg-httpsclient \
      pyasn1 \
      cpp-coveralls \
 && echo "python done"

# cmany
RUN ( \
       set -xe ; \
        cd /tmp ; \
        git clone --recursive https://github.com/biojppm/cmany -b dev ; \
        pip install ./cmany \
    ) \
 && echo "cmany done"

# valgrind
RUN apt-get install -y valgrind \
 && echo "valgrind done"

# helper to install compiler versions, etc
ADD c4core-install /usr/bin/c4core-install


## actions/checkout@v4 needs a different version of nodejs:
## node 18 and 20 failed with this:
##    error:node: /lib/x86_64-linux-gnu/libc.so.6: version `GLIBC_2.28' not found (required by node)
#RUN cd /tmp \
#  && git clone --recursive https://github.com/nodejs/node --branch v20.12.1 \
#  && cd node \
#  && /usr/bin/c4core-install g++-10 \
#  && env CXX=g++-10 ./configure \
#  && make -j `nproc` \
#  && make install \
#  && cd .. \
#  && rm -rf node
## actions/checkout@v4 needs a different version of nodejs:
## node 18 and 20 failed with this:
##    error:node: /lib/x86_64-linux-gnu/libc.so.6: version `GLIBC_2.28' not found (required by node)
## see https://tecadmin.net/how-to-install-nvm-on-ubuntu-20-04/
## see https://stackoverflow.com/questions/25899912/how-to-install-nvm-in-docker
## /usr/local/nvm or ~/.nvm , depending
#ENV NVM_DIR /usr/local/nvm
##ENV NVM_DIR ~/.nvm
#ENV NODE_VERSION 20.12.2
#ENV NODE_PATH $NVM_DIR/versions/node/v$NODE_VERSION/lib/node_modules
#ENV PATH $NVM_DIR/versions/node/v$NODE_VERSION/bin:$PATH
#SHELL ["/bin/bash", "--login", "-i", "-c"]  # https://stackoverflow.com/questions/25899912/how-to-install-nvm-in-docker/60137919#60137919
#RUN mkdir $NVM_DIR \
#  && curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.35.2/install.sh | bash \
#  && whoami \
#  && echo "NVM_DIR=$NVM_DIR" \
#  && . $NVM_DIR/nvm.sh \
#  && nvm install $NODE_VERSION \
#  && nvm alias default $NODE_VERSION \
#  && nvm use default \
#  # node 20 doesn't work because it requires glib-2.28
#  #&& set -x \
#  #&& which node \
#  #&& `which node` --version \
#  #&& node --version
#
##
## so install glibc-2.28!
##
#RUN echo "glibc-2.28" \
#  && apt install -y gawk \
#  && cd /tmp \
#  && wget -c https://ftp.gnu.org/gnu/glibc/glibc-2.28.tar.gz \
#  && tar -zxf glibc-2.28.tar.gz \
#  && cd glibc-2.28 \
#  && ../configure --prefix=/opt/glibc-2.28 \
#  && make -j `nproc` \
#  && make install \
#  && cd .. \
#  && rm -fr glibc-2.28 glibc-2.28.tar.gz
##
## Patch the installed Node 20 to work with /opt/glibc-2.28 instead:
##
#RUN echo "patching node to use glibc 2.28" \
#  && apt install -y patchelf \
#  && patchelf --set-interpreter \
#  #&& /opt/glibc-2.28/lib/ld-linux-x86_64.so.2 --set-rpath /opt/glibc-2.28/lib/:/lib/aarch64-linux-gnu/:/usr/lib/aarch64-linux-gnu/ /usr/local/bin/node


# Standard Cleanup on Debian images
RUN apt-get -y clean \
  && apt-get -y autoclean \
  && apt-get -y autoremove \
  && rm -rf /var/lib/apt/lists/* \
  && rm -rf /var/cache/debconf/*-old \
  && rm -rf /usr/share/doc/* \
  && rm -rf /usr/share/man/?? \
  && rm -rf /usr/share/man/??_* \
  && echo "cleanup done"
