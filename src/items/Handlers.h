#pragma once

#include "Cache.h"
#include "Base.h"
#include "drogon/HttpRequest.h"
#include "drogon/HttpResponse.h"
#include "drogon/orm/DbClient.h"
#include "drogon/utils/FunctionTraits.h"
#include <json/value.h>

class ItemHandler {
public:
    static void HandlerGetItemMini (const drogon::HttpRequestPtr &, std::function<void (const drogon::HttpResponsePtr &)> &&,
        const drogon::orm::DbClientPtr &, const std::string &, MyCache<Item>&);
    
    static void HandlerAddItem_temporary (const drogon::HttpRequestPtr &, std::function<void (const drogon::HttpResponsePtr &)> &&,
        const drogon::orm::DbClientPtr &);

    static void HandlerUpdItem (const drogon::HttpRequestPtr &, std::function<void (const drogon::HttpResponsePtr &)> &&,
        const drogon::orm::DbClientPtr &);
};
