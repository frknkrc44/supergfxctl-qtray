// Copyright (C) 2022-2024 Furkan Karcıoğlu <https://github.com/frknkrc44>
//
// This file is part of SuperGfxCtl-QTray project,
// and licensed under GNU Affero General Public License v3.
// See the GNU Affero General Public License for more details.
//
// All rights reserved. See COPYING, AUTHORS.
//

#ifndef __MY_WINDOW_H__
#define __MY_WINDOW_H__

#include <QDialog>
#include <QSystemTrayIcon>
#include <QThreadPool>

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
class QRunnable;
QT_END_NAMESPACE


class Window : public QDialog
{
    Q_OBJECT

public:
    Window();

    void setVisible(bool visible) override;
    void setTrayIcon();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();

private:
    void createTrayIcon();
    std::string executeCmd(const char* cmd);
    std::string getGPUStatus();
    std::string getGPUPower();
    std::list<std::string> getSupportedGPUModes();

    void setGpuMode(const char* mode);

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    const char* GPU_STATUS_INTEGRATED       = "Integrated";
    const char* GPU_STATUS_HYBRID           = "Hybrid";
    const char* GPU_STATUS_NVIDIA_NOMODESET = "NvidiaNoModeset";
    const char* GPU_STATUS_VFIO             = "Vfio";
    const char* GPU_STATUS_EGPU             = "AsusEgpu";
    const char* GPU_STATUS_DISCRETE         = "AsusMuxDgpu";
    const char* GPU_STATUS_NONE             = "Unknown";

    const char* POWER_STATUS_ACTIVE         = "active";
    const char* POWER_STATUS_SUSPENDED      = "suspended";
    const char* POWER_STATUS_OFF            = "off";
    const char* POWER_STATUS_DGPU_DISABLED  = "dgpu_disabled";
    const char* POWER_STATUS_MUX_DISCREET   = "asus_mux_discreet";
    const char* POWER_STATUS_UNKNOWN        = GPU_STATUS_NONE;
};

class RefreshTask : public QRunnable
{
public:
    RefreshTask(Window* window);
    void run() override;
    Window* usedWindow;

};

#endif
