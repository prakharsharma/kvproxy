#include <iostream>
#include <fstream>
#include <sstream>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "Worker.h"
#include "../proto/ProtoUtils.h"
#include "../utils/string_utils/StringUtils.hh"
#include "../utils/dbg_utils/Macros.h"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace kvproxy_proto;
using namespace kvproxy_data_store_driver;
using namespace kvproxy_worker;
using namespace kvproxy_proto_utils;
using namespace gen_utils;
using namespace kvproxy_msg_driver;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("kvproxy.worker"));

Worker::Worker(boost::unordered_map<unsigned, DataStoreDriver*> driverMap,
        const MsgDriver *msgDriver) :
    m_dataStoreDriverMap(driverMap), m_msgDriver(msgDriver) {
    TRACE(std::cout, "");
}

Worker::~Worker() {
    // m_response pointer is deleted by the caller of the
    // Worker::response() function
    LOG4CXX_TRACE(logger, "");
}

Response* Worker::response() const {
    // caller of this function is supposed to have memory allocated for it.
    TRACE(std::cout, "");
    LOG4CXX_TRACE(logger, "");
    return m_response;
}

DataStoreDriver* Worker::dataStoreDriver(const Request &request)
throw(DataStoreNotFoundException) {
    TRACE(std::cout, "");
    std::string val;
    if (!m_msgDriver->requestHasDataStoreTypeId(request)) {
        TRACE(std::cout, "");
        LOG4CXX_TRACE(logger, "");
        std::stringstream ss;
        ss<<"["<<__FUNCTION__<<"@"<<__FILE__<<":"<<__LINE__<<"] "
            <<"Data store id should be supplied in request.";
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_INVALID_REQ_ERR,
                    ss.str(), val);
        throw DataStoreNotFoundException(ss.str());
    }
    unsigned id = m_msgDriver->requestDataStoreTypeId(request);
    if (m_dataStoreDriverMap.find(id) == m_dataStoreDriverMap.end()) {
        TRACE(std::cout, "");
        LOG4CXX_TRACE(logger, "");
        std::stringstream ss;
        ss<<"["<<__FUNCTION__<<"@"<<__FILE__<<":"<<__LINE__<<"] "
            <<"Data store not found for id "<<id<<".";
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_INVALID_REQ_ERR,
                    ss.str(), val);
        throw DataStoreNotFoundException(ss.str());
    }
    return m_dataStoreDriverMap.find(id)->second;
}

void Worker::lookup(const Request &request)
throw(KeyNotFoundException, InvalidRequestException, DataStoreNotFoundException) {
    TRACE(std::cout, "");
    m_response = new Response();
    std::string val, hdrPayld;
    if (!m_msgDriver->requestHasKey(request)) {
        TRACE(std::cout, "");
        LOG4CXX_TRACE(logger, "");
        std::stringstream ss;
        ss<<"["<<__FUNCTION__<<"@"<<__FILE__<<":"<<__LINE__<<"] "
            <<"A valid lookup request should have payload, and key.";
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_INVALID_REQ_ERR,
                    ss.str(), val);
        throw InvalidRequestException(ss.str());
    }
    DataStoreDriver *m_dataStoreDriver = dataStoreDriver(request);
    std::string key = m_msgDriver->requestKey(request);
    try {
        TRACE(std::cout, "");
        val = m_dataStoreDriver->lookup(key);
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_OK,
                    hdrPayld, val);
    } catch (KeyNotFoundException &e) {
        TRACE(std::cout, "");
        LOG4CXX_TRACE(logger, "");
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_KEY_NOT_FOUND_ERR,
                    e.what(), val);
        throw e;
    } catch (...) {
        TRACE(std::cout, "");
        LOG4CXX_TRACE(logger, "");
        hdrPayld.append("Lookup error at [")
            .append(__FUNCTION__).append("@").append(__FILE__).append(":")
            .append(string_utils::toString<int>(__LINE__)).append("]");
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_OTHER_ERR,
                    hdrPayld, val);
    }
}

void Worker::insert(const Request &request)
throw(KeyPresentException, InsertionException, InvalidRequestException,
        DataStoreNotFoundException) {
    TRACE(std::cout, "");
    m_response = new Response();
    std::string hdrPayld, respData;
    if (!m_msgDriver->requestHasKey(request) ||
            !m_msgDriver->requestHasVal(request)) {
        TRACE(std::cout, "");
        LOG4CXX_TRACE(logger, "");
        std::stringstream ss;
        ss<<"["<<__FUNCTION__<<"@"<<__FILE__<<":"<<__LINE__<<"] "
            <<"A valid insertion request should have payload, key and"
            <<" value.";
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_INVALID_REQ_ERR,
                    ss.str(), respData);
        throw InvalidRequestException(ss.str());
    }
    DataStoreDriver *m_dataStoreDriver = dataStoreDriver(request);
    std::string key = m_msgDriver->requestKey(request);
    std::string val = m_msgDriver->requestVal(request);
    bool overwrite = request.payload().has_overwrite() &&
        request.payload().overwrite();
    try {
        m_dataStoreDriver->insert(key, val, overwrite);
        TRACE(std::cout, "");
        *m_response = ProtoUtils::response(Response_Header_ReturnCode_OK,
                hdrPayld, respData);
        TRACE(std::cout, "");
    } catch (KeyPresentException &e) {
        TRACE(std::cout, "");
        LOG4CXX_TRACE(logger, "");
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_KEY_PRESENT_ERR,
                    e.what(), respData);
    } catch (InsertionException &e) {
        TRACE(std::cout, "");
        LOG4CXX_TRACE(logger, "");
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_INSERTION_ERR,
                    e.what(), respData);
    } catch (...) {
        TRACE(std::cout, "");
        LOG4CXX_TRACE(logger, "");
        hdrPayld.append("Insertion error at [")
            .append(__FUNCTION__).append("@").append(__FILE__).append(":")
            .append(string_utils::toString<int>(__LINE__)).append("]");
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_OTHER_ERR,
                    hdrPayld, respData);
    }
}

void Worker::remove(const Request &request)
throw(KeyNotFoundException, DeletionException, InvalidRequestException,
        DataStoreNotFoundException) {
    TRACE(std::cout, "");
    m_response = new Response();
    std::string hdrPayld, respData;
    if (m_msgDriver->requestHasKey(request)) {
        LOG4CXX_TRACE(logger, "");
        std::stringstream ss;
        ss<<"["<<__FUNCTION__<<"@"<<__FILE__<<":"<<__LINE__<<"] "
            <<"A valid deletion request should have payload, and key.";
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_INVALID_REQ_ERR,
                    ss.str(), respData);
        throw InvalidRequestException(ss.str());
    }
    DataStoreDriver *m_dataStoreDriver = dataStoreDriver(request);
    std::string key = m_msgDriver->requestKey(request);
    try {
        m_dataStoreDriver->remove(key);
        *m_response = ProtoUtils::response(Response_Header_ReturnCode_OK,
                hdrPayld, respData);
    } catch (KeyNotFoundException &e) {
        TRACE(std::cout, "");
        LOG4CXX_TRACE(logger, "");
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_KEY_NOT_FOUND_ERR,
                    e.what(), respData);
    } catch (DeletionException &e) {
        TRACE(std::cout, "");
        LOG4CXX_TRACE(logger, "");
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_DELETION_ERR,
                    e.what(), respData);
    } catch (...) {
        TRACE(std::cout, "");
        LOG4CXX_TRACE(logger, "");
        hdrPayld.append("Deletion error at [")
            .append(__FUNCTION__).append("@").append(__FILE__).append(":")
            .append(string_utils::toString<int>(__LINE__)).append("]");
        *m_response =
            ProtoUtils::response(Response_Header_ReturnCode_OTHER_ERR,
                    hdrPayld, respData);
    }
}

