#include "application.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(res); // On s'assure que les fichiers QRC se chargent correctement

    // On lance l'application
    Application a(argc, argv);
    a.processEvents();
    a.newMainWindow();

    return a.exec();
}
