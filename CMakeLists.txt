# Copyright (C) 2022-2024 Furkan Karcıoğlu <https://github.com/frknkrc44>
#
# This file is part of SuperGfxCtl-QTray project,
# and licensed under GNU Affero General Public License v3.
# See the GNU Affero General Public License for more details.
#
# All rights reserved. See COPYING, AUTHORS.
#

cmake_minimum_required(VERSION 3.16)
project(supergfxctl-qtray LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

set(INSTALL_EXAMPLEDIR "")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_add_executable(supergfxctl-qtray
    main.cpp
    window.cpp window.h
)

target_link_libraries(supergfxctl-qtray PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

set(supergfxctl-qtray_resource_files
    "images/dgpu-suspended.png"
    "images/gpu-hybrid.png"
    "images/gpu-integrated.png"
    "images/gpu-vfio.png"
    "images/gpu-egpu.png"
)

qt_add_resources(supergfxctl-qtray "supergfxctl-qtray"
    PREFIX
        "/"
    FILES
        ${supergfxctl-qtray_resource_files}
)

install(TARGETS supergfxctl-qtray
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
