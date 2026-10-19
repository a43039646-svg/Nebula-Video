# SPDX-License-Identifier: GPL-3.0-or-later
cmake_minimum_required(VERSION 3.21)
project(NebulaVideo VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Widgets WebEngineCore WebEngineWidgets Multimedia MultimediaWidgets)

qt_add_executable(nebula-video
    src/main.cpp
)

target_link_libraries(nebula-video PRIVATE
    Qt6::Widgets
    Qt6::WebEngineCore
    Qt6::WebEngineWidgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
)

install(TARGETS nebula-video RUNTIME DESTINATION bin)
install(FILES data/org.nebula.Video.desktop DESTINATION share/applications)
install(FILES data/org.nebula.Video.svg DESTINATION share/icons/hicolor/scalable/apps)

set(CPACK_GENERATOR "TGZ")
find_program(NEBULA_DPKG_DEB dpkg-deb)
if(NEBULA_DPKG_DEB)
    list(APPEND CPACK_GENERATOR "DEB")
endif()
find_program(NEBULA_RPMBUILD rpmbuild)
if(NEBULA_RPMBUILD)
    list(APPEND CPACK_GENERATOR "RPM")
endif()
set(CPACK_PACKAGE_NAME "nebula-video")
set(CPACK_PACKAGE_VENDOR "Nebula Project")
set(CPACK_PACKAGE_CONTACT "Nebula Project")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Video browser and local media player")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nebula Project")
set(CPACK_DEBIAN_PACKAGE_SECTION "video")
if(NEBULA_DPKG_DEB)
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
endif()
set(CPACK_RPM_PACKAGE_AUTOREQPROV ON)
include(CPack)
