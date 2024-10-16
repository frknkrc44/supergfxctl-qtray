// Copyright (C) 2022 Furkan Karcıoğlu <https://github.com/frknkrc44>
//
// This file is part of SuperGfxCtl-QTray project,
// and licensed under GNU Affero General Public License v3.
// See the GNU Affero General Public License for more details.
//
// All rights reserved. See COPYING, AUTHORS.
//

#ifndef __MY_WINDOW_H__
#define __MY_WINDOW_H__

#include <QSystemTrayIcon>

#include <QDialog>
#include <list>

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
QT_END_NAMESPACE

class Window : public QDialog
{
    Q_OBJECT

public:
    Window();

    void setVisible(bool visible) override;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();

private:
    void createTrayIcon();
    std::string executeCmd(const char* cmd);
    std::string getGPUStatus();
    std::list<std::string> getSupportedGPUModes();

    void setGpuModeIntegrated();
    void setGpuModeHybrid();
    void setGpuModeCompute();
    void setGpuModeVfio();
    void setGpuModeEgpu();
    void setGpuMode(const char* mode);
    void setTrayIcon();

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    const char* GPU_STATUS_INTEGRATED       = "Integrated";
    const char* GPU_STATUS_HYBRID           = "Hybrid";
    const char* GPU_STATUS_NVIDIA_NOMODESET = "NvidiaNoModeset";
    const char* GPU_STATUS_VFIO             = "Vfio";
    const char* GPU_STATUS_EGPU             = "AsusEgpu";
    const char* GPU_STATUS_DISCRETE         = "AsusMuxDgpu";
    const char* GPU_STATUS_NONE             = "Unknown";
};

#endif
