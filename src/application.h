#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtWidgets/QApplication>
#include "mainwindow.h"

class Application : public QApplication
{
    public:
        Application(int &argc, char **argv);

    public slots:
        MainWindow *newMainWindow();
};

#endif // APPLICATION_H
