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
        void showLockScreen();
        void showSkinManager();
        void showConfigWindow();
        void showBrowserWindow();
        void showScreenshotViewer();

        void loadSettings();
        void toggleFullScreen();
        void updateTitleBar(QString title);

        bool isAuthenticated();

        void startBrowser();
        void startConfig();

    private:
        ScreenshotsViewer *mScreenshotViewer;
        BrowserWindow *mBrowserWindow;
        ConfigWindow *mConfigWindow;
        SkinManager *mSkinManager;
        LockScreen *mLockScreen;

        QSettings *mSettings;
        QStackedWidget *mStacker;
};

#endif // MAINWINDOW_H
