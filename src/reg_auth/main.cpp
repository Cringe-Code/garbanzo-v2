#include <cstdlib>
#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <functional>
#include <ostream>
#include "Handlers.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include "drogon/utils/FunctionTraits.h"
#include "items/Handlers.h"
#include <dotenv.h>
#include <string>

int32_t main() {
    dotenv::init();
    
    const std::string POSTGRES_CONN = std::getenv("POSTGRES_CONN");
    const std::string SERVER_ADDRESS = std::getenv("SERVER_ADDRESS");

    auto dbClient = drogon::orm::DbClient::newPgClient(POSTGRES_CONN, 1);
    
    drogon::app().registerHandler(
        "/register",
        [dbClient](const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
            HandlerRegister(req, std::move(callback), dbClient);
        },
        {drogon::Post}
    );

    drogon::app().registerHandler(
        "/sign_in",
        [dbClient](const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
            HandlerAuth(req, std::move(callback), dbClient);
        },
        {drogon::Post}
    );
    
    drogon::app().registerHandler(
        "/item/{item_name}",
        [dbClient](const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
            std::string item_id) {
            HandlerGetItemMini(req, std::move(callback), dbClient, item_id);
        },
        {drogon::Get}
    );

    drogon::app().registerHandler(
        "/test",
        [dbClient](const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
            HandlerAddItem_temporary(req, std::move(callback), dbClient);
        },
        {drogon::Post}
    );

    std::cout << "Server started on port " + SERVER_ADDRESS << std::endl;
    drogon::app().addListener("0.0.0.0", stoi(SERVER_ADDRESS)).run();
}