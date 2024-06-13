#pragma once 

#include <drogon/drogon.h>
#include "Base.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include "drogon/utils/FunctionTraits.h"
#include <functional>
#include <iostream>

inline void HandlerRegister (const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback,
const drogon::orm::DbClientPtr &dbClient) {
    auto json = req->jsonObject();
    
    User newUser(
        (*json)["login"].asString(),
        (*json)["email"].asString(),
        (*json)["password"].asString()
    );

    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::HttpStatusCode::k200OK);
    
    callback(resp);
}

