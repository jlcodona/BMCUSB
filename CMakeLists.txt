PROJECT(bmc_multidm)
cmake_minimum_required(VERSION 2.8)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2)

SUBDIRS(src)


