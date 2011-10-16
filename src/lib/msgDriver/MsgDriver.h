#ifndef _KVPROXY_MSG_DRIVER_H_
#define _KVPROXY_MSG_DRIVER_H_

#include <iostream>
#include <string>
#include "../proto/kvproxy.pb.h"

namespace kvproxy_msg_driver {

    class MsgDriver {
        public:
            MsgDriver() {}
            virtual bool requestHasTrace(const kvproxy_proto::Request &) const;
            virtual std::string requestTrace(const kvproxy_proto::Request &) const;
            virtual bool requestHasKey(const kvproxy_proto::Request &) const;
            virtual std::string requestKey(const kvproxy_proto::Request &) const;
            virtual bool requestHasVal(const kvproxy_proto::Request &) const;
            virtual std::string requestVal(const kvproxy_proto::Request &) const;
            virtual bool requestHasDataStoreTypeId(
                    const kvproxy_proto::Request &) const;
            virtual unsigned requestDataStoreTypeId(
                    const kvproxy_proto::Request &) const;
            virtual ~MsgDriver() {}
        private:
            MsgDriver(const MsgDriver&);
            MsgDriver& operator=(const MsgDriver&);
    };

};

#endif

