export BASE_DIR=../../..
export PROTO_UTILS=$(BASE_DIR)/lib/proto
export PROTO_PATH=/usr/bin

#ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS = -I/usr/local/include/ -I/usr/include/
AM_LDFLAGS = \
             -L/usr/local/lib/ -Wl,-rpath,/usr/local/lib \
             -L/usr/lib/ -Wl,-rpath,/usr/lib \
             -lprotobuf
lib_LTLIBRARIES = libexampleHelpers.la
BUILT_SOURCES = dbRecord.pb.cc
libexampleHelpers_la_SOURCES = dbRecord.pb.cc MessageUtils.cpp
libexampleHelpers_la_LDFLAGS = -version-info 0:0:0
libexampleHelpers_la_LIBADD =  $(PROTO_UTILS)/libkvproxy_proto.la
include_HEADERS = MessageUtils.h
CLEANFILES = dbRecord.pb.h dbRecord.pb.cc
SUBDIRS=
#dis_doc_DATA=README

# Application specific rules
dbRecord.pb.cc: dbRecord.pb.h

dbRecord.pb.h: ./dbRecord.proto
	$(PROTO_PATH)/protoc --cpp_out=./ dbRecord.proto
