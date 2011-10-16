#ifndef _KVPROXY_WORKER_H_
#define _KVPROXY_WORKER_H_

#include <string>
#include <stdexcept>
#include <boost/unordered_map.hpp>
#include "../storeDriver/DataStoreDriver.h"
#include "../proto/kvproxy.pb.h"
#include "../msgDriver/MsgDriver.h"

namespace kvproxy_worker {

    struct InvalidRequestException : public std::runtime_error {
        InvalidRequestException(std::string msg = "") : runtime_error(msg)
                                                        {}
    };

    struct DataStoreNotFoundException : public std::runtime_error {
        DataStoreNotFoundException(std::string msg = "") : runtime_error(msg)
                                                        {}
    };

    class Worker {
        public:
            Worker(boost::unordered_map<unsigned,
                    kvproxy_data_store_driver::DataStoreDriver* >,
                    const kvproxy_msg_driver::MsgDriver *msgDriver);
            virtual ~Worker();
            virtual void lookup(const kvproxy_proto::Request &)
                throw(kvproxy_data_store_driver::KeyNotFoundException,
                        InvalidRequestException, DataStoreNotFoundException);
            virtual void insert(const kvproxy_proto::Request &)
                throw(kvproxy_data_store_driver::KeyPresentException,
                        kvproxy_data_store_driver::InsertionException,
                        InvalidRequestException, DataStoreNotFoundException);
            virtual void remove(const kvproxy_proto::Request &)
                throw(kvproxy_data_store_driver::KeyNotFoundException,
                        kvproxy_data_store_driver::DeletionException,
                        InvalidRequestException, DataStoreNotFoundException);
            virtual kvproxy_proto::Response* response() const;
        protected:
            Worker(const Worker &);
            Worker& operator=(const Worker &);

            kvproxy_data_store_driver::DataStoreDriver* dataStoreDriver(
                    const kvproxy_proto::Request &)
                throw(DataStoreNotFoundException);

            boost::unordered_map<unsigned,
                kvproxy_data_store_driver::DataStoreDriver*> m_dataStoreDriverMap;
            //kvproxy_data_store_driver::DataStoreDriver *m_dataStoreDriver;
            // ProxyServer owns this pointer for deletion
            kvproxy_proto::Response *m_response;
            const kvproxy_msg_driver::MsgDriver *m_msgDriver;
        private:
    };

};

#endif
