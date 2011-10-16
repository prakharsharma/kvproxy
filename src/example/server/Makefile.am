export BASE_DIR=../..
export WORKER_UTILS=$(BASE_DIR)/lib/worker
export MSG_DRIVER_UTILS=$(BASE_DIR)/lib/msgDriver
export BDB_DRIVER_UTILS=$(BASE_DIR)/lib/storeDriver/bdb

#ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS = -I/usr/local/include/ -I/usr/include/
AM_LDFLAGS = \
             -L/usr/lib/ -Wl,-rpath,/usr/lib \
             -L/usr/local/lib/ -Wl,-rpath,/usr/local/lib \
             -llog4cxx \
             -lzmq \
             -lprotobuf \
             -ldb_cxx
SUBDIRS = lib
bin_PROGRAMS = DbProxy
DbProxy_SOURCES = DbProxy.cpp
DbProxy_LDADD =  $(WORKER_UTILS)/libkvproxy_worker.la \
                 $(BDB_DRIVER_UTILS)/libkvproxy_bdb_driver.la \
                 $(MSG_DRIVER_UTILS)/libkvproxy_msg_driver.la \
                 $(BASE_DIR)/lib/libkvproxy.la \
                 ./lib/libexampleHelpers.la
include_HEADERS =
#dis_doc_DATA=README

# Application specific rules
