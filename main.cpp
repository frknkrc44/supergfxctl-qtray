// Copyright (C) 2022-2024 Furkan Karcıoğlu <https://github.com/frknkrc44>
//
// This file is part of SuperGfxCtl-QTray project,
// and licensed under GNU Affero General Public License v3.
// See the GNU Affero General Public License for more details.
//
// All rights reserved. See COPYING, AUTHORS.
//

#include <thread>

#include <QApplication>
#include "window.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Window window;
    window.show();

    RefreshTask* task = new RefreshTask(&window);
    QThreadPool::globalInstance()->start(task);

    return app.exec();
}
