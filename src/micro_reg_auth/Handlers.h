#pragma once 

#include <drogon/drogon.h>
#include "Base.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include "drogon/utils/FunctionTraits.h"
#include <functional>
#include <bcrypt.h>

inline void HandlerRegister (const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback,
const drogon::orm::DbClientPtr &dbClient) {
    auto json = req->jsonObject();
    
    User newUser(
        (*json)["login"].asString(),
        (*json)["email"].asString(),
        (*json)["password"].asString()
    );

    auto resp = drogon::HttpResponse::newHttpResponse();

    dbClient->execSqlAsync("select count(*) from users where login=$1",
        [newUser, resp, callback, dbClient](const drogon::orm::Result &r) {
            if (r.size() > 0) {
                int count = r[0]["count"].as<int>();
                if (count > 0) {
                    resp->setStatusCode(drogon::k409Conflict);
                    callback(resp);
                    return;
                }
                else {
                    dbClient->execSqlAsync("insert into users (login, email, hash_password) values ($1, $2, $3)", 
                        [callback, resp](const drogon::orm::Result &result) {
                            if (result.affectedRows() > 0) {
                                resp->setStatusCode(drogon::k201Created);
                                resp->setBody("User registered successfully");
                                callback(resp);
                                return;
                            }
                            else {
                                resp->setStatusCode(drogon::k500InternalServerError);
                                resp->setBody("Failed to register user");
                                callback(resp);
                                return;
                            }
                        },
                        [](const drogon::orm::DrogonDbException &e) {
                            std::cerr << "error:" << e.base().what() << std::endl;
                        },
                        newUser.Login, newUser.Email, bcrypt::generateHash(newUser.Password)
                    );
                }
            }
            else {
                resp->setStatusCode(drogon::k500InternalServerError);
                resp->setBody("Failed to register user");
                callback(resp);
                return;
            }
        },
        [](const drogon::orm::DrogonDbException &e) {
            std::cerr << "error:" << e.base().what() << std::endl;
        },
    newUser.Login);
}

