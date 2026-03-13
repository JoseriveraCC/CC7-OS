#!/bin/bash
make clean
make
./log_analyzer access.log
