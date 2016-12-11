#ifndef MENUPANEL_H
#define MENUPANEL_H

#include "uix.h"

class MenuPanel : public Panel
{
    Q_OBJECT
    public:
        explicit MenuPanel(QWidget *parent = 0);

    signals:

    public slots:
        QToolBar *addGroup(QString name);
        void addSpacer();
        void loadSettings();

        void expandOrCollapse();

    private slots:
        void genActs();
        void search();

        void searchItem(QString text);

    private:
        QSettings *settings;
        QVBoxLayout *layBoxes;

        QAction *actSearch;
        QAction *actCollapseExpandAll;

        bool isExtended;
};

#endif // MENUPANEL_H
