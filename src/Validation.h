#pragma once

#include <cctype>
#include <string>
#include "Base.h"
#include "bcrypt.h"
#include "drogon/HttpAppFramework.h"
#include "drogon/orm/DbClient.h"
#include "drogon/orm/Exception.h"
#include "drogon/utils/FunctionTraits.h"
#include <regex>

namespace user_validation {
    inline bool validate_password (std::string &password) {
        if (password.size() < 6) return false;

        bool wasLow = false;
        bool wasUp = false;
        bool wasDigit = false;

        for (const auto &c : password) {
            if (std::isdigit(c)) {
                wasDigit = true;
            }
            else if (std::islower(c)) {
                wasLow = true;
            }
            else {
                wasUp = true;
            }
        }

        return wasDigit && wasLow && wasUp;
    }

    inline bool validate_email (std::string &email) {
        auto reg = std::regex(R"(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$)");

        return std::regex_match(email, reg) && (email.size() <= 50);
    }

    inline bool validate_login (std::string &login) {

        return 4 < login.size() && login.size() < 12;
    }

    inline bool validate_phone (std::string &phone) {
        auto reg = std::regex(R"(^[\+]?[(]?[0-9]{3}[)]?[-\s\.]?[0-9]{3}[-\s\.]?[0-9]{4,6}$)");
        return std::regex_match(phone, reg);
    }

    inline bool validate_reg_user (User &u) {
        return validate_password(u.Password) &&
            validate_email(u.Email) && validate_login(u.Login) && validate_phone(u.Phone);
    }

    inline int validate_auth_user (User &u, const drogon::orm::DbClientPtr &dbClient) {

        int valid = 1;

        dbClient->execSqlAsync("select hash_password from users where phone=$1 or login=$2 or email=$3", 
        [&u, &valid](const drogon::orm::Result &r) {
            if (r.size() > 0 && r[0].size() > 0) {
                std::string userPassword;
                userPassword = r[0][0].as<std::string>();

                if (!bcrypt::validatePassword(u.Password, userPassword)) {
                    valid = -1;
                    return;
                }

            }
            else {
                valid = 2;
                return;
            }
        }, 
        [&valid](const drogon::orm::DrogonDbException &e) {
            valid = 0;
            std::cerr << "error:" << e.base().what() << std::endl;
            return;
        },
        u.Phone, u.Login, u.Email);

        return valid;
    }
};