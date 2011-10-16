#include <iostream>
#include <string>
#include <boost/unordered_map.hpp>
#include "../../lib/ProxyServer.h"
#include "../../lib/worker/Worker.h"
#include "../../lib/storeDriver/bdb/BDbDriver.h"
#include "../../lib/utils/dbg_utils/Macros.h"
#include "./lib/MessageUtils.h"

using namespace kvproxy_server;
using namespace kvproxy_worker;
using namespace kvproxy_data_store_driver;
using namespace bdb_data_store_driver;
using namespace message_utils;
using namespace kvproxy_msg_driver;

int main (int argc, char **argv) {
    if (argc != 6) {
        std::cerr<<"Usage: "<<argv[0]<<" <appName> <protocol> <port/path>"
            <<" <db_dir> <bdb_file>"<<std::endl;
        return 1;
    }
    std::string home(argv[4]), dbFile(argv[5]), appName(argv[1]),
        protocol(argv[2]), port(argv[3]);
    TRACE(std::cout, "");
    BDbDriver bdbDriver(home, dbFile, 0, 64 * 1024);
    TRACE(std::cout, "");
    boost::unordered_map<unsigned, DataStoreDriver*> dataStoreTypeMap;
    dataStoreTypeMap[PERSON_STORE] = &bdbDriver;
    MsgDriver msgDriver;
    Worker worker(dataStoreTypeMap, &msgDriver);
    TRACE(std::cout, "");
    ProxyServer server(appName, protocol, port,
            &worker, &msgDriver, requestToString, responseToString);
    TRACE(std::cout, "");
    server.run();
    return 0;
}

