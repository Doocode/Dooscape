#include "bookmarkspanel.h"

BookmarksPanel::BookmarksPanel(QWidget *parent) : Panel(parent)
{
    setObjectName("Bookmarks");
    settings = new QSettings;

    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();

    lblTitle->setText(tr("Bookmarks"));
    connect(searchBar,SIGNAL(textEdited(QString)),this,SLOT(searchItem(QString)));

    genActs();

    toolBar->setIconSize(QSize(iconSize,iconSize));
    toolBar->addAction(actAddItem);
    toolBar->addAction(actSearch);

    layItems = new QVBoxLayout;
    layItems->setMargin(0);
    layItems->setSpacing(0);

    QFrame *frame = new QFrame;
    frame->setLayout(layItems);

    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(frame);

    signalExec = new QSignalMapper(this);
    connect(signalExec,SIGNAL(mapped(QString)),this,SIGNAL(exec(QString)));

    signalRemove = new QSignalMapper(this);
    connect(signalRemove,SIGNAL(mapped(int)),this,SLOT(removeItem(int)));

   /* signalEdit = new QSignalMapper(this);
    connect(signalEdit,SIGNAL(mapped(int)),this,SLOT(editItem(int)));*/
}

void BookmarksPanel::genActs()
{
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    actAddItem = new QAction(QIcon(iconColor+"add.png"),tr("Add to bookmarks"),this);
    connect(actAddItem,SIGNAL(triggered()),this,SIGNAL(addToBookmarks()));

    actSearch = new QAction(QIcon(iconColor+"search.png"),tr("Search"),this);
    actSearch->setCheckable(true);
    connect(actSearch,SIGNAL(triggered()),this,SLOT(search()));
}

void BookmarksPanel::updateItems()
{
    // Clean Layout
    QLayoutItem *item;
    while ((item = layItems->takeAt(0)) != 0)
    {
        item->widget()->deleteLater();
        delete item;
    }

    //Icon color
    QSize iconSize = QSize(settings->value("Interface/ToolBar/IconSize").toInt(),settings->value("Interface/ToolBar/IconSize").toInt());
    toolBar->setIconSize(iconSize);
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    actAddItem->setIcon(QIcon(iconColor+"add.png"));
    actSearch->setIcon(QIcon(iconColor+"search.png"));

    // Setup
    settings->beginGroup("Bookmarks");

    lblCount->setText(tr("%1 bookmark(s)").arg(QString::number(settings->childGroups().count())));

    for(int i(0);i<=settings->childGroups().count()-1;i++)
    {
        QString title = settings->value(QString::number(i)+"/Title").toString();
        QString url = settings->value(QString::number(i)+"/Url").toUrl().toString();
        QIcon icon = settings->value(QString::number(i)+"/Icon").value<QPixmap>();
        QString titleSmall = title;

        if(icon.isNull())
            icon = QIcon(iconColor+"empty.png");

        if(title.count()>20)
            titleSmall = title.left(20)+"...";

        BookmarksItem *btnItem = new BookmarksItem;
        btnItem->btnIcon->setIcon(icon);
        btnItem->btnTitle->setText(titleSmall);
        btnItem->btnTitle->setToolTip(title);
        btnItem->btnIcon->setToolTip(url);
        btnItem->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
        layItems->addWidget(btnItem);

        signalExec->setMapping(btnItem,url);
        connect(btnItem,SIGNAL(exec()),signalExec,SLOT(map()));

        signalRemove->setMapping(btnItem,i);
        connect(btnItem,SIGNAL(remove()),signalRemove,SLOT(map()));

        /*signalEdit->setMapping(btnItem,i);
        connect(btnItem,SIGNAL(edited()),signalEdit,SLOT(map()));*/
    }

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Expanding);
    layItems->addWidget(spacer);

    settings->endGroup();
}

void BookmarksPanel::search()
{
    if(searchBar->isHidden())
    {
        searchBar->show();
        searchBar->setFocus();
    }
    else
        searchBar->hide();

    searchBar->clear();
    searchItem("");
}

void BookmarksPanel::searchItem(QString text)
{
    int foundItems = 0;
    for(int i(0);i<layItems->count()-1;i++)
    {
        QString title = qobject_cast<BookmarksItem *>(layItems->itemAt(i)->widget())->btnTitle->toolTip();
        QString url = qobject_cast<BookmarksItem *>(layItems->itemAt(i)->widget())->btnIcon->toolTip();

        if(title.contains(text,Qt::CaseInsensitive) || url.contains(text,Qt::CaseInsensitive))
        {
            qobject_cast<BookmarksItem *>(layItems->itemAt(i)->widget())->show();
            foundItems = foundItems + 1;
        }
        else
            qobject_cast<BookmarksItem *>(layItems->itemAt(i)->widget())->hide();
    }

    if(text=="")
        lblCount->setText(tr("%1 bookmark(s)").arg(QString::number(layItems->count()-1)));
    else
        lblCount->setText(tr("%1 bookmark(s) found").arg(QString::number(foundItems)));
}

void BookmarksPanel::removeItem(int id)
{
    settings->beginGroup("Bookmarks");

    for(int i(0);i<=settings->childGroups().count();i++)
    {
        if(i>=id && i!=(settings->childGroups().count()-1))
        {
            //recupere value des i+1
            //remplace value des i

            QVariant title = settings->value(QString::number(i+1)+"/Title");
            QVariant url = settings->value(QString::number(i+1)+"/Url");
            QVariant icon = settings->value(QString::number(i+1)+"/Icon");
            QVariant screen = settings->value(QString::number(i+1)+"/Screen");
            QVariant date = settings->value(QString::number(i+1)+"/Date");
            QVariant time = settings->value(QString::number(i+1)+"/Time");

            settings->setValue(QString::number(i)+"/Title",title);
            settings->setValue(QString::number(i)+"/Url",url);
            settings->setValue(QString::number(i)+"/Icon",icon);
            settings->setValue(QString::number(i)+"/Screen",screen);
            settings->setValue(QString::number(i)+"/Date",date);
            settings->setValue(QString::number(i)+"/Time",time);
        }

        if(i==settings->childGroups().count()-1)
            settings->remove(QString::number(i));
    }

    settings->endGroup();

    updateItems();
}

// ITEM

BookmarksItem::BookmarksItem(QWidget *parent) : QFrame(parent)
{
    btnIcon = new QToolButton;
    btnTitle = new QToolButton;
    QPushButton *btnMenu = new QPushButton;
    setObjectName("Item");

    settings = new QSettings;
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();

    btnIcon->setObjectName("Icon");
    btnTitle->setObjectName("Title");
    btnMenu->setObjectName("Menu");

    btnIcon->setIconSize(QSize(iconSize,iconSize));
    btnMenu->setIconSize(QSize(iconSize,iconSize));
    btnTitle->setIconSize(QSize(0,0));

    btnTitle->setIcon(QIcon("none.png"));
    btnMenu->setIcon(QIcon(iconColor+"menu2.png"));

    btnTitle->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    btnIcon->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    btnTitle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    btnMenu->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);

    connect(btnIcon,SIGNAL(clicked()),this,SIGNAL(exec()));
    connect(btnTitle,SIGNAL(clicked()),this,SIGNAL(exec()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(btnIcon);
    mainLayout->addWidget(btnTitle);
    mainLayout->addWidget(btnMenu);
    setLayout(mainLayout);

    genActs();

    QMenu *mainMenu = new QMenu;
    mainMenu->addAction(actGoTo);
    mainMenu->addAction(actStartEdit);
    mainMenu->addAction(actRemove);

    mainMenu->addAction(actFinishEdit);
    mainMenu->addAction(actChooseIcon);
    mainMenu->addAction(actResetIcon);
    btnMenu->setMenu(mainMenu);
    setFixedHeight(40);
}

BookmarksItem::~BookmarksItem()
{

}

void BookmarksItem::genActs()
{
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";


    actGoTo = new QAction(QIcon(iconColor+"preview.png"),tr("Go to"),this);
    connect(actGoTo,SIGNAL(triggered()),this,SIGNAL(exec()));

    actStartEdit = new QAction(QIcon(iconColor+"edition.png"),tr("Edit"),this);
    actStartEdit->setVisible(false);
    connect(actStartEdit,SIGNAL(triggered()),this,SLOT(showEditor()));

    actFinishEdit = new QAction(QIcon(iconColor+"edition.png"),tr("Save"),this);
    connect(actFinishEdit,SIGNAL(triggered()),this,SLOT(showEditor()));
    actFinishEdit->setVisible(false);

    actRemove = new QAction(QIcon(iconColor+"cross.png"),tr("Delete"),this);
    connect(actRemove,SIGNAL(triggered()),this,SIGNAL(remove()));

    actChooseIcon = new QAction(QIcon(iconColor+"open.png"),tr("Choose an icon"),this);
    connect(actChooseIcon,SIGNAL(triggered()),this,SLOT(chooseIcon()));
    actChooseIcon->setVisible(false);

    actResetIcon = new QAction(QIcon(iconColor+"undo.png"),tr("Reset icon"),this);
    connect(actResetIcon,SIGNAL(triggered()),this,SLOT(resetIcon()));
    actResetIcon->setVisible(false);
}

void BookmarksItem::showEditor()
{

}

void BookmarksItem::resetIcon()
{
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    btnIcon->setIcon(QIcon(iconColor+"empty.png"));
}

void BookmarksItem::chooseIcon()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), tr("Picture (*.bmp *.gif *.jpeg *.jpg *.png);;All files (*)"));
    if(path.isEmpty())
        return;

    btnIcon->setIcon(QIcon(path));
}
