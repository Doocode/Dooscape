#include "tabbar.h"

TabBar::TabBar(QWidget *parent) : Panel(parent)
{
    setObjectName("TabBar");
    lblTitle->setText(tr("Tabs"));
    settings = new QSettings;
    iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();

    setMouseTracking(true);
    setAttribute(Qt::WA_NoMousePropagation,false);

    centralWidget = new QStackedWidget;
    centralWidget->setContentsMargins(0,0,0,0);
    centralWidget->addWidget(genTabsOpenWidget());
    centralWidget->addWidget(genTabsClosedWidget());

    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(centralWidget);
    scrollArea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    connect(searchBar,SIGNAL(textEdited(QString)),this,SLOT(searchTab(QString)));

    genActs();

    toolBar->setIconSize(QSize(iconSize,iconSize));
    toolBar->addAction(actAddTab);
    toolBar->addAction(actDuplicateTab);
    toolBar->addAction(actClosedTabs);
    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    toolBar->addWidget(spacer);
    toolBar->addAction(actSearch);

    openTabsMaps = new QSignalMapper(this);
    connect(openTabsMaps,SIGNAL(mapped(int)),this,SIGNAL(openTabs(int)));

    closeTabsMaps = new QSignalMapper(this);
    connect(closeTabsMaps,SIGNAL(mapped(int)),this,SIGNAL(closeTabs(int)));

    restoreTabsMaps = new QSignalMapper(this);
    connect(restoreTabsMaps,SIGNAL(mapped(int)),this,SIGNAL(restoreTab(int)));

    loadSettings();
}

TabBar::~TabBar()
{

}

void TabBar::genActs()
{
    actAddTab = new QAction(tr("New tab"),this);
    connect(actAddTab,SIGNAL(triggered()),this,SIGNAL(addTab()));

    actSearch = new QAction(tr("Search"),this);
    actSearch->setCheckable(true);
    connect(actSearch,SIGNAL(triggered()),this,SLOT(search()));

    actDuplicateTab = new QAction(tr("Duplicate tab"),this);
    connect(actDuplicateTab,SIGNAL(triggered()),this,SIGNAL(duplicateTab()));

    actClosedTabs = new QAction(tr("Closed tabs"),this);
    actClosedTabs->setCheckable(true);
    connect(actClosedTabs,SIGNAL(triggered()),this,SLOT(showClosedTabs()));
}

QFrame * TabBar::genTabsOpenWidget()
{
    tabsOpenLayout = new QVBoxLayout;
    tabsOpenLayout->setContentsMargins(20,5,20,20);
    tabsOpenLayout->setSpacing(10);
    tabsOpenLayout->setAlignment(Qt::AlignCenter);

    QFrame *centralWidget = new QFrame;
    centralWidget->setLayout(tabsOpenLayout);
    return centralWidget;
}

QFrame * TabBar::genTabsClosedWidget()
{
    tabsClosedLayout = new QVBoxLayout;
    tabsClosedLayout->setContentsMargins(20,5,20,20);
    tabsClosedLayout->setSpacing(10);
    tabsClosedLayout->setAlignment(Qt::AlignCenter);

    QFrame *centralWidget = new QFrame;
    centralWidget->setLayout(tabsClosedLayout);
    return centralWidget;
}

void TabBar::setTabWidget(QStackedWidget *tabs)
{
    // Clean Layout
    QLayoutItem *item;
    while ((item = tabsOpenLayout->takeAt(0)) != 0)
    {
        item->widget()->deleteLater();
        delete item;
    }

    // Update Tabs
    for(int i(0);i<tabs->count();i++)
    {
        QString title = qobject_cast<WebWidget *>(tabs->widget(i))->view()->title();
        QIcon icon(qobject_cast<WebWidget *>(tabs->widget(i))->view()->icon());
        bool loading(qobject_cast<WebWidget *>(tabs->widget(i))->isLoading());

        QString objectName = "Tab";
        if(tabs->widget(i)==tabs->currentWidget())
            objectName = "CurrentTab";

        TabButton *btnTab = new TabButton(objectName,scrollArea);
        btnTab->setTitle(title);
        btnTab->setIcon(icon);
        btnTab->setLoading(loading);
        btnTab->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        btnTab->setPixmap(qobject_cast<WebWidget *>(tabs->widget(i))->screenShot().scaledToWidth(230,Qt::SmoothTransformation));
        tabsOpenLayout->addWidget(btnTab);

        openTabsMaps->setMapping(btnTab,i);
        connect(btnTab,SIGNAL(clicked()),openTabsMaps,SLOT(map()));

        closeTabsMaps->setMapping(btnTab,i);
        connect(btnTab,SIGNAL(closeRequested()),closeTabsMaps,SLOT(map()));
    }

    // Add tab button

    TabButton *btnAddTab = new TabButton("NewTab");
    btnAddTab->setTitle(actAddTab->text());
    btnAddTab->setIcon(QIcon(iconColor+"add.png"));
    btnAddTab->setLoading(false);
    btnAddTab->setClosable(false);
    btnAddTab->setSpacerVisible(false);
    btnAddTab->setFixedHeight(40);
    btnAddTab->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    tabsOpenLayout->addWidget(btnAddTab);
    connect(btnAddTab,SIGNAL(clicked()),this,SIGNAL(addTab()));

    // -------------------

    lblCount->setText(tr("%1 opened tab(s)").arg(QString::number(tabs->count())));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    tabsOpenLayout->addWidget(spacer);

    updateClosedTabs();
}

void TabBar::updateClosedTabs()
{
    // Clean Layout
    QLayoutItem *item;
    while ((item = tabsClosedLayout->takeAt(0)) != 0)
    {
        item->widget()->deleteLater();
        delete item;
    }

    // Update Tabs
    settings->beginGroup("ClosedTabs");
    for(int i(0);i<settings->childGroups().count();i++)
    {
        QString folder = QString::number(i);
        QIcon icon = settings->value(folder+"/Icon").value<QPixmap>();
        QString title = settings->value(folder+"/Title").toString();
        QString url = settings->value(folder+"/Url").toString();
        QPixmap screen = settings->value(folder+"/Screen").value<QPixmap>();

        TabButton *btnTab = new TabButton("Tab",scrollArea);
        btnTab->setTitle(title);
        btnTab->setIcon(icon);
        btnTab->setToolTip(url);
        btnTab->setPixmap(screen.scaledToWidth(230,Qt::SmoothTransformation));
        btnTab->setLoading(false);
        btnTab->setClosable(false);
        btnTab->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        tabsClosedLayout->addWidget(btnTab);

        restoreTabsMaps->setMapping(btnTab,i);
        connect(btnTab,SIGNAL(clicked()),restoreTabsMaps,SLOT(map()));
       // connect(btnTab,SIGNAL(clicked()),this,SLOT(updateClosedTabs()));
    }
    settings->endGroup();

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    tabsClosedLayout->addWidget(spacer);
}

void TabBar::search()
{
    if(searchBar->isHidden())
    {
        searchBar->show();
        searchBar->setFocus();
    }
    else
        searchBar->hide();

    searchBar->clear();
    searchTab("");
}

void TabBar::showClosedTabs()
{
    if(centralWidget->currentIndex()==0)
    {
        lblCount->setText(tr("%1 closed tab(s)").arg(QString::number(tabsClosedLayout->count()-1)));
        centralWidget->setCurrentIndex(1);
    }
    else
    {
        lblCount->setText(tr("%1 opened tab(s)").arg(QString::number(tabsOpenLayout->count()-2)));
        centralWidget->setCurrentIndex(0);
    }

    if(searchBar->isVisible())
        searchTab(searchBar->text());
}

void TabBar::searchTab(QString text)
{
    int foundItems = 0;

    if(centralWidget->currentIndex()==0)
    {
        for(int i(0);i<tabsOpenLayout->count()-2;i++)
        {
            QString title = qobject_cast<TabButton *>(tabsOpenLayout->itemAt(i)->widget())->title();

            if(title.contains(text,Qt::CaseInsensitive))
            {
                qobject_cast<TabButton *>(tabsOpenLayout->itemAt(i)->widget())->show();
                foundItems = foundItems+1;
            }
            else
                qobject_cast<TabButton *>(tabsOpenLayout->itemAt(i)->widget())->hide();
        }

        if(text=="")
        {
            lblCount->setText(tr("%1 opened tab(s)").arg(QString::number(tabsOpenLayout->count()-2)));
            qobject_cast<TabButton *>(tabsOpenLayout->itemAt(tabsOpenLayout->count()-2)->widget())->show();
        }
        else
        {
            lblCount->setText(tr("%1 opened tab(s) found").arg(QString::number(foundItems)));
            qobject_cast<TabButton *>(tabsOpenLayout->itemAt(tabsOpenLayout->count()-2)->widget())->hide();
        }
    }
    if(centralWidget->currentIndex()==1)
    {
        for(int i(0);i<tabsClosedLayout->count()-1;i++)
        {
            QString title = qobject_cast<TabButton *>(tabsClosedLayout->itemAt(i)->widget())->title();

            if(title.contains(text,Qt::CaseInsensitive))
            {
                qobject_cast<TabButton *>(tabsClosedLayout->itemAt(i)->widget())->show();
                foundItems = foundItems+1;
            }
            else
                qobject_cast<TabButton *>(tabsClosedLayout->itemAt(i)->widget())->hide();
        }

        if(text=="")
            lblCount->setText(tr("%1 closed tab(s)").arg(QString::number(tabsClosedLayout->count()-2)));
        else
            lblCount->setText(tr("%1 closed tab(s) found").arg(QString::number(foundItems)));
    }
}

void TabBar::loadSettings()
{
    iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    actAddTab->setIcon(QIcon(iconColor+"add.png"));
    actSearch->setIcon(QIcon(iconColor+"search.png"));
    actDuplicateTab->setIcon(QIcon(iconColor+"tabs.png"));
    actClosedTabs->setIcon(QIcon(iconColor+"cross.png"));

    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();
    toolBar->setIconSize(QSize(iconSize,iconSize));
}

TabButton::TabButton(QString objectName, QWidget *parent) : QLabel(parent)
{
    setObjectName(objectName);
    QSettings *settings = new QSettings;
    iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    btnIcon = new QToolButton;
    btnTitle = new QToolButton;
    btnClose = new QToolButton;
    spacer = new QToolButton;
    loadLogo = new QProgressIndicator;

    btnIcon->setObjectName("Icon");
    btnTitle->setObjectName("Title");
    btnClose->setObjectName("Close");

    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    btnIcon->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    btnTitle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    btnClose->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    btnTitle->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    btnTitle->setIconSize(QSize(0,0));
    btnIcon->setIconSize(QSize(16,16));
    btnClose->setIconSize(QSize(16,16));
    loadLogo->setFixedSize(20,20);
    loadLogo->startAnimation();
    loadLogo->setColor(Qt::white);

    btnTitle->setIcon(QIcon(iconColor+"cross.png"));
    btnClose->setIcon(QIcon(iconColor+"cross.png"));

    connect(spacer,SIGNAL(clicked()),this,SIGNAL(clicked()));
    connect(btnIcon,SIGNAL(clicked()),this,SIGNAL(clicked()));
    connect(btnTitle,SIGNAL(clicked()),this,SIGNAL(clicked()));
    connect(btnClose,SIGNAL(clicked()),this,SIGNAL(closeRequested()));

    QHBoxLayout *layProgress = new QHBoxLayout;
    layProgress->setContentsMargins(12,2,1,0);
    layProgress->addWidget(loadLogo);

    progressWidget = new QWidget;
    progressWidget->setLayout(layProgress);

    int height(40);
    btnIcon->setFixedHeight(height);
    btnTitle->setFixedHeight(height);
    btnClose->setFixedHeight(height);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setContentsMargins(0,0,0,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(btnIcon);
    titleLayout->addWidget(progressWidget);
    titleLayout->addWidget(btnTitle);
    titleLayout->addWidget(btnClose);

    QFrame *titleBar = new QFrame;
    titleBar->setObjectName("TitleBar");
    titleBar->setLayout(titleLayout);
    titleBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(spacer);
    mainLayout->addWidget(titleBar);
    setLayout(mainLayout);

    setFixedSize(230,130);
}

void TabButton::setIcon(QIcon icon)
{
    if(icon.isNull())
        btnIcon->setIcon(QIcon(iconColor+"empty.png"));
    else
        btnIcon->setIcon(icon);
}

void TabButton::setTitle(QString title)
{
    btnTitle->setText(title);
}

void TabButton::setLoading(bool loading)
{
    if(loading)
    {
        btnIcon->hide();
        loadLogo->startAnimation();
        progressWidget->show();
    }
    else
    {
        btnIcon->show();
        loadLogo->stopAnimation();
        progressWidget->hide();
    }
}

void TabButton::setClosable(bool closable)
{
    btnClose->setVisible(closable);
}

void TabButton::setSpacerVisible(bool visible)
{
    spacer->setVisible(visible);
}

QString TabButton::title()
{
    return btnTitle->text();
}
