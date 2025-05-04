cmake_minimum_required(VERSION 3.16)

project(qtscreenshot VERSION 0.1 LANGUAGES CXX)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    commonresources.qrc
    main.cpp
    screenshot.cpp
    screenshot.h
    overlay.cpp
    overlay.h
    capture.cpp
    capture.h
    toast.cpp
    toast.h
    settings.cpp
    settings.h
    settings.ui
    configmanager.cpp
    configmanager.h
    animatedwidget.cpp
    animatedwidget.h
    galleryview.cpp
    galleryview.h
    imagelist.cpp
    imagelist.h
    imageview.cpp
    imageview.h
)

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

qt_finalize_executable(${PROJECT_NAME})
