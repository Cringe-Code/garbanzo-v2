#pragma once

#include <drogon/drogon.h>
#include <cstdint>
#include <iostream>


inline const std::string SECRET_KEY = "jfwheofiehifwoef";

class User {
public:
    std::string Login;
    std::string Email;
    std::string Phone;
    std::string Password;
    const std::string DeviceId;

    User(std::string login = "", std::string email = "", std::string phone = "", std::string password = "", std::string deviceId = "")
    : Login(std::move(login)), Email(std::move(email)), Phone(std::move(phone)), Password(std::move(password)), DeviceId(std::move(deviceId))
    {}  

    bool isAdmin() {
        return IsAdmin;
    }

    bool updAdmin(const drogon::orm::DbClientPtr &dbClient) {
        try {
            dbClient->execSqlSync("update users set is_admin=$1", !IsAdmin);
        }
        catch (drogon::orm::DrogonDbException &e) {
            std::cout << e.base().what() << std::endl;
            return false;
        }
        
        return true;
    }

private:
    bool IsAdmin;
};

struct Item {
    std::string Title;
    std::string Description;
    std::string PreviewLink;
    int64_t Weight;
    int64_t Cost;
    int64_t Rating;

    Item(std::string title = "", std::string description = "", std::string previewLink = "", 
        int64_t weight = 0, int64_t cost = 0, int64_t rating = 0)
        : Title(std::move(title)), Description(std::move(description)), PreviewLink(std::move(previewLink)),
        Weight(std::move(weight)), Cost(std::move(cost)), Rating(std::move(rating))
    {} 

    // Item& operator =(const Item &item) {
    //     if (&item != this) {
    //         Title = item.Title;
    //         Description = item.Description;
    //         PreviewLink = item.PreviewLink;
    //         Weight = item.Weight;
    //         Cost = item.Cost;
    //         Rating = item.Rating;
    //     }
    //     return *this;
    // }

};
