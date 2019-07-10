project(wrapped_key_value_storage)
find_package(protobuf REQUIRED)
find_package(stlab REQUIRED)

set(PATH_TO_PROTO ${CMAKE_CURRENT_SOURCE_DIR}/../proto)
file(GLOB ProtoFiles "${PATH_TO_PROTO}/*.proto")

# This will avoid all issues with protobuf
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generated/messages.pb.cc")
    message(FATAL_ERROR "Missing file ${CMAKE_CURRENT_SOURCE_DIR}/generated/messages.pb.cc, please first generate files with ${CMAKE_CURRENT_SOURCE_DIR}/../gen_proto script or configure the project with -DWITH_EXAMPLES=OFF")
endif()

add_library(wrapped_lib SHARED wrapped_lib.cpp generated/messages.pb.cc)

target_link_libraries(wrapped_lib PUBLIC key_value_storage)
target_link_libraries(wrapped_lib PRIVATE 
    protobuf::libprotoc
    protobuf::libprotobuf
    stlab::coroutines)
target_include_directories(wrapped_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/generated)
target_include_directories(wrapped_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(wrapped_lib PRIVATE ${CMAKE_CURRENT_BINARY_DIR})