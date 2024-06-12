#include <drogon/drogon.h>
#include <ostream>

using namespace drogon;

int main() {
    app().addListener("0.0.0.0", 8080)
        .setThreadNum(16)
        .run();
    std::cout << "Server started on port 8080" << std::endl;
}