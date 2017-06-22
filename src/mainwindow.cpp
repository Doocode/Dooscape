#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mStacker(0)
{

    mSettings = new QSettings;
    if(mSettings->value("Version")<QCoreApplication::applicationVersion())
    {
        Introducer *intro = new Introducer;
        setCentralWidget(intro);

        connect(intro,SIGNAL(startConfig()),this,SLOT(startConfig()));
        connect(intro,SIGNAL(startBrowser()),this,SLOT(startBrowser()));
    }
    else
        startBrowser();

    QShortcut *keyFullScreen = new QShortcut(QKeySequence("F11"),this);
    connect(keyFullScreen,SIGNAL(activated()),this,SLOT(toggleFullScreen()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(mStacker)
    {
        if(mStacker->currentWidget()==mBrowserWindow)
        {
            if(mSkinManager->canClose() && !mBrowserWindow->isDownloading())
            {
                if(mSettings->value("Security/Lock/AtClose").toBool() && !mSettings->value("Account/Password").toString().isEmpty() && !isAuthenticated())
                {
                    event->ignore();
                    return;
                }

                mBrowserWindow->saveState();
                event->accept();
            }
            else if(mSkinManager->canClose() && mBrowserWindow->isDownloading())
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Downloading files - Downloads manager"));
                msgBox.setIcon(QMessageBox::Question);
                msgBox.setText(tr("<h3>Downloads manager</h3>Do you want to cancel downloads ?"));
                msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Yes);
                msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
                msgBox.setButtonText(QMessageBox::No,tr("No"));
                msgBox.setButtonText(QMessageBox::Cancel,tr("Cancel"));
                int value = msgBox.exec();

                if(value == QMessageBox::Yes)
                {
                    if(mSettings->value("Security/Lock/AtClose").toBool() && !mSettings->value("Account/Password").toString().isEmpty() && !isAuthenticated())
                    {
                        event->ignore();
                        return;
                    }

                    mBrowserWindow->saveState();
                    event->accept();
                }
                else
                    event->ignore();
            }
            else
                event->ignore();
        }
        else if(mScreenshotViewer->isVisible())
        {
            event->ignore();
            if(mScreenshotViewer->canClose())
                showBrowserWindow();
        }
        else if(mStacker->currentWidget()==mLockScreen)
        {
            event->ignore();
            QMessageBox::critical(mLockScreen,tr("Error"),tr("Please unlock this browser if you want to close this window."),QMessageBox::Ok);
        }
        else if(mStacker->currentWidget()==mConfigWindow)
        {
            event->ignore();
            mConfigWindow->goBack();
        }
    }
}

void MainWindow::startBrowser()
{
    mBrowserWindow = new BrowserWindow;
    mConfigWindow = new ConfigWindow;
    mSkinManager = new SkinManager;
    mScreenshotViewer = new ScreenshotsViewer;
    mLockScreen = new LockScreen;

    connect(mBrowserWindow,SIGNAL(quit()),this,SLOT(close()));
    connect(mBrowserWindow,SIGNAL(showSkinManager()),this,SLOT(showSkinManager()));
    connect(mBrowserWindow,SIGNAL(showConfig()),this,SLOT(showConfigWindow()));
    connect(mBrowserWindow,SIGNAL(showScreenshot()),this,SLOT(showScreenshotViewer()));
    connect(mBrowserWindow,SIGNAL(showLocker()),this,SLOT(showLockScreen()));
    connect(mBrowserWindow,SIGNAL(fullScreen()),this,SLOT(toggleFullScreen()));
    connect(mBrowserWindow,SIGNAL(setTitle(QString)),this,SLOT(updateTitleBar(QString)));

    connect(mLockScreen,SIGNAL(showBrowser()),this,SLOT(showBrowserWindow()));
    connect(mSkinManager,SIGNAL(showBrowser()),this,SLOT(showSkinManager()));
    connect(mConfigWindow,SIGNAL(showBrowser()),this,SLOT(showBrowserWindow()));
    connect(mScreenshotViewer,SIGNAL(showBrowser()),this,SLOT(showBrowserWindow()));

    connect(mConfigWindow,SIGNAL(showBrowser()),mBrowserWindow,SLOT(loadConfig()));
    connect(mConfigWindow,SIGNAL(showBrowser()),mBrowserWindow,SLOT(updateAll()));
    connect(mConfigWindow,SIGNAL(showBrowser()),mSkinManager,SLOT(loadConfig()));
    connect(mConfigWindow,SIGNAL(showBrowser()),mSkinManager,SLOT(loadSettings()));
    connect(mBrowserWindow,SIGNAL(setScreenshot(QPixmap,QString)),mScreenshotViewer,SLOT(setScreen(QPixmap,QString)));

    mStacker = new QStackedWidget;
    mStacker->addWidget(mBrowserWindow);
    mStacker->addWidget(mConfigWindow);
    mStacker->addWidget(mLockScreen);
    mStacker->addWidget(mScreenshotViewer);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(mStacker);
    splitter->addWidget(mSkinManager);
    mSkinManager->hide();

    setCentralWidget(splitter);
    setWindowIcon(QIcon(":/img/logo.png"));

    mSkinManager->loadConfig();

    // Bug here
    if(mSettings->value("Security/Lock/AtStart").toBool() && !mSettings->value("Account/Password").toString().isEmpty() && !isAuthenticated())
        qApp->quit();
    // End bug

    loadSettings();
}

void MainWindow::showSkinManager()
{
    if(mSkinManager->isHidden())
    {
        mSkinManager->show();
        mSkinManager->start();
    }
    else
        mSkinManager->hide();
}

void MainWindow::showBrowserWindow()
{
    mStacker->setCurrentWidget(mBrowserWindow);
    if(sender()==mSkinManager)
        mSkinManager->hide();
}

void MainWindow::showConfigWindow()
{
    if(sender()==mBrowserWindow && mSettings->value("Security/Lock/ToConfig").toBool() && !mSettings->value("Account/Password").toString().isEmpty() && !isAuthenticated())
        return;

    mStacker->setCurrentWidget(mConfigWindow);
    mConfigWindow->loadSettings();

    if(mSettings->value("Interface/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(mConfigWindow,"geometry");
        animation->setDuration(350);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setStartValue(QRect(0,100,mConfigWindow->geometry().width(),0));
        animation->setEndValue(QRect(0,0,mConfigWindow->geometry().width(),mConfigWindow->geometry().height()));

        animation->start();
    }
}

void MainWindow::showLockScreen()
{
    if(mSettings->value("Interface/Animations").toBool())
    {
        QPixmap pixmap(this->width(),this->height());
        QPainter painter(&pixmap);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        this->render(&painter);
        mLockScreen->setScreenBrowser(pixmap);
    }

    mStacker->setCurrentWidget(mLockScreen);
    mLockScreen->loadSettings();
}

void MainWindow::showScreenshotViewer()
{
    mStacker->setCurrentWidget(mScreenshotViewer);
    mScreenshotViewer->loadSettings();
}

void MainWindow::toggleFullScreen()
{
    if(this->isFullScreen())
        showMaximized();
    else
        showFullScreen();
}

void MainWindow::updateTitleBar(QString title)
{
    setWindowTitle(title);
}

void MainWindow::startConfig()
{
    startBrowser();
    mSkinManager->loadConfig();
    mBrowserWindow->loadConfig();
    mBrowserWindow->startup();
    showConfigWindow();
}

// isAuthenticated() ask the user to type his password and return the true if it's correct
bool MainWindow::isAuthenticated()
{
    QInputDialog dialog;
    dialog.setWindowTitle(tr("Enter password"));
    dialog.setLabelText(tr("Please enter your password<br><strong>Indication</strong> : %1").arg(mSettings->value("Account/Indice").toString()));
    dialog.setTextEchoMode(QLineEdit::Password);
    dialog.setOkButtonText(tr("Ok"));
    dialog.setCancelButtonText(tr("Cancel"));
    dialog.exec();

    QString password = ConfigWindow::hashPassword(dialog.textValue());
    if(password.isEmpty())
        return false;
    else if(password != mSettings->value("Account/Password").toString())
    {
        QMessageBox::critical(this,tr("Error"),tr("Incorrect password or empty field"),QMessageBox::Ok);
        return false;
    }

    return true;
}

void MainWindow::loadSettings()
{
    mSkinManager->loadConfig();

    showBrowserWindow();
    mBrowserWindow->loadConfig();
    mBrowserWindow->startup();
}
