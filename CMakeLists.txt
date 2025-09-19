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

# if(NOT DEFINED MATH_WORKER_DIR)
#     message(FATAL_ERROR "MATH_WORKER_DIR is not defined!")
# endif()

# #MathWorker
# set(MATHWORKER_BUILD_SHARED ON)
# add_subdirectory(${MATH_WORKER_DIR} ${THIRD_PARTY_BUILD_DIR}/MathWorker EXCLUDE_FROM_ALL)

find_package(pugixml CONFIG REQUIRED)
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
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${mathworker_SOURCE_DIR}/include/
)

#TgBot
if(WIN32)
    
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE TgBot::TgBot)

else()

    target_link_libraries(${EXECUTABLE_NAME} PRIVATE /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})

endif()

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O1")
# set(Boost_USE_MULTITHREADED ON)

# include(FetchContent)
# FetchContent_Declare(
#   pugixml
#   GIT_REPOSITORY https://github.com/zeux/pugixml
#   GIT_TAG        origin/master
# )
# FetchContent_Declare(
#   libpme
#   GIT_REPOSITORY https://github.com/Igorantivirus/Parse-math-expression
#   GIT_TAG        origin/complexNums
# )
# FetchContent_MakeAvailable(pugixml libpme)

# find_package(Threads REQUIRED)
# find_package(OpenSSL REQUIRED)
# find_package(Boost COMPONENTS system REQUIRED)
# find_package(CURL)
# include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
# if (CURL_FOUND)
#     include_directories(${CURL_INCLUDE_DIRS})
#     add_definitions(-DHAVE_CURL)
# endif()

# add_executable(TelegrammBotCpp
#     main.cpp
#     AnekdotParser.hpp
#     BotResponser.hpp
#     BotUtils.hpp
#     MyApiKey.hpp
#     Randomiser.hpp
# )

# include_directories(${CURL_INCLUDE_DIR})
# target_link_libraries(TelegrammBotCpp ${CURL_LIBRARIES})

# target_link_libraries(TelegrammBotCpp pugixml)

# #target_link_libraries(TelegrammBotCpp libpme)
# target_include_directories(TelegrammBotCpp PRIVATE ${libpme_SOURCE_DIR})
# target_include_directories(TelegrammBotCpp PRIVATE "D:/dev/libraries/mylib")

# target_link_libraries(TelegrammBotCpp /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})

# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PagesData.xml ${CMAKE_BINARY_DIR}/bin/PagesData.xml COPYONLY)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ResponseData.xml ${CMAKE_BINARY_DIR}/bin/ResponseData.xml COPYONLY)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/UsserDB.xml ${CMAKE_BINARY_DIR}/bin/UsserDB.xml COPYONLY)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/FileHeaders/bd.td ${CMAKE_BINARY_DIR}/bin/bd.td COPYONLY)
