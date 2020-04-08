#!/bin/bash

cmake --build build/64 --target c4core-coverage-submit-codecov
cmake --build build/32 --target c4core-coverage-submit-codecov
cmake --build build/64 --target c4core-coverage-submit-coveralls
cmake --build build/32 --target c4core-coverage-submit-coveralls
