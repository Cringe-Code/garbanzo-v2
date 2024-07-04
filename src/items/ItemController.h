#pragma once

#include "Cache.h"
#include "Base.h"
#include "drogon/HttpController.h"
#include "drogon/HttpRequest.h"
#include "drogon/HttpResponse.h"
#include "drogon/orm/DbClient.h"
#include "drogon/utils/FunctionTraits.h"
#include <functional>
#include <json/value.h>
#include <memory>

class ItemController : public drogon::HttpController<ItemController> {
public:
    
    METHOD_LIST_BEGIN

    METHOD_ADD(ItemController::HandlerGetItemMini, "/m/{item_id}", drogon::Get);
    METHOD_ADD(ItemController::HandlerGetItemFull, "/f/{item_id}", drogon::Get);
    METHOD_ADD(ItemController::HandlerAddItem_temporary, "/test", drogon::Post);
    METHOD_ADD(ItemController::HandlerUpdItem, "/upd/{item_id}", drogon::Patch);
    // METHOD_ADD(ItemController::HandlerMarkFavourite, "/mark/{item_id}", drogon::Post);

    METHOD_LIST_END

    void HandlerGetItemMini (const drogon::HttpRequestPtr &, std::function<void (const drogon::HttpResponsePtr &)> &&,
        const std::string &);
    
    void HandlerGetItemFull (const drogon::HttpRequestPtr &, std::function<void (const drogon::HttpResponsePtr &)> &&,
        const std::string &);

    void HandlerAddItem_temporary (const drogon::HttpRequestPtr &, std::function<void (const drogon::HttpResponsePtr &)> &&);

    void HandlerUpdItem (const drogon::HttpRequestPtr &, std::function<void (const drogon::HttpResponsePtr &)> &&,
        const std::string &);
    
    // void HandlerMarkFavourite (const drogon::HttpRequestPtr &, std::function<void (const drogon::HttpResponsePtr &)> &&,
    //     const drogon::orm::DbClientPtr &, const std::string &, std::shared_ptr<MyCache<Item>>);
private:
    std::shared_ptr<MyCache<Item>> itemCache = std::make_shared<MyCache<Item>>(100);
};
