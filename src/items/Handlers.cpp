#include "Base.h"
#include <drogon/drogon.h>
#include <json/value.h>
#include "Cache.h"
#include "Tokens.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include <drogon/CacheMap.h>

#include "Handlers.h"

void ItemHandler::HandlerGetItemMini (const drogon::HttpRequestPtr &req, 
    std::function<void (const drogon::HttpResponsePtr &)> &&callback,
const drogon::orm::DbClientPtr &dbClient, const std::string &item_id, MyCache<Item> &itemCache) {

    auto itemId = item_id;

    auto resp = drogon::HttpResponse::newHttpResponse();

    if (itemCache.exists(item_id)) {
        Item item = itemCache.get(item_id);

        Json::Value jItem;
        jItem["title"] = item.Title,
        jItem["previewLink"] = item.PreviewLink,
        jItem["weight"] = item.Weight,
        jItem["cost"] = item.Cost,
        jItem["rating"] = item.Rating;

        resp->setStatusCode(drogon::k200OK);
        resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
        resp->setBody(jItem.toStyledString());
        callback(resp);
        return;
    }
    else {
        dbClient->execSqlAsync("select title, preview_link, weight, cost, rating from item where id=$1" , 
            [resp, callback, &itemCache, item_id](const drogon::orm::Result &r) {
                if (r.size() > 0) {

                    Item item (
                        r[0][0].as<std::string>(),
                        "",
                        r[0][1].as<std::string>(),
                        r[0][2].as<int64_t>(),
                        r[0][3].as<int64_t>(),
                        r[0][4].as<int64_t>()
                    );
                    
                    itemCache.insert(item, item_id);

                    Json::Value jItem;
                    jItem["title"] = item.Title,
                    jItem["previewLink"] = item.PreviewLink,
                    jItem["weight"] = item.Weight,
                    jItem["cost"] = item.Cost,
                    jItem["rating"] = item.Rating;

                    resp->setStatusCode(drogon::k200OK);
                    resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
                    resp->setBody(jItem.toStyledString());
                    callback(resp);
                    return;
                }
                else {
                    resp->setStatusCode(drogon::k404NotFound);
                    resp->setBody("no such item");  
                    callback(resp);
                    return;
                }
            }, 
            [resp, callback](const drogon::orm::DrogonDbException &e) {
                std::cerr << "error:" << e.base().what() << std::endl;
                resp->setStatusCode(drogon::k500InternalServerError);
                resp->setBody("db error");
                callback(resp);

                return;
            }, 
            itemId
        );
    }
}

void ItemHandler::HandlerAddItem_temporary (const drogon::HttpRequestPtr &req, 
    std::function<void (const drogon::HttpResponsePtr &)> &&callback,
const drogon::orm::DbClientPtr &dbClient) {
    auto json = req->jsonObject();

    Item item(
        (*json)["title"].as<std::string>(),
        (*json)["description"].as<std::string>(),
        (*json)["previewLink"].as<std::string>(),
        (*json)["weight"].as<int64_t>(),
        (*json)["cost"].as<int64_t>(),
        (*json)["rating"].as<int64_t>()
    );

    std::string id = (*json)["id"].as<std::string>();

    dbClient->execSqlSync("insert into item (id, title, preview_link, description, weight, cost, rating) values ($1, $2, $3, $4, $5, $6, $7)",
        id, item.Title, item.PreviewLink, item.Description, item.Weight, item.Cost, item.Rating);

    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k200OK);

    callback(resp);
}  

void ItemHandler::HandlerUpdItem (const drogon::HttpRequestPtr &req, 
    std::function<void (const drogon::HttpResponsePtr &)> &&callback,
const drogon::orm::DbClientPtr &dbClient) {
    
    auto json = req->jsonObject();

    auto jsonItem = (*json)["item"];
    Item item(
        jsonItem["title"].as<std::string>(),
        jsonItem["description"].as<std::string>(),
        jsonItem["previewLink"].as<std::string>(),
        (*json)["weight"].as<int64_t>(),
        (*json)["cost"].as<int64_t>(),
        (*json)["rating"].as<int64_t>()
    );

    auto jsonTokens = (*json)["tokens"];

    Tokens tokens(
        jsonTokens["access"].as<std::string>(),
        jsonTokens["refresh"].as<std::string>()
    );

    std::string deviceId = (*json)["deviceId"].as<std::string>();
    auto newTokens = tokens.update_tokens(deviceId, dbClient);

    auto resp = drogon::HttpResponse::newHttpResponse();

    if (newTokens.first == "") {
        resp->setStatusCode(drogon::k401Unauthorized);
        callback(resp);
        return;
    }

}