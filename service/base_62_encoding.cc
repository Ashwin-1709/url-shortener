#include "base_62_encoding.hh"

int64_t Base62Encoder::convert_base_10(std::string base62) {
    int64_t cur = 0;
    
    auto normalize = [&](char c) -> int64_t {
        if(c >= '0' and c <= '9') {
            return c - '0';
        } else if(c >= 'a' and c <= 'z') { 
            return c - 'a' + 10;
        }  
        return c - 'A' + 36;
    };

    for(auto &c: base62) {
        cur = cur * 62 + normalize(c);
    }

    return cur;
}