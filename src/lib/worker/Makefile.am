export CURR_DIR=`pwd`
export BASE_DIR=$(CURR_DIR)/..
export PROTO_UTILS=$(BASE_DIR)/proto
export MSG_DRIVER_UTILS=$(BASE_DIR)/msgDriver
export BOOST_CXX_LIB=/usr/include

ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS = -I/usr/local/include/ -I/usr/include/ -I$(BOOST_CXX_LIB)
AM_LDFLAGS = \
             -L/usr/local/lib/ -Wl,-rpath,/usr/local/lib \
             -L/usr/lib/ -Wl,-rpath,/usr/lib \
			 -L$(PROTO_UTILS) -Wl,-rpath,$(PROTO_UTILS) \
			 -L$(MSG_DRIVER_UTILS) -Wl,-rpath,$(MSG_DRIVER_UTILS) \
             -llog4cxx \
             -lkvproxy_proto \
             -lkvproxy_msg_driver
lib_LTLIBRARIES = libkvproxy_worker.la
libkvproxy_worker_la_SOURCES = Worker.cpp
libkvproxy_worker_la_LDFLAGS = -version-info 0:0:0
include_HEADERS = Worker.h
SUBDIRS=
#dis_doc_DATA=README

# Application specific rules
