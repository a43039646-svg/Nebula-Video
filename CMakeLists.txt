# SPDX-License-Identifier: GPL-3.0-or-later
cmake_minimum_required(VERSION 3.21)
project(NebulaVideo VERSION 0.1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Widgets WebEngineCore WebEngineWidgets Multimedia MultimediaWidgets)

qt_add_executable(nebula-video
    src/main.cpp
    src/nebula_video_window.cpp
    src/nebula_translator.cpp
    src/nebula_theme.cpp
)

qt_add_resources(nebula-video "app_translations"
    PREFIX "/locales"
    BASE "${CMAKE_CURRENT_SOURCE_DIR}/locales"
    FILES
        locales/ru-RU.json
        locales/es-ES.json
        locales/fr-FR.json
        locales/de-DE.json
        locales/pt-BR.json
        locales/zh-CN.json
        locales/ja-JP.json
        locales/uk-UA.json
        locales/be-BY.json
        locales/lv-LV.json
        locales/it-IT.json
        locales/pl-PL.json
        locales/tr-TR.json
        locales/nl-NL.json
        locales/ko-KR.json
        locales/ar-SA.json
        locales/hi-IN.json
        locales/id-ID.json
        locales/vi-VN.json
        locales/sv-SE.json
        locales/cs-CZ.json
        locales/ro-RO.json
        locales/da-DK.json
        locales/fi-FI.json
        locales/nb-NO.json
        locales/el-GR.json
        locales/bg-BG.json
        locales/sr-RS.json
        locales/hu-HU.json
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
set(CPACK_RPM_PACKAGE_LICENSE "GPL-3.0-or-later")
include(CPack)
