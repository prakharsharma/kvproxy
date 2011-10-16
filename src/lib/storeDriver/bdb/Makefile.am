export CURR_DIR=`pwd`
export BASE_DIR=$(CURR_DIR)/../..

#ACLOCAL_AMFLAGS = -I m4
SUBDIRS =
AM_CXXFLAGS = \
              -I/usr/local/include \
              -I/usr/include
AM_LDFLAGS = \
             -L/usr/local/lib/ -Wl,-rpath,/usr/local/lib \
             -L/usr/lib/ -Wl,-rpath,/usr/lib \
             -llog4cxx \
             -ldb_cxx
lib_LTLIBRARIES = libkvproxy_bdb_driver.la
libkvproxy_bdb_driver_la_SOURCES = BDbDriver.cpp
libkvproxy_bdb_driver_la_LDFLAGS = -version-info 0:0:0
include_HEADERS = BDbDriver.h

#bin_PROGRAMS = test
#test_SOURCES = driverTest.cpp
#test_LDADD = $(lib_LTLIBRARIES)
#dis_doc_DATA=README

# Application specific rules
