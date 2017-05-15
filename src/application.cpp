#include "application.h"

#define APP_NAME    tr("Dooscape") // Name of the application
#define APP_VERSION tr("2.0j") // Version of the application
#define ORG_NAME    tr("Doocode") // Name of the organization
#define ORG_DOMAIN  tr("http://doo.zz.vc/") // The domain of the organization

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    // Définition de quelques informations
    setApplicationName(APP_NAME);
    setApplicationVersion(APP_VERSION);
    setOrganizationName(ORG_NAME);
    setOrganizationDomain(ORG_DOMAIN);

    // Chargement de la traduction vers la langue du système
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QCoreApplication::applicationDirPath() + QDir::separator() + QString("lang_") + locale);
    QCoreApplication::installTranslator(&translator);
}

MainWindow *Application::newMainWindow()
{
    // Affiche le splash
    QPixmap pixmap(":/img/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();

    // Lance une nouvelle fenêtre du navigateur
    MainWindow *w = new MainWindow;
    w->showMaximized();

    // Ferme le splash
    splash.finish(w);
    return w;
}
