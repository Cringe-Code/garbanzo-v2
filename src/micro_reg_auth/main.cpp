#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <functional>
#include <ostream>
#include "Handlers.h"
#include "drogon/HttpTypes.h"

int32_t main() {
    
    drogon::orm::DbClientPtr dbClient;

    drogon::app().registerHandler(
        "/register",
        [dbClient](const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
            HandlerRegister(req, std::move(callback), dbClient);
        },
        {drogon::Post});
    
    std::cout << "Server started on port 8080" << std::endl;
    drogon::app().addListener("0.0.0.0", 8080).run();
}