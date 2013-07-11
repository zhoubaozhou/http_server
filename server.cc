#include "server.h"
#include <boost/bind.hpp>
#include <signal.h>

namespace http {
namespace server {

server::server(const std::string& address, const std::string& port,
        const std::string& doc_root)
    : io_service_(),
      signals_(io_service_),
      acceptor_(io_service_),
      connection_manager_(),
      new_connection_(),
      request_handler_(doc_root)
{
    signals_.add(SIGINT);
    signals_.add(SIGTERM); 
    signals_.async_wait(boost::bind(&server::handle_stop, this));

    using namespace boost::asio;
    ip::tcp::resolver resolver(io_service_);
    ip::tcp::resolver::query query(address, port);
    ip::tcp::endpoint endpoint = *resolver.resolve(query);

    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    start_accept();
}

};
};

