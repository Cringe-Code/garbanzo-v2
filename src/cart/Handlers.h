#pragma once 

#include <iostream>
#include <drogon/drogon.h>

class CartHandler {
public:
    static void HandlerAddToCart (const drogon::HttpRequestPtr &, std::function<void (const drogon::HttpResponsePtr &)> &&,
        const drogon::orm::DbClientPtr &);

    static void HandlerDeleteFromCart (const drogon::HttpRequestPtr &, std::function<void (const drogon::HttpResponsePtr &)> &&,
        const drogon::orm::DbClientPtr &);
        
    static void HandlerGetCart (const drogon::HttpRequestPtr &, std::function<void (const drogon::HttpResponsePtr &)> &&,
        const drogon::orm::DbClientPtr &);
};
