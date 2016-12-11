#include "mainmenu.h"

MainMenu::MainMenu(QWidget *parent) : QFrame(parent)
{
    QLabel *lblTitle = new QLabel(tr("Main menu"));
    lblTitle->setObjectName("Title");
    lblTitle->setWordWrap(true);
    lblTitle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    QToolBar *actionsBar = new QToolBar;
    actionsBar->setObjectName("ActionsBar");
    actionsBar->addWidget(lblTitle);
   // actionsBar->addWidget(actMenu);

    genFileMenu();
    genNavigMenu();
    genViewMenu();
    genToolsMenu();

    actGeneral = new QAction(tr("General"),this);
   // connect(actGeneral,SIGNAL(hovered()),this,SIGNAL(showGeneral()));
    connect(actGeneral,SIGNAL(triggered(bool)),this,SIGNAL(showGeneral()));

    actTabs = new QAction(tr("Tabs"),this);
   // connect(actTabs,SIGNAL(hovered()),this,SIGNAL(showTabs()));
    connect(actTabs,SIGNAL(triggered(bool)),this,SIGNAL(showTabs()));

    actReadingList = new QAction(tr("Reading list"),this);
   // connect(actReadingList,SIGNAL(hovered()),this,SIGNAL(showReadingList()));
    connect(actReadingList,SIGNAL(triggered(bool)),this,SIGNAL(showReadingList()));

    actBookmarks = new QAction(tr("Bookmarks"),this);
   // connect(actBookmarks,SIGNAL(hovered()),this,SIGNAL(showBookmarks()));
    connect(actBookmarks,SIGNAL(triggered(bool)),this,SIGNAL(showBookmarks()));

    actHistory = new QAction(tr("History"),this);
   // connect(actHistory,SIGNAL(hovered()),this,SIGNAL(showHistory()));
    connect(actHistory,SIGNAL(triggered(bool)),this,SIGNAL(showHistory()));

    actDownloads = new QAction(tr("Downloads"),this);
    actDownloads->setVisible(false);
   // connect(actDownloads,SIGNAL(hovered()),this,SIGNAL(showDownloads()));
    connect(actDownloads,SIGNAL(triggered(bool)),this,SIGNAL(showDownloads()));

    // act Msgs


    btnLock = new QToolButton;
    btnLock->setText(tr("Lock"));

    btnSkinEdit = new QToolButton;
    btnSkinEdit->setText(tr("Skin editor"));

    btnSettings = new QToolButton;
    btnSettings->setText(tr("Settings"));

    btnAbout = new QToolButton;
    btnAbout->setText(tr("About ")+QCoreApplication::applicationName());

    btnQuit = new QToolButton;
    btnQuit->setText(tr("Quit"));

    QWidget *spacer1 = new QWidget;
    spacer1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QWidget *spacer2 = new QWidget;
    spacer2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);

    barItem = new QToolBar;
    barItem->setOrientation(Qt::Vertical);
    barItem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    barItem->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    barItem->setObjectName("Items");
    barItem->addAction(actGeneral);
    barItem->addAction(actTabs);
    barItem->addAction(actReadingList);
    barItem->addAction(actBookmarks);
    barItem->addAction(actHistory);
    barItem->addAction(actDownloads);

    for(int i(0);i<barItem->actions().count();i++)
    {
        barItem->actions().at(i)->setCheckable(true);
        connect(barItem->actions().at(i),SIGNAL(triggered(bool)),this,SLOT(updateSelectedButton()));
    }

    toolBar = new QToolBar;
    toolBar->setObjectName("QuickBar");
    toolBar->addWidget(spacer2);
    toolBar->addWidget(btnLock);
    toolBar->addWidget(btnSkinEdit);
    toolBar->addWidget(btnSettings);
    toolBar->addWidget(btnAbout);
    toolBar->addWidget(btnQuit);

    QVBoxLayout *menuLayout = new QVBoxLayout;
    menuLayout->setMargin(0);
    menuLayout->addWidget(barItem);
    menuLayout->addWidget(spacer1);
    menuLayout->addWidget(toolBar);

    QFrame *widget = new QFrame;
    widget->setLayout(menuLayout);

    QScrollArea *scrollWidget = new QScrollArea;
    scrollWidget->setWidget(widget);
    scrollWidget->setWidgetResizable(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(actionsBar);
    mainLayout->addWidget(scrollWidget);

    setObjectName("ListMenu");
    setLayout(mainLayout);
}

MainMenu::~MainMenu()
{

}

void MainMenu::genFileMenu()
{
    screenFile = new QFrame;
}

void MainMenu::genNavigMenu()
{
    screenNavig = new QFrame;
}

void MainMenu::genViewMenu()
{
    screenView = new QFrame;
}

void MainMenu::genToolsMenu()
{
    screenTools = new QFrame;
}

void MainMenu::updateSelectedButton()
{

    for(int i(0);i<barItem->actions().count();i++)
        barItem->actions().at(i)->setChecked(false);

    qobject_cast<QAction *>(sender())->setChecked(true);
}

void MainMenu::setSelectedButton(int index)
{

    for(int i(0);i<barItem->actions().count();i++)
        barItem->actions().at(i)->setChecked(false);

    if(index<barItem->actions().count())
        barItem->actions().at(index)->setChecked(true);
}

void MainMenu::loadSettings()
{
    QSettings *settings = new QSettings;
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();
    iconSize = 16;

    barItem->setIconSize(QSize(iconSize,iconSize));
    toolBar->setIconSize(QSize(iconSize,iconSize));

    actGeneral->setIcon(QIcon(iconColor + "menu.png"));
    actTabs->setIcon(QIcon(iconColor + "tabs.png"));
    actReadingList->setIcon(QIcon(iconColor + "reading-list.png"));
    actBookmarks->setIcon(QIcon(iconColor + "bookmarks.png"));
    actHistory->setIcon(QIcon(iconColor + "history.png"));
    actDownloads->setIcon(QIcon(iconColor + "downloads.png"));
    btnLock->setIcon(QIcon(iconColor + "lock.png"));
    btnSkinEdit->setIcon(QIcon(iconColor + "skin.png"));
    btnSettings->setIcon(QIcon(iconColor + "config.png"));
    btnAbout->setIcon(QIcon(iconColor + "info.png"));
    btnQuit->setIcon(QIcon(iconColor + "cross.png"));
}
