cmake_minimum_required( VERSION 2.8)
set( CMAKE_CXX_FLAGS "-std=c++17" )

PROJECT(commsys)

set(CLIENT "client")
set(SERVER "server")

set(PROTO_FILES ${PROJECT_SOURCE_DIR}/utils/messgae.proto)
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROJECT_SOURCE_DIR}/utils/message.proto)



set(SRC_client
    # ${PROJECT_SOURCE_DIR}/src/client/client.cpp
    ${PROJECT_SOURCE_DIR}/src/client/mainWindow.cpp
    ${PROJECT_SOURCE_DIR}/src/client/loginWindow.cpp
    ${PROJECT_SOURCE_DIR}/src/client/loginSuccWindow.cpp
    ${PROJECT_SOURCE_DIR}/src/client/main.cpp
    ${PROJECT_SOURCE_DIR}/src/client/registerWindow.cpp
    # ${PROJECT_SOURCE_DIR}/utils/message.pb.cc
    ${PROTO_SRCS}
    ${PROJECT_SOURCE_DIR}/utils/transfer.cpp
)

set(SRC_server
    ${PROJECT_SOURCE_DIR}/src/server/main.cpp
    ${PROJECT_SOURCE_DIR}/src/server/server.cpp
    ${PROJECT_SOURCE_DIR}/src/server/dispatcher.cpp
    ${PROJECT_SOURCE_DIR}/src/server/loginProcess.cpp
    ${PROJECT_SOURCE_DIR}/src/server/registerProcess.cpp
    ${PROJECT_SOURCE_DIR}/src/server/SmsProcess.cpp
    # ${PROJECT_SOURCE_DIR}/utils/message.pb.cc
    ${PROTO_SRCS}
    ${PROJECT_SOURCE_DIR}/utils/transfer.cpp
)


include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/build
)

add_executable(client ${SRC_client})
add_executable(server ${SRC_server})

target_link_libraries(${CLIENT} protobuf pthread)
target_link_libraries(${SERVER} protobuf pthread)

# <------------ add hiredis dependency --------------->
find_path(HIREDIS_HEADER hiredis)
target_include_directories(${SERVER} PUBLIC ${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
target_link_libraries(${SERVER} ${HIREDIS_LIB})

# <------------ add redis-plus-plus dependency -------------->
# NOTE: this should be *sw* NOT *redis++*
find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(${SERVER} PUBLIC ${REDIS_PLUS_PLUS_HEADER})
find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(${SERVER} ${REDIS_PLUS_PLUS_LIB})
