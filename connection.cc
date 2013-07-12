#include "connection.h"
#include <vector>
#include <boost/bind.hpp>
#include "connection_manager.h"
#include "request_handler.h"

namespace http {
namespace server {

connection::connection(boost::asio::io_service& io_service,
        connection_manager& manager, request_handler& handler)
    : socket_(io_service),
      connection_manager_(manager),
      request_handler_(handler)
{
}

boost::asio::ip::tcp::socket& connection::socket()
{
    return socket_;
}

void connection::start()
{
    socket_.async_read_some(boost::asio::buffer(buffer_),
            boost::bind(&connection::handle_read, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
}

void connection::stop()
{
    socket_.close();
}

void connection::handle_read(const boost::system::error_code& error,
        std::size_t bytes_transferred)
{
    if (!error) {
        boost::tribool result;
        boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
                request_, buffer_.data(), buffer_.data() + bytes_transferred);

        if (result) {
            request_handler_.handle_request(request_, reply_);
            boost::asio::async_write(socket_, reply_.to_buffers(), 
                    boost::bind(&connection::handle_write, shared_from_this(),
                        boost::asio::placeholders::error));
        } else if (!result) {
            reply_ = reply::stock_reply(reply::bad_request);
            boost::asio::async_write(socket_, reply_.to_buffers(),
                    boost::bind(&connection::handle_write, shared_from_this(),
                        boost::asio::placeholders::error));
        } else {
            // 还有数据未读完，继续
            socket_.async_read_some(boost::asio::buffer(buffer_),
                    boost::bind(&connection::handle_read, shared_from_this(),
                        boost::asio::placeholders::error, 
                        boost::asio::placeholders::bytes_transferred));
        }

    } else if (boost::asio::error::operation_aborted != error) {
        connection_manager_.stop(shared_from_this());
    }
}

void connection::handle_write(const boost::system::error_code& error)
{
    if (!error) {
        boost::system::error_code ignored_error_code;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_error_code);
    }
    if (boost::asio::error::operation_aborted != error) {
        // TODO operation_aborted是什么意思呢？
        connection_manager_.stop(shared_from_this());
    }
}

};
};
