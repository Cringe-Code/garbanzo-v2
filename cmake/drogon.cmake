include(FetchContent)

FetchContent_Declare(
    drogon
    GIT_REPOSITORY https://github.com/drogonframework/drogon.git
)
FetchContent_MakeAvailable(drogon)

target_link_libraries(${PROJECT_NAME} PRIVATE drogon)