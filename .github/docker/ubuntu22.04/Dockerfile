ARG  EMSCRIPTEN_VERSION=latest
FROM ubuntu:22.04
FROM emscripten/emsdk:$EMSCRIPTEN_VERSION

LABEL org.opencontainers.image.source=https://github.com/biojppm/c4core
LABEL org.opencontainers.image.description="C++ build testing: gcc9-gcc12, clang9-clang16, arm-eabi-none, swig, emscripten"
LABEL org.opencontainers.image.licenses=MIT
LABEL maintainer=dev@jpmag.me
LABEL version=1.0
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

# gcc
RUN \
    # gcc 4.9 requires this
    wget -O /tmp/libmpfr4_3.1.4-1_amd64.deb http://security.ubuntu.com/ubuntu/pool/main/m/mpfr4/libmpfr4_3.1.4-1_amd64.deb \
 && dpkg -i /tmp/libmpfr4_3.1.4-1_amd64.deb \
 && apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 6AF7F09730B3F0A4 \
 && apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 40976EAF437D05B5 \
 && apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 3B4FE6ACC0B21F32 \
 && apt-add-repository --yes --no-update ppa:ubuntu-toolchain-r/test \
 && apt-add-repository --yes --no-update "deb http://dk.archive.ubuntu.com/ubuntu/ xenial main" \
 && apt-add-repository --yes --no-update "deb http://dk.archive.ubuntu.com/ubuntu/ xenial universe" \
 && apt-add-repository --yes --no-update "deb http://dk.archive.ubuntu.com/ubuntu/ bionic main" \
 && apt-add-repository --yes --no-update "deb http://dk.archive.ubuntu.com/ubuntu/ bionic universe" \
 && apt-get update \
 && apt-get install -y \
      #cpp-13 gcc-13 g++-13 g++-13-multilib libstdc++-13-dev lib32stdc++-13-dev \
      cpp-12 gcc-12 g++-12 g++-12-multilib libstdc++-12-dev lib32stdc++-12-dev \
      cpp-11 gcc-11 g++-11 g++-11-multilib libstdc++-11-dev lib32stdc++-11-dev \
      cpp-10 gcc-10 g++-10 g++-10-multilib libstdc++-10-dev lib32stdc++-10-dev \
      cpp-9  gcc-9  g++-9  g++-9-multilib  libstdc++-9-dev  lib32stdc++-9-dev \
      #cpp-8 gcc-8 g++-8 g++-8-multilib libstdc++-8-dev lib32stdc++-8-dev \
      #cpp-7 gcc-7 g++-7 g++-7-multilib libstdc++-7-dev lib32stdc++-7-dev \
      cpp-6 gcc-6 g++-6 \
      cpp-5 gcc-5 g++-5 \
      cpp-4.9 gcc-4.9 g++-4.9 \
      cpp-4.8 gcc-4.8 g++-4.8 \
 && echo "gcc done"

# clang
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key 2>/dev/null | apt-key add - && \
 && apt-add-repository --yes --no-update "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-9 main" && \
 && apt-add-repository --yes --no-update "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-10 main" && \
 && apt-add-repository --yes --no-update "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-11 main" && \
 && apt-add-repository --yes --no-update "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-12 main" && \
 && apt-add-repository --yes --no-update "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-13 main" && \
 && apt-add-repository --yes --no-update "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-14 main" && \
 && apt-add-repository --yes --no-update "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-15 main" && \
 && apt-add-repository --yes --no-update "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-16 main" && \
 && apt-get update \
 && apt-get install -y \
      clang-16 clang-tidy-16 \
      clang-15 clang-tidy-15 \
      clang-14 clang-tidy-14 \
      clang-13 clang-tidy-13 \
      clang-12 clang-tidy-12 \
      clang-11 clang-tidy-11 \
      clang-10 clang-tidy-10 \
      clang-9  clang-tidy-9 \
      #clang-8  clang-tidy-8 \
      #clang-7  clang-tidy-7 \
      #clang-6.0 clang-tidy-6.0 \
      #clang-5.0 clang-tidy-5.0 \
      #clang-4.0 clang-tidy-4.0 \
      #clang-3.9 clang-tidy-3.9 \
 && echo "clang done"

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

# arm-none-eabi
# taken from: https://askubuntu.com/questions/1243252/how-to-install-arm-none-eabi-gdb-on-ubuntu-20-04-lts-focal-fossa/1371525#1371525
RUN ( \
        echo "Creating gcc-arm-none-eabi debian package" ; \
        set -xe ; \
        ver=10.3-2021.10 ; \
        longver=15:10.3-2021.10-9 ; \
        name=/tmp/gcc-arm-none-eabi-$ver ; \
        rm -rf /tmp/gcc-arm-none-eabi-$ver* ; \
        echo "Downloading..." ; \
        wget -nv -O $name.tar "https://developer.arm.com/-/media/Files/downloads/gnu-rm/$ver/gcc-arm-none-eabi-$ver-x86_64-linux.tar.bz2" ; \
        echo "Extracting..." ; \
        ( cd /tmp ; tar -xf $name.tar ) ; \
        rm -f $name.tar ; \
        echo "Generating debian package..." ; \
        mkdir -p $name-dpkg/DEBIAN ; \
        mkdir -p $name-dpkg/usr ; \
        echo "Package: gcc-arm-none-eabi" >> $name-dpkg/DEBIAN/control ; \
        echo "Version: $longver" >> $name-dpkg/DEBIAN/control ; \
        echo "Architecture: amd64" >> $name-dpkg/DEBIAN/control ; \
        echo "Maintainer: maintainer" >> $name-dpkg/DEBIAN/control ; \
        echo "Description: Arm Embedded toolchain" >> $name-dpkg/DEBIAN/control ; \
        cat $name-dpkg/DEBIAN/control ; \
        mv -f $name/* $name-dpkg/usr/ ; \
        echo "Building debian package..." ; \
        dpkg-deb --build --root-owner-group $name-dpkg ; \
        mv -v $name-dpkg.deb $name.deb ; \
        dpkg -i $name.deb ; \
        echo "Testing install..." ; \
        arm-none-eabi-gcc --version ; \
        arm-none-eabi-g++ --version \
    ) \
 && echo "arm done"

RUN apt-get install -y \
      qemu \
 && echo "qemu done"

RUN apt-get install -y \
      valgrind \
 && echo "valgrind done"

# cmany
RUN ( cd tmp \
 && git clone --recursive https://github.com/biojppm/cmany -b dev \
 && cd cmany \
 && pip install . ) \
 && echo "cmany done"

RUN \
    # Standard Cleanup on Debian images
    &&  apt-get -y clean \
    &&  apt-get -y autoclean \
    &&  apt-get -y autoremove \
    &&  rm -rf /var/lib/apt/lists/* \
    &&  rm -rf /var/cache/debconf/*-old \
    &&  rm -rf /usr/share/doc/* \
    &&  rm -rf /usr/share/man/?? \
    &&  rm -rf /usr/share/man/??_* \
 && echo "cleanup done"
