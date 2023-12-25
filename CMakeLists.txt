cmake_minimum_required (VERSION 3.27)

# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(BOT_NAME "cringe")

project(${BOT_NAME} VERSION 1.0 DESCRIPTION "std::cringe discord bot")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(FetchContent)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/utils/utils.h
        src/utils/colors.h
        src/commands/commands.h
        src/utils/utils.cpp
        src/commands/info.cpp
        src/commands/challenges.cpp
        src/commands/voice.cpp
        src/commands/apis.cpp
        src/commands/messages.cpp
        src/listeners/message_listener.h
        src/listeners/message_listener.cpp
        src/utils/logger.h
        src/utils/logger.cpp
)

find_package(DPP REQUIRED)
find_library(MPG123_LIBRARY mpg123)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

if (DPP_FOUND)
    message("Found DPP! Yep")
    target_link_libraries(${BOT_NAME} ${DPP_LIBRARIES})
    target_include_directories(${BOT_NAME} PUBLIC ${DPP_INCLUDE_DIRS})
else()
    message(WARNING "Could not find DPP install. Building from source instead.")
    option(DPP_BUILD_TEST "" OFF)

    FetchContent_Declare(
            libdpp
            GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git
            GIT_TAG master)

    FetchContent_GetProperties(libdpp)
    if(NOT libdpp_POPULATED)
        FetchContent_Populate(libdpp)
        target_include_directories(${BOT_NAME} PUBLIC
                ${libdpp_SOURCE_DIR}/include
        )
        add_subdirectory(
                ${libdpp_SOURCE_DIR}
                ${libdpp_BINARY_DIR}
                EXCLUDE_FROM_ALL)
    endif()

    FetchContent_Declare(fmt
            GIT_REPOSITORY https://github.com/fmtlib/fmt.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(fmt)

    target_link_libraries(${BOT_NAME} dpp)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set_target_properties(${BOT_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)

target_include_directories(${BOT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENSSL_INCLUDE_DIR}
)

string(ASCII 27 Esc)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)

if(APPLE)
    if(CMAKE_APPLE_SILICON_PROCESSOR)
        set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl")
        set(FMT_ROOT_DIR "/opt/homebrew/Cellar/fmt/10.1.1")
    else()
        set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
        set(FMT_ROOT_DIR "/usr/local/opt/fmt/10.1.1")
    endif()
    find_package(OpenSSL REQUIRED)
    find_package(fmt REQUIRED)
else()
    find_package(OpenSSL REQUIRED)
endif()

target_include_directories(${BOT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENSSL_INCLUDE_DIR}
        "/opt/homebrew/Cellar/fmt/10.1.1"
        "/opt/homebrew/include/mpg123.h"
)

target_link_libraries(${BOT_NAME}
        dl
        ${CMAKE_THREAD_LIBS_INIT}
        ${OPENSSL_CRYPTO_LIBRARY}
        ${OPENSSL_SSL_LIBRARY}
        ${MPG123_LIBRARY}
        curl
        fmt::fmt
        spdlog::spdlog
)