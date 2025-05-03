// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QScreen>
#include <QDebug>

#include "screenshot.h"



[[gnu::noinline]] auto plus2(auto a, auto b) {
    return a + b;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qDebug() << plus2(1, 2);
    qDebug() << plus2((double) 1, static_cast<double>(2));

    Screenshot screenshot;
    screenshot.move(screenshot.screen()->availableGeometry().topLeft() + QPoint(20, 20));
    screenshot.show();

    return app.exec();
}


