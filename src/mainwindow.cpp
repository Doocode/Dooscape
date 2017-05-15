#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Introducer *starter = new Introducer;
    browser = new BrowserWindow;
    config = new ConfigWindow;
    skin = new SkinManager;
    screen = new ScreenshotsViewer;
    locker = new LockScreen;

    connect(browser,SIGNAL(quit()),this,SLOT(close()));
    connect(browser,SIGNAL(showSkinManager()),this,SLOT(showSkinManager()));
    connect(browser,SIGNAL(showConfig()),this,SLOT(showConfig()));
    connect(browser,SIGNAL(showLocker()),this,SLOT(showLocker()));
    connect(browser,SIGNAL(showScreenshot()),this,SLOT(showScreenshot()));
    connect(browser,SIGNAL(fullScreen()),this,SLOT(fullScreen()));
    connect(browser,SIGNAL(setTitle(QString)),this,SLOT(updateTitleBar(QString)));

    connect(locker,SIGNAL(showBrowser()),this,SLOT(showBrowser()));
    connect(skin,SIGNAL(showBrowser()),this,SLOT(showSkinManager()));
    connect(config,SIGNAL(showBrowser()),this,SLOT(showBrowser()));
    connect(screen,SIGNAL(showBrowser()),this,SLOT(showBrowser()));
    connect(starter,SIGNAL(showConfig()),this,SLOT(startConfig()));

    connect(starter,SIGNAL(showLoader()),this,SLOT(showLoader()));

    connect(starter,SIGNAL(startBrowsing()),this,SLOT(loadSettings()));
    connect(config,SIGNAL(showBrowser()),browser,SLOT(loadConfig()));
    connect(config,SIGNAL(showBrowser()),browser,SLOT(updateAll()));
    connect(config,SIGNAL(showBrowser()),skin,SLOT(loadConfig()));
    connect(config,SIGNAL(showBrowser()),skin,SLOT(loadSettings()));
    connect(browser,SIGNAL(setScreenshot(QPixmap,QString)),screen,SLOT(setScreen(QPixmap,QString)));


    QShortcut *keyFullScreen = new QShortcut(QKeySequence("F11"),this);
    connect(keyFullScreen,SIGNAL(activated()),this,SLOT(fullScreen()));

    central = new QStackedWidget;
    central->addWidget(genLoaderScreen());
    central->addWidget(starter);
    central->addWidget(browser);
    central->addWidget(config);
    central->addWidget(locker);
    central->addWidget(screen);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(central);
    splitter->addWidget(skin);
    skin->hide();

    setCentralWidget(splitter);

    setWindowIcon(QIcon(":/img/logo.png"));

    settings = new QSettings;

    skin->loadConfig();
    if(settings->value("Security/Lock/AtStart").toBool() && !settings->value("Account/Password").toString().isEmpty() && !isAuthenticated())
        qApp->quit();

    loadSettings();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(central->currentWidget()==browser)
    {
        if(skin->canClose() && !browser->isDownloading())
        {
            if(settings->value("Security/Lock/AtClose").toBool() && !settings->value("Account/Password").toString().isEmpty() && !isAuthenticated())
            {
                event->ignore();
                return;
            }

            browser->saveState();
            event->accept();
        }
        else if(skin->canClose() && browser->isDownloading())
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
                if(settings->value("Security/Lock/AtClose").toBool() && !settings->value("Account/Password").toString().isEmpty() && !isAuthenticated())
                {
                    event->ignore();
                    return;
                }

                browser->saveState();
                event->accept();
            }
            else
                event->ignore();
        }
        else
            event->ignore();
    }
    else if(screen->isVisible())
    {
        event->ignore();
        if(screen->canClose())
            showBrowser();
    }
    else if(central->currentWidget()==locker)
    {
        event->ignore();
        QMessageBox::critical(locker,tr("Error"),tr("Please unlock this browser if you want to close this window."),QMessageBox::Ok);
    }
    else if(central->currentWidget()==config)
    {
        event->ignore();
        config->goBack();
    }
}

QFrame *MainWindow::genLoaderScreen()
{
    QProgressBar *progress = new QProgressBar;
    progress->setValue(0);
    progress->setRange(0,0);
    progress->setFixedWidth(300);
    progress->setAlignment(Qt::AlignCenter);

    QLabel *lblLogo = new QLabel;
    lblLogo->setPixmap(QPixmap(":img/logo.png"));
    lblLogo->setAlignment(Qt::AlignCenter);
    lblLogo->setFixedSize(200,200);
    lblLogo->setScaledContents(true);

    QLabel *lblText = new QLabel(tr("Loading"));
    lblText->setAlignment(Qt::AlignCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(50,0,50,100);
    mainLayout->setAlignment(Qt::AlignBottom);
    mainLayout->addWidget(lblLogo);
    mainLayout->addWidget(progress);
    mainLayout->addWidget(lblText);
    mainLayout->setAlignment(lblLogo,Qt::AlignHCenter);
    mainLayout->setAlignment(progress,Qt::AlignHCenter);
    mainLayout->setAlignment(lblText,Qt::AlignHCenter);

    QFrame *loaderScreen = new QFrame;
    loaderScreen->setStyleSheet("QFrame{background: rgb(60,60,60);} QLabel{color: white;font-size: 16px; font-family: arial, serif;} QProgressBar{border: none; background: rgba(0,0,0,.5); height: 5px; margin: 10px;} QProgressBar::chunk{border: none; background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 transparent, stop: .3 transparent, stop: .5 rgba(255,255,255,.7), stop: .7 transparent, stop: 1 transparent);}");
    loaderScreen->setLayout(mainLayout);

    return loaderScreen;
}

void MainWindow::showSkinManager()
{
    if(skin->isHidden())
    {
        skin->show();
        skin->start();
    }
    else
        skin->hide();
}

void MainWindow::showBrowser()
{
    central->setCurrentWidget(browser);
    if(sender()==skin)
        skin->hide();

    /*if(settings->value("Interface/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(browser,"geometry");
        animation->setDuration(450);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setStartValue(QRect(0,100,browser->geometry().width(),browser->geometry().height()));
        animation->setStartValue(QRect(100,0,browser->geometry().width(),browser->geometry().height()));
        animation->setEndValue(QRect(0,0,browser->geometry().width(),browser->geometry().height()));
        animation->start();
    }*/
}

void MainWindow::showConfig()
{
    if(sender()==browser && settings->value("Security/Lock/ToConfig").toBool() && !settings->value("Account/Password").toString().isEmpty() && !isAuthenticated())
        return;

    central->setCurrentWidget(config);
    config->loadSettings();

    if(settings->value("Interface/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(config,"geometry");
        animation->setDuration(350);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setStartValue(QRect(0,100,config->geometry().width(),0));
        animation->setEndValue(QRect(0,0,config->geometry().width(),config->geometry().height()));

        animation->start();
    }
}

void MainWindow::showLocker()
{
    QPixmap pixmap(this->width(),this->height());
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    this->render(&painter);
    locker->setScreenBrowser(pixmap);

    central->setCurrentWidget(locker);
    locker->loadSettings();
}

void MainWindow::showLoader()
{
    central->setCurrentIndex(0);
}

void MainWindow::showScreenshot()
{
    central->setCurrentWidget(screen);
    screen->loadSettings();
}

void MainWindow::fullScreen()
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
    skin->loadConfig();
    browser->loadConfig();
    browser->startup();
    showConfig();
}

// isAuthenticated() ask the user to type his password and return the true if it's correct
bool MainWindow::isAuthenticated()
{
    QInputDialog dialog;
    dialog.setWindowTitle(tr("Enter password"));
    dialog.setLabelText(tr("Please enter your password<br><strong>Indication</strong> : %1").arg(settings->value("Account/Indice").toString()));
    dialog.setTextEchoMode(QLineEdit::Password);
    dialog.setOkButtonText(tr("Ok"));
    dialog.setCancelButtonText(tr("Cancel"));
    dialog.exec();

    QString password = ConfigWindow::hashPassword(dialog.textValue());
    if(password.isEmpty())
        return false;
    else if(password != settings->value("Account/Password").toString())
    {
        QMessageBox::critical(this,tr("Error"),tr("Incorrect password or empty field"),QMessageBox::Ok);
        return false;
    }

    return true;
}

void MainWindow::loadSettings()
{
    if(settings->value("Version")<QCoreApplication::applicationVersion())
    {
        central->setCurrentIndex(1);
    }
    else
    {
        skin->loadConfig();

        showBrowser();
        browser->loadConfig();
        browser->startup();
    }
}
