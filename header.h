#ifndef HTTP_HEADER_H
#define HTTP_HEADER_H

#include <string>

namespace http {
namespace server {

struct header
{
    std::string name;
    std::string value;
};

};
};

#endif
