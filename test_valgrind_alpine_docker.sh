#!/bin/bash

docker build -f Dockerfile_alpine . --tag alpine-valgrind-image
docker rm -f alpine-valgrind-container
docker run --user root --name alpine-valgrind-container alpine-valgrind-image && docker rm -f alpine-valgrind-container