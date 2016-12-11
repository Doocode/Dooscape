#ifndef MAINMENU_H
#define MAINMENU_H

#include <QtWidgets>
#include "uix.h"

class MainMenu : public QFrame
{
    Q_OBJECT
    public:
        explicit MainMenu(QWidget *parent = 0);
        ~MainMenu();

        QToolButton *btnLock;
        QToolButton *btnSkinEdit;
        QToolButton *btnSettings;
        QToolButton *btnAbout;
        QToolButton *btnQuit;

    signals:
        void showGeneral();
        void showTabs();
        void showReadingList();
        void showBookmarks();
        void showHistory();
        void showDownloads();

    public slots:
        void setSelectedButton(int index);
        void loadSettings();

    private slots:
        void genFileMenu();
        void genNavigMenu();
        void genViewMenu();
        void genToolsMenu();

        void updateSelectedButton();

    private:
        QAction *actGeneral;
        QAction *actTabs;
        QAction *actReadingList;
        QAction *actBookmarks;
        QAction *actHistory;
        QAction *actDownloads;

        QFrame *screenFile;
        QFrame *screenNavig;
        QFrame *screenView;
        QFrame *screenTools;

        QToolBar *barItem;
        QToolBar *toolBar;
};

#endif // MAINMENU_H
