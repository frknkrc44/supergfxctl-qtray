// Copyright (C) 2022 Furkan Karcıoğlu <https://github.com/frknkrc44>
//
// This file is part of SuperGfxCtl-QTray project,
// and licensed under GNU Affero General Public License v3.
// See the GNU Affero General Public License for more details.
//
// All rights reserved. See COPYING, AUTHORS.
//

#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>

#include "window.h"

#include <QAction>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>

const char* appendStr(const char* s, const char* append) {
    char* new_str = (char*) malloc(sizeof(char) * (strlen(s) + strlen(append) + 1));
    strcpy(new_str, s);
    strcat(new_str, append);
    return reinterpret_cast<const char*>(new_str);
}

std::string Window::executeCmd(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;

    auto pipe = popen(cmd, "r"); // get rid of shared_ptr

    if (!pipe) throw std::runtime_error("popen() failed!");

    while (!feof(pipe)) {
        if (fgets(buffer.data(), 128, pipe) != nullptr)
            result += buffer.data();
    }

    auto rc = pclose(pipe);

    if (rc == EXIT_SUCCESS) { // == 0

    } else if (rc == EXIT_FAILURE) {  // EXIT_FAILURE is not used by all programs, maybe needs some adaptation.

    }
    return result;
}

std::string Window::getGPUStatus()
{
    std::string out = executeCmd("supergfxctl -g");
    return out.replace(out.find("\n"), 1, "");
}

std::list<std::string> Window::getSupportedGPUModes()
{
    std::string out = executeCmd("supergfxctl -s");
    out = out.substr(1, out.size() - 3);
    std::list<std::string> modes;

    size_t pos = 0;
    std::string delimiter = ", ";
    while ((pos = out.find(delimiter)) != std::string::npos) {
        modes.push_back(out.substr(0, pos));
        out.erase(0, pos + delimiter.length());
    }
    modes.push_back(out);

    return modes;
}

void Window::setVisible(bool visible)
{
}

void Window::closeEvent(QCloseEvent *event)
{
}

Window::Window()
{
    createTrayIcon();

    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &Window::messageClicked);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &Window::iconActivated);

    trayIcon->show();
}

void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
}

void Window::messageClicked()
{
}

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);

    std::cout << "Supported modes: " << std::endl;
    for (auto const& mode : getSupportedGPUModes()) {
        const char* mode_display_c = appendStr(mode.c_str(), "");
        std::cout << mode_display_c << std::endl;

        if (strcmp(mode_display_c, "Asus") == 0) {
            mode_display_c += 4;
        }

        QAction *action = new QAction(tr(appendStr("&", mode_display_c)), this);
        connect(action, &QAction::triggered, this, [=](){ setGpuMode(mode.c_str()); });
        trayIconMenu->addAction(action);
    }

    trayIconMenu->addSeparator();

    QAction *quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    setTrayIcon();
}

void Window::setTrayIcon()
{
    const char* status = getGPUStatus().c_str();

    std::cout << "Enabled mode: " << status << std::endl;

    if(strcmp(status, GPU_STATUS_HYBRID) == 0) {
        trayIcon->setIcon(QIcon(":/images/gpu-hybrid.png"));
    } else if(strcmp(status, GPU_STATUS_VFIO) == 0) {
        trayIcon->setIcon(QIcon(":/images/gpu-vfio.png"));
    } else if(strcmp(status, GPU_STATUS_EGPU) == 0) {
        trayIcon->setIcon(QIcon(":/images/gpu-egpu.png"));
    } else {
        trayIcon->setIcon(QIcon(":/images/gpu-integrated.png"));
    }
}

void Window::setGpuMode(const char* mode)
{
    std::string out = executeCmd(appendStr("supergfxctl -m ", mode));
    std::cout << out.c_str();
    
    setTrayIcon();

    trayIcon->showMessage(
        tr("Mode changed successfully"),
        tr(out.c_str()),
        QSystemTrayIcon::Information,
        5000
    );

    if(out.find("Required") != std::string::npos) {
        QMessageBox::information(
            this, 
            tr("User action required"), 
            tr(out.c_str())
        );
    }
}
