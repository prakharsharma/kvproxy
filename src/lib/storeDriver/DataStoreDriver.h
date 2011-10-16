#ifndef _DATA_STORE_DRIVER_H_
#define _DATA_STORE_DRIVER_H_

#include <stdexcept>

namespace kvproxy_data_store_driver {

    struct KeyPresentException : public std::runtime_error {
        KeyPresentException(std::string msg = "") : runtime_error(msg) {}
    };

    struct KeyNotFoundException : public std::runtime_error {
        KeyNotFoundException(std::string msg = "") : runtime_error(msg) {}
    };

    struct DriverCreationException : public std::runtime_error {
        DriverCreationException(std::string msg = "") : runtime_error(msg)
                                                        {}
    };

    struct InsertionException : public std::runtime_error {
        InsertionException(std::string msg = "") : runtime_error(msg) {}
    };

    struct DeletionException: public std::runtime_error {
        DeletionException(std::string msg = "") : runtime_error(msg) {}
    };

    class DataStoreDriver {
        public:
            virtual ~DataStoreDriver() {};
            virtual std::string lookup(const std::string &) const
                throw(KeyNotFoundException) = 0;
            virtual void insert(const std::string &, const std::string &,
                    bool overwrite = false) throw(KeyPresentException,
                        InsertionException) = 0;
            virtual void remove(const std::string &)
                throw(KeyNotFoundException, DeletionException) = 0;
        private:
    };

};

#endif
