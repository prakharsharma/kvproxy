#include <iostream>
#include <zmq.hpp>
#include <unistd.h>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "ProxyServer.h"
#include "storeDriver/DataStoreDriver.h"
#include "utils/dbg_utils/Macros.h"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace kvproxy_server;
using namespace kvproxy_worker;
using namespace kvproxy_proto;
using namespace kvproxy_data_store_driver;
using namespace kvproxy_msg_driver;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("kvproxy.proxyServer"));

void ProxyServer::run() {
    TRACE(std::cout, "");
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    std::string addr = m_protocol + "://*:" + m_port;
    socket.bind(addr.c_str());
    while (true) {
        TRACE(std::cout, "");
        zmq::message_t z_req;
        socket.recv(&z_req);
        std::string s_req((char *)z_req.data(),
                z_req.size());
        Request req;
        req.ParseFromString(s_req);
        // if trace is in Request_Header, then change log level of appender
        // to trace
        if (m_msgDriver->requestHasTrace(req)) {
            TRACE(std::cout, "");
            logger->setLevel(log4cxx::Level::getTrace());
        }
        if (m_reqPrinter) {
            TRACE(std::cout, "");
            LOG4CXX_TRACE(logger, "Request: "<<(m_reqPrinter(req)));
        }
        Response *resp = NULL;
        // All the anlytics will be logged in this function
        switch (req.header().type()) {
            case Request_Header_Type_LOOKUP:
                {
                    TRACE(std::cout, "");
                    START_LATENCY_MEASURE(latency);
                    try {
                        m_worker->lookup(req);
                    } catch (KeyNotFoundException &e) {
                        TRACE(std::cout, "");
                        LOG4CXX_DEBUG(logger, "Key not found in store: "
                                <<e.what());
                    }
                    resp = m_worker->response();
                    END_LATENCY_MEASURE(latency, "latency", m_sb, true);
                    END_LATENCY_MEASURE(latency, "lookupLatency", m_sb, true);
                    break;
                }
            case Request_Header_Type_INSERT:
                {
                    TRACE(std::cout, "");
                    START_LATENCY_MEASURE(latency);
                    try {
                        m_worker->insert(req);
                    } catch (KeyPresentException &e) {
                        TRACE(std::cout, "");
                        LOG4CXX_DEBUG(logger, "Key already present in"
                                " store: "<<e.what());
                    } catch (InsertionException &e) {
                        TRACE(std::cout, "");
                        LOG4CXX_DEBUG(logger, "Insertion error: "
                                <<e.what());
                    }
                    resp = m_worker->response();
                    END_LATENCY_MEASURE(latency, "latency", m_sb, true);
                    END_LATENCY_MEASURE(latency, "insertionLatency", m_sb, true);
                    break;
                }
            case Request_Header_Type_REMOVE:
                {
                    TRACE(std::cout, "");
                    START_LATENCY_MEASURE(latency);
                    try {
                        m_worker->remove(req);
                    } catch (KeyNotFoundException &e) {
                        TRACE(std::cout, "");
                        LOG4CXX_DEBUG(logger, "Key not found in store: "
                                <<e.what());
                    } catch (DeletionException &e) {
                        TRACE(std::cout, "");
                        LOG4CXX_DEBUG(logger, "Deletion error: "
                                <<e.what());
                    }
                    resp = m_worker->response();
                    END_LATENCY_MEASURE(latency, "latency", m_sb, true);
                    END_LATENCY_MEASURE(latency, "deletionLatency", m_sb, true);
                    break;
                }
            default:
                {
                    TRACE(std::cout, "");
                    break;
                }
        };
        if (m_respPrinter) {
            TRACE(std::cout, "");
            LOG4CXX_TRACE(logger, "Response : "<<(m_respPrinter(*resp)));
        }
        std::string s_resp;
        resp->SerializeToString(&s_resp);
        delete resp;
        zmq::message_t reply(s_resp.size());
        memcpy((void *)reply.data(), (void *)s_resp.c_str(),
                s_resp.size());
        socket.send(reply);
    }
}

ProxyServer::~ProxyServer() {
}

ProxyServer::ProxyServer(std::string appName, std::string proto,
        std::string port, Worker *worker, MsgDriver *msgDriver,
        RequestPrintFunc reqPrinter = NULL,
        ResponsePrintFunc respPrinter = NULL) : m_protocol(proto),
                                     m_port(port),
                                     m_worker(worker),
                                     m_reqPrinter(reqPrinter),
                                     m_msgDriver(msgDriver),
                                     m_respPrinter(respPrinter) {
                          }
//

