#!/bin/bash
#
# Build script to generate main program and route creator

g++ --std=c++14 -Wall -Werror -pedantic -o dijkstras.out Reader.cpp galaxy.cpp main.cpp

g++ --std=c++14 -Wall -Werror -pedantic -o route_creator.out Reader.cpp galaxy.cpp route_creator.cpp