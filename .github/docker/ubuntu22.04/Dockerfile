ARG  EMSCRIPTEN_VERSION=latest
FROM ubuntu:22.04
FROM emscripten/emsdk:$EMSCRIPTEN_VERSION

LABEL org.opencontainers.image.source=https://github.com/biojppm/c4core
LABEL org.opencontainers.image.description="C++ build testing: gcc9-gcc12, clang9-clang16, arm-eabi-none, swig, emscripten"
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
      git \
      build-essential \
      swig \
      musl-dev \
    && \
    echo "basics done"

# cmake
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add - \
 && wget -O /tmp/libssl1.1_1.1.1f-1ubuntu2_amd64.deb http://archive.ubuntu.com/ubuntu/pool/main/o/openssl/libssl1.1_1.1.1f-1ubuntu2_amd64.deb \
 && dpkg -i /tmp/libssl1.1_1.1.1f-1ubuntu2_amd64.deb \
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
        cd /tmp ; \
        git clone --recursive https://github.com/biojppm/cmany -b dev ; \
        pip install ./cmany \
    ) \
 && echo "cmany done"

# valgrind
RUN apt-get install -y valgrind \
 && echo "valgrind done"

ADD c4core-install /usr/bin/c4core-install

RUN \
    # Standard Cleanup on Debian images
    apt-get -y clean \
    &&  apt-get -y autoclean \
    &&  apt-get -y autoremove \
    &&  rm -rf /var/lib/apt/lists/* \
    &&  rm -rf /var/cache/debconf/*-old \
    &&  rm -rf /usr/share/doc/* \
    &&  rm -rf /usr/share/man/?? \
    &&  rm -rf /usr/share/man/??_* \
 && echo "cleanup done"
