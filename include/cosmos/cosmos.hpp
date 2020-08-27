
#ifndef COSMOS_COSMOS
#define COSMOS_COSMOS

#include <gigamonkey/schema/hd.hpp>
#include <gigamonkey/address.hpp>
#include <nlohmann/json.hpp>

namespace cosmos {
    using namespace Gigamonkey;
    using json = nlohmann::json;
    
    using string = std::string;
    
    using pubkey = Gigamonkey::Bitcoin::pubkey;
    
    using address = Gigamonkey::Bitcoin::address;
    
    using signature = Gigamonkey::Bitcoin::signature;
    
    using hd_pubkey = Gigamonkey::Bitcoin::hd::bip32::pubkey;
};

#endif 
