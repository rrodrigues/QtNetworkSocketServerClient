cmake_minimum_required(VERSION 3.14)

project(client LANGUAGES CXX)

add_executable(client
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
)
target_link_libraries(client Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(client Qt${QT_VERSION_MAJOR}::Network)
