#ifndef _PROXY_SERVER_H_
#define _PROXY_SERVER_H_

#include <string>
//#include <ymonsb.h>
#include "worker/Worker.h"
#include "proto/kvproxy.pb.h"
#include "msgDriver/MsgDriver.h"

typedef std::string (*RequestPrintFunc)(const kvproxy_proto::Request &);
typedef std::string (*ResponsePrintFunc)(const kvproxy_proto::Response &);

namespace kvproxy_server {

    class ProxyServer {
        public:
            void run();
            ProxyServer(std::string appName, std::string proto, std::string port,
                    kvproxy_worker::Worker *worker,
                    kvproxy_msg_driver::MsgDriver *, RequestPrintFunc,
                    ResponsePrintFunc);
            ~ProxyServer();
        private:
            ProxyServer(const ProxyServer &);
            ProxyServer& operator=(const ProxyServer &);
            
            std::string m_protocol;
            std::string m_port;
            kvproxy_worker::Worker *m_worker;
            const kvproxy_msg_driver::MsgDriver *m_msgDriver;
            //ymonsb m_sb;
            RequestPrintFunc m_reqPrinter;
            ResponsePrintFunc m_respPrinter;
    };

};

#endif
