#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <map>
#include <string>

std::string get_http_body(std::string http_request);
std::string form_http_response(std::string short_url);
#endif