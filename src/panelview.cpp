#include "panelview.h"

PanelView::PanelView(QWidget *parent) : QFrame(parent)
{
    setFixedWidth(350);
    setObjectName("PanelView");

    listMenu = new ListMenu;
    listMenu->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    tab1 = new MenuPanel;
    tab2 = new ReadingList;
    tab3 = new BookmarksPanel;
    tab4 = new HistoryPanel;
    tab5 = new DownloadsPanel;
    tab6 = new AboutSoft;

    connect(listMenu,SIGNAL(showGeneral()),this,SLOT(showGeneral()));
    connect(listMenu,SIGNAL(showReadingList()),this,SLOT(showReadingList()));
    connect(listMenu,SIGNAL(showBookmarks()),this,SLOT(showBookmarks()));
    connect(listMenu,SIGNAL(showHistory()),this,SLOT(showHistory()));
    connect(listMenu,SIGNAL(showDownloads()),this,SLOT(showDownloads()));

    /*connect(listMenu,SIGNAL(showGeneral()),this,SLOT(showMenu()));
    connect(listMenu,SIGNAL(showReadingList()),this,SLOT(showMenu()));
    connect(listMenu,SIGNAL(showBookmarks()),this,SLOT(showMenu()));
    connect(listMenu,SIGNAL(showHistory()),this,SLOT(showMenu()));
    connect(listMenu,SIGNAL(showDownloads()),this,SLOT(showMenu()));*/

    connect(tab1->actBack,SIGNAL(triggered(bool)),this,SLOT(showMenu()));
    connect(tab2->actBack,SIGNAL(triggered(bool)),this,SLOT(showMenu()));
    connect(tab3->actBack,SIGNAL(triggered(bool)),this,SLOT(showMenu()));
    connect(tab4->actBack,SIGNAL(triggered(bool)),this,SLOT(showMenu()));
    connect(tab5->actBack,SIGNAL(triggered(bool)),this,SLOT(showMenu()));
    connect(tab6->actBack,SIGNAL(triggered(bool)),this,SLOT(showMenu()));

    QHBoxLayout *layCentral = new QHBoxLayout;
    layCentral->setMargin(0);
    layCentral->addWidget(listMenu);

    setLayout(layCentral);

    central = new QStackedWidget(listMenu);
    central->hide();
    central->addWidget(tab1);
    central->addWidget(tab2);
    central->addWidget(tab3);
    central->addWidget(tab4);
    central->addWidget(tab5);
    central->addWidget(tab6);
}

PanelView::~PanelView()
{

}

void PanelView::resizeEvent(QResizeEvent *event)
{
    if(central->isVisible())
        central->setGeometry(QRect(0,0,350,this->geometry().height()));
    else
        central->setGeometry(QRect(360,0,350,this->geometry().height()));

    event->accept();
}

void PanelView::showGeneral()
{
    central->setCurrentWidget(tab1);

    if(central->isHidden())
        showMenu();
}

void PanelView::showReadingList()
{
    central->setCurrentWidget(tab2);

    tab2->updateItems();

    if(central->isHidden())
        showMenu();
}

void PanelView::showBookmarks()
{
    central->setCurrentWidget(tab3);

    tab3->updateItems();

    if(central->isHidden())
        showMenu();
}

void PanelView::showHistory()
{
    central->setCurrentWidget(tab4);

    if(central->isHidden())
        showMenu();
}

void PanelView::showDownloads()
{
    central->setCurrentWidget(tab5);

    if(central->isHidden())
        showMenu();
}

void PanelView::showAbout()
{
    central->setCurrentWidget(tab6);

    if(central->isHidden())
        showMenu();
}

void PanelView::showMenu()
{
    QSettings *settings = new QSettings;

    if(central->isHidden())
    {
        central->setGeometry(QRect(0,0,350,this->geometry().height()));
        central->show();

        if(settings->value("Interface/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(central,"geometry");
            animation->setDuration(700);
            animation->setEasingCurve(QEasingCurve::OutExpo);
            animation->setStartValue(QRect(0,central->geometry().height(),central->geometry().width(),central->geometry().height()));
            animation->setEndValue(QRect(0,0,central->geometry().width(),central->geometry().height()));
            animation->start();
        }
    }
    else
    {
      /*  if(settings->value("Interface/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(listMenu,"geometry");
            animation->setDuration(500);
            animation->setEasingCurve(QEasingCurve::OutExpo);
            animation->setStartValue(QRect(0,-(central->geometry().height()/2),listMenu->geometry().width(),listMenu->geometry().height()));
            animation->setEndValue(QRect(0,0,listMenu->geometry().width(),listMenu->geometry().height()));
            animation->start();
        }*/

        if(settings->value("Interface/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(central,"geometry");
            animation->setDuration(700);
            animation->setEasingCurve(QEasingCurve::OutExpo);
            animation->setStartValue(QRect(0,0,central->geometry().width(),central->geometry().height()));
            animation->setEndValue(QRect(0,central->geometry().height(),central->geometry().width(),central->geometry().height()));
            animation->start();
            connect(animation,SIGNAL(finished()),central,SLOT(hide()));
        }
        else
        {
            central->setGeometry(QRect(360,0,350,this->geometry().height()));
            central->hide();
        }
    }
}

QString PanelView::currentScreen()
{
    QString value;
    if(central->isHidden())
        value = "home";
    else
    {
        if(central->currentWidget()==tab1)
            value = "menu";
        if(central->currentWidget()==tab2)
            value = "readlist";
        if(central->currentWidget()==tab3)
            value = "bookmarks";
        if(central->currentWidget()==tab4)
            value = "history";
        if(central->currentWidget()==tab5)
            value = "downloads";
    }
    return value;
}

void PanelView::loadSettings()
{
    listMenu->loadSettings();
    tab1->loadSettings();
    tab2->updateItems();
    tab3->updateItems();
    tab4->loadSettings();
    tab5->loadSettings();
    tab6->loadSettings();
}

// List Menu

ListMenu::ListMenu(QWidget *parent) : QFrame(parent)
{
    QLabel *lblTitle = new QLabel(tr("Main menu"));
    lblTitle->setObjectName("Title");
    lblTitle->setWordWrap(true);
    lblTitle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    QToolBar *actionsBar = new QToolBar;
    actionsBar->setObjectName("ActionsBar");
    actionsBar->addWidget(lblTitle);

    genFileMenu();
    genNavigMenu();
    genViewMenu();
    genToolsMenu();

    actGeneral = new QAction(tr("All actions"),this);
    connect(actGeneral,SIGNAL(triggered(bool)),this,SIGNAL(showGeneral()));

    actReadingList = new QAction(tr("Reading list"),this);
    connect(actReadingList,SIGNAL(triggered(bool)),this,SIGNAL(showReadingList()));

    actBookmarks = new QAction(tr("Bookmarks"),this);
    connect(actBookmarks,SIGNAL(triggered(bool)),this,SIGNAL(showBookmarks()));

    actHistory = new QAction(tr("History"),this);
    connect(actHistory,SIGNAL(triggered(bool)),this,SIGNAL(showHistory()));

    actDownloads = new QAction(tr("Downloads"),this);
    //actDownloads->setVisible(false);
    connect(actDownloads,SIGNAL(triggered(bool)),this,SIGNAL(showDownloads()));

    btnScreen = new QAction(tr("Take a screenshot"),this);
    btnLock = new QAction(tr("Lock"),this);
    btnSkinEdit = new QAction(tr("Skin editor"),this);
    btnSettings = new QAction(tr("Settings"),this);
    btnAbout = new QAction(tr("About %1").arg(QCoreApplication::applicationName()),this);
    btnQuit = new QAction(tr("Quit"),this);

    listActs << actGeneral << actReadingList << actBookmarks << actHistory << actDownloads << btnScreen << btnLock << btnSkinEdit << btnSettings << btnAbout << btnQuit;

    menuLayout = new QGridLayout;
    menuLayout->setMargin(0);
    menuLayout->setSpacing(0);

    QFrame *widget = new QFrame;
    widget->setObjectName("Items");
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

ListMenu::~ListMenu()
{

}

void ListMenu::genFileMenu()
{
    screenFile = new QFrame;
}

void ListMenu::genNavigMenu()
{
    screenNavig = new QFrame;
}

void ListMenu::genViewMenu()
{
    screenView = new QFrame;
}

void ListMenu::genToolsMenu()
{
    screenTools = new QFrame;
}

void ListMenu::loadSettings()
{
    QSettings *settings = new QSettings;
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    //int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();

   // barItem->setIconSize(QSize(iconSize,iconSize));

    actGeneral->setIcon(QIcon(iconColor + "empty.png"));
    actReadingList->setIcon(QIcon(iconColor + "reading-list.png"));
    actBookmarks->setIcon(QIcon(iconColor + "bookmarks.png"));
    actHistory->setIcon(QIcon(iconColor + "history.png"));
    actDownloads->setIcon(QIcon(iconColor + "downloads.png"));
    btnScreen->setIcon(QIcon(iconColor + "screen.png"));
    btnLock->setIcon(QIcon(iconColor + "lock.png"));
    btnSkinEdit->setIcon(QIcon(iconColor + "skin.png"));
    btnSettings->setIcon(QIcon(iconColor + "config.png"));
    btnAbout->setIcon(QIcon(iconColor + "info.png"));
    btnQuit->setIcon(QIcon(iconColor + "cross.png"));

    // Clean Layout
    QLayoutItem *item;
    while ((item = menuLayout->takeAt(0)) != 0)
    {
        item->widget()->deleteLater();
        delete item;
    }

    // Update icons
    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    int i = 0;
    for(i;i<listActs.count();i++)
    {
        QToolButton *btnItem = new QToolButton;
        btnItem->setDefaultAction(listActs.at(i));
        btnItem->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);
        btnItem->setMinimumHeight(100);
        btnItem->setMaximumHeight(130);
        btnItem->setIconSize(QSize(0,0));

        QLabel *lblIcon = new QLabel;
        lblIcon->setPixmap(listActs.at(i)->icon().pixmap(25,25));
        lblIcon->setMouseTracking(false);
        lblIcon->setObjectName("Icon");
        lblIcon->setAlignment(Qt::AlignCenter);
        lblIcon->setMinimumHeight(35);
        QLabel *lblText = new QLabel(listActs.at(i)->text());
        lblText->setWordWrap(true);
        lblText->setMouseTracking(false);
        lblText->setAlignment(Qt::AlignCenter);
        lblText->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        lblText->setObjectName("Text");

        QVBoxLayout *layBtn = new QVBoxLayout;
        layBtn->setMargin(0);
        layBtn->setSpacing(5);
        layBtn->addWidget(lblIcon);
        layBtn->setAlignment(lblText,Qt::AlignCenter);
        layBtn->addWidget(lblText);
        layBtn->setAlignment(lblText,Qt::AlignCenter);
        layBtn->setAlignment(Qt::AlignCenter);
        btnItem->setLayout(layBtn);

        int mod = i%3;
        if(mod==0)
            menuLayout->addWidget(btnItem,i,mod);
        if(mod==1)
            menuLayout->addWidget(btnItem,i-1,mod);
        if(mod==2)
            menuLayout->addWidget(btnItem,i-2,mod);
    }

    menuLayout->addWidget(spacer,i+1,0,1,2);
}
