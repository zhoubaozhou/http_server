#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "reply.h"
#include "request_handler.h"
#include "request_parser.h"


class connection_manager;

namespace http {
namespace server {

class connection
    : public boost::enable_shared_from_this<connection>,
      private boost::noncopyable
{
public:
    explicit connection(boost::asio::io_service& io_service, 
                        connection_manager& manager, request_handler& handler);

    boost::asio::ip::tcp::socket& socket();

    void start();

    void stop();

private:
    void handle_read(const boost::system::error_code& error,
                     std::size_t bytes_transferred);

    void handle_write(const boost::system::error_code& error);

    boost::asio::ip::tcp::socket socket_;

    connection_manager& connection_manager_;

    request_handler& request_handler_;

    boost::array<char, 8192> buffer_;

    request request_;

    request_handler request_handler_;

    reply reply_;
};

typedef boost::shared_ptr<connection> connection_ptr;

};
};


#endif
