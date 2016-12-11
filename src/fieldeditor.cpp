#include "fieldeditor.h"

FieldEditor::FieldEditor(QWidget *parent) : QFrame(parent)
{
    settings = new QSettings;
    mainToolBar = new QToolBar;
    btnIcon = new QPushButton;
    titleBar = new QLineEdit;
    btnMenu = new QPushButton;
    scrollArea = new QScrollArea;

    mainToolBar->addWidget(btnIcon);
    mainToolBar->addWidget(titleBar);
    mainToolBar->addWidget(btnMenu);

    titleBar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(titleBar,SIGNAL(returnPressed()),this,SIGNAL(execute()));
    connect(titleBar,SIGNAL(textEdited(QString)),this,SLOT(updateQuery()));
    connect(titleBar,SIGNAL(textChanged(QString)),this,SLOT(updateQuery()));
    connect(titleBar,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showMenu()));

    genActions();
    genEditBar();
    genMenu();

    editBar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(editBar,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showMenu()));

    btnMenu->setMenu(mainMenu);

    mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(mainToolBar);
    mainLayout->addWidget(editBar);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);

    layItems = new QVBoxLayout;
    layItems->setContentsMargins(0,0,0,0);
    layItems->setSpacing(0);

    QFrame *frameItems = new QFrame;
    frameItems->setLayout(layItems);
    scrollArea->setWidget(frameItems);
    scrollArea->setWidgetResizable(true);

    loadSettings();
}

FieldEditor::~FieldEditor()
{

}

void FieldEditor::genActions()
{
    actPasteGo = new QAction(tr("Paste and go"),this);
    connect(actPasteGo,SIGNAL(triggered()),titleBar,SLOT(clear()));
    connect(actPasteGo,SIGNAL(triggered()),titleBar,SLOT(paste()));
    connect(actPasteGo,SIGNAL(triggered()),this,SIGNAL(execute()));

    actPasteSearch = new QAction(tr("Paste and search"),this);
    connect(actPasteSearch,SIGNAL(triggered()),titleBar,SLOT(clear()));
    connect(actPasteSearch,SIGNAL(triggered()),titleBar,SLOT(paste()));
    connect(actPasteSearch,SIGNAL(triggered()),this,SLOT(searchQuery()));

    actBack = new QAction(tr("Back"),this);
    connect(actBack,SIGNAL(triggered()),this,SIGNAL(showBrowser()));

    actUndo = new QAction(tr("Undo"),this);
    connect(actUndo,SIGNAL(triggered()),titleBar,SLOT(undo()));

    actRedo = new QAction(tr("Redo"),this);
    connect(actRedo,SIGNAL(triggered()),titleBar,SLOT(redo()));

    actCut = new QAction(tr("Cut"),this);
    connect(actCut,SIGNAL(triggered()),titleBar,SLOT(cut()));

    actCopy = new QAction(tr("Copy"),this);
    connect(actCopy,SIGNAL(triggered()),titleBar,SLOT(copy()));

    actPaste = new QAction(tr("Paste"),this);
    connect(actPaste,SIGNAL(triggered()),titleBar,SLOT(paste()));

    actSelectAll = new QAction(tr("Select all"),this);
    connect(actSelectAll,SIGNAL(triggered()),titleBar,SLOT(selectAll()));

    actClear = new QAction(tr("Clear"),this);
    connect(actClear,SIGNAL(triggered()),titleBar,SLOT(clear()));
    connect(actClear,SIGNAL(triggered()),titleBar,SLOT(setFocus()));
}

void FieldEditor::genEditBar()
{
    editBar = new QToolBar;
    editBar->setObjectName("EditBar");
    //editBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QToolButton *btnPasteGo = new QToolButton;
    btnPasteGo->setDefaultAction(actPasteGo);
    btnPasteGo->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QToolButton *btnPasteSearch = new QToolButton;
    btnPasteSearch->setDefaultAction(actPasteSearch);
    btnPasteSearch->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    editBar->addAction(actBack);
    editBar->addWidget(btnPasteGo);
    editBar->addWidget(btnPasteSearch);
    editBar->addSeparator();
    editBar->addAction(actUndo);
    editBar->addAction(actRedo);
    editBar->addSeparator();
    editBar->addAction(actCut);
    editBar->addAction(actCopy);
    editBar->addAction(actPaste);
    editBar->addSeparator();
    editBar->addAction(actSelectAll);
    editBar->addAction(actClear);
}

void FieldEditor::genMenu()
{
    mainMenu = new QMenu;
    mainMenu->addAction(actBack);
    mainMenu->addSeparator();
    mainMenu->addAction(actPasteGo);
    mainMenu->addAction(actPasteSearch);
    mainMenu->addSeparator();
    mainMenu->addAction(actUndo);
    mainMenu->addAction(actRedo);
    mainMenu->addSeparator();
    mainMenu->addAction(actCut);
    mainMenu->addAction(actCopy);
    mainMenu->addAction(actPaste);
    mainMenu->addSeparator();
    mainMenu->addAction(actSelectAll);
    mainMenu->addAction(actClear);
}

void FieldEditor::loadSettings()
{
    // Toolbar position
    if(settings->value("Interface/Position/Toolbar").toString()=="top")
        mainLayout->setDirection(QBoxLayout::TopToBottom);
    else
        mainLayout->setDirection(QBoxLayout::BottomToTop);

    // Icon Color
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    btnIcon->setIcon(QIcon(iconColor + "empty.png"));
    btnMenu->setIcon(QIcon(iconColor + "menu.png"));
    actBack->setIcon(QIcon(iconColor+"back.png"));
    actPasteGo->setIcon(QIcon(iconColor+"check.png"));
    actPasteSearch->setIcon(QIcon(iconColor+"search.png"));
    actUndo->setIcon(QIcon(iconColor+"undo.png"));
    actRedo->setIcon(QIcon(iconColor+"redo.png"));
    actCut->setIcon(QIcon(iconColor+"cut.png"));
    actCopy->setIcon(QIcon(iconColor+"copy.png"));
    actPaste->setIcon(QIcon(iconColor+"paste.png"));
    actSelectAll->setIcon(QIcon(iconColor+"select-all.png"));
    actClear->setIcon(QIcon(iconColor+"cross.png"));


    // Icons on toolbar
    QSize iconSize(settings->value("Interface/ToolBar/IconSize").toInt(),settings->value("Interface/ToolBar/IconSize").toInt());
    btnIcon->setIconSize(iconSize);
    btnMenu->setIconSize(iconSize);
    editBar->setIconSize(iconSize);
}

void FieldEditor::showMenu()
{
    mainMenu->popup(QCursor::pos());
}

void FieldEditor::updateQuery()
{
    // Clean Layout
    QLayoutItem *item;
    while ((item = layItems->takeAt(0)) != 0)
    {
        item->widget()->deleteLater();
        delete item;
    }


    // Get query and icon color
    QString query = titleBar->text();
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    if(query.count()>40)
        query = query.left(15) + "..." + query.right(15);


    // Default actions
    QLabel *lblDefaultActions = new QLabel(tr("Default actions"));
    if(!query.isEmpty())
        layItems->addWidget(lblDefaultActions);

    btnGoTo = new QToolButton;
    btnGoTo->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnGoTo->setText(query);
    btnGoTo->setIcon(QIcon(iconColor+"check.png"));
    btnGoTo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    if(!query.isEmpty())
        layItems->addWidget(btnGoTo);

    btnSearch = new QToolButton;
    btnSearch->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnSearch->setText(query);
    btnSearch->setIcon(QIcon(iconColor+"search.png"));
    btnSearch->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    if(!query.isEmpty())
        layItems->addWidget(btnSearch);

    connect(btnGoTo,SIGNAL(clicked()),this,SLOT(itemClicked()));
    connect(btnSearch,SIGNAL(clicked()),this,SLOT(itemClicked()));

    layItems->addWidget(genReadingList());
    layItems->addWidget(genBookmarks());
    layItems->addWidget(genHistory());

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Expanding);
    layItems->addWidget(spacer);
}

void FieldEditor::itemClicked()
{
    if(sender()==btnGoTo)
        emit goToUrl(titleBar->text());
    if(sender()==btnSearch)
        searchQuery();

    else
        emit goToUrl(qobject_cast<QToolButton *>(sender())->toolTip());
}

void FieldEditor::searchQuery()
{
    QSettings settings;
    QString prefix = settings.value("Navigation/PrefixSearch").toString();
    QString suffix = settings.value("Navigation/SuffixSearch").toString();

    emit goToUrl(prefix+titleBar->text()+suffix);
}

QFrame *FieldEditor::genReadingList()
{
    // Get query and icon color
    QString query = titleBar->text();
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    settings->beginGroup("ReadingList");

    QVBoxLayout *layReadList = new QVBoxLayout;
    layReadList->setMargin(0);
    layReadList->setSpacing(0);

    QLabel *lblReadingList = new QLabel(tr("Reading list"));
    layReadList->addWidget(lblReadingList);

    QVBoxLayout *layReadListMore = new QVBoxLayout;
    layReadListMore->setMargin(0);
    layReadListMore->setSpacing(0);

    HideBlock *blockReadListMore = new HideBlock(tr("Display / hide all results"));
    blockReadListMore->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    blockReadListMore->setBlockLayout(layReadListMore);

    int itemReadingList = 0;
    for(int i(0);i<=settings->childGroups().count()-1;i++)
    {
        QString title = settings->value(QString::number(i)+"/Title").toString();
        QString url = settings->value(QString::number(i)+"/Url").toUrl().toString();

        if(title.contains(query,Qt::CaseInsensitive) || url.contains(query,Qt::CaseInsensitive))
        {
            QToolButton *btnItem = new QToolButton;
            btnItem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            btnItem->setToolTip("<h3>" + title + "</h3>" + url);

            if(title.count()>20)
                title = title.left(20) + "...";
            if(url.count()>45)
                url = url.left(25) + "..." + url.right(20);
            QString text = title + " - " + url;
            btnItem->setText(text);

            btnItem->setIcon(settings->value(QString::number(i)+"/Icon").value<QPixmap>());
            if(btnItem->icon().isNull())
                btnItem->setIcon(QIcon(iconColor+"empty.png"));
            btnItem->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
            connect(btnItem,SIGNAL(clicked()),this,SLOT(itemClicked()));

            if(itemReadingList<3)
                layReadList->addWidget(btnItem);
            else
                layReadListMore->addWidget(btnItem);

            itemReadingList = itemReadingList +1;
        }
    }

    settings->endGroup();

    QFrame *frameReadingList = new QFrame;

    if(layReadListMore->count()>0)
        layReadList->addWidget(blockReadListMore);
    if(layReadList->count()>1)
        frameReadingList->setLayout(layReadList);

    return frameReadingList;
}

QFrame *FieldEditor::genBookmarks()
{
    // Get query and icon color
    QString query = titleBar->text();
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    settings->beginGroup("Bookmarks");

    QVBoxLayout *layItems = new QVBoxLayout;
    layItems->setMargin(0);
    layItems->setSpacing(0);

    QLabel *lblTitle = new QLabel(tr("Bookmarks"));
    layItems->addWidget(lblTitle);

    QVBoxLayout *layItemsMore = new QVBoxLayout;
    layItemsMore->setMargin(0);
    layItemsMore->setSpacing(0);

    HideBlock *blockItemsMore = new HideBlock(tr("Display / hide all results"));
    blockItemsMore->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    blockItemsMore->setBlockLayout(layItemsMore);

    int itemBookmarks = 0;
    for(int i(0);i<=settings->childGroups().count()-1;i++)
    {
        QString title = settings->value(QString::number(i)+"/Title").toString();
        QString url = settings->value(QString::number(i)+"/Url").toUrl().toString();

        if(title.contains(query,Qt::CaseInsensitive) || url.contains(query,Qt::CaseInsensitive))
        {
            QToolButton *btnItem = new QToolButton;
            btnItem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            btnItem->setToolTip("<h3>" + title + "</h3>" + url);

            if(title.count()>20)
                title = title.left(20) + "...";
            if(url.count()>45)
                url = url.left(25) + "..." + url.right(20);
            QString text = title + " - " + url;
            btnItem->setText(text);

            btnItem->setIcon(settings->value(QString::number(i)+"/Icon").value<QPixmap>());
            if(btnItem->icon().isNull())
                btnItem->setIcon(QIcon(iconColor+"empty.png"));
            btnItem->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
            connect(btnItem,SIGNAL(clicked()),this,SLOT(itemClicked()));

            if(itemBookmarks<3)
                layItems->addWidget(btnItem);
            else
                layItemsMore->addWidget(btnItem);

            itemBookmarks = itemBookmarks +1;
        }
    }

    settings->endGroup();

    QFrame *frameItems = new QFrame;

    if(layItemsMore->count()>0)
        layItems->addWidget(blockItemsMore);
    if(layItems->count()>1)
        frameItems->setLayout(layItems);

    return frameItems;
}

QFrame *FieldEditor::genHistory()
{
    // Get query and icon color
    QString query = titleBar->text();
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    QVBoxLayout *layItems = new QVBoxLayout;
    layItems->setMargin(0);
    layItems->setSpacing(0);

    QLabel *lblTitle = new QLabel(tr("History"));
    layItems->addWidget(lblTitle);

    QVBoxLayout *layItemsMore = new QVBoxLayout;
    layItemsMore->setMargin(0);
    layItemsMore->setSpacing(0);

    HideBlock *blockItemsMore = new HideBlock(tr("Display / hide all results"));
    blockItemsMore->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    blockItemsMore->setBlockLayout(layItemsMore);

    int itemHistory = 0;
    for(int i(0);i<historyItems.count();i++)
    {
        QString title = historyItems.at(i).title();
        QString url = historyItems.at(i).url().toString();

        if(title.contains(query,Qt::CaseInsensitive) || url.contains(query,Qt::CaseInsensitive))
        {
            QToolButton *btnItem = new QToolButton;
            btnItem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            btnItem->setToolTip("<h3>" + title + "</h3>" + url);

            if(title.count()>20)
                title = title.left(20) + "...";
            if(url.count()>45)
                url = url.left(25) + "..." + url.right(20);
            QString text = title + " - " + url;
            btnItem->setText(text);

            btnItem->setIcon(settings->value(QString::number(i)+"/Icon").value<QPixmap>());
            if(btnItem->icon().isNull())
                btnItem->setIcon(QIcon(iconColor+"empty.png"));
            btnItem->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
            connect(btnItem,SIGNAL(clicked()),this,SLOT(itemClicked()));

            if(itemHistory<3)
                layItems->addWidget(btnItem);
            else
                layItemsMore->addWidget(btnItem);

            itemHistory = itemHistory +1;
        }
    }

    QFrame *frameItems = new QFrame;

    if(layItemsMore->count()>0)
        layItems->addWidget(blockItemsMore);
    if(layItems->count()>1)
        frameItems->setLayout(layItems);

    return frameItems;
}

void FieldEditor::setHistoryItems(QList<QWebEngineHistoryItem> list)
{
    historyItems = list;
}
