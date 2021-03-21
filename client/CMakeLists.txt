cmake_minimum_required(VERSION 3.14)

project(client
    VERSION 0.0.2
    LANGUAGES CXX
)

add_executable(client MACOSX_BUNDLE
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
)
target_link_libraries(client Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(client Qt${QT_VERSION_MAJOR}::Network)

target_compile_definitions(client PUBLIC APP_VERSION_STR="${client_VERSION}")

configure_file(config.h.in config.h)
target_include_directories(client PUBLIC "${PROJECT_BINARY_DIR}")

install(TARGETS client DESTINATION client)
include(DeployQt)
# config_install(client)
