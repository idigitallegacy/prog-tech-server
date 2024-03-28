
#include "../include/BoostBeastInclude.h"

#include "../processor/GetRequestProcessor.h"
#include "../processor/PostRequestProcessor.h"

class http_connection : public std::enable_shared_from_this<http_connection> {
public:
    explicit http_connection(tcp::socket socket)
            : socket_(std::move(socket)) {
    }

    // Initiate the asynchronous operations associated with the connection.
    void start(GetRequestProcessor *const getRP, PostRequestProcessor *const postRP) {
        std::cout << "Creating session at " << socket_.remote_endpoint().address().to_string() << ":" << socket_.remote_endpoint().port() << '\n';
        read_request(getRP, postRP);
        check_deadline();
    }

private:
    // The socket for the currently connected client.
    tcp::socket socket_;

    // The buffer for performing reads.
    beast::flat_buffer buffer_{65536};

    // The request message.
    http::request<http::dynamic_body> request_;

    // The response message.
    http::response<http::dynamic_body> response_;

    // The timer for putting a deadline on connection processing.
    net::steady_timer deadline_{
            socket_.get_executor(), std::chrono::seconds(60)
    };

    bool *listening_condition_ = nullptr;

    // Asynchronously receive a complete request message.
    void read_request(GetRequestProcessor *const getRP, PostRequestProcessor *const postRP) {
        auto self = shared_from_this();

        http::async_read(
                socket_,
                buffer_,
                request_,
                [self, getRP, postRP](beast::error_code ec,
                       std::size_t bytes_transferred) {
                    boost::ignore_unused(bytes_transferred);
                    if (!ec)
                        self->process_request(getRP, postRP);
                });
    }

    // Determine what needs to be done with the request message.
    void process_request(GetRequestProcessor *const getRP, PostRequestProcessor *const postRP) {
        response_.version(request_.version());
        response_.keep_alive(false);

        switch (request_.method()) {
            case http::verb::get:
                response_.result(http::status::ok);
                response_.set(http::field::server, "Boost.Asio");
                create_response(getRP, postRP);
                break;

            case http::verb::post:
                postRP->parse_request(request_);
                response_.result(http::status::ok);
                response_.set(http::field::server, "Boost.Asio");
                create_response(getRP, postRP);
                break;

            default:
                // We return responses indicating an error if
                // we do not recognize the request method.
                response_.result(http::status::bad_request);
                response_.set(http::field::content_type, "text/plain");
                beast::ostream(response_.body())
                        << "Invalid request-method '"
                        << std::string(request_.method_string())
                        << "'";
                break;
        }

        write_response();
    }

    // Construct a response message based on the program state.
    void create_response(GetRequestProcessor *const getRP, PostRequestProcessor *const postRP) {
        switch (request_.method()) {
            case http::verb::get: {
                getRP->parse_request(request_);
                response_ = getRP->get_response();
                getRP->clear_response();
                break;
            }

            case http::verb::post: {
                postRP->parse_request(request_);
                break;
            }
        }
    }

    // Asynchronously transmit the response message.
    void
    write_response() {
        auto self = shared_from_this();

        response_.content_length(response_.body().size());

        http::async_write(
                socket_,
                response_,
                [self](beast::error_code ec, std::size_t) {
                    self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                    self->deadline_.cancel();
                });
    }

    // Check whether we have spent enough time on this connection.
    void check_deadline() {
        auto self = shared_from_this();

        deadline_.async_wait(
                [self](beast::error_code ec) {
                    if (!ec) {
                        // Close socket to cancel any outstanding operation.
                        self->socket_.close(ec);
                    }
                });
    }
};