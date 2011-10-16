#include <iostream>
#include <string>
#include <zmq.hpp>
#include "../server/lib/MessageUtils.h"
#include "../server/lib/dbRecord.pb.h"
#include "../../lib/proto/kvproxy.pb.h"
#include "../../lib/utils/string_utils/StringUtils.hh"
#include "../../lib/utils/dbg_utils/Macros.h"

using namespace gen_utils::string_utils;
using namespace kvproxy_proto;
using namespace message_utils;
using namespace myApp_proto;

void handleOp(zmq::socket_t &socket, int opCode) {
    std::string s_req, trace, line;
    std::cout<<"Trace: ";
    std::getline(std::cin, trace);
    Request req;
    Id id;
    Person person;
    Request_Header_Type type;
    switch (opCode) {
        case 1:
            type = Request_Header_Type_LOOKUP;
            std::cout<<"id: ";
            line.clear();
            std::getline(std::cin, line);
            id = makeId(fromString<int>(line));
            req = makeRequest(type, id, person, false, trace);
            break;
        case 3:
            type = Request_Header_Type_REMOVE;
            std::cout<<"id: ";
            line.clear();
            std::getline(std::cin, line);
            id = makeId(fromString<int>(line));
            req = makeRequest(type, id, person, false, trace);
            break;
        case 2:
            {
                type = Request_Header_Type_INSERT;
                std::cout<<"id: ";
                line.clear();
                std::getline(std::cin, line);
                id = makeId(fromString<int>(line));
                line.clear();
                std::cout<<"Name: ";
                std::getline(std::cin, line);
                std::string ph, ovr;
                std::cout<<"Phone: ";
                std::getline(std::cin, ph);
                std::cout<<"Overwrite: (y/n) ";
                std::getline(std::cin, ovr);
                person = makePerson(line, ph);
                req = makeRequest(type, id, person, ovr.compare("y") == 0,
                        trace);
                break;
            }
    };
    std::cout<<"Request: "<<requestToString(req);
    TRACE(std::cout, "");
    req.SerializeToString(&s_req);
    zmq::message_t z_req(s_req.size());
    memcpy(z_req.data(), (void *)s_req.data(), s_req.size());
    socket.send(z_req);
    zmq::message_t z_resp;
    socket.recv(&z_resp);
    std::string s_resp((char *)z_resp.data(), z_resp.size());
    Response resp;
    resp.ParseFromString(s_resp);
    std::cout<<"Response = "<<responseToString(resp);
}

void interact(zmq::socket_t &socket) {
    bool cont = true;
    while (cont) {
        std::string line;
        std::cout<<"Operation: (i/d/l): ";
        std::getline(std::cin, line);
        switch (line.at(0)) {
            case 'l':
            case 'L':
                handleOp(socket, 1);
                break;
            case 'i':
            case 'I':
                handleOp(socket, 2);
                break;
            case 'd':
            case 'D':
                handleOp(socket, 3);
                break;
            default:
                break;
        };
        line.clear();
        std::cout<<"Continue? (y/n): ";
        std::getline(std::cin, line);
        cont = line.at(0) == 'y' || line.at(0) == 'Y';
    }
}

int main (int argc, char **argv) {
    if (argc != 4) {
        std::cerr<<"Usage: "<<argv[0]<<" <host> <protocol> <port/path>"
            <<std::endl;
        exit(1);
    }
    std::string host(argv[1]), protocol(argv[2]), port(argv[3]);
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(protocol.append("://").append(host).append(":").
            append(port).c_str());
    interact(socket);
    return 0;
}

