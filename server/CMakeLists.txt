cmake_minimum_required(VERSION 3.14)

project(server LANGUAGES CXX)

add_executable(server
    main.cpp
    server.h
    server.cpp
    threadpool.h
    threadpool.cpp
    connectionhandler.h
    connectionhandler.cpp
)
target_link_libraries(server Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(server Qt${QT_VERSION_MAJOR}::Network)
