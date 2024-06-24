#include "drogon/HttpRequest.h"
#include "drogon/HttpResponse.h"
#include "drogon/orm/DbClient.h"
#include "drogon/utils/FunctionTraits.h"
#include <json/value.h>

class ItemHandler {
public:
    static void HandlerGetItemMini (const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback,
        const drogon::orm::DbClientPtr &dbClient, const std::string item_id);
    
    static void HandlerAddItem_temporary (const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback,
        const drogon::orm::DbClientPtr &dbClient);

    static void HandlerUpdItem (const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback,
        const drogon::orm::DbClientPtr &dbClient);
};
