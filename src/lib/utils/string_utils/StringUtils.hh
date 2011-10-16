#ifndef _STRING_UTILS_HH_
#define _STRING_UTILS_HH_

#include <iostream>
#include <string>
#include <utility>
#include <sstream>
#include <vector>

namespace gen_utils {
    namespace string_utils {
        template<typename T> std::string toString(const T &data) {
            std::stringstream ss;
            ss<<data;
            return ss.str();
        }
        template<typename T> std::string toString(const std::vector<T> &vec) {
            std::string retVal = "[";
            for (typename std::vector<T>::const_iterator i = vec.begin(); i !=
                    vec.end(); i++) {
                if (i != vec.begin()) {
                    retVal += ", ";
                }
                retVal += toString<T>(*i);
            }
            retVal += "]";
            return retVal;
        }
        template<typename T> T fromString(const std::string &str) {
            T retVal;
            std::stringstream ss(str);
            ss>>retVal;
            return retVal;
        }
        template<typename T> std::vector<T> tokenizer(const std::string
                &input, const std::string &delim, bool rev = false) {
            std::vector<T> tokens;
            tokens.clear();
            if (input.empty())
                return tokens;
            size_t pos = input.find(delim),
                   start = 0;
            while (pos != std::string::npos) {
                if (rev) {
                    tokens.insert(tokens.begin(),
                            fromString<T>(input.substr(start, pos - start)));
                } else {
                    tokens.insert(tokens.end(),
                            fromString<T>(input.substr(start, pos - start)));
                }
                //tokens.push_back(fromString<T>(input.substr(start, pos - start)));
                start = pos + delim.size();
                pos = input.find(delim, start);
            }
            if (rev) {
                tokens.insert(tokens.begin(), fromString<T>(input.substr(start)));
            } else {
                tokens.insert(tokens.end(), fromString<T>(input.substr(start)));
            }
            //tokens.push_back(fromString<T>(input.substr(start)));
            return tokens;
        }
        template<typename T1, typename T2> std::pair<T1, T2> pairify(const
                std::string &input, const std::string &delim) {
            size_t pos = input.find(delim),
                   start = 0;
            if (pos != std::string::npos) {
                return std::pair<T1, T2>(
                        T1(input.substr(start, pos - start))
                        , T2(input.substr(pos + delim.size())));
            }
            return std::pair<T1, T2>(T1(), T2());
        }
    }
}

#endif

