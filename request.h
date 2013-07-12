#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H


namespace http {
namespace server {

struct request 
{
    std::string method;
    std::string uri;
    int http_version_major;
    int http_version_minor;
    std::vector<header> headers;
};

};
};

#endif
