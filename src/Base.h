#pragma once

#include <iostream>


inline std::string SECRET_KEY = "jfwheofiehifwoef";

struct User {
    std::string Login;
    std::string Email;
    std::string Phone;
    std::string Password;

    User(std::string login = "", std::string email = "", std::string phone = "", std::string password = "")
    : Login(std::move(login)), Email(std::move(email)), Phone(std::move(phone)), Password(std::move(password))
    {}
};

