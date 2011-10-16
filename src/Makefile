export PKG_CONFIG_PATH=/home/y/lib/pkgconfig
export BASE_DIR=$(shell pwd)/..

ROOT=/home/y
include $(ROOT)/share/yahoo_cfg/Make.defs

CLEAN +=

ifeq ($(YMK_OS), FreeBSD)
OPTFLAG += -ftemplate-depth-50
endif

SUBDIRS = \
          lib \
          example \

include $(ROOT)/share/yahoo_cfg/Make.rules
# Do not place any rules or targets above this line

