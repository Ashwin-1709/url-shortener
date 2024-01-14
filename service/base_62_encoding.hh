#ifndef BASE_62_ENCODING_H
#define BASE_62_ENCODING_H

#include <map>
#include <algorithm>
#include <string>

class Base62Encoder {
    public:
        std::string mapping = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        Base62Encoder(const int64_t &random_offset) : random_offset(random_offset) {}
        int64_t convert_base_10(std::string base62);
        std::string convert_base_62(int64_t base10);
        std::string encode_url(std::string url);
        std::string decode_url(std::string encoded_url);
    private:
        std::map<std::string, int64_t>url_to_counter;
        std::map<int64_t, std::string>counter_to_url;
        int64_t random_offset;
        std::string invalid_mapping_response = ":( this shortened URL does not exist\n";
};

#endif