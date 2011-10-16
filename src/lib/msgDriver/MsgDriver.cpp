#include "MsgDriver.h"
#include "../proto/kvproxy.pb.h"

using namespace kvproxy_msg_driver;
using namespace kvproxy_proto;

bool MsgDriver::requestHasTrace(const Request &req) const {
    return req.has_header() && req.header().has_trace() &&
        !req.header().trace().empty() && req.header().trace().size();
}

std::string MsgDriver::requestTrace(const Request &req) const {
    return req.header().trace();
}

bool MsgDriver::requestHasKey(const Request &req) const {
    return req.has_payload() && req.payload().has_key() &&
        !req.payload().key().empty() && req.payload().key().size();
}

std::string MsgDriver::requestKey(const Request &req) const {
    return req.payload().key();
}

bool MsgDriver::requestHasVal(const Request &req) const {
    return req.has_payload() && req.payload().has_value() &&
        !req.payload().value().empty() && req.payload().value().size();
}

std::string MsgDriver::requestVal(const Request &req) const {
    return req.payload().value();
}

bool MsgDriver::requestHasDataStoreTypeId(const Request &req) const {
    return req.has_header() && req.header().has_datastoredriverid();
}

unsigned MsgDriver::requestDataStoreTypeId(const Request &req) const {
    return req.header().datastoredriverid();
}

