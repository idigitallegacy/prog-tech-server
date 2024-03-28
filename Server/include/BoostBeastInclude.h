//
// Created by elfxe on 14.02.2024.
//

#ifndef DOTAPARSERSERVER_BOOSTBEASTINCLUDE_H
#define DOTAPARSERSERVER_BOOSTBEASTINCLUDE_H

#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "boost/asio.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
#endif //DOTAPARSERSERVER_BOOSTBEASTINCLUDE_H
