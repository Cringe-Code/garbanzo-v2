#pragma once 

#include <drogon/drogon.h>
#include "Base.h"
#include "Tokens.h"
#include "Validation.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include "drogon/utils/FunctionTraits.h"
#include <functional>
#include <bcrypt.h>
#include <json/value.h>
#include <string>

inline void HandlerRegister (const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback,
const drogon::orm::DbClientPtr &dbClient) {
    auto json = req->jsonObject();
    
    User newUser(
        (*json)["login"].asString(),
        (*json)["email"].asString(),
        (*json)["phone"].asString(),
        (*json)["password"].asString()
    );

    auto resp = drogon::HttpResponse::newHttpResponse();

    if (!user_validation::validate_reg_user(newUser)) {
        resp->setStatusCode(drogon::k400BadRequest);
        resp->setBody("Invalid users data");
        callback(resp);
        return;
    }

    dbClient->execSqlAsync("select count(*) from users where login=$1",
        [newUser, resp, callback, dbClient](const drogon::orm::Result &r) {
            if (r.size() > 0) {
                int count = r[0]["count"].as<int>();
                if (count > 0) {
                    resp->setStatusCode(drogon::k409Conflict);
                    resp->setBody("User with same login exists");
                    callback(resp);
                    return;
                }
                else {
                    dbClient->execSqlAsync("insert into users (login, email, hash_password) values ($1, $2, $3)", 
                        [callback, resp, newUser](const drogon::orm::Result &result) {
                            if (result.affectedRows() > 0) {
                                Json::Value jUser;
                                jUser["login"] = newUser.Login;
                                jUser["email"] = newUser.Email;
                                jUser["phone"] = newUser.Phone;

                                resp->setStatusCode(drogon::k201Created);
                                resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
                                resp->setBody(jUser.toStyledString());

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

inline void HandlerAuth (const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback, 
const drogon::orm::DbClientPtr &dbClient) {
    auto json = req->jsonObject();

    User authUser(
        (*json)["login"].asString(),
        (*json)["email"].asString(),
        (*json)["phone"].asString(),
        (*json)["password"].asString()
    );

    auto resp = drogon::HttpResponse::newHttpResponse();


    int status = user_validation::validate_auth_user(authUser, dbClient); // 0 - internal error, 0 - unauth, 1 - OK, 2 - bad request
    if (status == -1) {
        resp->setStatusCode(drogon::k401Unauthorized);
        resp->setBody("Wrong login or password field");
        callback(resp);
        return;
    }

    if (status == 0) {
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
        return;
    }

    if (status == 2) {
        resp->setStatusCode(drogon::k400BadRequest);
        resp->setBody("Empty auth fields");
        callback(resp);
        return;
    }

    auto row = dbClient->execSqlSync("select id from users where login=$1", authUser.Login);
    
    std::string user_id = row[0][0].as<std::string>();

    std::string access_token = generate_jwt_access_token(user_id);
    std::string refresh_token = generate_jwt_refresh_token(user_id);

    dbClient->execSqlSync("insert into tokens (user_id, access_token, refresh_token) "
        "values ($1, $2, $3)", user_id, bcrypt::generateHash(access_token), bcrypt::generateHash(refresh_token));


    Json::Value jTokens;
    jTokens["access_token"] = access_token;
    jTokens["refresh_token"] = refresh_token;

    resp->setStatusCode(drogon::k200OK);
    resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
    resp->setBody(jTokens.toStyledString());

    callback(resp);
}
