#include <cstdlib>
#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <ostream>
#include <drogon/CacheMap.h>
#include <dotenv.h>
#include <string>

int32_t main() {

    dotenv::init();
    
    const std::string POSTGRES_CONN = std::getenv("POSTGRES_CONN");
    const std::string SERVER_ADDRESS = std::getenv("SERVER_ADDRESS");
    drogon::app().loadConfigFile("./src/config.json");

    auto dbClient = drogon::orm::DbClient::newPgClient(POSTGRES_CONN, 1);

    std::cout << "Server started on port " + SERVER_ADDRESS << std::endl;
    drogon::app().addListener("0.0.0.0", stoi(SERVER_ADDRESS)).run();
}
