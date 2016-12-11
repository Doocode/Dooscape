#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include "addressbar.h"
#include "fieldeditor.h"
#include "panelview.h"
#include "webview.h"

class BrowserWindow : public QMainWindow
{
    Q_OBJECT
    public:
        explicit BrowserWindow(QWidget *parent = 0);
        ~BrowserWindow();
        WebView *currentView();
        WebWidget *currentWidget();
        WebView *openInNewTab();
        bool isDownloading();

    protected:
        void resizeEvent(QResizeEvent *event);

    signals:
        void showSkinManager();
        void showConfig();
        void showLocker();
        void showScreenshot();
        void setScreenshot(QPixmap screen,QString name);
        void fullScreen();
        void setTitle(QString title);
        void quit();

    public slots:
        void startup();
        void loadConfig();
        void updateAll();
        void saveState();

    private slots:
        void genUI();
        void genActs();
        void genMenu();
        void genToolBar();
        void genPanels();
        void genMenuPanel();
        void genHistory();

        void openFile();
        void saveFile();

        void newTab();
        void duplicateTab();
        void closeTab();
        void closeTab(int index);
        void restoreTab();
        void restoreTab(int index);

        void addToBookmarks();
        void addToReadingList();
        void goToUrl(QString url);
        void goToItem(); // Fo history

        void showPanel();
        void showCompleter();
        void showMenu();
        void showTabsList();
        void showFindText();

        void hideAllPanels();

        void zoomIn();
        void zoomOut();
        void resetZoom();

        void findPreviousWord();
        void findNextWord();

        void execNavig();
        void goToUrl();
        void openDefaultBrowser();
        void runJavaScriptCode();
        void takeScreenshot();
        void aboutWindow();
        void openOrganisationDomain();

        void updateActs();
        void loadStarted();
        void loadProgress(int value);
        void loadFinished();
        void urlChanged(QUrl url);
        void iconChanged();
        void titleChanged(QString title);

    private:
        QStackedWidget *tabs;
        QToolBar *mainToolBar;
        QToolBar *tabToolBar;
        QToolBar *barFindText;
        QTabBar *tabBar;
        QLineEdit *editFindText;
        QCheckBox *checkCaseSensitive;
        QSettings *settings;
        QToolButton *btnHidePanel;
        QToolButton *btnHideTabView;
        QVBoxLayout *centralLayout;
        QFrame *central;

        AddressBar *address;
        FieldEditor *cmdPane;
        PanelView *sidePane;
        TabBar *tabView;

        QString iconColor;

        QMenu *mainMenu;
        QMenu *menuFile;
        QMenu *menuNavig;
        QMenu *menuView;
        QMenu *menuAddTo;
        QMenu *menuTools;
        QMenu *menuCmd;

        QAction *actOpenFile;
        QAction *actSaveFile;

        QAction *actGoBack;
        QAction *actGoForward;
        QAction *actReload;
        QAction *actStop;
        QAction *actGoHome;

        QAction *actNewTab;
        QAction *actCloseTab;
        QAction *actDuplicateTab;
        QAction *actRestoreTab;

        QAction *actZoomIn;
        QAction *actZoomOut;
        QAction *actResetZoom;
        QAction *actFullScreen;

        QAction *actShowTabsList;
        QAction *actShowReadList;
        QAction *actShowBookmarks;
        QAction *actShowHistory;
        QAction *actShowDownloads;
        QAction *actShowMenu;

        QAction *actAddToBookmarks;
        QAction *actAddToReadingList;

        QAction *actSkinManager;
        QAction *actLock;
        QAction *actOpenUrlInOtherBrowser;

        QAction *actShowFindText;
        QAction *actHideFindText;
        QAction *actFindBack;
        QAction *actFindForward;

        QAction *actInvertColors;
        QAction *actTranslate;
        QAction *actScreen;

        QAction *actAbout;
        QAction *actConfig;
        QAction *actQuit;
};

#endif // BROWSERWINDOW_H
