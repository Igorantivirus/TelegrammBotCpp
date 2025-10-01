cmake_minimum_required(VERSION 3.16)
project(TelegrammBotCpp)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_NAME ${PROJECT_NAME})
set(THIRD_PARTY_BUILD_DIR "${CMAKE_BINARY_DIR}/_3rdparty")

FetchContent_Declare(
    MathWorker
    GIT_REPOSITORY https://github.com/Igorantivirus/MathWorker.git
    GIT_TAG        origin/master
)
FetchContent_MakeAvailable(MathWorker)

find_package(pugixml CONFIG REQUIRED)
find_package(CURL REQUIRED)

#TgBot
if(WIN32)

    find_path(TGBOT_INCLUDE_DIR NAMES "tgbot/tgbot.h" PATHS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
    find_library(TGBOT_LIBRARY NAMES "TgBot" PATHS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib")

    if (NOT TGBOT_INCLUDE_DIR)
        message(FATAL_ERROR "Не удалось найти заголовочные файлы TgBot!")
    endif()

    if (NOT TGBOT_LIBRARY)
        message(FATAL_ERROR "Не удалось найти библиотеку TgBot!")
    endif()

    add_library(TgBot::TgBot INTERFACE IMPORTED)
    target_include_directories(TgBot::TgBot INTERFACE ${TGBOT_INCLUDE_DIR})
    target_link_libraries(TgBot::TgBot INTERFACE ${TGBOT_LIBRARY})

else()

    find_package(Threads REQUIRED)
    find_package(OpenSSL REQUIRED)
    find_package(Boost COMPONENTS system filesystem REQUIRED)
    include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
    if (CURL_FOUND)
        include_directories(${CURL_INCLUDE_DIRS})
        add_definitions(-DHAVE_CURL)
    endif()


endif()
#end TgBot

add_executable(${EXECUTABLE_NAME}
    src/main.cpp
)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE 
    pugixml::pugixml
    MathWorker::MathWorker
    CURL::libcurl
)

#TgBot
if(WIN32)
    
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE TgBot::TgBot)

else()

    target_link_libraries(${EXECUTABLE_NAME} PRIVATE /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})

endif()