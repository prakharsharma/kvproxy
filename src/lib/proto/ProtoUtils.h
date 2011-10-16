#ifndef _KVPROXY_PROTO_UTILS_H_
#define _KVPROXY_PROTO_UTILS_H_

#include <string>
#include "kvproxy.pb.h"

namespace kvproxy_proto_utils {

    class ProtoUtils {
        private:
        public:
            static kvproxy_proto::Response
                response(kvproxy_proto::Response_Header_ReturnCode rc,
                        const std::string &hdrPayload, const std::string &data);
    };
    
};

#endif
