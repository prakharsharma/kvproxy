export PROTO_PATH=/usr/bin

#ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS = -I/usr/local/include/ -I/usr/include/
AM_LDFLAGS = \
             -L/usr/local/lib/ -Wl,-rpath \
             -L/usr/lib/ -Wl,-rpath \
             -lprotobuf
lib_LTLIBRARIES = libkvproxy_proto.la
BUILT_SOURCES = kvproxy.pb.cc
libkvproxy_proto_la_SOURCES = kvproxy.pb.cc ProtoUtils.cpp
libkvproxy_proto_la_LDFLAGS = -version-info 0:0:0
include_HEADERS = kvproxy.pb.h
CLEANFILES = kvproxy.pb.h kvproxy.pb.cc
SUBDIRS=
#dis_doc_DATA=README

# Application specific rules
kvproxy.pb.cc: kvproxy.pb.h

kvproxy.pb.h: ./kvproxy.proto
	$(PROTO_PATH)/protoc --cpp_out=./ kvproxy.proto
