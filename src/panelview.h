#ifndef PANELVIEW_H
#define PANELVIEW_H

#include <QtWidgets/QtWidgets>
#include "menupanel.h"
#include "bookmarkspanel.h"
#include "downloadspanel.h"
#include "historypanel.h"
#include "readinglist.h"
#include "tabbar.h"
#include "aboutsoft.h"
#include "screenshots.h"

class ListMenu : public QFrame
{
    Q_OBJECT
    public:
        explicit ListMenu(QWidget *parent = 0);
        ~ListMenu();

        QAction *btnScreen;
        QAction *btnLock;
        QAction *btnSkinEdit;
        QAction *btnSettings;
        QAction *btnAbout;
        QAction *btnQuit;

    signals:
        void showGeneral();
        void showTabs();
        void showReadingList();
        void showBookmarks();
        void showHistory();
        void showDownloads();

    public slots:
        void loadSettings();

    private slots:
        void genFileMenu();
        void genNavigMenu();
        void genViewMenu();
        void genToolsMenu();

    private:
        QAction *actGeneral;
        QAction *actReadingList;
        QAction *actBookmarks;
        QAction *actHistory;
        QAction *actDownloads;

        QList<QAction*> listActs;
        QGridLayout *menuLayout;

        QFrame *screenFile;
        QFrame *screenNavig;
        QFrame *screenView;
        QFrame *screenTools;

        //QToolBar *barItem;
};

class PanelView : public QFrame
{
    Q_OBJECT
    public:
        explicit PanelView(QWidget *parent = 0);
        ~PanelView();

        ListMenu *listMenu;
        MenuPanel *tab1;
        ReadingList *tab2;
        BookmarksPanel *tab3;
        HistoryPanel *tab4;
        DownloadsPanel *tab5;
        AboutSoft *tab6;

    protected:
        void resizeEvent(QResizeEvent *event);

    signals:

    public slots:
        void showGeneral();
        void showReadingList();
        void showBookmarks();
        void showHistory();
        void showDownloads();
        void showAbout();

        void showMenu();

        void loadSettings();

        QString currentScreen();

    private:
        QStackedWidget *central;
};

#endif // PANELVIEW_H
