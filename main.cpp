// Copyright (C) 2022 Furkan Karcıoğlu <https://github.com/frknkrc44>
//
// This file is part of SuperGfxCtl-QTray project,
// and licensed under GNU Affero General Public License v3.
// See the GNU Affero General Public License for more details.
//
// All rights reserved. See COPYING, AUTHORS.
//

#include <QApplication>
#include "window.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Window window;
    window.show();

    return app.exec();
}