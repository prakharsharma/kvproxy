export CURR_DIR=`pwd`
export BASE_DIR=$(CURR_DIR)
export PROTO_UTILS=$(BASE_DIR)/proto
export MSG_DRIVER_UTILS=$(BASE_DIR)/msgDriver

#ACLOCAL_AMFLAGS = -I m4
SUBDIRS = proto \
          msgDriver \
          worker
AM_CXXFLAGS = -I/usr/local/include/ -I/usr/include/
AM_LDFLAGS = \
             -L/usr/local/lib/ -Wl,-rpath,/usr/local/lib \
             -L/usr/lib/ -Wl,-rpath,/usr/lib \
			 -L$(PROTO_UTILS) -Wl,-rpath,$(PROTO_UTILS) \
			 -L$(MSG_DRIVER_UTILS) -Wl,-rpath,$(MSG_DRIVER_UTILS) \
             -llog4cxx \
             -lzmq \
             -lkvproxy_proto \
             -lkvproxy_msg_driver
lib_LTLIBRARIES = libkvproxy.la
libkvproxy_la_SOURCES = ProxyServer.cpp
libkvproxy_la_LDFLAGS = -version-info 0:0:0
include_HEADERS = ProxyServer.h
#dis_doc_DATA=README

# Application specific rules
