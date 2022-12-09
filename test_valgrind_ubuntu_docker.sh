#!/bin/bash

docker build -f Dockerfile_ubuntu . --tag ubuntu-valgrind-image
docker rm -f ubuntu-valgrind-container
docker run --user root --name ubuntu-valgrind-container ubuntu-valgrind-image && docker rm -f ubuntu-valgrind-container