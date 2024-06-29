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
};

