#include "encoding_service.hh"

std::string EncodingService::encode(const std::string &url) {
    if(encoding_algorithm_instance == "base62") {
        return base62_encoder->encode_url(url);
    }
    return "Error in initialising encoding algorithm instance";
}

std::string EncodingService::decode(const std::string &url) {
    if(encoding_algorithm_instance == "base62") {
        return base62_encoder->decode_url(url);
    }
    return "Error in initialising encoding algorithm instance";
}