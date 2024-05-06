cmake_minimum_required(VERSION 3.25)

set(BOT_NAME "cringe")

project(${BOT_NAME} VERSION 1.0 DESCRIPTION "std::cringe discord bot")

include(FetchContent)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/utils/listeners/cringe_logger.cpp
        src/utils/embed/cringe_embed.cpp
        src/utils/audio/cringe_song.cpp
        src/utils/audio/cringe_queue.cpp
        src/utils/audio/cringe_audio_helpers.cpp
        src/commands/voice/join_command.cpp
        src/commands/voice/play_command.cpp
        src/commands/voice/queue_command.cpp
        src/commands/chat/info_command.cpp
        src/commands/chat/chat_command.cpp
        src/commands/chat/user_command.cpp
        src/commands/voice/skip_command.cpp
        include/commands/voice/skip_command.h
        src/commands/chat/confession_command.cpp
        include/commands/chat/confession_command.h
        include/commands/api/reddit_command.h
        src/commands/api/reddit_command.cpp
        src/commands/chat/imagine_command.cpp
        include/commands/chat/imagine_command.h
        include/commands/chat/chat_command.h
        include/commands/chat/user_command.h
        src/utils/listeners/cringe_slashcommand.cpp
        include/utils/listeners/cringe_slashcommand.h
        src/commands/chat/describe_command.cpp
        include/commands/chat/describe_command.h
        include/utils/misc/base64.h
        src/utils/misc/base64.cpp
        src/utils/validators/validate_chat.cpp
        src/utils/database/cringe_database.cpp
        include/connectors/cringe_database.h
        include/validators/validate_chat.h
        src/utils/http/cringe_request.cpp
        src/utils/audio/cringe_audio_streaming.cpp
        src/utils/http/cringe_api.cpp
        src/utils/http/cringe_api.cpp
        src/utils/misc/cringe_helpers.cpp
        src/utils/misc/cringe_helpers.cpp
        include/commands/chat/info_command.h
        include/utils/audio/cringe_audio_helpers.h
        include/utils/audio/cringe_audio_streaming.h
        include/utils/audio/cringe_queue.h
        include/utils/audio/cringe_song.h
        include/utils/database/cringe_database.h
        include/utils/embed/cringe_embed.h
        include/utils/http/cringe_api.h
        include/utils/http/cringe_request.h
        include/utils/listeners/cringe_logger.h
        include/utils/validators/validate_chat.h
        include/utils/misc/cringe_helpers.h
        src/commands/chat/code_command.cpp
        include/commands/chat/code_command.h
        src/utils/audio/cringe_yt.cpp
        include/utils/audio/cringe_yt.h
        include/utils/audio/cringe_song_new.h
        src/utils/bot/cringe_bot.cpp
        include/utils/bot/cringe_bot.h
)

include(ExternalProject)
include(FindPackageHandleStandardArgs)
find_package(SQLite3 REQUIRED)
find_package(DPP REQUIRED)
find_package(Threads REQUIRED)
#find_package(OpenSSL REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(CURL REQUIRED)
find_library(FFMPEG_LIB avcodec avformat avutil avfilter)
if(CURL_FOUND)
    message("FOUND CURL!")
else()
    message("COULDNT FIND CURL")
endif()
if (DPP_FOUND)
    message("Found DPP! Yep")
else ()
    message(WARNING "Could not find DPP install. Building from source instead.")
    option(DPP_BUILD_TEST "" OFF)
    FetchContent_Declare(
            libdpp
            GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git
            GIT_TAG master)
    FetchContent_GetProperties(libdpp)
    if (NOT libdpp_POPULATED)
        FetchContent_Populate(libdpp)
        target_include_directories(${BOT_NAME} PUBLIC
                ${libdpp_SOURCE_DIR}/include
        )
        add_subdirectory(
                ${libdpp_SOURCE_DIR}
                ${libdpp_BINARY_DIR}
                EXCLUDE_FROM_ALL)
    endif ()
    FetchContent_Declare(fmt
            GIT_REPOSITORY https://github.com/fmtlib/fmt.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(fmt)
    target_link_libraries(${BOT_NAME} dpp)
endif ()
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set_target_properties(${BOT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
target_include_directories(${BOT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
#        ${OPENSSL_INCLUDE_DIR}
        ${SQLite3_INCLUDE_DIRS}
        ${FFMPEG_INCLUDE_DIRS}
        ${CURL_INCLUDE_DIR}
        ${DPP_INCLUDE_DIRS}
)
string(ASCII 27 Esc)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
target_link_libraries(${BOT_NAME}
        dl
        ${CMAKE_THREAD_LIBS_INIT}
#        ${OPENSSL_CRYPTO_LIBRARY}
#        ${OPENSSL_SSL_LIBRARY}
        ${SQLite3_LIBRARIES}
        ${CURL_LIBRARIES}
        fmt::fmt
        spdlog::spdlog
        ${DPP_LIBRARIES}
        ${FFMPEG_LIBRARIES}
)