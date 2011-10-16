#ifndef _MESSAGE_UTILS_H_
#define _MESSAGE_UTILS_H_

#include <iostream>
#include <sstream>
#include <string>
#include "dbRecord.pb.h"
#include "../../../lib/proto/kvproxy.pb.h"

namespace message_utils {

    enum AppStore {
        PERSON_STORE = 0
    };

    myApp_proto::Id makeId(int id);
    myApp_proto::Person makePerson(const std::string &name,
            const std::string &ph);
    kvproxy_proto::Request makeRequest(kvproxy_proto::Request_Header_Type type,
            const myApp_proto::Id &,
            const myApp_proto::Person &, bool overwrite,
            const std::string &trace);

    std::string idToString(const myApp_proto::Id &);
    std::string personToString(const myApp_proto::Person &);

    std::string requestToString(const kvproxy_proto::Request &);
    std::string responseToString(const kvproxy_proto::Response &);

};

#endif

