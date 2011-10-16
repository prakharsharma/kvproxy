#include "../DataStoreDriver.h"
#include "BDbDriver.h"
#include <iostream>
#include <string>
#include <vector>

using namespace kvproxy_data_store_driver;
using namespace bdb_data_store_driver;

std::string getInput(const std::string q) {
    std::string line;
    std::cout<<q;
    std::getline(std::cin, line);
    return line;
}

void interface(DataStoreDriver &driver) {
    bool cont = true;
    while (cont) {
        std::string line;
        std::cout<<"Action(1:Lookup, 2:Insert, 3:Remove): ";
        std::getline(std::cin, line);
        switch(line.at(0)) {
            case '1':
                {
                    std::string key = getInput("Key: ");
                    std::cout<<"Value = "<<driver.lookup(key)<<std::endl;
                    break;
                }
            case '2':
                {
                    std::string key = getInput("Key: ");
                    std::string val = getInput("Value: ");
                    std::string overwrite = getInput("Overwrite(y/n): ");
                    driver.insert(key, val, !overwrite.empty() &&
                            (overwrite.at(0) == 'y' || overwrite.at(0) ==
                             'Y'));
                    break;
                }
            case '3':
                {
                    std::string key = getInput("Key: ");
                    driver.remove(key);
                    break;
                }
            default:
                break;
        };
        std::cout<<"Wanna continue(y/n): ";
        line.clear();
        std::getline(std::cin, line);
        cont = !line.empty() && (line.at(0) == 'y' || line.at(0) == 'Y');
    }
}

int main (int argc, char **argv) {
    std::string home("/tmp/drivertest/");
    std::string dbName("haha.db");
    DataStoreDriver *driver = new BDbDriver(home, dbName, 0, 64 * 1024);
    interface(*driver);
    return 0;
}

