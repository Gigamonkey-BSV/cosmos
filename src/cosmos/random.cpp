// Copyright (c) 2020 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#include <cosmos/random.hpp>
#include <cosmos/config.hpp>
#include <data/crypto/NIST_DRBG.hpp>
#include <gigamonkey/timestamp.hpp>
#include <iostream>

namespace cosmos {
    
    ptr<data::crypto::random> random() {
        static ptr<data::crypto::random> Random{
            data::crypto::nist::drbg::HMAC_DRBG, 
            std::static_pointer_cast<data::crypto::entropy>(std::make_shared<data::crypto::entropy_sum>(
                std::static_pointer_cast<data::crypto::entropy>(std::make_shared<data::crypto::user_entropy>(
                    "Please provide entropy for the random number generator by typing random keys on your keyboard.", 
                    "Sufficient entropy provided. Thanks!" , std::cout, std::cin),
                std::static_pointer_cast<data::crypto::entropy>(std::make_shared<Gigamonkey::bitcoind_entropy>())),
            config::drbg_personalization(), 
            static_cast<uint32_little>(Gigamonkey::timestamp::now())};
        return Random;
    }
}
