#include "configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent) : QMainWindow(parent)
{
    genUI();
}

ConfigWindow::~ConfigWindow()
{

}

void ConfigWindow::genUI()
{
    setObjectName("ConfigWindow");

    settings = new QSettings;
    iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    int size = settings->value("Interface/ToolBar/IconSize").toInt();
    iconSize = QSize(size,size);

    genActs();
    genMenu();
    genToolBar();

    genCustomize();
    genNavigation();
    genInterface();
    genSkins();
    genDownloads();
    genSecurity();
    genOptions();
    genReset();

    central = new QStackedWidget;
    central->addWidget(tabCustomize);
    central->addWidget(tabNavigation);
    central->addWidget(tabInterface);
    central->addWidget(tabSkins);
    central->addWidget(tabDownloads);
    central->addWidget(tabSecurity);
    central->addWidget(tabOptions);
    central->addWidget(tabReset);
    setCentralWidget(central);

    listTab = new TabList;
    listTab->addTab(tabCustomize->title(),0);
    listTab->addTab(tabNavigation->title(),1);
    listTab->addTab(tabInterface->title(),2);
   /* listTab->addTab(tr("Completer"),2);
    listTab->addTab(tabSkins->title(),3);*/
    listTab->addTab(tabDownloads->title(),4);
    listTab->addTab(tabSecurity->title(),5);
    listTab->addTab(tabOptions->title(),6);
    listTab->addTab(tabReset->title(),7);
    listTab->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    connect(listTab,SIGNAL(showTab(int)),central,SLOT(setCurrentIndex(int)));
    connect(listTab,SIGNAL(showTab(int)),this,SLOT(resizeSplitter()));

    QList<int> listSizes;
    listSizes << 300 << 1000;

    center = new QSplitter;
    center->setContentsMargins(0,0,0,0);
    center->addWidget(listTab);
    center->addWidget(central);
    center->setSizes(listSizes);

    centralLayout = new QVBoxLayout;
    centralLayout->setContentsMargins(0,0,0,0);
    centralLayout->setSpacing(0);
    centralLayout->addWidget(mainToolBar);
    centralLayout->addWidget(center);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(centralLayout);

    setCentralWidget(centralWidget);
}

void ConfigWindow::genActs()
{
    actBack = new QAction(tr("Back"),this);
    connect(actBack,SIGNAL(triggered()),this,SLOT(goBack()));

    actRefresh = new QAction(tr("Refresh"),this);
    connect(actRefresh,SIGNAL(triggered()),this,SLOT(loadSettings()));
}

void ConfigWindow::genMenu()
{
    mainMenu = new QMenu;
    mainMenu->addAction(actBack);
    mainMenu->addSeparator();
    mainMenu->addAction(actRefresh);
}

void ConfigWindow::genToolBar()
{
    mainToolBar = new QToolBar(tr("Main tool bar"));
    mainToolBar->setMovable(false);
   // mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    mainToolBar->addAction(actBack);
   // mainToolBar->addSeparator();

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);
    mainToolBar->addWidget(spacer);

    btnMenu = new QPushButton;
    btnMenu->setMenu(mainMenu);
    mainToolBar->addWidget(btnMenu);
}

void ConfigWindow::genCustomize()
{
    QLabel *lblColor = new QLabel(tr("Main color"));
    editColor = new PreviewBox;
    connect(editColor,SIGNAL(editRequested()),this,SLOT(chooseColor()));

    QLabel *lblBgImg = new QLabel(tr("Background image"));
    editBgImg = new PreviewBox;
    connect(editBgImg,SIGNAL(editRequested()),this,SLOT(chooseBgImage()));

    QLabel *lblProfile = new QLabel(tr("Profile photo"));
    editProfile = new PreviewBox;
    connect(editProfile,SIGNAL(editRequested()),this,SLOT(chooseBgImage()));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(lblColor,0,0);
    mainLayout->addWidget(editColor,1,0);
    mainLayout->addWidget(lblBgImg,0,1);
    mainLayout->addWidget(editBgImg,1,1);
    mainLayout->addWidget(lblProfile,0,2);
    mainLayout->addWidget(editProfile,1,2);
    mainLayout->addWidget(spacer,3,3);

    tabCustomize = new SettingsPanel;
    tabCustomize->setTitle(tr("Customize"));
    tabCustomize->setMainLayout(mainLayout);
}

void ConfigWindow::genNavigation()
{
    QLabel *lblHomePage = new QLabel(tr("Home page"));
    lblHomePage->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    editHomePage = new QLineEdit;
    connect(editHomePage,SIGNAL(textEdited(QString)),this,SLOT(editing()));

    QLabel *lblSearchUrl = new QLabel(tr("Search URL"));
    editPrefixSearchUrl = new QLineEdit;
    connect(editPrefixSearchUrl,SIGNAL(textEdited(QString)),this,SLOT(editing()));
    QLabel *lblSearch = new QLabel(tr("search words"));
    editSuffixSearchUrl = new QLineEdit;
    connect(editSuffixSearchUrl,SIGNAL(textEdited(QString)),this,SLOT(editing()));

    QLabel *lblUserAgent = new QLabel(tr("User agent"));
    checkCustomUserAgent = new QCheckBox(tr("Use a custom user agent"));
    checkCustomUserAgent->setVisible(false);
    lblUserAgent->hide();
    editUserAgent = new QLineEdit;
    connect(checkCustomUserAgent,SIGNAL(toggled(bool)),editUserAgent,SLOT(setVisible(bool)));
    connect(checkCustomUserAgent,SIGNAL(clicked()),this,SLOT(editing()));
    connect(editUserAgent,SIGNAL(textChanged(QString)),this,SLOT(editing()));

    QGridLayout *layNavigationUrl = new QGridLayout;
    layNavigationUrl->setContentsMargins(20,20,20,20);
    layNavigationUrl->setSpacing(10);

    layNavigationUrl->addWidget(lblUserAgent,0,0);
    layNavigationUrl->addWidget(checkCustomUserAgent,1,0);
    layNavigationUrl->addWidget(editUserAgent,1,1,1,3);

    layNavigationUrl->addWidget(lblHomePage,2,0);
    layNavigationUrl->addWidget(editHomePage,2,1,1,3);

    layNavigationUrl->addWidget(lblSearchUrl,3,0);
    layNavigationUrl->addWidget(editPrefixSearchUrl,3,1);
    layNavigationUrl->addWidget(lblSearch,3,2);
    layNavigationUrl->addWidget(editSuffixSearchUrl,3,3);

    HideBlock *boxNavigationUrl = new HideBlock(tr("General"));
    boxNavigationUrl->setBlockLayout(layNavigationUrl);
    boxNavigationUrl->showBlock();

    // ---------------------------------------

    radioEmptyStartup = new QRadioButton(tr("Show empty page"));
    radioHomeStartup = new QRadioButton(tr("Show home page"));
    radioSpecificStartup = new QRadioButton(tr("Show specific page"));
    editSpecificStartup = new QLineEdit;
    editSpecificStartup->hide();

    connect(radioEmptyStartup,SIGNAL(clicked()),this,SLOT(editing()));
    connect(radioHomeStartup,SIGNAL(clicked()),this,SLOT(editing()));
    connect(radioSpecificStartup,SIGNAL(clicked()),this,SLOT(editing()));
    connect(editSpecificStartup,SIGNAL(textChanged(QString)),this,SLOT(editing()));
    connect(radioSpecificStartup,SIGNAL(toggled(bool)),editSpecificStartup,SLOT(setVisible(bool)));

    QGridLayout *layStartup = new QGridLayout;
    layStartup->setContentsMargins(20,20,20,20);
    layStartup->setSpacing(10);

    layStartup->addWidget(radioEmptyStartup,0,0);
    layStartup->addWidget(radioHomeStartup,1,0);
    layStartup->addWidget(radioSpecificStartup,2,0);
    layStartup->addWidget(editSpecificStartup,3,1);

    HideBlock *boxStartup = new HideBlock(tr("At startup"));
    boxStartup->setBlockLayout(layStartup);
    boxStartup->showBlock();

    // -----------------------------------------

    radioEmptyNewTab = new QRadioButton(tr("Show empty page"));
    radioHomeNewTab = new QRadioButton(tr("Show home page"));
    radioSpecificNewTab = new QRadioButton(tr("Show specific page"));
    editSpecificNewTab = new QLineEdit;
    editSpecificNewTab->hide();

    connect(radioEmptyNewTab,SIGNAL(clicked()),this,SLOT(editing()));
    connect(radioHomeNewTab,SIGNAL(clicked()),this,SLOT(editing()));
    connect(radioSpecificNewTab,SIGNAL(clicked()),this,SLOT(editing()));
    connect(editSpecificNewTab,SIGNAL(textChanged(QString)),this,SLOT(editing()));
    connect(radioSpecificNewTab,SIGNAL(toggled(bool)),editSpecificNewTab,SLOT(setVisible(bool)));

    QGridLayout *layNewTab = new QGridLayout;
    layNewTab->setContentsMargins(20,20,20,20);
    layNewTab->setSpacing(10);

    layNewTab->addWidget(radioEmptyNewTab,0,0);
    layNewTab->addWidget(radioHomeNewTab,1,0);
    layNewTab->addWidget(radioSpecificNewTab,2,0);
    layNewTab->addWidget(editSpecificNewTab,3,1);

    HideBlock *boxNewTab = new HideBlock(tr("When I open a new tab"));
    boxNewTab->setBlockLayout(layNewTab);
    boxNewTab->showBlock();

    // -----------------------------------------

    // GrpBox Navigation (normal/private/perso)

    // ---------------------------------------

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(boxNavigationUrl);
    mainLayout->addWidget(boxStartup);
    mainLayout->addWidget(boxNewTab);
    mainLayout->addWidget(spacer);

    tabNavigation = new SettingsPanel;
    tabNavigation->setTitle(tr("Navigation"));
    tabNavigation->setMainLayout(mainLayout);
}

void ConfigWindow::genInterface()
{
    int minWidth(270);

    checkToolBarAddTab = new QToolButton;
    checkToolBarBack = new QToolButton;
    checkToolBarForward = new QToolButton;
    checkToolBarReload = new QToolButton;
    checkToolBarHome = new QToolButton;
    checkToolBarOpenUrlInOtherBrowser = new QToolButton;
    checkToolBarLock = new QToolButton;
    checkToolBarFull = new QToolButton;
    checkToolBarReadList = new QToolButton;
    checkToolBarBookmarks = new QToolButton;
    checkToolBarHistory = new QToolButton;
    checkToolBarDownloads = new QToolButton;
    checkToolBarScreen = new QToolButton;
    checkToolBarSkin = new QToolButton;
    checkToolBarConfig = new QToolButton;

    checkToolBarAddTab->setToolTip(tr("New tab"));
    checkToolBarBack->setToolTip(tr("Back"));
    checkToolBarForward->setToolTip(tr("Forward"));
    checkToolBarReload->setToolTip(tr("Reload/Stop"));
    checkToolBarHome->setToolTip(tr("Home"));
    checkToolBarOpenUrlInOtherBrowser->setToolTip(tr("Open in a external browser"));
    checkToolBarLock->setToolTip(tr("Lock"));
    checkToolBarScreen->setToolTip(tr("Screenshot"));
    checkToolBarFull->setToolTip(tr("Fullscreen"));
    checkToolBarReadList->setToolTip(tr("Reading list"));
    checkToolBarBookmarks->setToolTip(tr("Bookmarks"));
    checkToolBarHistory->setToolTip(tr("History"));
    checkToolBarDownloads->setToolTip(tr("Downloads"));
    checkToolBarSkin->setToolTip(tr("Skin editor"));
    checkToolBarConfig->setToolTip(tr("Settings"));

    QWidget *spacerIcon = new QWidget;
    spacerIcon->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    layToolBar = new QHBoxLayout;
    layToolBar->setContentsMargins(20,20,20,20);
    layToolBar->setSpacing(10);

    layToolBar->addWidget(checkToolBarAddTab);
    layToolBar->addWidget(checkToolBarBack);
    layToolBar->addWidget(checkToolBarForward);
    layToolBar->addWidget(checkToolBarReload);
    layToolBar->addWidget(checkToolBarHome);
    layToolBar->addWidget(checkToolBarOpenUrlInOtherBrowser);
    layToolBar->addWidget(checkToolBarReadList);
    layToolBar->addWidget(checkToolBarBookmarks);
    layToolBar->addWidget(checkToolBarHistory);
    layToolBar->addWidget(checkToolBarDownloads);
    layToolBar->addWidget(checkToolBarScreen);
    layToolBar->addWidget(checkToolBarFull);
    layToolBar->addWidget(checkToolBarLock);
    layToolBar->addWidget(checkToolBarSkin);
    layToolBar->addWidget(checkToolBarConfig);

    for(int i(0);i<layToolBar->count();i++)
    {
        qobject_cast<QToolButton *>(layToolBar->itemAt(i)->widget())->setIconSize(iconSize);
        qobject_cast<QToolButton *>(layToolBar->itemAt(i)->widget())->setCheckable(true);
        qobject_cast<QToolButton *>(layToolBar->itemAt(i)->widget())->setAutoRaise(true);
        qobject_cast<QToolButton *>(layToolBar->itemAt(i)->widget())->setObjectName("ToolbarIcons");
        connect(qobject_cast<QToolButton *>(layToolBar->itemAt(i)->widget()),SIGNAL(clicked()),this,SLOT(editing()));
    }

    layToolBar->addWidget(spacerIcon);

    HideBlock *blockIcons = new HideBlock(tr("Icon on the toolbar"));
    blockIcons->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    blockIcons->setBlockLayout(layToolBar);
    blockIcons->showBlock();

    // --------------------------------------------

    radioTopToolBar = new QRadioButton(tr("At the top"));
    radioBottomToolBar = new QRadioButton(tr("At the bottom"));
    connect(radioTopToolBar,SIGNAL(clicked()),this,SLOT(editing()));
    connect(radioBottomToolBar,SIGNAL(clicked()),this,SLOT(editing()));

    QGridLayout *layPosition = new QGridLayout;
    layPosition->setContentsMargins(20,20,20,20);
    layPosition->setSpacing(10);
    layPosition->addWidget(radioTopToolBar,0,0);
    layPosition->addWidget(radioBottomToolBar,1,0);

    HideBlock *blockPosition = new HideBlock(tr("Position of the toolbar"));
    blockPosition->setFixedWidth(minWidth);
    blockPosition->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    blockPosition->setBlockLayout(layPosition);
    blockPosition->showBlock();

    // --------------------------------------------

    editToolBarIconSize = new QSlider(Qt::Horizontal);
    editToolBarIconSize->setMinimum(10);
    editToolBarIconSize->setMaximum(70);
    editToolBarIconSize->setFixedHeight(35);
    editToolBarIconSize->setPageStep(2);
    connect(editToolBarIconSize,SIGNAL(valueChanged(int)),this,SLOT(editing()));
    connect(editToolBarIconSize,SIGNAL(valueChanged(int)),this,SLOT(iconSizeEditing()));

    lblIconSize = new QLabel;
    lblIconSize->setObjectName("IconSizeLabel");

    QGridLayout *layIconSize = new QGridLayout;
    layIconSize->setMargin(20);
    layIconSize->addWidget(editToolBarIconSize,0,0);
    layIconSize->addWidget(lblIconSize,0,1);

    HideBlock *blockIconSize = new HideBlock(tr("Icon size"));
    blockIconSize->setFixedWidth(minWidth);
    blockIconSize->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    blockIconSize->setBlockLayout(layIconSize);
    blockIconSize->showBlock();

    // --------------------------------------------

    radioBlackIcons = new QRadioButton(tr("Black"));
    radioWhiteIcons = new QRadioButton(tr("White"));

    connect(radioBlackIcons,SIGNAL(clicked()),this,SLOT(editing()));
    connect(radioWhiteIcons,SIGNAL(clicked()),this,SLOT(editing()));

    QVBoxLayout *layIconColor = new QVBoxLayout;
    layIconColor->setContentsMargins(20,20,20,20);
    layIconColor->setSpacing(10);
    layIconColor->addWidget(radioBlackIcons);
    layIconColor->addWidget(radioWhiteIcons);

    HideBlock *blockIconColor = new HideBlock(tr("Icon color"));
    blockIconColor->setBlockLayout(layIconColor);
    blockIconColor->setFixedWidth(minWidth);
    blockIconColor->showBlock();

    // --------------------------------------------

    radioRightCompleter = new QRadioButton(tr("Right"));
    radioLeftCompleter = new QRadioButton(tr("Left"));

    connect(radioRightCompleter,SIGNAL(clicked()),this,SLOT(editing()));
    connect(radioLeftCompleter,SIGNAL(clicked()),this,SLOT(editing()));

    QVBoxLayout *layCompleter = new QVBoxLayout;
    layCompleter->setContentsMargins(20,20,20,20);
    layCompleter->setSpacing(10);
    layCompleter->addWidget(radioRightCompleter);
    layCompleter->addWidget(radioLeftCompleter);

    HideBlock *blockCompleter = new HideBlock(tr("Position of the completer"));
    blockCompleter->setBlockLayout(layCompleter);
    blockCompleter->setFixedWidth(minWidth);
    blockCompleter->showBlock();

    // --------------------------------------------

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(blockIcons,0,0,1,3);
    mainLayout->addWidget(blockPosition,1,0);
    mainLayout->addWidget(blockIconColor,1,1);
    mainLayout->addWidget(blockIconSize,1,2);
    mainLayout->addWidget(blockCompleter,2,0);
    mainLayout->setAlignment(blockPosition,Qt::AlignTop);
    mainLayout->setAlignment(blockIconSize,Qt::AlignTop);
    mainLayout->setAlignment(blockIconColor,Qt::AlignTop);
    mainLayout->setAlignment(blockCompleter,Qt::AlignTop);

    mainLayout->addWidget(spacer,3,3);

    tabInterface = new SettingsPanel;
    tabInterface->setTitle(tr("Interface"));
    tabInterface->setMainLayout(mainLayout);
}

void ConfigWindow::genSkins()
{
    model = new QDirModel;
    listView = new QListView;
    listView->setModel(model);
    listView->setRootIndex(model->index(QCoreApplication::applicationDirPath()+QDir::separator()+"skins"));
    listView->setIconSize(QSize(32,32));
    listView->setSelectionBehavior(QAbstractItemView::SelectRows);
   // connect(listView,SIGNAL(clicked(QModelIndex)),this,SLOT(editFile()));

    checkAutoPreview = new QCheckBox(tr("Auto preview"));

    QToolButton *btnEdit = new QToolButton;
    btnEdit->setText(tr("Open in \"Skin editor\""));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(15);
    mainLayout->setMargin(0);
    mainLayout->addWidget(checkAutoPreview,0,0);
    mainLayout->addWidget(spacer,0,1);
    mainLayout->addWidget(btnEdit,0,2);
    mainLayout->addWidget(listView,1,0,1,3);

    tabSkins = new SettingsPanel;
    tabSkins->setTitle(tr("Skins"));
    tabSkins->setMainLayout(mainLayout);
    tabSkins->setEnabled(false);
}

void ConfigWindow::genDownloads()
{
    radioDownloadAskSave = new QRadioButton(tr("Ask where to save the file"));
    radioDownloadSaveAs = new QRadioButton(tr("Save the file in a specific location"));
    editDownloadPath = new QLineEdit;
    connect(editDownloadPath,SIGNAL(textEdited(QString)),this,SLOT(editing()));
    connect(radioDownloadAskSave,SIGNAL(clicked()),this,SLOT(editing()));
    connect(radioDownloadSaveAs,SIGNAL(clicked()),this,SLOT(editing()));

    editDownloadPath->setSizePolicy(QSizePolicy::Expanding,editDownloadPath->sizePolicy().verticalPolicy());

    QWidget *littleSpacer = new QWidget;
    littleSpacer->setFixedWidth(50);

    QToolButton *btnExplore = new QToolButton;
    btnExplore->setText(tr("..."));
    connect(btnExplore,SIGNAL(clicked(bool)),this,SLOT(chooseDownloadPath()));

    QGridLayout *layBeforeDownload = new QGridLayout;
    layBeforeDownload->setMargin(20);
    layBeforeDownload->setSpacing(10);
    layBeforeDownload->addWidget(radioDownloadAskSave,0,0,1,3);
    layBeforeDownload->addWidget(radioDownloadSaveAs,1,0,1,3);
    layBeforeDownload->addWidget(littleSpacer,2,0);
    layBeforeDownload->addWidget(editDownloadPath,2,1);
    layBeforeDownload->addWidget(btnExplore,2,2);

    HideBlock *blockBeforeDownload = new HideBlock(tr("Before download"));
    blockBeforeDownload->setBlockLayout(layBeforeDownload);
    blockBeforeDownload->showBlock();

    // --------------------------

    checkDownloadOpenFile = new QCheckBox(tr("Open the file"));
    checkDownloadNotification = new QCheckBox(tr("Show a notification"));
    checkDownloadRemoveList = new QCheckBox(tr("Remove from the download list"));
    connect(checkDownloadOpenFile,SIGNAL(clicked()),this,SLOT(editing()));
    connect(checkDownloadNotification,SIGNAL(clicked()),this,SLOT(editing()));
    connect(checkDownloadRemoveList,SIGNAL(clicked()),this,SLOT(editing()));

    QVBoxLayout *layAfterDownload = new QVBoxLayout;
    layAfterDownload->setMargin(20);
    layAfterDownload->setSpacing(10);
    layAfterDownload->addWidget(checkDownloadOpenFile);
    layAfterDownload->addWidget(checkDownloadNotification);
    checkDownloadNotification->setVisible(false);
    layAfterDownload->addWidget(checkDownloadRemoveList);

    HideBlock *blockAfterDownload = new HideBlock(tr("After download"));
    blockAfterDownload->setBlockLayout(layAfterDownload);
    blockAfterDownload->showBlock();

    // --------------------------

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(10);
    mainLayout->setMargin(0);
    mainLayout->addWidget(blockBeforeDownload,0,0);
    mainLayout->addWidget(blockAfterDownload,1,0);
    mainLayout->addWidget(spacer,2,1);

    tabDownloads = new SettingsPanel;
    tabDownloads->setTitle(tr("Downloads"));
    tabDownloads->setMainLayout(mainLayout);
}

void ConfigWindow::genSecurity()
{
    QLabel *lblUserName = new QLabel(tr("Pseudo"));
    editUserName = new QLineEdit;
    editUserName->setMaximumWidth(400);
    connect(editUserName,SIGNAL(textChanged(QString)),this,SLOT(editing()));

    QLabel *lblPass = new QLabel(tr("Password"));

    QToolButton *btnEditPass = new QToolButton;
    btnEditPass->setText(tr("Use a new password"));
    btnEditPass->setEnabled(false);
    connect(btnEditPass,SIGNAL(clicked(bool)),this,SLOT(choosePassword()));

    QWidget *spacer1 = new QWidget;
    spacer1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);

    QGridLayout *layUser = new QGridLayout;
    layUser->setContentsMargins(20,20,20,20);
    layUser->setSpacing(10);
    layUser->setAlignment(Qt::AlignLeft);

    layUser->addWidget(lblUserName,0,0);
    layUser->addWidget(editUserName,0,1);
    layUser->addWidget(lblPass,1,0);
    layUser->addWidget(btnEditPass,1,1);
    layUser->addWidget(spacer1,2,2);

    HideBlock *boxUser = new HideBlock(tr("Account"));
    boxUser->setBlockLayout(layUser);
    boxUser->showBlock();

    // ----------------------------

    checkLockAtStartup = new QCheckBox(tr("At Startup"));
    checkUnlockToClose = new QCheckBox(tr("Closing"));
    checkLockConfig = new QCheckBox(tr("Open Config Window"));
    checkLockLocker = new QCheckBox(tr("To Unlock"));

    connect(checkLockAtStartup,SIGNAL(clicked()),this,SLOT(editing()));
    connect(checkUnlockToClose,SIGNAL(clicked()),this,SLOT(editing()));
    connect(checkLockConfig,SIGNAL(clicked()),this,SLOT(editing()));
    connect(checkLockLocker,SIGNAL(clicked()),this,SLOT(editing()));

    QGridLayout *layAskPass = new QGridLayout;
    layAskPass->setSpacing(10);
    layAskPass->setContentsMargins(20,20,20,20);
    layAskPass->setAlignment(Qt::AlignLeft);
    layAskPass->addWidget(checkLockAtStartup);
    layAskPass->addWidget(checkUnlockToClose);
    layAskPass->addWidget(checkLockConfig);
    layAskPass->addWidget(checkLockLocker);

    HideBlock *boxAskPass = new HideBlock(tr("Ask password"));
    boxAskPass->setBlockLayout(layAskPass);
    boxAskPass->showBlock();

    // --------------------------

    QWidget *spacer2 = new QWidget;
    spacer2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(10,10,10,10);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(boxUser);
    mainLayout->addWidget(boxAskPass);
    mainLayout->addWidget(spacer2);

    tabSecurity = new SettingsPanel;
    tabSecurity->setTitle(tr("Security"));
    tabSecurity->setMainLayout(mainLayout);
}

void ConfigWindow::genOptions()
{
    checkJavaScript = new QCheckBox(tr("Enable JavaScript"));
    checkJavascriptCanOpenWindow = new QCheckBox(tr("JavaScript can open windows"));
    checkJavascriptCanAccessClipboard = new QCheckBox(tr("JavaScript can access clipboard"));

    QVBoxLayout *layNavigation = new QVBoxLayout;
    layNavigation->setMargin(20);
    layNavigation->setSpacing(10);
    layNavigation->addWidget(checkJavaScript);
    layNavigation->addWidget(checkJavascriptCanOpenWindow);
    layNavigation->addWidget(checkJavascriptCanAccessClipboard);

    HideBlock *boxNavigation = new HideBlock(tr("Navigation"));
    boxNavigation->setBlockLayout(layNavigation);
    boxNavigation->showBlock();

    // --------------------------

    checkCurrentPageOnTitleBar = new QCheckBox(tr("Display the title of the current tab in the title bar"));
    checkAnimations = new QCheckBox(tr("Enable animations"));

    QVBoxLayout *layInterface = new QVBoxLayout;
    layInterface->setMargin(20);
    layInterface->setSpacing(10);
    layInterface->addWidget(checkCurrentPageOnTitleBar);
    layInterface->addWidget(checkAnimations);

    HideBlock *boxInterface = new HideBlock(tr("Interface"));
    boxInterface->setBlockLayout(layInterface);
    boxInterface->showBlock();

    // --------------------------

    checkTabBarEnabled = new QCheckBox(tr("Enable tab bar"));
    checkTabBarClosable = new QCheckBox(tr("Closable tabs"));
    checkTabBarMovable = new QCheckBox(tr("Movable tabs"));
    checkTabBarVisibleWithLessTwoTabs = new QCheckBox(tr("Hide tab bar when it contains less than 2 tabs"));

    checkTabBarMovable->setHidden(true);
    checkTabBarVisibleWithLessTwoTabs->setHidden(true);

    QVBoxLayout *layTabs = new QVBoxLayout;
    layTabs->setMargin(20);
    layTabs->setSpacing(10);
    layTabs->addWidget(checkTabBarEnabled);
    layTabs->addWidget(checkTabBarClosable);
    layTabs->addWidget(checkTabBarMovable);
    layTabs->addWidget(checkTabBarVisibleWithLessTwoTabs);

    HideBlock *boxTabs = new HideBlock(tr("Tab bar"));
    boxTabs->setBlockLayout(layTabs);
    boxTabs->showBlock();

    // --------------------------

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0,10,0,0);
    mainLayout->addWidget(boxNavigation,0,0);
    mainLayout->addWidget(boxTabs,1,0);
    mainLayout->addWidget(boxInterface,2,0);
    mainLayout->addWidget(spacer,3,1);

    connect(checkTabBarEnabled,SIGNAL(clicked()),this,SLOT(editing()));
    connect(checkTabBarVisibleWithLessTwoTabs,SIGNAL(clicked()),this,SLOT(editing()));
    connect(checkTabBarClosable,SIGNAL(clicked()),this,SLOT(editing()));
    connect(checkTabBarMovable,SIGNAL(clicked()),this,SLOT(editing()));
    connect(checkJavaScript,SIGNAL(clicked()),this,SLOT(editing()));
    connect(checkJavascriptCanOpenWindow,SIGNAL(clicked()),this,SLOT(editing()));
    connect(checkJavascriptCanAccessClipboard,SIGNAL(clicked()),this,SLOT(editing()));
    connect(checkCurrentPageOnTitleBar,SIGNAL(clicked()),this,SLOT(editing()));
    connect(checkAnimations,SIGNAL(clicked()),this,SLOT(editing()));

    tabOptions = new SettingsPanel;
    tabOptions->setTitle(tr("Options"));
    tabOptions->setMainLayout(mainLayout);
}

void ConfigWindow::genReset()
{
    QLabel *textView = new QLabel;
    textView->setWordWrap(true);
    textView->setText(tr("If you click \"Start Reset\", you accept close browser (loose all opened tabs) and delete :<ul><li>Account</li><li>Color background</li><li>Home Page</li><li>Elements of Interface</li><!--<li>History</li>--><li>Bookmarks</li><li>Our config</li></ul>Do you continue ?"));
    textView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    QToolButton *btnResetAll = new QToolButton;
    btnResetAll->setIcon(QIcon(":/img/forward.png"));
    btnResetAll->setIconSize(QSize(0,0));
    btnResetAll->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnResetAll->setText(tr("Start reset"));
    connect(btnResetAll,SIGNAL(clicked()),this,SLOT(reset()));

    QVBoxLayout *layResetAll = new QVBoxLayout;
    layResetAll->setMargin(20);
    layResetAll->setSpacing(10);
    layResetAll->addWidget(textView);
    layResetAll->addWidget(btnResetAll);

    HideBlock *blockResetAll = new HideBlock(tr("Reset all"));
    blockResetAll->setBlockLayout(layResetAll);
    blockResetAll->showBlock();

    // --------------------------

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(10);
    mainLayout->setMargin(0);
    mainLayout->addWidget(blockResetAll);
    mainLayout->addWidget(spacer,1,1);

    tabReset = new SettingsPanel;
    tabReset->setTitle(tr("Reset"));
    tabReset->setMainLayout(mainLayout);
}

void ConfigWindow::chooseColor()
{
    QColor choosedColor = QColorDialog::getColor(editColor->color);
    if(choosedColor.name()=="#000000")
        return;

    editColor->setColor(choosedColor.name());

    editing();
}

void ConfigWindow::chooseBgImage()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), tr("Picture (*.bmp *.gif *.jpeg *.jpg *.png);;All files (*)"));
    if(path.isEmpty())
        return;

    if(sender()==editBgImg)
        editBgImg->setBgImage(path);
    else if(sender()==editProfile)
        editProfile->setBgImage(path);

    editing();
}

void ConfigWindow::chooseDownloadPath()
{
    QString path = QFileDialog::getExistingDirectory(this);
    if(path.isEmpty())
        return;

    editDownloadPath->setText(path);
}

void ConfigWindow::choosePassword()
{
    // If the user has a password, ask to enter
    if(!settings->value("Account/Password").toString().isEmpty())
    {
        bool ok;
        QString password = QInputDialog::getText(this,
                                            tr("Enter password"),
                                            tr("Please enter your current password"),
                                            QLineEdit::Password, "", &ok);
        if (!ok && password.isEmpty())
            return;

        if(hashPassword(password) != settings->value("Account/Password").toString())
        {
            QMessageBox::critical(this,tr("Error"),tr("Incorrect password or empty field"),QMessageBox::Ok);
            return;
        }
    }

    editing();
    hashedPassword = "",
        passIndice = "";

    // Now, enter the new password
    bool ok;
    QString pass1 = QInputDialog::getText(this,
                                        tr("Change password"),
                                        tr("Please enter a new password"),
                                        QLineEdit::Password, "", &ok);
    if (!ok && pass1.isEmpty())
    {
        QMessageBox::warning(this,
                                 tr("No password set"),
                                 tr("You have not entered a password, so that browser will not be secured by password."),
                                 QMessageBox::Ok);
        return;
    }

    // Next, retype the new password
    QString pass2 = QInputDialog::getText(this,
                                        tr("Change password"),
                                        tr("Please retype the new password"),
                                        QLineEdit::Password, "", &ok);
    if (!ok && pass2.isEmpty())
    {
        QMessageBox::warning(this,
                                 tr("Operation canceled"),
                                 tr("You have not retyped the password, so that browser will not be secured by password."),
                                 QMessageBox::Ok);
        return;
    }

    // If passwords don't match, retype it
    while (pass1 != pass2)
    {
        pass2 = QInputDialog::getText(this,
                                        tr("Wrong password"),
                                        tr("Passwords do not match. Please retype the new password"),
                                        QLineEdit::Password, "", &ok);

        if (!ok && pass2.isEmpty())
        {
            QMessageBox::warning(this,
                                     tr("Operation canceled"),
                                     tr("You have not retyped the password, so that browser will not be secured by password."),
                                     QMessageBox::Ok);
            return;
        }
    }

    // To finish, enter the indication
    QString indice = QInputDialog::getText(this,
                                        tr("Indication"),
                                        tr("Please type an indication for your new password"),
                                        QLineEdit::Normal, "", &ok);
    if (!ok && indice.isEmpty())
    {
        QMessageBox::warning(this,
                                 tr("Operation canceled"),
                                 tr("You have canceled the operation, so that browser will not be secured by password."),
                                 QMessageBox::Ok);
        return;
    }

    hashedPassword = hashPassword(pass1);
    passIndice = indice;
}

QString ConfigWindow::hashPassword(QString password)
{
    int i = 10;
    while(i>0)
    {
        i--;
        password = "vfzeuki78vbDFg6vzdg6hbh" + password + "QIDlivgJKys65fhb6oRFJytcdgj";
        password = QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Sha1).toHex();
    }

    return password;
}

void ConfigWindow::reset()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Settings"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(tr("Do you want to reset all settings ?"));
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
    msgBox.setButtonText(QMessageBox::No,tr("No"));
    msgBox.setButtonText(QMessageBox::Cancel,tr("Cancel"));
    int value = msgBox.exec();

    if(value == QMessageBox::Yes)
    {
        settings->clear();

        QWebEngineProfile::defaultProfile()->clearAllVisitedLinks();
        QWebEngineProfile::defaultProfile()->scripts()->clear();
        QDir folder(QWebEngineProfile::defaultProfile()->persistentStoragePath());
        folder.removeRecursively();

        qDebug()<<QWebEngineProfile::defaultProfile()->persistentStoragePath();

        QUrl url(QUrl(QCoreApplication::applicationFilePath(), QUrl::TolerantMode));
        url.setScheme("file");
        QDesktopServices::openUrl(url);

        qApp->quit();
    }
}

void ConfigWindow::editing()
{
    isEdited = true;
}

void ConfigWindow::iconSizeEditing()
{
    lblIconSize->setText(QString::number(editToolBarIconSize->value())+tr("px"));
}

void ConfigWindow::goBack()
{
    if(center->sizes().last()==0)  // If panel is hidden
        saveSettings();
    else if(center->sizes().first()==0)  // If list is hidden
    {
        QList<int> listSizes;
        listSizes << 1000 << 0;

        center->setSizes(listSizes);

        if(settings->value("Interface/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(listTab,"geometry");
            animation->setDuration(700);
            animation->setEasingCurve(QEasingCurve::OutExpo);
            animation->setStartValue(QRect(0,listTab->geometry().height(),listTab->geometry().width(),listTab->geometry().height()));
            animation->setEndValue(QRect(0,0,listTab->geometry().width(),listTab->geometry().height()));
            animation->start();
        }
    }
    else
        saveSettings();
}

void ConfigWindow::resizeSplitter()
{
    if(center->sizes().last()==0)  // If panel is hidden
    {
        QList<int> listSizes;
        listSizes << 0 << 1000;

        center->setSizes(listSizes);

        if(settings->value("Interface/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(central,"geometry");
            animation->setDuration(450);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            animation->setStartValue(QRect(central->geometry().x(),-100,central->geometry().width(),central->geometry().height()+100));
            animation->setEndValue(QRect(central->geometry().x(),0,central->geometry().width(),central->geometry().height()));
            animation->start();
        }
    }
}

void ConfigWindow::loadSettings()
{
    // Toolbar position
    if(settings->value("Interface/Position/Toolbar").toString()=="top")
        centralLayout->setDirection(QBoxLayout::TopToBottom);
    else
        centralLayout->setDirection(QBoxLayout::BottomToTop);

    // Icon Size
    int size = settings->value("Interface/ToolBar/IconSize").toInt();
    iconSize = QSize(size,size);

    mainToolBar->setIconSize(iconSize);
    btnMenu->setIconSize(iconSize);

    for(int i(0);i<layToolBar->count()-1;i++)
        qobject_cast<QToolButton *>(layToolBar->itemAt(i)->widget())->setIconSize(iconSize);

    // Icon Color
    iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    btnMenu->setIcon(QIcon(iconColor+"menu.png"));
    actBack->setIcon(QIcon(iconColor+"back.png"));
    actRefresh->setIcon(QIcon(iconColor+"reload.png"));

    checkToolBarAddTab->setIcon(QIcon(iconColor+"add.png"));
    checkToolBarBack->setIcon(QIcon(iconColor+"back.png"));
    checkToolBarForward->setIcon(QIcon(iconColor+"forward.png"));
    checkToolBarReload->setIcon(QIcon(iconColor+"reload.png"));
    checkToolBarHome->setIcon(QIcon(iconColor+"home.png"));
    checkToolBarOpenUrlInOtherBrowser->setIcon(QIcon(iconColor+"external.png"));
    checkToolBarSkin->setIcon(QIcon(iconColor+"skin.png"));
    checkToolBarLock->setIcon(QIcon(iconColor+"lock.png"));
    checkToolBarConfig->setIcon(QIcon(iconColor+"config.png"));
    checkToolBarFull->setIcon(QIcon(iconColor+"full-screen.png"));
    checkToolBarReadList->setIcon(QIcon(iconColor+"reading-list.png"));
    checkToolBarBookmarks->setIcon(QIcon(iconColor+"bookmarks.png"));
    checkToolBarHistory->setIcon(QIcon(iconColor+"history.png"));
    checkToolBarDownloads->setIcon(QIcon(iconColor+"downloads.png"));
    checkToolBarScreen->setIcon(QIcon(iconColor+"screen.png"));

    // CUSTOMIZE
    editColor->setColor(settings->value("Interface/Color").toString());
    editBgImg->setBgImage(settings->value("Interface/BgImgUrl").toString());
    editProfile->setBgImage(settings->value("Account/Photo").toString());

    // NAVIGATION
    radioEmptyStartup->setChecked(settings->value("Navigation/Startup/Empty").toBool());
    radioHomeStartup->setChecked(settings->value("Navigation/Startup/Home").toBool());
    radioSpecificStartup->setChecked(settings->value("Navigation/Startup/Specific").toBool());
    editSpecificStartup->setText(settings->value("Navigation/Startup/SpecificPage").toString());

    radioEmptyNewTab->setChecked(settings->value("Navigation/NewTab/Empty").toBool());
    radioHomeNewTab->setChecked(settings->value("Navigation/NewTab/Home").toBool());
    radioSpecificNewTab->setChecked(settings->value("Navigation/NewTab/Specific").toBool());
    editSpecificNewTab->setText(settings->value("Navigation/NewTab/SpecificPage").toString());

    editHomePage->setText(settings->value("Navigation/Home").toString());
    editPrefixSearchUrl->setText(settings->value("Navigation/PrefixSearch").toString());
    editSuffixSearchUrl->setText(settings->value("Navigation/SuffixSearch").toString());

    checkCustomUserAgent->setChecked(settings->value("Navigation/UserAgent/Enable").toBool());
    editUserAgent->setText(settings->value("Navigation/UserAgent/Value").toString());
    editUserAgent->setVisible(settings->value("Navigation/UserAgent/Enable").toBool());

    // ToolBar
    if(settings->value("Interface/Position/Toolbar").toString()=="top")
        radioTopToolBar->setChecked(true);
    else if(settings->value("Interface/Position/Toolbar").toString()=="bottom")
        radioBottomToolBar->setChecked(true);

    editToolBarIconSize->setValue(settings->value("Interface/ToolBar/IconSize").toInt());

    checkToolBarAddTab->setChecked(settings->value("Interface/ToolBar/AddTab").toBool());
    checkToolBarBack->setChecked(settings->value("Interface/ToolBar/Back").toBool());
    checkToolBarForward->setChecked(settings->value("Interface/ToolBar/Forward").toBool());
    checkToolBarReload->setChecked(settings->value("Interface/ToolBar/Reload").toBool());
    checkToolBarHome->setChecked(settings->value("Interface/ToolBar/Home").toBool());
    checkToolBarOpenUrlInOtherBrowser->setChecked(settings->value("Interface/ToolBar/OpenUrlInOtherBrowser").toBool());
    checkToolBarSkin->setChecked(settings->value("Interface/ToolBar/Skin").toBool());
    checkToolBarLock->setChecked(settings->value("Interface/ToolBar/Lock").toBool());
    checkToolBarConfig->setChecked(settings->value("Interface/ToolBar/Config").toBool());
    checkToolBarFull->setChecked(settings->value("Interface/ToolBar/Full").toBool());
    checkToolBarReadList->setChecked(settings->value("Interface/ToolBar/ReadList").toBool());
    checkToolBarBookmarks->setChecked(settings->value("Interface/ToolBar/Bookmarks").toBool());
    checkToolBarHistory->setChecked(settings->value("Interface/ToolBar/History").toBool());
    checkToolBarDownloads->setChecked(settings->value("Interface/ToolBar/Downloads").toBool());
    checkToolBarScreen->setChecked(settings->value("Interface/ToolBar/Screen").toBool());

    if(settings->value("Interface/IconColor").toString()=="black")
    {
        radioBlackIcons->setChecked(true);
        radioWhiteIcons->setChecked(false);
    }
    else if(settings->value("Interface/IconColor").toString()=="white")
    {
        radioBlackIcons->setChecked(false);
        radioWhiteIcons->setChecked(true);
    }

    if(settings->value("Interface/ToolBar/Completer")=="right")
        radioRightCompleter->setChecked(true);
    else
        radioLeftCompleter->setChecked(true);

    // SKINS
    checkAutoPreview->setChecked(settings->value("Interface/Skins/AutoPreview").toBool());

    // DOWNLOADS
    if(settings->value("Downloads/AskSave").toBool())
    {
        radioDownloadAskSave->setChecked(true);
        radioDownloadSaveAs->setChecked(false);
    }
    else
    {
        radioDownloadAskSave->setChecked(false);
        radioDownloadSaveAs->setChecked(true);
    }
    editDownloadPath->setText(settings->value("Downloads/DownloadPath").toString());

    checkDownloadOpenFile->setChecked(settings->value("Downloads/OpenAfterDownload").toBool());
    checkDownloadNotification->setChecked(settings->value("Downloads/ShowNotification").toBool());
    checkDownloadRemoveList->setChecked(settings->value("Downloads/RemoveList").toBool());

    // SECURITY
    editUserName->setText(settings->value("Account/Pseudo").toString());
    hashedPassword = settings->value("Account/Password").toString();
    passIndice = settings->value("Account/Indice").toString();
    checkLockAtStartup->setChecked(settings->value("Security/Lock/AtStart").toBool());
    checkUnlockToClose->setChecked(settings->value("Security/Lock/AtClose").toBool());
    checkLockConfig->setChecked(settings->value("Security/Lock/ToConfig").toBool());
    checkLockLocker->setChecked(settings->value("Security/Lock/Locker").toBool());

    // BOOKMARKS
   // bkmManager->updateItems();

    // OPTIONS
    checkJavaScript->setChecked(settings->value("Navigation/Options/EnableJavaScript").toBool());
    checkJavascriptCanOpenWindow->setChecked(settings->value("Navigation/Options/EnableJavascriptCanOpenWindow").toBool());
    checkJavascriptCanAccessClipboard->setChecked(settings->value("Navigation/Options/EnableJavascriptCanAccessClipboard").toBool());
    checkCurrentPageOnTitleBar->setChecked(settings->value("Navigation/Options/CurrentPageOnTitleBar").toBool());
    checkAnimations->setChecked(settings->value("Interface/Animations").toBool());
    checkTabBarEnabled->setChecked(settings->value("Interface/Tabbar/Enable").toBool());
    checkTabBarVisibleWithLessTwoTabs->setChecked(settings->value("Interface/Tabbar/VisibleWithLessTwoTabs").toBool());
    checkTabBarClosable->setChecked(settings->value("Interface/Tabbar/Closable").toBool());
    checkTabBarMovable->setChecked(settings->value("Interface/Tabbar/Movable").toBool());

    central->setCurrentIndex(0);
    listTab->setCurrent(1);

    isEdited = false;
}

void ConfigWindow::saveSettings()
{
    if(isEdited)
    {
        if(editUserName->text().isEmpty())
        {
            QMessageBox::critical(this,
                                  tr("Empty field"),
                                  tr("The field \"Pseudo\" is empty"),
                                  QMessageBox::Ok);

            return;
        }

        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Settings"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("Do you want to save the new settings ?"));
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
        msgBox.setButtonText(QMessageBox::No,tr("No"));
        msgBox.setButtonText(QMessageBox::Cancel,tr("Cancel"));
        int value = msgBox.exec();

        if(value == QMessageBox::Yes)
        {
            settings->setValue("Version",QCoreApplication::applicationVersion());

            settings->setValue("Interface/Color",editColor->color);
            settings->setValue("Interface/BgImgUrl",editBgImg->bgImage);
            if(radioBlackIcons->isChecked())
                settings->setValue("Interface/IconColor","black");
            else
                settings->setValue("Interface/IconColor","white");

            settings->setValue("Navigation/Startup/Empty",radioEmptyStartup->isChecked());
            settings->setValue("Navigation/Startup/Home",radioHomeStartup->isChecked());
            settings->setValue("Navigation/Startup/Specific",radioSpecificStartup->isChecked());
            settings->setValue("Navigation/Startup/SpecificPage",editSpecificStartup->text());
            settings->setValue("Navigation/NewTab/Empty",radioEmptyNewTab->isChecked());
            settings->setValue("Navigation/NewTab/Home",radioHomeNewTab->isChecked());
            settings->setValue("Navigation/NewTab/Specific",radioSpecificNewTab->isChecked());
            settings->setValue("Navigation/NewTab/SpecificPage",editSpecificNewTab->text());
            settings->setValue("Navigation/Home",editHomePage->text());
            settings->setValue("Navigation/PrefixSearch",editPrefixSearchUrl->text());
            settings->setValue("Navigation/SuffixSearch",editSuffixSearchUrl->text());
            settings->setValue("Navigation/UserAgent/Enable",checkCustomUserAgent->isChecked());
            settings->setValue("Navigation/UserAgent/Value",editUserAgent->text());

            settings->setValue("Interface/Tabbar/Enable",checkTabBarEnabled->isChecked());
            settings->setValue("Interface/Tabbar/VisibleWithLessTwoTabs",checkTabBarVisibleWithLessTwoTabs->isChecked());
            settings->setValue("Interface/Tabbar/Closable",checkTabBarClosable->isChecked());
            settings->setValue("Interface/Tabbar/Movable",checkTabBarMovable->isChecked());

            if(radioTopToolBar->isChecked())
                settings->setValue("Interface/Position/Toolbar","top");
            else if(radioBottomToolBar->isChecked())
                settings->setValue("Interface/Position/Toolbar","bottom");

            settings->setValue("Interface/ToolBar/IconSize",editToolBarIconSize->value());

            settings->setValue("Interface/ToolBar/AddTab",checkToolBarAddTab->isChecked());
            settings->setValue("Interface/ToolBar/Back",checkToolBarBack->isChecked());
            settings->setValue("Interface/ToolBar/Forward",checkToolBarForward->isChecked());
            settings->setValue("Interface/ToolBar/Reload",checkToolBarReload->isChecked());
            settings->setValue("Interface/ToolBar/Home",checkToolBarHome->isChecked());
            settings->setValue("Interface/ToolBar/OpenUrlInOtherBrowser",checkToolBarOpenUrlInOtherBrowser->isChecked());
            settings->setValue("Interface/ToolBar/Skin",checkToolBarSkin->isChecked());
            settings->setValue("Interface/ToolBar/Lock",checkToolBarLock->isChecked());
            settings->setValue("Interface/ToolBar/Config",checkToolBarConfig->isChecked());
            settings->setValue("Interface/ToolBar/Full",checkToolBarFull->isChecked());
            settings->setValue("Interface/ToolBar/ReadList",checkToolBarReadList->isChecked());
            settings->setValue("Interface/ToolBar/Bookmarks",checkToolBarBookmarks->isChecked());
            settings->setValue("Interface/ToolBar/History",checkToolBarHistory->isChecked());
            settings->setValue("Interface/ToolBar/Downloads",checkToolBarDownloads->isChecked());
            settings->setValue("Interface/ToolBar/Screen",checkToolBarScreen->isChecked());
            if(radioLeftCompleter->isChecked())
                settings->setValue("Interface/ToolBar/Completer","left");
            else
                settings->setValue("Interface/ToolBar/Completer","right");

            settings->setValue("Interface/Skins/AutoPreview",checkAutoPreview->isChecked());

            settings->setValue("Downloads/AskSave",radioDownloadAskSave->isChecked());
            settings->setValue("Downloads/DownloadPath",editDownloadPath->text());
            settings->setValue("Downloads/OpenAfterDownload",checkDownloadOpenFile->isChecked());
            settings->setValue("Downloads/ShowNotification",checkDownloadNotification->isChecked());
            settings->setValue("Downloads/RemoveList",checkDownloadRemoveList->isChecked());

            settings->setValue("Account/Photo",editProfile->bgImage);
            settings->setValue("Account/Pseudo",editUserName->text());
            settings->setValue("Account/Password",hashedPassword);
            settings->setValue("Account/Indice",passIndice);

            settings->setValue("Security/Lock/AtStart",checkLockAtStartup->isChecked());
            settings->setValue("Security/Lock/AtClose",checkUnlockToClose->isChecked());
            settings->setValue("Security/Lock/ToConfig",checkLockConfig->isChecked());
            settings->setValue("Security/Lock/Locker",checkLockLocker->isChecked());

            settings->setValue("Navigation/Options/EnableJavaScript",checkJavaScript->isChecked());
            settings->setValue("Navigation/Options/EnableJavascriptCanOpenWindow",checkJavascriptCanOpenWindow->isChecked());
            settings->setValue("Navigation/Options/EnableJavascriptCanAccessClipboard",checkJavascriptCanAccessClipboard->isChecked());
            settings->setValue("Navigation/Options/CurrentPageOnTitleBar",checkCurrentPageOnTitleBar->isChecked());
            settings->setValue("Interface/Animations",checkAnimations->isChecked());

          //  bkmManager->saveBookmarks();
        }
        if(value == QMessageBox::Cancel)
            return;
    }

    isEdited = false;
    emit showBrowser();
}

// PANEL

SettingsPanel::SettingsPanel(QWidget *parent) :
    QFrame(parent)
{
    setObjectName("Panel");

    lblTitle = new QLabel;
    lblTitle->setObjectName("Title");
    lblTitle->setWordWrap(true);

    scrollWidget = new QScrollArea;
    scrollWidget->setWidgetResizable(true);
    scrollWidget->setAttribute(Qt::WA_AcceptTouchEvents, true);

    mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(lblTitle,0,0);
    mainLayout->addWidget(scrollWidget,1,0);
    setLayout(mainLayout);
}

void SettingsPanel::setTitle(QString title)
{
    lblTitle->setText(title);
}

void SettingsPanel::setMainLayout(QLayout *layout)
{
    layout->setContentsMargins(35,5,35,35);
    QFrame *widget = new QFrame;
    widget->setLayout(layout);

    scrollWidget->setWidget(widget);
    scrollWidget->setWidgetResizable(true);
}

QString SettingsPanel::title()
{
    return lblTitle->text();
}

// TABLIST

TabList::TabList(QWidget *parent) :
    QFrame(parent)
{
    setObjectName("TabList");
    QLabel *title = new QLabel(tr("Settings"));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);

    layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(title);
    layout->addWidget(spacer);
    setLayout(layout);

    map = new QSignalMapper;
    connect(map,SIGNAL(mapped(int)),this,SIGNAL(showTab(int)));
}

void TabList::addTab(QString title, int index)
{
    QToolButton *btnItem = new QToolButton;
    btnItem->setText(title);
    btnItem->setIcon(QIcon(":/img/menu.png"));
    btnItem->setIconSize(QSize(0,0));
    btnItem->setCheckable(true);
    btnItem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnItem->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    layout->insertWidget(layout->count()-1,btnItem);
    connect(btnItem,SIGNAL(clicked()),map,SLOT(map()));
    connect(btnItem,SIGNAL(clicked()),this,SLOT(setCurrent()));

    map->setMapping(btnItem,index);
}

QString TabList::nameAt(int index)
{
    QToolButton *item = qobject_cast<QToolButton *>(layout->itemAt(index)->widget());
    return item->text();
}

void TabList::setCurrent()
{
    for(int i(1);i<=layout->count()-2;i++)
        qobject_cast<QToolButton *>(layout->itemAt(i)->widget())->setChecked(false);

    qobject_cast<QToolButton *>(sender())->setChecked(true);
}

void TabList::setCurrent(int index)
{
    for(int i(1);i<=layout->count()-2;i++)
        qobject_cast<QToolButton *>(layout->itemAt(i)->widget())->setChecked(false);

    qobject_cast<QToolButton *>(layout->itemAt(index)->widget())->setChecked(true);
}
