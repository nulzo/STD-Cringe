cmake_minimum_required(VERSION 3.25)

set(BOT_NAME "cringe")

project(${BOT_NAME} VERSION 1.0 DESCRIPTION "std::cringe discord bot")

include(FetchContent)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/commands/challenge/ChallengeCommand.cpp
        src/listeners/message_listener.cpp
        src/utils/logger.cpp
        src/utils/CringeEmbed.cpp
        src/utils/util.cpp
        src/utils/audio/CringeSong.cpp
        src/utils/audio/CringeQueue.cpp
        src/utils/audio/CringeAudio.cpp
        src/commands/voice/JoinCommand.cpp
        src/commands/voice/PlayCommand.cpp
        src/commands/voice/QueueCommand.cpp
        src/commands/misc/InfoCommand.cpp
        src/commands/misc/GetMessagesCommand.cpp
        src/commands/misc/EthanCommand.cpp
        src/commands/challenge/ChallengeCommand.cpp
        src/commands/api/ChatCommand.cpp
        src/commands/api/UserCommand.cpp
        src/commands/voice/SkipCommand.cpp
        include/commands/voice/SkipCommand.h
        src/commands/misc/ConfessionCommand.cpp
        include/commands/misc/ConfessionCommand.h
        include/commands/api/RedditCommand.h
        src/commands/api/RedditCommand.cpp
        src/commands/api/ImageCommand.cpp
        include/commands/api/ImageCommand.h
        include/commands/api/TalkCommand.h
        src/commands/api/TalkCommand.cpp
        include/commands/api/ChatCommand.h
        include/commands/api/UserCommand.h
        src/listeners/SlashcommandListener.cpp
        include/listeners/SlashcommandListener.h
        src/commands/api/DescribeCommand.cpp
        include/commands/api/DescribeCommand.h
        include/utils/base64.h
        src/utils/base64.cpp
)

include(FindPackageHandleStandardArgs)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(CURL REQUIRED)
find_library(FFMPEG_LIB avcodec avformat avutil avfilter)

if(CURL_FOUND)
    message("FOUND CURL!")
else()
    message("COULDNT FIND CURL")
endif()

# Use the installed DPP libraries
find_package(DPP)

if (DPP_FOUND)
    message("Found DPP! Yep")
else ()
    include(ExternalProject)
    ExternalProject_Add(
            dpp_external_project
            GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git
            GIT_TAG master  # Replace with the desired branch or tag
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/dpp_install
    )
endif ()
#    message(WARNING "Could not find DPP install. Building from source instead.")
#    option(DPP_BUILD_TEST "" OFF)
#    FetchContent_Declare(
#            libdpp
#            GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git
#            GIT_TAG master)
#    FetchContent_GetProperties(libdpp)
#    if (NOT libdpp_POPULATED)
#        FetchContent_Populate(libdpp)
#        target_include_directories(${BOT_NAME} PUBLIC
#                ${libdpp_SOURCE_DIR}/include
#        )
#        add_subdirectory(
#                ${libdpp_SOURCE_DIR}
#                ${libdpp_BINARY_DIR}
#                EXCLUDE_FROM_ALL)
#    endif ()
#
#    target_link_libraries(${BOT_NAME} dpp)
#endif ()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set_target_properties(${BOT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)

target_include_directories(${BOT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENSSL_INCLUDE_DIR}
        ${FFMPEG_INCLUDE_DIRS}
        ${CURL_INCLUDE_DIR}
        ${DPP_INCLUDE_DIRS}
)

string(ASCII 27 Esc)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)

target_link_libraries(${BOT_NAME}
        dl
        ${CMAKE_THREAD_LIBS_INIT}
        ${OPENSSL_CRYPTO_LIBRARY}
        ${OPENSSL_SSL_LIBRARY}
        ${CURL_LIBRARIES}
        spdlog::spdlog
        ${DPP_LIBRARIES}
        ${FFMPEG_LIBRARIES}
)