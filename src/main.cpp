#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(":/img/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();

    a.processEvents();

    a.setApplicationName("Dooscape");
    a.setApplicationVersion("2.0i");
    a.setOrganizationName("Doocode");
    a.setOrganizationDomain("http://doo.zz.vc/");

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QCoreApplication::applicationDirPath() + QDir::separator() + QString("dooscape_") + locale);
    QCoreApplication::installTranslator(&translator);

    MainWindow w;
    w.showMaximized();


    splash.finish(&w);

    return a.exec();
}
