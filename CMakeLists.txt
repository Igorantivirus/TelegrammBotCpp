cmake_minimum_required(VERSION 3.16)
project(TelegrammBotCpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O1")
set(Boost_USE_MULTITHREADED ON)

include(FetchContent)
FetchContent_Declare(
  pugixml
  GIT_REPOSITORY https://github.com/zeux/pugixml
  GIT_TAG        origin/master
)
FetchContent_Declare(
  libpme
  GIT_REPOSITORY https://github.com/Igorantivirus/Parse-math-expression
  GIT_TAG        origin/complexNums
)
FetchContent_MakeAvailable(pugixml libpme)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)
include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

add_executable(TelegrammBotCpp
    main.cpp
    AnekdotParser.hpp
    MyApiKey.hpp
    Randomiser.hpp
)

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(TelegrammBotCpp ${CURL_LIBRARIES})

target_link_libraries(TelegrammBotCpp pugixml)

#target_link_libraries(TelegrammBotCpp libpme)
target_include_directories(TelegrammBotCpp PRIVATE ${libpme_SOURCE_DIR})

target_link_libraries(TelegrammBotCpp /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PagesData.xml ${CMAKE_BINARY_DIR}/bin/PagesData.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data.xml ${CMAKE_BINARY_DIR}/bin/data.xml COPYONLY)
