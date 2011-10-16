#include <iostream>
#include "ProtoUtils.h"
#include "../utils/dbg_utils/Macros.h"

using namespace kvproxy_proto;
using namespace kvproxy_proto_utils;

Response ProtoUtils::response(Response_Header_ReturnCode rc, const
        std::string &hdrPayload, const std::string &data) {
    TRACE(std::cout, "");
    Response retVal;
    Response_Header *hdr = retVal.mutable_header();
    hdr->set_returncode(rc);
    if (!hdrPayload.empty() && hdrPayload.size()) {
        TRACE(std::cout, "");
        hdr->set_payload(hdrPayload);
    }
    if (!data.empty() && data.size()) {
        TRACE(std::cout, "");
        retVal.set_payload(data);
    }
    TRACE(std::cout, "");
    return retVal;
}

