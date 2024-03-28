#include "definitions.h"
#include <string>

#include <boost/asio.hpp>

#include "Server/ServerPlugin.h"

int main(int argc, char *argv[]) {
    auto const address = net::ip::make_address("127.0.0.1");
    auto port = static_cast<unsigned short>(std::stoi("25000"));

    net::io_context ioc{1};

    tcp::acceptor acceptor{ioc, {address, port}};
    tcp::socket socket{ioc};

    ServerPlugin plugin;
    plugin.listen(acceptor, socket);

    ioc.run();
}