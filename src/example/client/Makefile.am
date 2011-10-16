export BASE_DIR=../..
export PROTO_UTILS=$(BASE_DIR)/lib/proto

#ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS = -I/usr/local/include/ -I/usr/include/
AM_LDFLAGS = \
             -L/usr/lib/ -Wl,-rpath,/usr/lib \
             -L/usr/local/lib/ -Wl,-rpath,/usr/local/lib \
             -lprotobuf \
             -lzmq
SUBDIRS =
bin_PROGRAMS = client
client_SOURCES = Client.cpp
client_LDADD =  \
                 $(PROTO_UTILS)/libkvproxy_proto.la \
                 ../server/lib/libexampleHelpers.la
include_HEADERS =
#dis_doc_DATA=README

# Application specific rules
