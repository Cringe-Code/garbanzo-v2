#pragma once 

#include <drogon/drogon.h>
#include "drogon/HttpResponse.h"
#include "drogon/utils/FunctionTraits.h"
#include <functional>
#include <bcrypt.h>
#include <json/value.h>

class RegAuthHandler {
public:
    static void HandlerRegister (const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback,
        const drogon::orm::DbClientPtr &dbClient);  

    static void HandlerAuth (const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback, 
        const drogon::orm::DbClientPtr &dbClient);
};
