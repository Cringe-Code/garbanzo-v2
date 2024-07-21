#pragma once 

#include <drogon/drogon.h>
#include "drogon/HttpController.h"
#include "drogon/HttpResponse.h"
#include "drogon/utils/FunctionTraits.h"
#include <functional>
#include <bcrypt.h>
#include <json/value.h>

class RegAuthController : public drogon::HttpController<RegAuthController> {
public:

    METHOD_LIST_BEGIN

    METHOD_ADD(HandlerRegister, "/register", drogon::Post);
    METHOD_ADD(HandlerAuth, "/sign_in", drogon::Post);

    METHOD_LIST_END
    
    static void HandlerRegister (const drogon::HttpRequestPtr &req, 
        std::function<void (const drogon::HttpResponsePtr &)> &&callback);  

    static void HandlerAuth (const drogon::HttpRequestPtr &req, 
        std::function<void (const drogon::HttpResponsePtr &)> &&callback);
        
};
