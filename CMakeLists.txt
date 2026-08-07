cmake_minimum_required(VERSION 3.19)
project(SpriteEditor LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(RESOURCE_FILES spriteeditor.qrc)
QT_ADD_RESOURCES(PROJECT_RESOURCES ${RESOURCE_FILES})

qt_add_executable(SpriteEditor
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    editarea.cpp
    editarea.h
    colorrect.cpp
    colorrect.h
    palette.cpp
    palette.h
    ${PROJECT_RESOURCES}
)

target_link_libraries(SpriteEditor
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS SpriteEditor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET SpriteEditor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})