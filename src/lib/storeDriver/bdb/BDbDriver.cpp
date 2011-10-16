#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <cstdio>
#include "BDbDriver.h"
#include "../../utils/string_utils/StringUtils.hh"
#include "../../utils/dbg_utils/Macros.h"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace kvproxy_data_store_driver;
using namespace bdb_data_store_driver;
using namespace gen_utils;

static log4cxx::LoggerPtr
logger(log4cxx::Logger::getLogger("kvproxy.dataStoreDriver"));

BDbDriver::BDbDriver(std::string dbHome, std::string dbPath, int flags,
        int pageSz) {
    TRACE(std::cout, "");
    try {
        m_dbenv = new DbEnv(0);
        m_dbenv->set_cachesize(0, 64 * 1024, 0);
        m_dbenv->open(dbHome.c_str(),
                0
                | DB_CREATE
                | DB_INIT_LOCK
                | DB_INIT_LOG
                | DB_INIT_MPOOL
                | DB_INIT_TXN
                | DB_THREAD
                , 0);
        m_db = new Db(m_dbenv, 0);
        if (!pageSz) {
            m_db->set_pagesize(pageSz);
        }
        m_db->open(NULL, dbPath.c_str(), NULL, DB_BTREE,
                flags
                | DB_CREATE
                | DB_THREAD
                | DB_AUTO_COMMIT
                | DB_READ_UNCOMMITTED
                , 0);
    } catch (DbException &e) {
        throw DriverCreationException("Can not create Berkeley DB driver"
                " for file: " + dbPath + ". Error = " + e.what());
    } catch (std::exception &e) {
        throw DriverCreationException("Can not create Berkeley DB driver"
                " for file: " + dbPath + ".");
    }
}

BDbDriver::~BDbDriver() {
    if (m_dbenv) {
        m_db->close(0);
        m_dbenv->close(0);
    }
}

std::string BDbDriver::lookup(const std::string &key) const
throw(KeyNotFoundException) {
    TRACE(std::cout, "");
    Dbt d_key((void *)key.data(), key.size());
    //d_key.set_flags(DB_DBT_MALLOC);
    //int exists = m_db->exists(NULL, &d_key, 0);
    //if (exists  == DB_NOTFOUND) {
    //    TRACE(std::cout, "");
    //    std::stringstream ss;
    //    if (logger->getLevel()->toString().compare("TRACE") == 0) {
    //        ss<<"["<<__FUNCTION__<<"@"<<__FILE__<<":"<<__LINE__<<"] ";
    //    }
    //    ss<<"Key not found in Berkeley DB.\n";
    //    throw KeyNotFoundException(ss.str());
    //}
    Dbt val;
    val.set_flags(DB_DBT_MALLOC);
    int exists = m_db->get(NULL, &d_key, &val, DB_READ_UNCOMMITTED);
    if (exists == DB_NOTFOUND) {
        TRACE(std::cout, "");
        std::stringstream ss;
        //if (logger->getLevel()->toString().compare("TRACE") == 0) {
            ss<<"["<<__FUNCTION__<<"@"<<__FILE__<<":"<<__LINE__<<"] ";
        //}
        ss<<"Key not found in Berkeley DB.\n";
        throw KeyNotFoundException(ss.str());
    }
    std::string retVal((const char *)val.get_data(), val.get_size());
    free(val.get_data());
    TRACE(std::cout, "");
    return retVal;
}

void BDbDriver::insert(const std::string &key, const std::string &val,
        bool overwrite) throw(KeyPresentException, InsertionException) {
    TRACE(std::cout, "");
    Dbt d_key((void *)key.data(), key.size());
    Dbt d_val((void *)val.data(), val.size());
    int flags = overwrite ? 0 : DB_NOOVERWRITE;
    try {
        TRACE(std::cout, "");
        int ret = m_db->put(NULL, &d_key, &d_val, flags);
        if (ret == DB_KEYEXIST) {
            TRACE(std::cout, "");
            std::stringstream ss;
            //if (logger->getLevel()->toString().compare("TRACE") == 0) {
                ss<<"["<<__FUNCTION__<<"@"<<__FILE__<<":"<<__LINE__<<"] ";
            //}
            ss<<"Key already present in Berkeley DB.\n";
            throw KeyPresentException(ss.str());
        }
    } catch (DbException &e) {
        TRACE(std::cout, "");
        std::stringstream ss;
        //if (logger->getLevel()->toString().compare("TRACE") == 0) {
            ss<<"["<<__FUNCTION__<<"@"<<__FILE__<<":"<<__LINE__<<"] ";
        //}
        ss<<"Insertion into Berkeley DB Failed.\n";
        throw InsertionException(ss.str());
    } catch (std::exception &e) {
        TRACE(std::cout, "");
        std::stringstream ss;
        //if (logger->getLevel()->toString().compare("TRACE") == 0) {
            ss<<"["<<__FUNCTION__<<"@"<<__FILE__<<":"<<__LINE__<<"] ";
        //}
        ss<<"Insertion into Berkeley DB Failed.\n";
        throw InsertionException(ss.str());
    }
    TRACE(std::cout, "");
}

void BDbDriver::remove(const std::string &key) throw(KeyNotFoundException,
        DeletionException) {
    try {
        Dbt d_key((void *)key.data(), key.size());
        int ret = m_db->del(NULL, &d_key, 0);
        if (ret == DB_NOTFOUND) {
            TRACE(std::cout, "");
            std::stringstream ss;
            //if (logger->getLevel()->toString().compare("TRACE") == 0) {
                ss<<"["<<__FUNCTION__<<"@"<<__FILE__<<":"<<__LINE__<<"] ";
            //}
            ss<<"Key not found in Berkeley DB for deletion.\n";
            throw KeyNotFoundException(ss.str());
        }
    } catch (DbException &e) {
        TRACE(std::cout, "");
        std::stringstream ss;
        //if (logger->getLevel()->toString().compare("TRACE") == 0) {
            ss<<"["<<__FUNCTION__<<"@"<<__FILE__<<":"<<__LINE__<<"] ";
        //}
        ss<<"Error while deleting from Berkeley DB. Error = "<<e.what()<<"\n";
        throw DeletionException(ss.str());
    } catch (std::exception &e) {
        TRACE(std::cout, "");
        std::stringstream ss;
        //if (logger->getLevel()->toString().compare("TRACE") == 0) {
            ss<<"["<<__FUNCTION__<<"@"<<__FILE__<<":"<<__LINE__<<"] ";
        //}
        ss<<"Error while deleting from Berkeley DB. Error = "<<e.what()<<"\n";
        throw DeletionException(ss.str());
    }
}

