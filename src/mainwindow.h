#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "browserwindow.h"
#include "configwindow.h"
#include "introducer.h"
#include "lockscreen.h"
#include "skinmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    protected:
        void closeEvent(QCloseEvent *event);

    private slots:
        void showSkinManager();
        void showBrowser();
        void showConfig();
        void showLocker();
        void showLoader();
        void showScreenshot();
        void loadSettings();
        void fullScreen();
        void startConfig();
        bool isAuthenticated();
        QFrame *genLoaderScreen();

        void updateTitleBar(QString title);

    private:
        QStackedWidget *central;
        SkinManager *skin;
        BrowserWindow *browser;
        ConfigWindow *config;
        LockScreen *locker;
        ScreenshotsViewer *screen;
        QSettings *settings;
};

#endif // MAINWINDOW_H
