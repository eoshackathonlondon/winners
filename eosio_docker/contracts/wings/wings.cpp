#include <eosiolib/eosio.hpp>
using namespace eosio;
using std::string;

class wings : public eosio::contract {
  private:
    struct private_data_type {
        string data;
        uint64_t nonce;
        uint32_t checksum;
    };

    /// @abi table users i64
    struct user_type {
        account_name account;
        public_key encryption_key;
        string name;
        uint8_t age;
        string profile_pic_url;
        private_data_type private_data;

        auto primary_key() const { return account; }
    };

    typedef eosio::multi_index<N(users), user_type> users_table;

  public:
    using contract::contract;

    /// @abi action
    void setdata(user_type data) {
        require_auth(data.account);

        users_table users(_self, _self);
        auto user_itr = users.find(data.account);

        if (user_itr == users.end()) {
            users.emplace(_self, [&](auto &user) {
                user.account = data.account;
                user.encryption_key = data.encryption_key;
                user.name = data.name;
                user.age = data.age;
                user.profile_pic_url = data.profile_pic_url;
                user.private_data = data.private_data;
            });
        } else {
            users.modify(user_itr, _self, [&](auto &user) {
                user.encryption_key = data.encryption_key;
                user.name = data.name;
                user.age = data.age;
                user.profile_pic_url = data.profile_pic_url;
                user.private_data = data.private_data;
            });
        }
    }
};

EOSIO_ABI(wings, (setdata))
