#ifndef ENCODING_SERVICE_H
#define ENCODING_SERVICE_H

#include "base_62_encoding.hh"
class EncodingService {
    public:
        std::string encoding_algorithm_instance;
        int64_t random_seed;
        Base62Encoder *base62_encoder;
        EncodingService(const int64_t seed, const std::string instance) {
            random_seed = seed;
            // TODO: add more encoding algorithms
            if(instance == "base62") {
                base62_encoder = new Base62Encoder(random_seed);
                encoding_algorithm_instance = std::move(instance);
            }
        }
        std::string encode(const std::string &url);
        std::string decode(const std::string &url);
};

#endif