export CURR_DIR=`pwd`
export BASE_DIR=$(CURR_DIR)/..
export PROTO_UTILS=$(BASE_DIR)/proto

ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS = -I/usr/local/include/ -I/usr/include/
AM_LDFLAGS = \
             -L/usr/local/lib/ -Wl,-rpath,/usr/local/lib \
             -L/usr/lib/ -Wl,-rpath,/usr/lib \
			 -L$(PROTO_UTILS) -Wl,-rpath,$(PROTO_UTILS) \
             -lkvproxy_proto
lib_LTLIBRARIES = libkvproxy_msg_driver.la
libkvproxy_msg_driver_la_SOURCES = MsgDriver.cpp
libkvproxy_msg_driver_la_LDFLAGS = -version-info 0:0:0
include_HEADERS = MsgDriver.h
SUBDIRS=
#dis_doc_DATA=README

# Application specific rules
