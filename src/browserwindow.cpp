#include "browserwindow.h"

BrowserWindow::BrowserWindow(QWidget *parent) : QMainWindow(parent)
{
    genUI();
}

BrowserWindow::~BrowserWindow()
{

}

void BrowserWindow::genUI()
{
    settings = new QSettings;
    iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    tabs = new QStackedWidget(this);

    genActs();
    genMenu();
    genToolBar();

    connect(tabs,SIGNAL(currentChanged(int)),this,SLOT(updateAll()));
    connect(tabBar,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    connect(tabBar,SIGNAL(currentChanged(int)),tabs,SLOT(setCurrentIndex(int)));
    connect(tabs,SIGNAL(currentChanged(int)),tabBar,SLOT(setCurrentIndex(int)));

    QHBoxLayout *tabsLayout = new QHBoxLayout;
    tabsLayout->setMargin(0);
    tabsLayout->setSpacing(0);
    tabsLayout->addWidget(tabToolBar);
    tabsLayout->addWidget(tabs);

    centralLayout = new QVBoxLayout;
    centralLayout->setMargin(0);
    centralLayout->setSpacing(0);
    //centralLayout->addWidget(tabToolBar);
    centralLayout->addWidget(mainToolBar);
    centralLayout->addWidget(barFindText);
    //centralLayout->addWidget(tabs);
    centralLayout->addLayout(tabsLayout);

    central = new QFrame;
    central->setLayout(centralLayout);
    setCentralWidget(central);
    genPanels();

   /* loadConfig();
    startup();*/
}

void BrowserWindow::genActs()
{
    actOpenFile = new QAction(tr("Open file"),this);
    connect(actOpenFile,SIGNAL(triggered()),this,SLOT(openFile()));

    actSaveFile = new QAction(tr("Save file"),this);
    actSaveFile->setShortcut(QKeySequence::Save);
    actSaveFile->setVisible(false);
    connect(actSaveFile,SIGNAL(triggered()),this,SLOT(saveFile()));


    actShowTabsList = new QAction(tr("Tabs"),this);
    connect(actShowTabsList,SIGNAL(triggered()),this,SLOT(showTabsList()));
    connect(actShowTabsList,SIGNAL(hovered()),this,SLOT(showTabsList()));

    actShowReadList = new QAction(tr("Reading list"),this);
    connect(actShowReadList,SIGNAL(triggered()),this,SLOT(showPanel()));

    actShowBookmarks = new QAction(tr("Bookmarks"),this);
    connect(actShowBookmarks,SIGNAL(triggered()),this,SLOT(showPanel()));

    actShowHistory = new QAction(tr("History"),this);
    connect(actShowHistory,SIGNAL(triggered()),this,SLOT(showPanel()));

    actShowDownloads = new QAction(tr("Downloads"),this);
    connect(actShowDownloads,SIGNAL(triggered()),this,SLOT(showPanel()));

    actShowMenu = new QAction(tr("Main menu"),this);
   /* connect(actShowMenu,SIGNAL(triggered()),this,SLOT(showMenu()));
    connect(actShowMenu,SIGNAL(hovered()),this,SLOT(showPanel()));*/
    connect(actShowMenu,SIGNAL(triggered()),this,SLOT(showPanel()));


    actGoBack = new QAction(tr("Go back"),this);
    connect(actGoBack,SIGNAL(triggered()),this,SLOT(execNavig()));

    actGoForward = new QAction(tr("Go forward"),this);
    connect(actGoForward,SIGNAL(triggered()),this,SLOT(execNavig()));

    actReload = new QAction(tr("Reload"),this);
    actReload->setShortcut(QKeySequence::Refresh);
    connect(actReload,SIGNAL(triggered()),this,SLOT(execNavig()));

    actStop = new QAction(tr("Stop"),this);
    actStop->setShortcut(QKeySequence::Refresh);
    connect(actStop,SIGNAL(triggered()),this,SLOT(execNavig()));

    actGoHome = new QAction(tr("Go home"),this);
    connect(actGoHome,SIGNAL(triggered()),this,SLOT(execNavig()));


    actNewTab = new QAction(tr("New tab"), this);
    connect(actNewTab, SIGNAL(triggered()), this, SLOT(newTab()));

    actCloseTab = new QAction(tr("Close tab"), this);
    connect(actCloseTab, SIGNAL(triggered()), this, SLOT(closeTab()));

    actDuplicateTab = new QAction(tr("Duplicate tab"), this);
    connect(actDuplicateTab, SIGNAL(triggered()), this, SLOT(duplicateTab()));

    actRestoreTab = new QAction(tr("Restore closed tab"), this);
    connect(actRestoreTab, SIGNAL(triggered()), this, SLOT(restoreTab()));


    actZoomIn = new QAction(tr("Zoom In"), this);
    connect(actZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));

    actZoomOut = new QAction(tr("Zoom Out"), this);
    connect(actZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));

    actResetZoom = new QAction(tr("Reset Zoom"), this);
    connect(actResetZoom, SIGNAL(triggered()), this, SLOT(resetZoom()));

    actFullScreen = new QAction(tr("Full screen"), this);
    connect(actFullScreen, SIGNAL(triggered()), this, SIGNAL(fullScreen()));


    actAddToReadingList = new QAction(tr("Add to reading list"),this);
    connect(actAddToReadingList,SIGNAL(triggered()),this,SLOT(addToReadingList()));

    actAddToBookmarks = new QAction(tr("Add to bookmarks"),this);
    actAddToBookmarks->setEnabled(true);
    connect(actAddToBookmarks,SIGNAL(triggered()),this,SLOT(addToBookmarks()));


    actLock = new QAction(tr("Lock this browser"),this);
    connect(actLock,SIGNAL(triggered()),this,SIGNAL(showLocker()));

    actSkinManager = new QAction(tr("Skin editor"),this);
    connect(actSkinManager,SIGNAL(triggered()),this,SIGNAL(showSkinManager()));

    actOpenUrlInOtherBrowser = new QAction(tr("Open in a external browser"),this);
    connect(actOpenUrlInOtherBrowser,SIGNAL(triggered()),this,SLOT(openDefaultBrowser()));


    actShowFindText = new QAction(tr("Find text"),this);
    actShowFindText->setVisible(false);
    connect(actShowFindText,SIGNAL(triggered()),this,SLOT(showFindText()));

    actHideFindText = new QAction(tr("Close"),this);
    connect(actHideFindText,SIGNAL(triggered()),this,SLOT(showFindText()));

    actFindBack = new QAction(tr("Find the previous word"),this);
    connect(actFindBack,SIGNAL(triggered()),this,SLOT(findPreviousWord()));

    actFindForward = new QAction(tr("Find the next word"),this);
    connect(actFindForward,SIGNAL(triggered()),this,SLOT(findNextWord()));


    actInvertColors = new QAction(tr("Invert colors"),this);
    connect(actInvertColors,SIGNAL(triggered()),this,SLOT(runJavaScriptCode()));

    actTranslate = new QAction(tr("Translate"),this);
    connect(actTranslate,SIGNAL(triggered()),this,SLOT(runJavaScriptCode()));

    actScreen = new QAction(tr("Take a screenshot"),this);
    connect(actScreen,SIGNAL(triggered()),this,SLOT(takeScreenshot()));


    actConfig = new QAction(tr("Settings"),this);
    connect(actConfig,SIGNAL(triggered()),this,SIGNAL(showConfig()));

    actAbout = new QAction(tr("About ") + QCoreApplication::applicationName(),this);
    connect(actAbout,SIGNAL(triggered()),this,SLOT(aboutWindow()));

    actQuit = new QAction(tr("Quit"),this);
    connect(actQuit,SIGNAL(triggered()),this,SIGNAL(quit()));

    actGoBack->setVisible(false);
    actGoForward->setVisible(false);
    actReload->setVisible(false);
    actStop->setVisible(false);


    QShortcut *keyShowTabs = new QShortcut(QKeySequence("F4"),this);
    connect(keyShowTabs,SIGNAL(activated()),actShowTabsList,SLOT(trigger()));

    QShortcut *keyFieldBar = new QShortcut(QKeySequence("F6"),this);
    connect(keyFieldBar,SIGNAL(activated()),this,SLOT(showCompleter()));

    QShortcut *keyAddTab = new QShortcut(QKeySequence::AddTab,this);
    connect(keyAddTab,SIGNAL(activated()),this,SLOT(newTab()));

    QShortcut *keyCloseTab = new QShortcut(QKeySequence("Ctrl+W"),this);
    connect(keyCloseTab,SIGNAL(activated()),this,SLOT(closeTab()));

    QShortcut *keyRestoreTab = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T),this);
    connect(keyRestoreTab,SIGNAL(activated()),this,SLOT(restoreTab()));

    QShortcut *keyOpenFile = new QShortcut(QKeySequence::Open,this);
    connect(keyOpenFile,SIGNAL(activated()),actOpenFile,SLOT(trigger()));

    QShortcut *keyGoBack = new QShortcut(QKeySequence::Back,this);
    connect(keyGoBack,SIGNAL(activated()),actGoBack,SLOT(trigger()));

    QShortcut *keyGoForward = new QShortcut(QKeySequence::Forward,this);
    connect(keyGoForward,SIGNAL(activated()),actGoForward,SLOT(trigger()));

    QShortcut *keyZoomIn = new QShortcut(QKeySequence::ZoomIn,this);
    connect(keyZoomIn,SIGNAL(activated()),actZoomIn,SLOT(trigger()));

    QShortcut *keyZoomOut = new QShortcut(QKeySequence::ZoomOut,this);
    connect(keyZoomOut,SIGNAL(activated()),actZoomOut,SLOT(trigger()));

    QShortcut *keyResetZoom = new QShortcut(QKeySequence("Ctrl+0"),this);
    connect(keyResetZoom,SIGNAL(activated()),actResetZoom,SLOT(trigger()));

    QShortcut *keyLock = new QShortcut(QKeySequence("Ctrl+L"),this);
    connect(keyLock,SIGNAL(activated()),actLock,SLOT(trigger()));

    QShortcut *keyFindText = new QShortcut(QKeySequence("Ctrl+F"),this);
    keyFindText->setEnabled(actShowFindText->isVisible());
    connect(keyFindText,SIGNAL(activated()),actShowFindText,SLOT(trigger()));

    QShortcut *keyAbout = new QShortcut(QKeySequence("F1"),this);
    connect(keyAbout,SIGNAL(activated()),actAbout,SLOT(trigger()));

    QShortcut *keyQuit = new QShortcut(QKeySequence::Quit,this);
    connect(keyQuit,SIGNAL(activated()),actQuit,SLOT(trigger()));
}

void BrowserWindow::genMenu()
{
    mainMenu = new QMenu(tr("Main menu"));

        menuFile = mainMenu->addMenu(tr("File"));
        menuFile->addAction(actOpenFile);
        menuFile->addAction(actSaveFile);

        menuNavig = mainMenu->addMenu(tr("Navigation"));
        menuNavig->addAction(actGoBack);
        menuNavig->addAction(actGoForward);
        menuNavig->addAction(actReload);
        menuNavig->addAction(actStop);
        menuNavig->addAction(actGoHome);
        menuNavig->addSeparator();
        menuNavig->addAction(actOpenUrlInOtherBrowser);

        menuView = mainMenu->addMenu(tr("View"));
        menuView->addAction(actZoomIn);
        menuView->addAction(actZoomOut);
        menuView->addAction(actResetZoom);
        menuView->addSeparator();
        menuView->addAction(actFullScreen);

        menuTools = mainMenu->addMenu(tr("Tools"));
        menuTools->addAction(actSkinManager);
        menuTools->addAction(actLock);
        menuTools->addSeparator();
        menuTools->addAction(actScreen);
        menuTools->addAction(actShowFindText);
        menuTools->addAction(actTranslate);
        menuTools->addAction(actInvertColors);

    mainMenu->addSeparator();
    mainMenu->addAction(actShowReadList);
    mainMenu->addAction(actShowBookmarks);
    mainMenu->addAction(actShowHistory);
    mainMenu->addAction(actShowDownloads);
    mainMenu->addSeparator();
    mainMenu->addAction(actConfig);
    mainMenu->addAction(actAbout);
    mainMenu->addSeparator();
    mainMenu->addAction(actQuit);


    menuCmd = new QMenu;
   // menuCmd->addAction("Get the url of this page");
    menuAddTo = menuCmd->addMenu(tr("Add this page to"));
    menuAddTo->addAction(actAddToReadingList);
    menuAddTo->addAction(actAddToBookmarks);
}

void BrowserWindow::genToolBar()
{
    // Tabbar
    tabBar = new QTabBar;
    tabBar->setShape(QTabBar::RoundedWest);
    tabBar->setElideMode(Qt::ElideRight);
    //tabBar->setDocumentMode(false);

    tabToolBar = new QToolBar;
    tabToolBar->setOrientation(Qt::Vertical);
    tabToolBar->setObjectName("TabsToolBar");
    tabToolBar->addWidget(tabBar);

    tabToolBar->addAction(actNewTab);
    //tabToolBar->addAction(actRestoreTab);

    //Main toolbar
    mainToolBar = new QToolBar;

    //Find text
    barFindText = new QToolBar;
    barFindText->setObjectName("FindText");
    barFindText->hide();

    editFindText = new QLineEdit;
    editFindText->setPlaceholderText(tr("Type here your text"));
    barFindText->addWidget(editFindText);

    barFindText->addAction(actFindBack);
    barFindText->addAction(actFindForward);

    checkCaseSensitive = new QCheckBox(tr("Case sensitive"));
    barFindText->addWidget(checkCaseSensitive);

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    barFindText->addWidget(spacer);

    barFindText->addAction(actHideFindText);
}

void BrowserWindow::genPanels()
{
    QAction *actHide = new QAction(this);
    connect(actHide,SIGNAL(triggered()),this,SLOT(hideAllPanels()));
   // connect(actHide,SIGNAL(hovered()),this,SLOT(hideAllPanels()));

    QAction *actHide2 = new QAction(this);
    connect(actHide2,SIGNAL(triggered()),this,SLOT(showTabsList()));
    connect(actHide2,SIGNAL(hovered()),this,SLOT(showTabsList()));

    btnHidePanel = new QToolButton(central);
    btnHidePanel->setObjectName("TranslucidArea");
    btnHidePanel->move(0,0);
    btnHidePanel->hide();
    btnHidePanel->setDefaultAction(actHide);

    btnHideTabView = new QToolButton(central);
    btnHideTabView->setObjectName("TranslucidArea");
    btnHideTabView->move(0,0);
    btnHideTabView->hide();
    btnHideTabView->setDefaultAction(actHide2);

    cmdPane = new FieldEditor(central);
    cmdPane->hide();
    //cmdPane->btnIcon->setMenu(menuCmd);
    connect(cmdPane,SIGNAL(execute()),this,SLOT(goToUrl()));
    connect(cmdPane,SIGNAL(goToUrl(QString)),this,SLOT(goToUrl(QString)));
    connect(cmdPane,SIGNAL(execute()),this,SLOT(showCompleter()));
    connect(cmdPane,SIGNAL(showBrowser()),this,SLOT(showCompleter()));
    connect(cmdPane,SIGNAL(goToUrl(QString)),this,SLOT(showCompleter()));

    sidePane = new PanelView(central);
    sidePane->hide();

    genMenuPanel();

    connect(sidePane->listMenu->btnScreen,SIGNAL(triggered()),actScreen,SLOT(trigger()));
    connect(sidePane->listMenu->btnLock,SIGNAL(triggered()),actLock,SLOT(trigger()));
    connect(sidePane->listMenu->btnSkinEdit,SIGNAL(triggered()),actSkinManager,SLOT(trigger()));
    connect(sidePane->listMenu->btnSettings,SIGNAL(triggered()),actConfig,SLOT(trigger()));
    connect(sidePane->listMenu->btnAbout,SIGNAL(triggered()),actAbout,SLOT(trigger()));
    connect(sidePane->listMenu->btnQuit,SIGNAL(triggered()),actQuit,SLOT(trigger()));

    connect(sidePane->tab2,SIGNAL(exec(QString)),this,SLOT(goToUrl(QString)));
    connect(sidePane->tab2,SIGNAL(addToList()),this,SLOT(addToReadingList()));

    connect(sidePane->tab3,SIGNAL(addToBookmarks()),this,SLOT(addToBookmarks()));
    connect(sidePane->tab3,SIGNAL(exec(QString)),this,SLOT(goToUrl(QString)));

    connect(sidePane->tab5,SIGNAL(showDownloads()),this,SLOT(showPanel()));

    connect(sidePane->tab6,SIGNAL(openOrganisationDomain()),this,SLOT(openOrganisationDomain()));

    tabView = new TabBar(central);
    tabView->hide();
    tabView->lblTitle->setFixedWidth(0);
    tabView->actBack->setVisible(false);
    tabView->setGeometry(-305,0,300,central->geometry().height());

    connect(tabView,SIGNAL(addTab()),this,SLOT(newTab()));
    connect(tabView,SIGNAL(duplicateTab()),this,SLOT(duplicateTab()));
    connect(tabView,SIGNAL(openTabs(int)),tabs,SLOT(setCurrentIndex(int)));
    connect(tabView,SIGNAL(openTabs(int)),this,SLOT(hideAllPanels()));
    connect(tabView,SIGNAL(closeTabs(int)),this,SLOT(closeTab(int)));
    connect(tabView,SIGNAL(restoreTab(int)),this,SLOT(restoreTab(int)));

    connect(tabView,SIGNAL(addTab()),this,SLOT(showTabsList()));
    connect(tabView,SIGNAL(duplicateTab()),this,SLOT(showTabsList()));
    connect(tabView,SIGNAL(openTabs(int)),this,SLOT(showTabsList()));
    connect(tabView,SIGNAL(closeTabs(int)),this,SLOT(showTabsList()));
    connect(tabView,SIGNAL(restoreTab(int)),this,SLOT(showTabsList()));
}

void BrowserWindow::genMenuPanel()
{
    QToolBar *grpFile = sidePane->tab1->addGroup(tr("File"));
    grpFile->addAction(actOpenFile);
    grpFile->addAction(actSaveFile);
    grpFile->addSeparator();
    grpFile->addAction(actFullScreen);
    grpFile->addAction(actQuit);
    grpFile->addSeparator();

    QToolBar *grpNavig = sidePane->tab1->addGroup(tr("Navigation"));
    grpNavig->addAction(actGoBack);
    grpNavig->addAction(actGoForward);
    grpNavig->addAction(actReload);
    grpNavig->addAction(actStop);
    grpNavig->addAction(actGoHome);
    grpNavig->addAction(actOpenUrlInOtherBrowser);
    grpNavig->addSeparator();

    QToolBar *grpTabs = sidePane->tab1->addGroup(tr("Tabs"));
    grpTabs->addAction(actNewTab);
    grpTabs->addAction(actCloseTab);
    grpTabs->addAction(actDuplicateTab);
    grpTabs->addAction(actRestoreTab);
    grpTabs->addSeparator();

    QToolBar *grpZoom = sidePane->tab1->addGroup(tr("Zoom"));
    grpZoom->addAction(actZoomIn);
    grpZoom->addAction(actZoomOut);
    grpZoom->addAction(actResetZoom);
    grpZoom->addSeparator();

    QToolBar *grpActJS = sidePane->tab1->addGroup(tr("Actions on the page"));
    grpActJS->addAction(actScreen);
    grpActJS->addAction(actShowFindText);
    grpActJS->addAction(actTranslate);
    grpActJS->addAction(actInvertColors);
    grpActJS->addSeparator();

    QToolBar *grpSideMenu = sidePane->tab1->addGroup(tr("Side menu"));
    grpSideMenu->addAction(actShowReadList);
    grpSideMenu->addAction(actShowBookmarks);
    grpSideMenu->addAction(actShowHistory);
    grpSideMenu->addAction(actShowDownloads);
    grpSideMenu->addSeparator();

    QToolBar *grpOthers = sidePane->tab1->addGroup(tr("This browser"));
    grpOthers->addAction(actLock);
    grpOthers->addAction(actSkinManager);
    grpOthers->addAction(actConfig);
    grpOthers->addSeparator();
    grpOthers->addAction(actAbout);
    grpOthers->addAction(actQuit);
    grpOthers->addSeparator();

    sidePane->tab1->addSpacer();
    sidePane->tab1->setContextMenuPolicy(Qt::CustomContextMenu);
    sidePane->tab1->expandOrCollapse();
    connect(sidePane->tab1,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showMenu()));
}

void BrowserWindow::resizeEvent(QResizeEvent *event)
{
    // BtnHide
    btnHidePanel->resize(central->size().width(),central->size().height());
    btnHideTabView->resize(central->size().width(),central->size().height());

    // SidePane
    if(sidePane->isVisible())
        sidePane->setGeometry(central->geometry().width()-sidePane->geometry().width(),0,sidePane->geometry().width(),central->geometry().height());
    else
        sidePane->setGeometry(central->geometry().width()+30,0,sidePane->geometry().width(),central->geometry().height());

    // CmdPanel
    if(cmdPane->isVisible())
        cmdPane->setGeometry(0,0,central->size().width(),central->size().height());
    else
        cmdPane->setGeometry(0,-central->geometry().height(),central->size().width(),central->size().height());

    // TabView
    if(tabView->isVisible())
        tabView->setGeometry(0,0,300,central->geometry().height());
    else
        tabView->setGeometry(-305,0,300,central->geometry().height());

    event->accept();
}

// Slots de fonctions

WebView *BrowserWindow::currentView()
{
    return qobject_cast<WebWidget *>(tabs->currentWidget())->view();
}

WebWidget *BrowserWindow::currentWidget()
{
    return qobject_cast<WebWidget *>(tabs->currentWidget());
}

void BrowserWindow::newTab()
{
    WebWidget *webView = new WebWidget;

    connect(webView->view(),SIGNAL(loadStarted()),this,SLOT(loadStarted()));
    connect(webView->view(),SIGNAL(loadProgress(int)),this,SLOT(loadProgress(int)));
    connect(webView->view(),SIGNAL(loadFinished(bool)),this,SLOT(loadFinished()));
    connect(webView->view(),SIGNAL(urlChanged(QUrl)),this,SLOT(urlChanged(QUrl)));
    connect(webView->view(),SIGNAL(iconChanged()),this,SLOT(iconChanged()));
    connect(webView->view(),SIGNAL(titleChanged(QString)),this,SLOT(titleChanged(QString)));

    int index = tabs->addWidget(webView);
    tabs->setCurrentIndex(index);
    tabBar->addTab(QIcon(iconColor+"empty.png"),tr("New tab"));

    if(settings->value("Navigation/NewTab/Home").toBool())
        currentView()->load(QUrl(settings->value("Navigation/Home").toString()));
    else if(settings->value("Navigation/NewTab/Specific").toBool())
        currentView()->load(QUrl(settings->value("Navigation/NewTab/SpecificPage").toString()));

    if(tabView->isVisible())
        actShowTabsList->trigger();


    if(settings->value("Interface/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(webView,"geometry");
        animation->setDuration(500);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setStartValue(QRect(0,0,tabs->geometry().width(),0));
        animation->setEndValue(QRect(0,0,tabs->geometry().width(),tabs->geometry().height()));
        animation->start();
    }
}

WebView *BrowserWindow::openInNewTab()
{
    WebWidget *webView = new WebWidget(this);

    connect(webView->view(),SIGNAL(loadStarted()),this,SLOT(loadStarted()));
    connect(webView->view(),SIGNAL(loadProgress(int)),this,SLOT(loadProgress(int)));
    connect(webView->view(),SIGNAL(loadFinished(bool)),this,SLOT(loadFinished()));
    connect(webView->view(),SIGNAL(urlChanged(QUrl)),this,SLOT(urlChanged(QUrl)));
    connect(webView->view(),SIGNAL(iconChanged()),this,SLOT(iconChanged()));
    connect(webView->view(),SIGNAL(titleChanged(QString)),this,SLOT(titleChanged(QString)));

    /*int index = */tabs->addWidget(webView);
    tabBar->addTab(QIcon(iconColor+"empty.png"),tr("Loading"));
    //tabs->setCurrentIndex(index);
    QToolTip::showText(QCursor::pos(),tr("<p style='font-size: 19px; padding: 15px;'>A new tab is opened</p>"));

    if(tabView->isVisible())
        actShowTabsList->trigger();

    return webView->view();
}

void BrowserWindow::duplicateTab()
{
    QString url = currentView()->url().toString();
    WebWidget *webView = new WebWidget;

    connect(webView->view(),SIGNAL(loadStarted()),this,SLOT(loadStarted()));
    connect(webView->view(),SIGNAL(loadProgress(int)),this,SLOT(loadProgress(int)));
    connect(webView->view(),SIGNAL(loadFinished(bool)),this,SLOT(loadFinished()));
    connect(webView->view(),SIGNAL(urlChanged(QUrl)),this,SLOT(urlChanged(QUrl)));
    connect(webView->view(),SIGNAL(iconChanged()),this,SLOT(iconChanged()));
    connect(webView->view(),SIGNAL(titleChanged(QString)),this,SLOT(titleChanged(QString)));

    int index = tabs->addWidget(webView);
    tabBar->addTab(QIcon(iconColor+"empty.png"),tr("Duplicate tab"));
    tabs->setCurrentIndex(index);

    webView->view()->load(url);

    if(tabView->isVisible())
        actShowTabsList->trigger();
}

void BrowserWindow::closeTab()
{
    settings->beginGroup("ClosedTabs");
    QString folder = QString::number(settings->childGroups().count());
    settings->setValue(folder+"/Icon",currentView()->icon());
    settings->setValue(folder+"/Title",currentView()->title());
    settings->setValue(folder+"/Url",currentView()->url());
    settings->setValue(folder+"/Screen",currentWidget()->screenShot());
    settings->endGroup();

    if (tabs->count() > 1)
    {
        tabBar->removeTab(tabs->currentIndex());
        delete currentWidget();
        if(tabView->isVisible())
            tabView->setTabWidget(tabs);

        if(settings->value("Interface/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(currentView(),"geometry");
            animation->setDuration(500);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            animation->setStartValue(QRect(0,100,tabs->geometry().width(),tabs->geometry().height()));
            animation->setEndValue(QRect(0,0,tabs->geometry().width(),tabs->geometry().height()));
            animation->start();
        }
    }
    else
        emit quit();
}

void BrowserWindow::closeTab(int index)
{
    settings->beginGroup("ClosedTabs");
    QString folder = QString::number(settings->childGroups().count());
    settings->setValue(folder+"/Icon",qobject_cast<WebWidget *>(tabs->widget(index))->view()->icon());
    settings->setValue(folder+"/Title",qobject_cast<WebWidget *>(tabs->widget(index))->view()->title());
    settings->setValue(folder+"/Url",qobject_cast<WebWidget *>(tabs->widget(index))->view()->url());
    settings->setValue(folder+"/Screen",qobject_cast<WebWidget *>(tabs->widget(index))->screenShot());
    settings->endGroup();

    if (tabs->count() > 1)
    {
        tabBar->removeTab(index);
        delete tabs->widget(index);
        if(tabView->isVisible())
            tabView->setTabWidget(tabs);

        if(settings->value("Interface/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(currentView(),"geometry");
            animation->setDuration(500);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            animation->setStartValue(QRect(0,100,tabs->geometry().width(),tabs->geometry().height()));
            animation->setEndValue(QRect(0,0,tabs->geometry().width(),tabs->geometry().height()));
            animation->start();
        }
    }
    else
        emit quit();
}

void BrowserWindow::restoreTab()
{
    settings->beginGroup("ClosedTabs");
    if(settings->childGroups().count()==0)
    {
        settings->endGroup();
        return;
    }
    QString folder = QString::number(settings->childGroups().count()-1);
    QIcon icon = settings->value(folder+"/Icon").value<QPixmap>();
    QString title = settings->value(folder+"/Title").toString();
    QString url = settings->value(folder+"/Url").toString();
    settings->remove(folder);
    settings->endGroup();

    WebWidget *webView = new WebWidget;
    connect(webView->view(),SIGNAL(loadStarted()),this,SLOT(loadStarted()));
    connect(webView->view(),SIGNAL(loadProgress(int)),this,SLOT(loadProgress(int)));
    connect(webView->view(),SIGNAL(loadFinished(bool)),this,SLOT(loadFinished()));
    connect(webView->view(),SIGNAL(urlChanged(QUrl)),this,SLOT(urlChanged(QUrl)));
    connect(webView->view(),SIGNAL(iconChanged()),this,SLOT(iconChanged()));
    connect(webView->view(),SIGNAL(titleChanged(QString)),this,SLOT(titleChanged(QString)));

    int index = tabs->addWidget(webView);
    tabs->setCurrentIndex(index);
    tabBar->addTab(QIcon(icon),title);

    webView->view()->load(QUrl(url));

    if(settings->value("Interface/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(webView,"geometry");
        animation->setDuration(500);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setStartValue(QRect(0,0,tabs->geometry().width(),0));
        animation->setEndValue(QRect(0,0,tabs->geometry().width(),tabs->geometry().height()));
        animation->start();
    }
}

void BrowserWindow::restoreTab(int index)
{
    settings->beginGroup("ClosedTabs");
    if(settings->childGroups().count()==0)
    {
        settings->endGroup();
        return;
    }
    QString folder = QString::number(index);
    QIcon icon = settings->value(folder+"/Icon").value<QPixmap>();
    QString title = settings->value(folder+"/Title").toString();
    QString url = settings->value(folder+"/Url").toString();
    settings->remove(folder);
    settings->endGroup();

    WebWidget *webView = new WebWidget;
    connect(webView->view(),SIGNAL(loadStarted()),this,SLOT(loadStarted()));
    connect(webView->view(),SIGNAL(loadProgress(int)),this,SLOT(loadProgress(int)));
    connect(webView->view(),SIGNAL(loadFinished(bool)),this,SLOT(loadFinished()));
    connect(webView->view(),SIGNAL(urlChanged(QUrl)),this,SLOT(urlChanged(QUrl)));
    connect(webView->view(),SIGNAL(iconChanged()),this,SLOT(iconChanged()));
    connect(webView->view(),SIGNAL(titleChanged(QString)),this,SLOT(titleChanged(QString)));

    int currentindex = tabs->addWidget(webView);
    tabs->setCurrentIndex(currentindex);
    tabBar->addTab(QIcon(icon),title);

    webView->view()->load(QUrl(url));

    if(settings->value("Interface/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(webView,"geometry");
        animation->setDuration(500);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setStartValue(QRect(0,0,tabs->geometry().width(),0));
        animation->setEndValue(QRect(0,0,tabs->geometry().width(),tabs->geometry().height()));
        animation->start();
    }
}

void BrowserWindow::showCompleter()
{
    if(cmdPane->isVisible())
    {
        if(settings->value("Interface/Animations").toBool())
        {
            if(settings->value("Interface/Position/Toolbar").toString()=="top")
            {
                QPropertyAnimation *animation = new QPropertyAnimation(cmdPane,"geometry");
                animation->setDuration(250);
                animation->setEasingCurve(QEasingCurve::OutCubic);
                animation->setStartValue(cmdPane->geometry());
                animation->setEndValue(QRect(0,-central->geometry().height(),central->geometry().width(),central->geometry().height()));
                animation->start();
                connect(animation,SIGNAL(finished()),cmdPane,SLOT(hide()));
            }
            else if(settings->value("Interface/Position/Toolbar").toString()=="bottom")
            {
                QPropertyAnimation *animation = new QPropertyAnimation(cmdPane,"geometry");
                animation->setDuration(250);
                animation->setEasingCurve(QEasingCurve::OutCubic);
                animation->setStartValue(cmdPane->geometry());
                animation->setEndValue(QRect(0,central->geometry().height(),central->geometry().width(),central->geometry().height()));
                animation->start();
                connect(animation,SIGNAL(finished()),cmdPane,SLOT(hide()));
            }
        }
        else
        {
            if(settings->value("Interface/Position/Toolbar").toString()=="top")
                cmdPane->setGeometry(0,-central->geometry().height(),central->geometry().width(),central->geometry().height());
            if(settings->value("Interface/Position/Toolbar").toString()=="bottom")
                cmdPane->setGeometry(0,central->geometry().height(),central->geometry().width(),central->geometry().height());
            cmdPane->hide();
        }
    }
    else if(cmdPane->isHidden())
    {
        cmdPane->show();
        cmdPane->loadSettings();
        cmdPane->setHistoryItems(currentView()->history()->items());
        cmdPane->titleBar->setText(currentView()->url().toString());
        cmdPane->btnIcon->setIcon(currentView()->icon());
        if(currentView()->icon().isNull())
            cmdPane->btnIcon->setIcon(QIcon(iconColor+"empty.png"));

        if(settings->value("Interface/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(cmdPane,"geometry");
            animation->setDuration(450);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            animation->setStartValue(cmdPane->geometry());
            animation->setStartValue(QRect(address->geometry().x(),cmdPane->geometry().y()+200,0,central->geometry().height()));
            animation->setKeyValueAt(.7,QRect(address->geometry().x(),0,address->geometry().width(),central->geometry().height()));
            animation->setEndValue(QRect(0,0,central->geometry().width(),central->geometry().height()));
            animation->start();
        }
        else
            cmdPane->setGeometry(0,0,central->geometry().width(),central->geometry().height());

        if(sidePane->isVisible())
        {
            if(settings->value("Interface/Animations").toBool())
            {
                if(settings->value("Interface/Position/Toolbar").toString()=="top")
                {
                    QPropertyAnimation *animation = new QPropertyAnimation(sidePane,"geometry");
                    animation->setDuration(250);
                    animation->setEasingCurve(QEasingCurve::OutCubic);
                    animation->setStartValue(sidePane->geometry());
                    animation->setEndValue(QRect(tabs->geometry().width()-sidePane->geometry().width(),-sidePane->geometry().height()-30,sidePane->geometry().width(),500));
                    animation->start();
                    connect(animation,SIGNAL(finished()),sidePane,SLOT(hide()));
                }
                else if(settings->value("Interface/Position/Toolbar").toString()=="bottom")
                {
                    QPropertyAnimation *animation = new QPropertyAnimation(sidePane,"geometry");
                    animation->setDuration(250);
                    animation->setEasingCurve(QEasingCurve::OutCubic);
                    animation->setStartValue(sidePane->geometry());
                    animation->setEndValue(QRect(tabs->geometry().width()-sidePane->geometry().width(),tabs->geometry().height()+30,sidePane->geometry().width(),500));
                    animation->start();
                    connect(animation,SIGNAL(finished()),sidePane,SLOT(hide()));
                }
            }
            else
            {
                if(settings->value("Interface/Position/Toolbar").toString()=="top")
                    sidePane->setGeometry(tabs->geometry().width()-sidePane->geometry().width(),-sidePane->geometry().height()-30,sidePane->geometry().width(),500);
                else if(settings->value("Interface/Position/Toolbar").toString()=="bottom")
                    sidePane->setGeometry(tabs->geometry().width()-sidePane->geometry().width(),tabs->geometry().height()+30,sidePane->geometry().width(),500);
                sidePane->hide();
            }
        }

        cmdPane->setFocus();
        cmdPane->titleBar->setFocus();
        cmdPane->titleBar->selectAll();
    }
}

void BrowserWindow::showPanel()
{
    // BtnHide
    btnHidePanel->resize(central->size().width(),central->size().height());

    if(sidePane->isVisible())
    {
        if((sender()==actShowMenu && sidePane->currentScreen()=="home") || (sender()==actShowReadList && sidePane->currentScreen()=="readlist") || (sender()==actShowBookmarks && sidePane->currentScreen()=="bookmarks") || (sender()==actShowHistory && sidePane->currentScreen()=="history") || (sender()==actShowDownloads && sidePane->currentScreen()=="downloads"))
        {
            if(settings->value("Interface/Animations").toBool())
            {
                QPropertyAnimation *animation = new QPropertyAnimation(sidePane,"geometry");
                animation->setDuration(250);
                animation->setEasingCurve(QEasingCurve::OutCubic);
                animation->setStartValue(sidePane->geometry());
                animation->setEndValue(QRect(central->geometry().width()+30,0,sidePane->geometry().width(),central->geometry().height()));
                animation->start();
                connect(animation,SIGNAL(finished()),sidePane,SLOT(hide()));
            }
            else
            {
                sidePane->setGeometry(central->geometry().width()+30,0,sidePane->geometry().width(),central->geometry().height());
                sidePane->hide();
            }

            btnHidePanel->hide();
        }

        if(sender()==actShowMenu && sidePane->currentScreen()!="home")
            sidePane->showMenu();
        if(sender()==actShowReadList && sidePane->currentScreen()!="readlist")
            sidePane->showReadingList();
        if(sender()==actShowBookmarks && sidePane->currentScreen()!="bookmarks")
            sidePane->showBookmarks();
        if(sender()==actShowHistory && sidePane->currentScreen()!="history")
            sidePane->showHistory();
        if((sender()==actShowDownloads || sender()==sidePane->tab5) && sidePane->currentScreen()!="downloads")
            sidePane->showDownloads();
    }
    else if(sidePane->isHidden())
    {
        btnHidePanel->show();
        sidePane->show();

        if(settings->value("Interface/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(sidePane,"geometry");
            animation->setDuration(250);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            animation->setStartValue(QRect(central->geometry().width()+30,0,sidePane->geometry().width(),central->geometry().height()));
            animation->setEndValue(QRect(central->geometry().width()-sidePane->geometry().width(),0,sidePane->geometry().width(),central->geometry().height()));
            animation->start();
        }
        else
            sidePane->setGeometry(central->geometry().width()-sidePane->geometry().width(),0,sidePane->geometry().width(),central->geometry().height());

        //sidePane->showGeneral();

        if(sender()==actShowReadList)
            sidePane->showReadingList();
        if(sender()==actShowBookmarks)
            sidePane->showBookmarks();
        if(sender()==actShowHistory)
            sidePane->showHistory();
        if((sender()==actShowDownloads || sender()==sidePane->tab5))
            sidePane->showDownloads();

        genHistory();

        if(cmdPane->isVisible())
        {
            if(settings->value("Interface/Animations").toBool())
            {
                if(settings->value("Interface/Position/Toolbar").toString()=="top")
                {
                    QPropertyAnimation *animation = new QPropertyAnimation(cmdPane,"geometry");
                    animation->setDuration(250);
                    animation->setEasingCurve(QEasingCurve::OutCubic);
                    animation->setStartValue(cmdPane->geometry());
                    animation->setEndValue(QRect(0,-tabs->geometry().height(),tabs->geometry().width(),tabs->geometry().height()));
                    animation->start();
                    connect(animation,SIGNAL(finished()),cmdPane,SLOT(hide()));
                }
                else if(settings->value("Interface/Position/Toolbar").toString()=="bottom")
                {
                    QPropertyAnimation *animation = new QPropertyAnimation(cmdPane,"geometry");
                    animation->setDuration(250);
                    animation->setEasingCurve(QEasingCurve::OutCubic);
                    animation->setStartValue(cmdPane->geometry());
                    animation->setEndValue(QRect(0,tabs->geometry().height(),tabs->geometry().width(),tabs->geometry().height()));
                    animation->start();
                    connect(animation,SIGNAL(finished()),cmdPane,SLOT(hide()));
                }
            }
            else
            {
                if(settings->value("Interface/Position/Toolbar").toString()=="top")
                    cmdPane->setGeometry(0,-tabs->geometry().height(),tabs->geometry().width(),tabs->geometry().height());
                if(settings->value("Interface/Position/Toolbar").toString()=="bottom")
                    cmdPane->setGeometry(0,tabs->geometry().height(),tabs->geometry().width(),tabs->geometry().height());
                cmdPane->hide();
            }
        }
    }
}

void BrowserWindow::showTabsList()
{
    if(!tabView->isHidden())
    {
        btnHideTabView->hide();

        if(settings->value("Interface/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(tabView,"geometry");
            animation->setDuration(250);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            animation->setStartValue(tabView->geometry());
            animation->setEndValue(QRect(-305,0,300,central->geometry().height()));
            animation->start();
            connect(animation,SIGNAL(finished()),tabView,SLOT(hide()));
        }
        else
        {
            tabView->setGeometry(-305,0,300,central->geometry().height());
            tabView->hide();
        }
    }
    else
    {
        tabView->show();
        tabView->setTabWidget(tabs);

        if(settings->value("Interface/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(tabView,"geometry");
            animation->setDuration(250);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            animation->setStartValue(QRect(-305,0,300,central->geometry().height()));
            animation->setEndValue(QRect(0,0,300,central->geometry().height()));
            animation->start();
            connect(animation,SIGNAL(finished()),btnHideTabView,SLOT(show()));
        }
        else
        {
            tabView->setGeometry(0,0,300,central->geometry().height());
            btnHideTabView->show();
        }

        if(cmdPane->isVisible())
            showCompleter();
    }
}

void BrowserWindow::showFindText()
{
    barFindText->setVisible(barFindText->isHidden());

    if(barFindText->isHidden())
    {
        editFindText->setStyleSheet("background-color: transparent;");
        currentView()->findText("");
    }
}

void BrowserWindow::hideAllPanels()
{
    if(sidePane->isVisible())
    {
        if(settings->value("Interface/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(sidePane,"geometry");
            animation->setDuration(250);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            animation->setStartValue(sidePane->geometry());
            animation->setEndValue(QRect(central->geometry().width()+30,0,sidePane->geometry().width(),central->geometry().height()));
            animation->start();
            connect(animation,SIGNAL(finished()),sidePane,SLOT(hide()));
        }
        else
        {
            sidePane->setGeometry(central->geometry().width()+30,0,sidePane->geometry().width(),central->geometry().height());
            sidePane->hide();
        }

        if(sidePane->currentScreen()!="home")
            sidePane->showMenu();
    }

    btnHidePanel->hide();
}

void BrowserWindow::showMenu()
{
    mainMenu->popup(QCursor::pos());
}

void BrowserWindow::genHistory()
{
    // Clean Layout
    QLayoutItem *item;
    while ((item = sidePane->tab4->layItems->takeAt(0)) != 0)
    {
        item->widget()->deleteLater();
        delete item;
    }

    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();
    QList<QWebEngineHistoryItem> historyItems = currentView()->history()->items();

    for(int i(0);i<historyItems.count();i++)
    {
        QToolButton *item = new QToolButton;
        item->setIcon(QIcon(iconColor+"empty.png"));
        item->setText(historyItems.at(i).title());
        item->setFixedHeight(40);
        item->setIconSize(QSize(iconSize,iconSize));
        item->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        item->setToolTip(historyItems.at(i).url().toString());
        item->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Expanding);

        if(i == currentView()->history()->currentItemIndex())
            item->setObjectName("Current");

        if(item->text().count()>25)
            item->setText(item->text().left(25)+"...");

        connect(item,SIGNAL(clicked(bool)),this,SLOT(goToItem()));

        sidePane->tab4->layItems->addWidget(item);
    }

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    sidePane->tab4->layItems->addWidget(spacer);
}

void BrowserWindow::startup()
{
    settings->beginGroup("LastSession");

    bool lastSessionProperlyClosed = settings->value("ProperlyClosed").toBool();
    int countTabs = settings->childGroups().count();
    QString listTabs;

    for(int i(0);i<countTabs;i++)
    {
        /*QPixmap pixmap = settings->value(QString::number(i)+"/Screen").value<QPixmap>();
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        pixmap.save(&buffer, "PNG");
        QPixmap icon = settings->value(QString::number(i)+"/Icon").value<QPixmap>();
        QByteArray byteArray2;
        QBuffer buffer(&byteArray2);
        icon.save(&buffer, "PNG");*/
        //listTabs = listTabs+"<br /><li><img height=\"100\" src=\"data:image/png;base64," + byteArray.toBase64() + "\"/><br/><img height=\"20\" width=\"20\" src=\"data:image/png;base64," + byteArray2.toBase64() + "\"/><br /><strong>"+settings->value(QString::number(i)+"/Title").toString()+"</strong><br/>"+settings->value(QString::number(i)+"/Url").toString()+"</li>";
        listTabs = listTabs+"<li><strong>"+settings->value(QString::number(i)+"/Title").toString()+"</strong><br/>"+settings->value(QString::number(i)+"/Url").toString()+"</li>";
    }

    settings->endGroup();

    if(lastSessionProperlyClosed || countTabs==0)
    {
        WebWidget *webView = new WebWidget;
        connect(webView->view(),SIGNAL(loadStarted()),this,SLOT(loadStarted()));
        connect(webView->view(),SIGNAL(loadProgress(int)),this,SLOT(loadProgress(int)));
        connect(webView->view(),SIGNAL(loadFinished(bool)),this,SLOT(loadFinished()));
        connect(webView->view(),SIGNAL(urlChanged(QUrl)),this,SLOT(urlChanged(QUrl)));
        connect(webView->view(),SIGNAL(iconChanged()),this,SLOT(iconChanged()));
        connect(webView->view(),SIGNAL(titleChanged(QString)),this,SLOT(titleChanged(QString)));
        connect(webView->view()->page()->profile(),SIGNAL(downloadRequested(QWebEngineDownloadItem*)),sidePane->tab5,SLOT(downloadRequested(QWebEngineDownloadItem*)));

        int index = tabs->addWidget(webView);
        tabBar->addTab(QIcon(iconColor+"empty.png"),tr("Startup"));
        tabs->setCurrentIndex(index);

        if(settings->value("Navigation/Startup/Home").toBool())
            webView->view()->load(QUrl(settings->value("Navigation/Home").toString()));
        else if(settings->value("Navigation/Startup/Specific").toBool())
            webView->view()->load(QUrl(settings->value("Navigation/Startup/SpecificPage").toString()));

        settings->remove("ClosedTabs");
        settings->remove("LastSession");
        settings->beginGroup("LastSession");
        settings->setValue("ProperlyClosed",false);
        settings->setValue("0/Title",currentView()->title());
        settings->setValue("0/Url",currentView()->url());
        settings->setValue("0/Icon",currentView()->icon());
        settings->setValue("0/Screen",currentWidget()->screenShot());
        settings->endGroup();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Restore the previous session"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("<p>At the previous session, the browser was not closed properly. These are not properly closed tabs :</p><ul>%1</ul><p>Do you want to restore the previous session ?<p>").arg(listTabs));
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
        msgBox.setButtonText(QMessageBox::No,tr("No"));
        int value = msgBox.exec();

        if(value == QMessageBox::Yes)
        {
            for(int i(0);i<countTabs;i++)
            {
                WebWidget *webView = new WebWidget;
                connect(webView->view(),SIGNAL(loadStarted()),this,SLOT(loadStarted()));
                connect(webView->view(),SIGNAL(loadProgress(int)),this,SLOT(loadProgress(int)));
                connect(webView->view(),SIGNAL(loadFinished(bool)),this,SLOT(loadFinished()));
                connect(webView->view(),SIGNAL(urlChanged(QUrl)),this,SLOT(urlChanged(QUrl)));
                connect(webView->view(),SIGNAL(iconChanged()),this,SLOT(iconChanged()));
                connect(webView->view(),SIGNAL(titleChanged(QString)),this,SLOT(titleChanged(QString)));
                connect(webView->view()->page()->profile(),SIGNAL(downloadRequested(QWebEngineDownloadItem*)),sidePane->tab5,SLOT(downloadRequested(QWebEngineDownloadItem*)));

                QUrl url = settings->value("LastSession/"+QString::number(i)+"/Url").toUrl();
                tabs->addWidget(webView);
                tabBar->addTab(QIcon(iconColor+"empty.png"),tr("Last session"));

                webView->view()->load(url);
            }
            tabs->setCurrentIndex(settings->value("LastSession/CurrentIndex").toInt()); //qDebug()<<setting... marche pas bug
        }
        else if(value == QMessageBox::No)
        {
            WebWidget *webView = new WebWidget;
            connect(webView->view(),SIGNAL(loadStarted()),this,SLOT(loadStarted()));
            connect(webView->view(),SIGNAL(loadProgress(int)),this,SLOT(loadProgress(int)));
            connect(webView->view(),SIGNAL(loadFinished(bool)),this,SLOT(loadFinished()));
            connect(webView->view(),SIGNAL(urlChanged(QUrl)),this,SLOT(urlChanged(QUrl)));
            connect(webView->view(),SIGNAL(iconChanged()),this,SLOT(iconChanged()));
            connect(webView->view(),SIGNAL(titleChanged(QString)),this,SLOT(titleChanged(QString)));
            connect(webView->view()->page()->profile(),SIGNAL(downloadRequested(QWebEngineDownloadItem*)),sidePane->tab5,SLOT(downloadRequested(QWebEngineDownloadItem*)));

            int index = tabs->addWidget(webView);
            tabBar->addTab(QIcon(iconColor+"empty.png"),tr("Startup"));
            tabs->setCurrentIndex(index);

            if(settings->value("Navigation/Startup/Home").toBool())
                webView->view()->load(QUrl(settings->value("Navigation/Home").toString()));
            else if(settings->value("Navigation/Startup/Specific").toBool())
                webView->view()->load(QUrl(settings->value("Navigation/Startup/SpecificPage").toString()));

            settings->remove("ClosedTabs");
            settings->remove("LastSession");
            settings->beginGroup("LastSession");
            settings->setValue("ProperlyClosed",false);
            settings->setValue("0/Title",currentView()->title());
            settings->setValue("0/Url",currentView()->url());
            settings->setValue("0/Icon",currentView()->icon());
            settings->setValue("0/Screen",currentWidget()->screenShot());
            settings->endGroup();
        }
    }
}

void BrowserWindow::openDefaultBrowser()
{
    QDesktopServices::openUrl(currentView()->url());
}

void BrowserWindow::runJavaScriptCode()
{
    QString code = "";
    if(sender()==actInvertColors)
        code = "javascript: (function () { var css = 'html {-webkit-filter: invert(100%);}/* img {-webkit-filter: invert(100%);*/}',head = document.getElementsByTagName('head')[0],style = document.createElement('style'); if (!window.counter) { window.counter = 1;} else  { window.counter ++; if (window.counter % 2 == 0) { var css ='html {-webkit-filter: invert(0%);} /*img {-webkit-filter: invert(0%);}*/'} }; style.type = 'text/css'; if (style.styleSheet){style.styleSheet.cssText = css;} else {style.appendChild(document.createTextNode(css));} head.appendChild(style);}());";
    if(sender()==actTranslate)
        code = tr("javascript:var t=((window.getSelection&&window.getSelection())||(document.getSelection&&document.getSelection())||(document.selection&&document.selection.createRange&&document.selection.createRange().text));var e=(document.charset||document.characterSet);if(t!=''){location.href='http://translate.google.com/?text='+t+'&hl=fr&langpair=auto|en&tbb=1&ie='+e;}else{location.href='http://translate.google.com/translate?u='+encodeURIComponent(location.href)+'&hl=fr&langpair=auto|en&tbb=1&ie='+e;};"); // Translates from Google : https://translate.google.com/translate_buttons

    currentView()->page()->runJavaScript(code);
}

void BrowserWindow::findPreviousWord()
{
    /*QString words = editFindText->text();
    bool find;

    if(checkCaseSensitive->isChecked())
    {
        currentView()->findText(words, QWebEnginePage::FindBackward | QWebEnginePage::FindCaseSensitively, find);

        if (find)
            editFindText->setStyleSheet("background-color: #bbffbb;");
        else
            editFindText->setStyleSheet("background-color: #ffb3b3;");
    }
    else
    {
        currentView()->findText(words, QWebEnginePage::FindBackward, find);

        if (find)
            editFindText->setStyleSheet("background-color: #bbffbb;");
        else
          editFindText->setStyleSheet("background-color: #ffb3b3;");
    }*/
}

void BrowserWindow::findNextWord()
{
   /* QString words = editFindText->text();
    bool find;

    if(checkCaseSensitive->isChecked())
    {
        currentView()->findText(words, QWebEnginePage::FindCaseSensitively, find);

        if(find)
            editFindText->setStyleSheet("background-color: #bbffbb;");
        else
            editFindText->setStyleSheet("background-color: #ffb3b3;");
    }
    else
    {
        currentView()->findText(words, QWebEnginePage::FindFlags(), [this](bool found) {
        if (found)
            editFindText->setStyleSheet("background-color: #bbffbb;");
        else
            editFindText->setStyleSheet("background-color: #ffb3b3;");
        });
    }*/
}

void BrowserWindow::takeScreenshot()
{
    emit setScreenshot(currentWidget()->screenShot(),currentView()->title());
    emit showScreenshot();
}

void BrowserWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("All files (*);;Web Page (*.html *.xhtlm *.htm *.shtm);;Text File (*.txt);;Picture (*.bmp *.gif *.jpeg *.jpg *.png)"));
    if(!path.isEmpty())
        currentView()->load(QUrl::fromUserInput(path));

    currentView()->setFocus();
}

void BrowserWindow::saveFile()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), QString(QDir::homePath()+QDir::separator()+currentView()->url().fileName()), tr("All Files (*);;HTML Files (*.html);;PHP Files (*.php);;CSS Files (*.css);;JavaScript Files (*.js);;Text Files (*.txt)"));
    if (!path.isEmpty())
    {
        QFile fileToSave(path);
        if(fileToSave.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream flux(&fileToSave);
            //flux << currentView()->page()->mainFrame()->toHtml();//bug
            fileToSave.close();
        }
    }

    currentView()->setFocus();
}

void BrowserWindow::zoomIn()
{
    if(settings->value("Interface/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(currentView(), "zoomFactor");
        animation->setDuration(250);
        animation->setStartValue(currentView()->zoomFactor());
        animation->setEndValue(currentView()->zoomFactor()+0.25);

        animation->start();
    }
    else
        currentView()->setZoomFactor(currentView()->zoomFactor()+0.25);

    currentView()->setFocus();
}

void BrowserWindow::zoomOut()
{
    if(settings->value("Interface/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(currentView(), "zoomFactor");
        animation->setDuration(250);
        animation->setStartValue(currentView()->zoomFactor());
        animation->setEndValue(currentView()->zoomFactor()-0.25);

        animation->start();
    }
    else
        currentView()->setZoomFactor(currentView()->zoomFactor()-0.25);

    currentView()->setFocus();
}

void BrowserWindow::resetZoom()
{

    if(settings->value("Interface/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(currentView(), "zoomFactor");
        animation->setDuration(800);
        animation->setEasingCurve(QEasingCurve::OutElastic);
        animation->setStartValue(currentView()->zoomFactor());
        animation->setEndValue(1);

        animation->start();
    }
    else
        currentView()->setZoomFactor(1);

    currentView()->setFocus();
}

void BrowserWindow::execNavig()
{
    if(sender()==actGoBack)
        currentView()->back();
    else if(sender()==actGoForward)
        currentView()->forward();
    else if(sender()==actReload)
        currentView()->reload();
    else if(sender()==actStop)
        currentView()->stop();
    else if(sender()==actGoHome)
        currentView()->load(QUrl(settings->value("Navigation/Home").toString()));
}

void BrowserWindow::addToBookmarks()
{
    settings->beginGroup("Bookmarks");

    int folder = settings->childGroups().count();

    if(!currentView()->icon().isNull())
        settings->setValue(QString::number(folder)+"/Icon",currentView()->icon().pixmap(55,55));
    settings->setValue(QString::number(folder)+"/Title",currentView()->title());
    settings->setValue(QString::number(folder)+"/Url",currentView()->url());
    settings->setValue(QString::number(folder)+"/Screen",currentWidget()->screenShot());
    settings->setValue(QString::number(folder)+"/Date",QDate::currentDate());
    settings->setValue(QString::number(folder)+"/Time",QTime::currentTime());

    settings->endGroup();

    sidePane->tab3->updateItems();
}

void BrowserWindow::addToReadingList()
{
    settings->beginGroup("ReadingList");

    int folder = settings->childGroups().count();

    if(!currentView()->icon().isNull())
        settings->setValue(QString::number(folder)+"/Icon",currentView()->icon().pixmap(55,55));
    settings->setValue(QString::number(folder)+"/Title",currentView()->title());
    settings->setValue(QString::number(folder)+"/Url",currentView()->url());

    settings->endGroup();

    sidePane->tab2->updateItems();
}

void BrowserWindow::goToUrl(QString url)
{
    hideAllPanels();

    currentView()->setFocus();
    currentView()->load(QUrl::fromUserInput(url));
}

void BrowserWindow::goToUrl()
{
    QString text = cmdPane->titleBar->text();
    if(text.left(7)=="http://" || text.left(8)=="https://" || text.left(9)=="localhost" || text.contains("."))
        currentView()->load(QUrl::fromUserInput(text));
    else
        currentView()->load(QUrl(settings->value("Navigation/PrefixSearch").toString() + text + settings->value("Navigation/SuffixSearch").toString()));

    hideAllPanels();
}

bool BrowserWindow::isDownloading()
{
    return sidePane->tab5->isDownloading();
}

void BrowserWindow::goToItem()
{
    currentView()->load(QUrl::fromUserInput(qobject_cast<QToolButton *>(sender())->toolTip()));

    hideAllPanels();
}

void BrowserWindow::updateActs()
{
    actGoBack->setVisible(currentView()->page()->history()->canGoBack());
    actGoForward->setVisible(currentView()->page()->history()->canGoForward());
}

void BrowserWindow::loadStarted()
{
    if(sender()==currentView())
    {
        updateActs();
        actReload->setVisible(false);
        actStop->setVisible(true);

        address->setProgression(0);
    }

    if(tabView->isVisible())
        tabView->setTabWidget(tabs);
}

void BrowserWindow::loadProgress(int value)
{
    if(sender()==currentView())
    {
        updateActs();
        actReload->setVisible(false);
        actStop->setVisible(true);

        address->setProgression(value);
    }
}

void BrowserWindow::loadFinished()
{
    if(sender()==currentView())
    {
        updateActs();
        actReload->setVisible(true);
        actStop->setVisible(false);

        //qDebug()<<finished;
        // Afficher notification quand fnished = false : page pas bien charger voulez vous recommencer ?

        address->setProgression(0);
    }

    if(tabView->isVisible())
        tabView->setTabWidget(tabs);

    updateAll();
}

void BrowserWindow::urlChanged(QUrl url)
{
    if(sender()==currentView())
    {
        address->lineEdit->setToolTip(url.toString());
    }

    updateAll();
}

void BrowserWindow::iconChanged()
{
    QIcon icon(iconColor+"empty.png");
    if(!currentView()->icon().isNull())
        icon = currentView()->icon();

    if(sender()==currentView())
        address->btnIcon->setIcon(icon);

    tabBar->setTabIcon(tabs->indexOf(qobject_cast<WebWidget *>(sender()->parent()->parent())),icon);

    if(tabView->isVisible())
        tabView->setTabWidget(tabs);

    updateAll();
}

void BrowserWindow::titleChanged(QString title)
{
    if(title.count()>30)
        title = title.left(30)+"...";

    if(settings->value("Navigation/Options/CurrentPageOnTitleBar").toBool() && !title.isEmpty() && sender()==currentView())
    {
        emit setTitle(QCoreApplication::applicationName()+" - "+title);

        address->lineEdit->setText(currentView()->title());
    }
    else
        emit setTitle(QCoreApplication::applicationName());

    tabBar->setTabText(tabs->indexOf(qobject_cast<WebWidget *>(sender()->parent()->parent())),title);

    if(tabView->isVisible())
        tabView->setTabWidget(tabs);

    updateAll();
}

void BrowserWindow::updateAll()
{
    updateActs();
    tabBar->setCurrentIndex(tabs->currentIndex());

/*    QPushButton *btn = new QPushButton;
    btn->setIcon(QIcon(":/img/white/cross.png"));
    btn->setStyleSheet("border: none; background: transparent; margin-right: 5px;");
    btn->setIconSize(QSize(16,16));
    tabs->tabBar()->setTabButton(0,QTabBar::RightSide,btn);*/

    actReload->setVisible(!currentWidget()->isLoading());
    actStop->setVisible(currentWidget()->isLoading());
    address->setProgression(currentWidget()->progression);
    QString title = currentView()->title();
    if(title.count()>30)
        title = title.left(30)+"...";
    address->lineEdit->setText(title);
    address->lineEdit->setToolTip(currentView()->url().toString());
    address->btnIcon->setIcon(QIcon(currentView()->icon()));
    if(currentView()->icon().isNull())
        address->btnIcon->setIcon(QIcon(iconColor+"empty.png"));

    settings->beginGroup("LastSession");
    settings->setValue("CurrentIndex",tabs->currentIndex());
    for(int i(0);i<tabs->count();i++)
    {
        settings->setValue(QString::number(i)+"/Title",qobject_cast<WebWidget *>(tabs->widget(i))->view()->title());
        settings->setValue(QString::number(i)+"/Url",qobject_cast<WebWidget *>(tabs->widget(i))->view()->url());
        settings->setValue(QString::number(i)+"/Icon",qobject_cast<WebWidget *>(tabs->widget(i))->view()->icon());
        settings->setValue(QString::number(i)+"/Screen",qobject_cast<WebWidget *>(tabs->widget(i))->screenShot());
    }
    settings->endGroup();
}

void BrowserWindow::aboutWindow()
{
    sidePane->showAbout();
}

void BrowserWindow::openOrganisationDomain()
{
    WebWidget *webView = new WebWidget;

    connect(webView->view(),SIGNAL(loadStarted()),this,SLOT(loadStarted()));
    connect(webView->view(),SIGNAL(loadProgress(int)),this,SLOT(loadProgress(int)));
    connect(webView->view(),SIGNAL(loadFinished(bool)),this,SLOT(loadFinished()));
    connect(webView->view(),SIGNAL(urlChanged(QUrl)),this,SLOT(urlChanged(QUrl)));
    connect(webView->view(),SIGNAL(iconChanged()),this,SLOT(iconChanged()));
    connect(webView->view(),SIGNAL(titleChanged(QString)),this,SLOT(titleChanged(QString)));

    int index = tabs->addWidget(webView);
    tabBar->addTab(QIcon(iconColor+"empty.png"),tr("Loading"));
    tabs->setCurrentIndex(index);

    currentView()->load(QUrl(QCoreApplication::organizationDomain()));
}

void BrowserWindow::loadConfig()
{
    settings->beginGroup("Interface");

    // Toolbar position
    if(settings->value("Position/Toolbar").toString()=="top")
    {
        /*tabBar->setShape(QTabBar::TriangularNorth);
        tabBar->setObjectName("TopTabBar");*/
        centralLayout->setDirection(QBoxLayout::TopToBottom);
    }
    else
    {
        /*tabBar->setShape(QTabBar::TriangularSouth);
        tabBar->setObjectName("BottomTabBar");*/
        centralLayout->setDirection(QBoxLayout::BottomToTop);
    }

    //Tabbar
    if(settings->value("Tabbar/Enable").toBool())
        tabToolBar->show();
    else
        tabToolBar->hide();

   // tabBar->setTabBarAutoHide(settings->value("Tabbar/VisibleWithLessTwoTabs").toBool());
    tabBar->setTabsClosable(settings->value("Tabbar/Closable").toBool());
    tabBar->setMovable(settings->value("Tabbar/Movable").toBool());

    // Icon Color
    iconColor = ":/img/"+settings->value("IconColor").toString()+"/";

    actNewTab->setIcon(QIcon(iconColor+"add.png"));
    actCloseTab->setIcon(QIcon(iconColor+"cross.png"));
    actDuplicateTab->setIcon(QIcon(iconColor+"tabs.png"));
    actRestoreTab->setIcon(QIcon(iconColor+"undo.png"));
    actOpenFile->setIcon(QIcon(iconColor+"open.png"));
    actSaveFile->setIcon(QIcon(iconColor+"save.png"));
    actShowTabsList->setIcon(QIcon(iconColor+"tabs.png"));
    actShowReadList->setIcon(QIcon(iconColor+"reading-list.png"));
    actShowBookmarks->setIcon(QIcon(iconColor+"bookmarks.png"));
    actShowHistory->setIcon(QIcon(iconColor+"history.png"));
    actShowDownloads->setIcon(QIcon(iconColor+"downloads.png"));
    actGoBack->setIcon(QIcon(iconColor+"back.png"));
    actGoForward->setIcon(QIcon(iconColor+"forward.png"));
    actReload->setIcon(QIcon(iconColor+"reload.png"));
    actStop->setIcon(QIcon(iconColor+"cross.png"));
    actGoHome->setIcon(QIcon(iconColor+"home.png"));
    actZoomIn->setIcon(QIcon(iconColor+"zoom-in.png"));
    actZoomOut->setIcon(QIcon(iconColor+"zoom-out.png"));
    actResetZoom->setIcon(QIcon(iconColor+"reset.png"));
    actFullScreen->setIcon(QIcon(iconColor+"full-screen.png"));
    actLock->setIcon(QIcon(iconColor+"lock.png"));
    actAddToReadingList->setIcon(QIcon(iconColor+"reading-list.png"));
    actAddToBookmarks->setIcon(QIcon(iconColor+"bookmarks.png"));
    actSkinManager->setIcon(QIcon(iconColor+"skin.png"));
    actOpenUrlInOtherBrowser->setIcon(QIcon(iconColor+"external.png"));
    actShowFindText->setIcon(QIcon(iconColor+"search.png"));
    actHideFindText->setIcon(QIcon(iconColor+"cross.png"));
    actFindBack->setIcon(QIcon(iconColor+"back.png"));
    actFindForward->setIcon(QIcon(iconColor+"forward.png"));
    actInvertColors->setIcon(QIcon(iconColor+"config.png"));
    actTranslate->setIcon(QIcon(iconColor+"config.png"));
    actScreen->setIcon(QIcon(iconColor+"screen.png"));
    actConfig->setIcon(QIcon(iconColor+"config.png"));
    actAbout->setIcon(QIcon(iconColor+"info.png"));
    actQuit->setIcon(QIcon(iconColor+"cross.png"));
    actShowMenu->setIcon(QIcon(iconColor+"menu.png"));

    menuFile->setIcon(QIcon(iconColor+"file.png"));
    menuNavig->setIcon(QIcon(iconColor+"reload.png"));
    menuView->setIcon(QIcon(iconColor+"zoom.png"));
    menuAddTo->setIcon(QIcon(iconColor+"add.png"));
    menuTools->setIcon(QIcon(iconColor+"config.png"));

    // Icons on toolbar
    int iconSize = settings->value("ToolBar/IconSize").toInt();
    tabToolBar->setIconSize(QSize(iconSize,iconSize));
    mainToolBar->setIconSize(QSize(iconSize,iconSize));
    barFindText->setIconSize(QSize(iconSize,iconSize));

    address = new AddressBar;
    address->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    address->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    address->setFixedWidth(300);
    connect(address,SIGNAL(showCompleter()),this,SLOT(showCompleter()));

    mainToolBar->clear();
    mainToolBar->addAction(actShowTabsList);
    if(settings->value("ToolBar/Completer")=="left")
        mainToolBar->addWidget(address);
    if(settings->value("ToolBar/AddTab").toBool())
        mainToolBar->addAction(actNewTab);
    if(settings->value("ToolBar/Back").toBool())
        mainToolBar->addAction(actGoBack);
    if(settings->value("ToolBar/Forward").toBool())
        mainToolBar->addAction(actGoForward);
    if(settings->value("ToolBar/Reload").toBool())
    {
        mainToolBar->addAction(actReload);
        mainToolBar->addAction(actStop);
    }
    if(settings->value("ToolBar/Home").toBool())
        mainToolBar->addAction(actGoHome);
    if(settings->value("ToolBar/OpenUrlInOtherBrowser").toBool())
        mainToolBar->addAction(actOpenUrlInOtherBrowser);
    if(settings->value("ToolBar/ReadList").toBool())
        mainToolBar->addAction(actShowReadList);
    if(settings->value("ToolBar/Bookmarks").toBool())
        mainToolBar->addAction(actShowBookmarks);
    if(settings->value("ToolBar/History").toBool())
        mainToolBar->addAction(actShowHistory);
    if(settings->value("ToolBar/Downloads").toBool())
        mainToolBar->addAction(actShowDownloads);
    if(settings->value("ToolBar/Screen").toBool())
        mainToolBar->addAction(actScreen);
    if(settings->value("ToolBar/Lock").toBool())
        mainToolBar->addAction(actLock);
    if(settings->value("ToolBar/Full").toBool())
        mainToolBar->addAction(actFullScreen);
    if(settings->value("ToolBar/Skin").toBool())
        mainToolBar->addAction(actSkinManager);
    if(settings->value("ToolBar/Config").toBool())
        mainToolBar->addAction(actConfig);

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainToolBar->addWidget(spacer);

    if(settings->value("ToolBar/Completer")=="right")
        mainToolBar->addWidget(address);
    mainToolBar->addAction(actShowMenu);

    cmdPane->btnIcon->setIcon(QIcon(iconColor+"empty.png"));
    cmdPane->loadSettings();
    cmdPane->updateQuery();

    settings->endGroup();

    sidePane->loadSettings();
    tabView->loadSettings();
    tabView->setTabWidget(tabs);
    //genHistory();

    // WebWidgets Settings
    bool enableJavaScript = settings->value("Navigation/Options/EnableJavaScript").toBool();
    bool enableJavascriptCanOpenWindow = settings->value("Navigation/Options/EnableJavascriptCanOpenWindow").toBool();
    bool enableJavascriptCanAccessClipboard = settings->value("Navigation/Options/EnableJavascriptCanAccessClipboard").toBool();
    bool enableCustomUserAgent = settings->value("Navigation/UserAgent/Enable").toBool();

    for(int i(0);i<tabs->count();i++)
    {
        qobject_cast<WebWidget *>(tabs->widget(i))->view()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled,enableJavaScript);
        qobject_cast<WebWidget *>(tabs->widget(i))->view()->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows,enableJavascriptCanOpenWindow);
        qobject_cast<WebWidget *>(tabs->widget(i))->view()->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard,enableJavascriptCanAccessClipboard);
        if(enableCustomUserAgent)
            qobject_cast<WebWidget *>(tabs->widget(i))->view()->page()->profile()->setHttpUserAgent(settings->value("Navigation/UserAgent/Value").toString());
        //qDebug()<<settings->value("Navigation/UserAgent/Value").toString();
    }
}

void BrowserWindow::saveState()
{
    // Save session
    settings->beginGroup("LastSession");

    settings->setValue("ProperlyClosed",true);

    for(int i(0);i<tabs->count();i++)
    {
        QString title = qobject_cast<WebWidget *>(tabs->widget(i))->view()->title();
        QUrl url = qobject_cast<WebWidget *>(tabs->widget(i))->view()->url();
        QIcon icon = qobject_cast<WebWidget *>(tabs->widget(i))->view()->icon();
        QPixmap screen = qobject_cast<WebWidget *>(tabs->widget(i))->screenShot();

        settings->setValue(QString::number(i)+"/Title",title);
        settings->setValue(QString::number(i)+"/Url",url);
        settings->setValue(QString::number(i)+"/Icon",icon);
        settings->setValue(QString::number(i)+"/Screen",screen);
    }

    settings->endGroup();
}
