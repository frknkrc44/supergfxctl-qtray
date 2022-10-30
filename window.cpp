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
    out = out.replace(out.find("\n"), 1, "").replace(out.find("["), 1, "").replace(out.find("]"), 1, "");
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

char asciitolower(char in) {
    if (in <= 'Z' && in >= 'A')
        return in - ('Z' - 'z');
    return in;
}

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);

    std::cout << "Supported modes: " << std::endl;
    for (auto const& i : getSupportedGPUModes()) {
        std::string mode(i.begin(), i.end());
        std::transform(mode.begin(), mode.end(), mode.begin(), asciitolower);

        std::cout << mode.c_str() << std::endl;

        if(strcmp(mode.c_str(), GPU_STATUS_INTEGRATED) == 0) {
            integratedAction = new QAction(tr("&Integrated"), this);
            connect(integratedAction, &QAction::triggered, this, &Window::setGpuModeIntegrated);
            trayIconMenu->addAction(integratedAction);
        }

        if(strcmp(mode.c_str(), GPU_STATUS_HYBRID) == 0) {
            hybridAction = new QAction(tr("&Hybrid"), this);
            connect(hybridAction, &QAction::triggered, this, &Window::setGpuModeHybrid);
            trayIconMenu->addAction(hybridAction);
        }

        if(strcmp(mode.c_str(), GPU_STATUS_COMPUTE) == 0) {
            computeAction = new QAction(tr("&Compute"), this);
            connect(computeAction, &QAction::triggered, this, &Window::setGpuModeCompute);
            trayIconMenu->addAction(computeAction);
        }

        if(strcmp(mode.c_str(), GPU_STATUS_VFIO) == 0) {
            vfioAction = new QAction(tr("&Vfio"), this);
            connect(vfioAction, &QAction::triggered, this, &Window::setGpuModeVfio);
            trayIconMenu->addAction(vfioAction);
        }

        if(strcmp(mode.c_str(), GPU_STATUS_EGPU) == 0) {
            egpuAction = new QAction(tr("&Egpu"), this);
            connect(egpuAction, &QAction::triggered, this, &Window::setGpuModeEgpu);
            trayIconMenu->addAction(egpuAction);
        }
    }

    trayIconMenu->addSeparator();

    quitAction = new QAction(tr("&Quit"), this);
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

    if(strcmp(status, GPU_STATUS_INTEGRATED) == 0) {
        trayIcon->setIcon(QIcon(":/images/gpu-integrated.png"));
    } else if(strcmp(status, GPU_STATUS_HYBRID) == 0) {
        trayIcon->setIcon(QIcon(":/images/gpu-hybrid.png"));
    } else if(strcmp(status, GPU_STATUS_COMPUTE) == 0) {
        trayIcon->setIcon(QIcon(":/images/gpu-compute.png"));
    } else if(strcmp(status, GPU_STATUS_VFIO) == 0) {
        trayIcon->setIcon(QIcon(":/images/gpu-vfio.png"));
    } else if(strcmp(status, GPU_STATUS_EGPU) == 0) {
        trayIcon->setIcon(QIcon(":/images/gpu-egpu.png"));
    }
}

void Window::setGpuModeIntegrated()
{
    setGpuMode(GPU_STATUS_INTEGRATED);
}

void Window::setGpuModeHybrid()
{
    setGpuMode(GPU_STATUS_HYBRID);
}

void Window::setGpuModeCompute()
{
    setGpuMode(GPU_STATUS_COMPUTE);
}

void Window::setGpuModeVfio()
{
    setGpuMode(GPU_STATUS_VFIO);
}

void Window::setGpuModeEgpu()
{
    setGpuMode(GPU_STATUS_EGPU);
}

void Window::setGpuMode(const char* mode)
{
    const char* cmd = "supergfxctl -m ";
    char* new_str;
    if((new_str = (char*) malloc(strlen(cmd)+strlen(mode)+1)) != NULL){
        new_str[0] = '\0';
        strcat(new_str, cmd);
        strcat(new_str, mode);
    }

    std::string out = executeCmd(new_str);
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
            tr(
                "Restart your computer or type this to the terminal: \n\n"
                "sudo systemctl restart display-manager"
            )
        );
    }
}