#include <stdexcept>
#include "MessageUtils.h"
#include "../../../lib/utils/string_utils/StringUtils.hh"
#include "../../../lib/utils/dbg_utils/Macros.h"

using namespace myApp_proto;
using namespace kvproxy_proto;
using namespace gen_utils;

namespace message_utils {

    Id makeId(int id) {
        TRACE(std::cout, "");
        Id retVal;
        retVal.set_id(id);
        return retVal;
    }

    Person makePerson(const std::string &name, const std::string &ph) {
        TRACE(std::cout, "");
        Person person;
        if (!name.empty() && name.size())
            person.set_name(name);
        if (!ph.empty() && ph.size())
            person.set_phone(ph);
        return person;
    }

    Request makeRequest(Request_Header_Type type, const Id &id, const
            Person &person, bool overwrite, const std::string &trace) {
        TRACE(std::cout, "");
        Request request;
        Request_Header *hdr = request.mutable_header();
        if (!trace.empty() && trace.size()) {
            hdr->set_trace(trace);
        }
        hdr->set_datastoredriverid(PERSON_STORE);
        Request_Payload *pyld = request.mutable_payload();
        std::string k;
        id.SerializeToString(&k);
        pyld->set_key(k);
        switch (type) {
            case Request_Header_Type_LOOKUP:
                {
                    hdr->set_type(Request_Header_Type_LOOKUP);
                    break;
                }
            case Request_Header_Type_INSERT:
                {
                    hdr->set_type(Request_Header_Type_INSERT);
                    std::string v;
                    person.SerializeToString(&v);
                    pyld->set_value(v);
                    pyld->set_overwrite(overwrite);
                    break;
                }
            case Request_Header_Type_REMOVE:
                {
                    hdr->set_type(Request_Header_Type_REMOVE);
                    break;
                }
            default:
                throw std::runtime_error("Only one of lookup/insert/remove"
                        " operations supported");
        };
        return request;
    }

    std::string idToString(const Id &id) {
        TRACE(std::cout, "");
        return std::string("Id: {\nid: ")
            .append(string_utils::toString<int>(id.id()))
            .append("\n}\n");
    }

    std::string personToString(const Person &person) {
        TRACE(std::cout, "");
        return std::string("Person: {\n")
            .append("name: \"")
            .append(person.name())
            .append("\"")
            .append("\n, phone: \"")
            .append(person.phone())
            .append("\"")
            .append("\n}\n");
    }

    std::string requestToString(const Request &request) {
        TRACE(std::cout, "");
        std::string retVal("{\n");
        if (request.has_header()) {
            retVal.append("Header: {");
            if (request.header().has_type()) {
                retVal.append("type: ")
                    .append(string_utils::toString<int>(request.header().type()));
            }
            if (request.header().has_trace()) {
                retVal.append("trace: ").append(request.header().trace());
            }
            retVal.append("}\n");
        }
        if (request.has_payload()) {
            retVal.append("Payload: {");
            if (request.payload().has_key()) {
                Id id;
                id.ParseFromString(request.payload().key());
                retVal.append(idToString(id));
            }
            if (request.payload().has_value()) {
                Person person;
                person.ParseFromString(request.payload().value());
                retVal.append(personToString(person));
            }
            retVal.append("}\n");
        }
        retVal.append("}");
        return retVal;
    }

    std::string responseToString(const Response &response) {
        TRACE(std::cout, "");
        std::string retVal("Response: {\n");
        if (response.has_header()) {
            retVal.append("Header: {\n");
            if (response.header().has_returncode())
                retVal.append("returnCode: ")
                    .append(string_utils::toString<int>(response.header().returncode()));
            if (response.header().has_payload()) {
                retVal.append("payload: ").append(response.header().payload());
            }
            retVal.append("}\n");
        }
        if (response.has_payload()) {
            retVal.append("Payload: {\n");
            Person person;
            person.ParseFromString(response.payload());
            retVal.append(personToString(person));
            retVal.append("}\n");
        }
        retVal.append("}\n");
        return retVal;
    }

};

