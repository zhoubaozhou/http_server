#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.h"

int main(int argc, char* argv)
{
    try {
        if (4 != argc) {
            std::cerr << "Usage: http_server <address> <port> <doc_root>\n";
            return 1;
        }

        // Initialise the server
        http::server::server s(argv[1], argv[2], argv[3]);

        // Run the server until stopped
        s.run();
    } catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}
