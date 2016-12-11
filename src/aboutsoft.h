#ifndef ABOUTSOFT_H
#define ABOUTSOFT_H

#include "uix.h"

class AboutSoft : public Panel
{
    Q_OBJECT
    public:
        explicit AboutSoft(QWidget *parent = 0);

    signals:
        void openOrganisationDomain();

    public slots:
        void loadSettings();

    private:
        QSettings *settings;
};

#endif // ABOUTSOFT_H
