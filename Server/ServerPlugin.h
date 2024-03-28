//
// Created by elfxe on 10.02.2024.
//

#ifndef DOTAPARSERSERVER_SERVERPLUGIN_H
#define DOTAPARSERSERVER_SERVERPLUGIN_H

#include "include/BoostBeastInclude.h"

#include "core/Server.h"
#include "processor/PostRequestProcessor.h"
#include "processor/GetRequestProcessor.h"
#include "struct/ServerOptions.h"
#include <thread>

class ServerPlugin {
public:
    ServerPlugin(
            ServerOptions serverOptions = ServerOptions())
            : get_request_processor_(serverOptions.getRequestProcessor),
              post_request_processor_(serverOptions.postRequestProcessor)
    {
    }

    GetRequestProcessor *const get_request_processor_;
    PostRequestProcessor *const post_request_processor_;

    void listen(tcp::acceptor &acceptor, tcp::socket &socket) {
        // "Loop" forever accepting new connections.
        acceptor.async_accept(socket,
                              [&](beast::error_code ec) {
                                  if (!ec)
                                      std::make_shared<http_connection>(std::move(socket))->start(get_request_processor_, post_request_processor_);
                                  listen(acceptor, socket);
                              });
    }

private:
    net::io_context ioc_;
};


#endif //DOTAPARSERSERVER_SERVERPLUGIN_H
