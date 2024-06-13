#pragma once

#include <iostream>

struct User {
    std::string Login;
    std::string Email;
    std::string Password;

    User(std::string &&Login = "", std::string &&Email = "", std::string &&Password =  "") 
        : Login(Login), Email(Email), Password(Password) {};
};