#!/bin/bash

docker build . --tag ubuntu-valgrind-image

# docker run --user root -v /$(pwd)/:/project/ --name ubuntu-valgrind-container ubuntu-valgrind-image
docker run --user root --name ubuntu-valgrind-container ubuntu-valgrind-image && docker rm -f ubuntu-valgrind-container