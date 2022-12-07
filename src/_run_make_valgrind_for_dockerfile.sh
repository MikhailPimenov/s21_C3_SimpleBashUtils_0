#!/bin/bash

make clean

valgrind_output=$(make valgrind)
