
#ifndef COSMOS_KEYS_KEYS
#define COSMOS_KEYS_KEYS

#include <cosmos.hpp>

namespace cosmos {
    
    template <typename pubkey>
    struct signing_key {
        virtual bool valid() const = 0;
        
        virtual pubkey to_public() const = 0;
        
        virtual signature sign(const digest256& d) const = 0;
        
        virtual string wif() const = 0;
    };
        
    struct hd_signing_key : signing_key<hd_pubkey> {
        virtual ptr<hd_signing_key> derive(list<uint32>) const = 0;
        
        virtual string xstring() const = 0;
    };
    
    void from_json(const json& j, ptr<signing_key<pubkey>>&);
    
    void from_json(const json& j, ptr<hd_signing_key>&);
    
    struct keys {
        list<hd_signing_key> HDMasters;
        map<address, signing_key> Keys;
    };
    
    void to_json(json& j, const keys& p);

    void from_json(const json& j, keys& p);
    
    struct secret final : signing_key<pubkey> {
        Gigamonkey::Bitcoin::secret Secret;
        
        bool valid() const override;
        
        pubkey to_public() const override;
        
        signature sign(const digest& d) const override;
        
        virtual string wif() const override;
    };
    
    void to_json(json& j, const secret& p);

    void from_json(const json& j, secret& p);
    
    class encrypted_secret final : public signing_key<pubkey> {
        data::crypto::locked<32> Encrypted;
        
        secret decrypt() const {
            string z;
            data::crypto::decrypted d{Encrypted.decrypt()};
            z.resize(d.size());
            std::copy(d.begin(), d.end(), z.begin());
            secret x{z};
            if (!x.valid()) throw data::crypto::decrypted::fail{};
            return x;
        }
        
        bool valid() const override {
            return true;
        }
        
        pubkey to_public() const override {
            return decrypt().to_public();
        }
        
        signature sign(const digest& d) const override {
            return decrypt().sign(d);
        }
    };
    
    void to_json(json& j, const encrypted_secret& p);

    void from_json(const json& j, encrypted_secret& p);
    
    struct hd_secret final : hd_signing_key {
        Gigamonkey::Bitcoin::hd::bip32::secret Secret;
        
        bool valid() const override;
        
        pubkey to_public() const override;
        
        signature sign(const digest& d) const override;
    
        ptr<hd_signing_key> derive(list<uint32> l) const override {
            return decrypt().derive(l);
        }
        
        virtual string wif() const;
    };
    
    void to_json(json& j, const hd_secret& p);

    void from_json(const json& j, hd_secret& p);
    
    struct encrypted_hd_secret final : hd_signing_key {
        data::crypto::locked<32> Encrypted;
        
        secret decrypt() const {
            string z;
            data::crypto::decrypted d{Encrypted.decrypt()};
            z.resize(d.size());
            std::copy(d.begin(), d.end(), z.begin());
            secret x = secret::read(z);
            if (!x.valid()) throw data::crypto::decrypted::fail{};
            return x;
        }
        
        bool valid() const {
            return true;
        }
        
        pubkey to_public() const override {
            return decrypt().to_public();
        }
        
        signature sign(const digest& d) const override {
            return decrypt().sign(d);
        }
    
        ptr<hd_signing_key> derive(list<uint32> l) const override {
            return decrypt().derive(l);
        }
    };
    
    void to_json(json& j, const encrypted_hd_secret& p);

    void from_json(const json& j, encrypted_hd_secret& p);
        
    struct hd_derivation final : hd_signing_key {
        ptr<hd_signing_key> Master;
        list<uint32> Path;
    
        ptr<hd_signing_key> derive() const {
            return Master->derive(Path);
        }
        
        bool valid() const override {
            return Master->valid();
        }
    
        ptr<hd_signing_key> derive(list<uint32> l) const override {
            return std::make_shared<derivation>(Master, Path << l);
        }
        
        pubkey to_public() const override {
            return derive()->to_public();
        }
        
        signature sign(const digest256& d) const override {
            return derive()->sign(d);
        }
    };
    
    void to_json(json& j, const hd_derivation& p);

    void from_json(const json& j, hd_derivation& p);
    
    inline bool secret::valid() const {
        return Secret.valid();
    }
    
    inline pubkey secret::to_public() const {
        return Secret.to_public();
    }
    
    inline signature secret::sign(const digest& d) const {
        return Secret.sign(d):
    }
};

#endif 

