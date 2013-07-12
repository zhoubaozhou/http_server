#ifndef HTTP_REQUEST_PARSER_H
#define HTTP_REQUEST_PARSER_H

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

namespace http {
namespace server {

struct request;

class request_handler
{
public:
    // Construct ready to parse the request method.
    request_handler();

    // Reset to initial parser state.
    void reset();

    template<typename InputIterator>
    boost::tuple<boost::tribool, InputIterator> 
            parse(request& req, 
                  InputIterator begin, 
                  InputIterator end);

    {
        while (begin != end) {
            boost::tribool result = consume(req, *begin++);
            if (result || !result) {
                return boost::make_tuple(result, begin);
            }
        }
        boost::tribool result = boost::indeterminate;
        return boost::make_tuple(result, begin);
    } 

private:

    boost::tribool consume(request& req, char input);

    // Check if a byte is an HTTP character.
    static bool is_char(int c);

    // Check if a byte is an HTTP control character.
    static bool is_ctl(int c);

    // Check if a byte is defined as an HTTP tspecial character.
    static bool is_tspecial(int c);

    // Check if a byte is a digit.
    static bool is_digit(int c);

    // The current state 
    enum state {
        method_start,
        method,
        uri,
        http_version_h,
        http_version_t_1,
        http_version_t_2,
        http_version_p,
        http_version_slash,
        http_version_major_start,
        http_version_major,
        http_version_minor_start,
        http_version_minor,
        expecting_newline_1,
        header_line_start,
        header_lws,
        header_name,
        space_before_header_value,
        header_value,
        expecting_newline_2,
        expecting_newline_3
    } state_;
};

};
};

#endif
