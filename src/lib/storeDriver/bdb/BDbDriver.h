#ifndef _BDB_DRIVER_H_
#define _BDB_DRIVER_H_

#include "../DataStoreDriver.h"
#include <db_cxx.h>

namespace bdb_data_store_driver {

    class BDbDriver : public kvproxy_data_store_driver::DataStoreDriver {
        public:
            BDbDriver(std::string dbHome, std::string dbPath,
                    int flag = 0, int pageSz = 0);
            ~BDbDriver();
            std::string lookup(const std::string &) const
                throw(kvproxy_data_store_driver::KeyNotFoundException);
            void insert(const std::string &, const std::string &,
                    bool overwrite = false)
                throw(kvproxy_data_store_driver::KeyPresentException,
                        kvproxy_data_store_driver::InsertionException);
            void remove(const std::string &)
                throw(kvproxy_data_store_driver::KeyNotFoundException,
                        kvproxy_data_store_driver::DeletionException);
        private:
            BDbDriver(const BDbDriver &);
            BDbDriver& operator=(const BDbDriver &);
            Db *m_db;
            DbEnv *m_dbenv;
    };

};

#endif
