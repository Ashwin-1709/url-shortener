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

std::string Base62Encoder::convert_base_62(int64_t base10) {
    std::string base62;
    while(base10 > 0) {
        base62.push_back(mapping[base10 % 62]);
        base10 /= 62;
    }

    while(base62.length() < 7)
        base62.push_back('0');
    
    std::reverse(std::begin(base62), std::end(base62));
    return base62;
}

std::string Base62Encoder::encode_url(std::string url) {
    if(url_to_counter.count(url)) {
        return convert_base_62(url_to_counter[url]);
    }

    std::string encoded_url = convert_base_62(random_offset);
    counter_to_url[random_offset] = url;
    url_to_counter[url] = random_offset;
    random_offset++;
    return encoded_url;
}

std::string Base62Encoder::decode_url(std::string encoded_url) {
    int64_t base10 = convert_base_10(encoded_url);
    auto decoded_url_instance = counter_to_url.find(base10);

    if(decoded_url_instance == end(counter_to_url)) {
        return invalid_mapping_response;
    }

    auto [key, value] = *decoded_url_instance;
    return value;
}