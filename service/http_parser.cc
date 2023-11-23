#include "http_parser.hh"
#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/core/buffers_to_string.hpp>

namespace beast = boost::beast;
namespace http = beast::http;

std::string htmlTemplate = R"(
    <!DOCTYPE html>
    <html>
    <head>
        <title>Ashwin URL Shortening Service</title>
    </head>
    <body>
        <h1>{CONTENT}</h1>
    </body>
    </html>
)";

std::string get_http_body(std::string http_request) {    
    // buffer creation for parsing
    beast::flat_buffer buffer;
    http::request_parser<http::string_body> parser;
    beast::error_code code;

    parser.put(boost::asio::buffer(http_request), code);
    
    if(code) {
        std::cerr << "Error in http parsing: " << code.message() << '\n';
        exit(0); 
    }

    auto view = parser.get().target();
    

    // Trimming / from beginning of url and returning
    return std::string(begin(view) + 1, end(view));
}

std::string form_http_response(std::string short_url) {
    http::response<http::string_body> response;
    response.version(11);  // HTTP/1.1
    response.result(http::status::ok);
    response.set(http::field::server, "Ashwin's Server");
    response.set(http::field::content_type, "text/html");

    std::string content = htmlTemplate;
    size_t placeholderPos = content.find("{CONTENT}");
    if (placeholderPos != std::string::npos) {
        content.replace(placeholderPos, 9, "Your shortened URL is: " + short_url + "\nThanks for using the service :)\n");
    }

    response.body() = content;
    response.prepare_payload();

    std::stringstream ss;
    ss << response;
    
    return ss.str();
}