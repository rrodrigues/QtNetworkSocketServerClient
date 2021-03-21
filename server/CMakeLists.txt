cmake_minimum_required(VERSION 3.14)

project(server LANGUAGES CXX VERSION 0.0.1)

add_executable(server MACOSX_BUNDLE
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

configure_file(config.h.in config.h)
target_include_directories(server PUBLIC "${PROJECT_BINARY_DIR}")

install(TARGETS server DESTINATION server)
include(DeployQt)
# config_install(server)

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR ZIP)
include(CPack)
