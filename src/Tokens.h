#pragma once

#include <jwt-cpp/jwt.h>
#include <drogon/drogon.h>
#include <iostream>
#include "Base.h"
#include <bcrypt.h>

struct Token {
    std::string access;
    std::string refresh;
};

inline std::string generate_jwt_access_token (const std::string &user_id) {
    auto token = jwt::create()
        .set_type("JWS")
        .set_payload_claim("user_id", jwt::claim(user_id))
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{600})
        .sign(jwt::algorithm::hs256{SECRET_KEY});
    return token;
}

inline std::string generate_jwt_refresh_token(const std::string &user_id) {
    auto token = jwt::create()
        .set_type("JWS")
        .set_payload_claim("user_id", jwt::claim(user_id))
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{2592000})
        .sign(jwt::algorithm::hs256{SECRET_KEY});
    return token;
}

namespace token_validation {
    inline std::pair<bool, std::string> validate_access_token (std::string &access_token, const drogon::orm::DbClientPtr &dbClient) {
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{SECRET_KEY})
            .expires_at_leeway(60); 

        auto decoded_jwt = jwt::decode(access_token);

        try {
            verifier.verify(decoded_jwt);
        }
        catch (const std::exception& exc) {
            std::cerr << "Error while check access token " << exc.what() << "\n";
            return {false, ""};
        }
        
        auto user_id = decoded_jwt.get_payload_claim("user_id").as_string();

        auto rows = dbClient->execSqlSync("select access_token from tokens where user_id=$1", user_id);
        std::string db_hash_token = rows[0][0].as<std::string>();

        // валидация токена (не пароля) просто бкрипт тупо назвал свои функции
        if (!bcrypt::validatePassword(access_token, db_hash_token)) {
            return {false, ""};
        }
        return {true, user_id};
    }

    inline std::pair<bool, std::string> validate_refresh_token (std::string &refresh_token, const drogon::orm::DbClientPtr &dbClient) {
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{SECRET_KEY})
            .expires_at_leeway(2592000); 

        auto decoded_jwt = jwt::decode(refresh_token);

        try {
            verifier.verify(decoded_jwt);
        }
        catch (const std::exception& exc) {
            std::cerr << "Error while check refresh token: " << exc.what() << "\n";
            return {false, ""};
        }
        
        auto user_id = decoded_jwt.get_payload_claim("user_id").as_string();

        auto rows = dbClient->execSqlSync("select refresh_token from tokens where user_id=$1", user_id);
        std::string db_hash_token = rows[0][0].as<std::string>();
        
        // валидация токена (не пароля) просто бкрипт тупо назвал свои функции
        if (!bcrypt::validatePassword(refresh_token, db_hash_token)) {
            return {false, ""};
        }
        return {true, user_id};
    }
};

inline std::pair<std::string, std::string> update_tokens (const std::string &access_token, const std::string &refresh_token, 
    const drogon::orm::DbClientPtr &dbClient) {

    Token t = {
        access_token,
        refresh_token
    };

    auto val = token_validation::validate_access_token(t.access, dbClient);

    if (val.first) {
        return {access_token, refresh_token};
    }
    else {
        val = token_validation::validate_refresh_token(t.refresh, dbClient);

        if (!val.first) { // если refresh token невалидный, то юзер не получит новую пару токенов
            return {"", ""};
        }

        std::string new_refresh = generate_jwt_refresh_token(val.second);
        std::string new_access = generate_jwt_access_token(val.second);

        dbClient->execSqlSync("update tokens set hash_refresh_token=$1, hash_access_token=$2 where user_id=$3", 
            bcrypt::generateHash(new_refresh), bcrypt::generateHash(new_access), val.second);

        return {new_access, new_refresh};
    }
}
