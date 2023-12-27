cmake_minimum_required(VERSION 3.27)

# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(BOT_NAME "cringe")

project(${BOT_NAME} VERSION 1.0 DESCRIPTION "std::cringe discord bot")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(FetchContent)

include_directories(${BOT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/commands/challenge.cpp
        src/commands/voice.cpp
        src/commands/api.cpp
        src/commands/misc.cpp
        src/listeners/message_listener.cpp
        src/utils/logger.cpp
        src/utils/embed.cpp
        src/utils/util.cpp
)

include(FindPackageHandleStandardArgs)

# The default components were taken from a survey over other FindFFMPEG.cmake files
if (NOT FFmpeg_FIND_COMPONENTS)
    set(FFmpeg_FIND_COMPONENTS AVCODEC AVFORMAT AVUTIL OPUS)
endif ()

#
### Macro: set_component_found
#
# Marks the given component as found if both *_LIBRARIES AND *_INCLUDE_DIRS is present.
#
macro(set_component_found _component )
    if (${_component}_LIBRARIES AND ${_component}_INCLUDE_DIRS)
        # message(STATUS "  - ${_component} found.")
        set(${_component}_FOUND TRUE)
    else ()
        # message(STATUS "  - ${_component} not found.")
    endif ()
endmacro()

#
### Macro: find_component
#
# Checks for the given component by invoking pkgconfig and then looking up the libraries and
# include directories.
#
macro(find_component _component _pkgconfig _library _header)

    if (NOT WIN32)
        # use pkg-config to get the directories and then use these values
        # in the FIND_PATH() and FIND_LIBRARY() calls
        find_package(PkgConfig)
        if (PKG_CONFIG_FOUND)
            pkg_check_modules(PC_${_component} ${_pkgconfig})
        endif ()
    endif (NOT WIN32)

    find_path(${_component}_INCLUDE_DIRS ${_header}
            HINTS
            ${PC_${_component}_INCLUDEDIR}
            ${PC_${_component}_INCLUDE_DIRS}
            PATH_SUFFIXES
            ffmpeg
    )

    find_library(${_component}_LIBRARIES NAMES ${_library}
            HINTS
            ${PC_${_component}_LIBDIR}
            ${PC_${_component}_LIBRARY_DIRS}
    )

    set(${_component}_DEFINITIONS  ${PC_${_component}_CFLAGS_OTHER} CACHE STRING "The ${_component} CFLAGS.")
    set(${_component}_VERSION      ${PC_${_component}_VERSION}      CACHE STRING "The ${_component} version number.")

    set_component_found(${_component})

    mark_as_advanced(
            ${_component}_INCLUDE_DIRS
            ${_component}_LIBRARIES
            ${_component}_DEFINITIONS
            ${_component}_VERSION)

endmacro()


# Check for cached results. If there are skip the costly part.
if (NOT FFMPEG_LIBRARIES)

    # Check for all possible component.
    find_component(AVCODEC    libavcodec    avcodec  libavcodec/avcodec.h)
    find_component(AVFORMAT   libavformat   avformat libavformat/avformat.h)
    find_component(AVDEVICE   libavdevice   avdevice libavdevice/avdevice.h)
    find_component(AVUTIL     libavutil     avutil   libavutil/avutil.h)
    find_component(AVFILTER   libavfilter   avfilter libavfilter/avfilter.h)
    find_component(SWSCALE    libswscale    swscale  libswscale/swscale.h)
    find_component(POSTPROC   libpostproc   postproc libpostproc/postprocess.h)
    find_component(SWRESAMPLE libswresample swresample libswresample/swresample.h)

    # Check if the required components were found and add their stuff to the FFMPEG_* vars.
    foreach (_component ${FFmpeg_FIND_COMPONENTS})
        if (${_component}_FOUND)
            # message(STATUS "Required component ${_component} present.")
            set(FFMPEG_LIBRARIES   ${FFMPEG_LIBRARIES}   ${${_component}_LIBRARIES})
            set(FFMPEG_DEFINITIONS ${FFMPEG_DEFINITIONS} ${${_component}_DEFINITIONS})
            list(APPEND FFMPEG_INCLUDE_DIRS ${${_component}_INCLUDE_DIRS})
        else ()
            # message(STATUS "Required component ${_component} missing.")
        endif ()
    endforeach ()

    # Build the include path with duplicates removed.
    if (FFMPEG_INCLUDE_DIRS)
        list(REMOVE_DUPLICATES FFMPEG_INCLUDE_DIRS)
    endif ()

    # cache the vars.
    set(FFMPEG_INCLUDE_DIRS ${FFMPEG_INCLUDE_DIRS} CACHE STRING "The FFmpeg include directories." FORCE)
    set(FFMPEG_LIBRARIES    ${FFMPEG_LIBRARIES}    CACHE STRING "The FFmpeg libraries." FORCE)
    set(FFMPEG_DEFINITIONS  ${FFMPEG_DEFINITIONS}  CACHE STRING "The FFmpeg cflags." FORCE)

    mark_as_advanced(FFMPEG_INCLUDE_DIRS
            FFMPEG_LIBRARIES
            FFMPEG_DEFINITIONS)

endif ()

# Now set the noncached _FOUND vars for the components.
foreach (_component AVCODEC AVDEVICE AVFORMAT AVUTIL POSTPROCESS SWSCALE)
    set_component_found(${_component})
endforeach ()

# Compile the list of required vars
set(_FFmpeg_REQUIRED_VARS FFMPEG_LIBRARIES FFMPEG_INCLUDE_DIRS)
foreach (_component ${FFmpeg_FIND_COMPONENTS})
    list(APPEND _FFmpeg_REQUIRED_VARS ${_component}_LIBRARIES ${_component}_INCLUDE_DIRS)
endforeach ()

# Give a nice error message if some of the required vars are missing.
find_package_handle_standard_args(FFmpeg DEFAULT_MSG ${_FFmpeg_REQUIRED_VARS})

set(CMAKE_PREFIX_PATH "/opt/homebrew/lib/libopus.dylib")

find_package(DPP REQUIRED)
find_library(MPG123_LIBRARY mpg123)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
#find_package(opusenc REQUIRED)
#find_package(FFmpeg COMPONENTS AVCODEC AVFORMAT AVUTIL AVDEVICE REQUIRED)

if (DPP_FOUND)
    message("Found DPP! Yep")
    target_link_libraries(${BOT_NAME} ${DPP_LIBRARIES})
    target_include_directories(${BOT_NAME} PUBLIC ${DPP_INCLUDE_DIRS})
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

if (APPLE)
    if (CMAKE_APPLE_SILICON_PROCESSOR)
        set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl")
        set(FMT_ROOT_DIR "/opt/homebrew/Cellar/fmt/10.1.1")
    else ()
        set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
        set(FMT_ROOT_DIR "/usr/local/opt/fmt/10.1.1")
    endif ()
    find_package(OpenSSL REQUIRED)
    find_package(fmt REQUIRED)
else ()
    find_package(OpenSSL REQUIRED)
endif ()

target_include_directories(${BOT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENSSL_INCLUDE_DIR}
        "/opt/homebrew/Cellar/fmt/10.1.1"
        "/opt/homebrew/include/mpg123.h"
        "/opt/homebrew/bin/ffmpeg"
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
        ${FFMPEG_LIBRARIES}
        "/opt/homebrew/lib/libopus.dylib"
)
