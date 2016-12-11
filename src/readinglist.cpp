#include "readinglist.h"

ReadingList::ReadingList(QWidget *parent) : Panel(parent)
{
    setObjectName("ReadList");

    lblTitle->setText(tr("Reading list"));

    layItems = new QVBoxLayout;
    layItems->setMargin(0);
    layItems->setSpacing(0);

    QFrame *frame = new QFrame;
    frame->setLayout(layItems);
    scrollArea->setWidget(frame);

    signalExec = new QSignalMapper(this);
    connect(signalExec,SIGNAL(mapped(QString)),this,SIGNAL(exec(QString)));

    signalRemove = new QSignalMapper(this);
    connect(signalRemove,SIGNAL(mapped(int)),this,SLOT(removeItem(int)));

    signalEdit = new QSignalMapper(this);
    connect(signalEdit,SIGNAL(mapped(int)),this,SLOT(editItem(int)));

    connect(searchBar,SIGNAL(textEdited(QString)),this,SLOT(searchItem(QString)));

    btnAddItem = new QToolButton;
    btnAddItem->setToolTip(tr("Add this page to the reading list"));
    btnAddItem->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    connect(btnAddItem,SIGNAL(clicked()),this,SIGNAL(addToList()));

    btnSearch = new QToolButton;
    btnSearch->setToolTip(tr("Search"));
    btnSearch->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    btnSearch->setCheckable(true);
    connect(btnSearch,SIGNAL(clicked()),this,SLOT(search()));

    settings = new QSettings;
    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();

    toolBar->setIconSize(QSize(iconSize,iconSize));
    toolBar->addWidget(btnAddItem);
    toolBar->addWidget(btnSearch);
}

ReadingList::~ReadingList()
{

}

void ReadingList::updateItems()
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
    btnAddItem->setIcon(QIcon(iconColor+"add.png"));
    btnSearch->setIcon(QIcon(iconColor+"search.png"));

    // Setup
    settings->beginGroup("ReadingList");

    lblCount->setText(tr("%1 item(s)").arg(QString::number(settings->childGroups().count())));

    for(int i(0);i<=settings->childGroups().count()-1;i++)
    {
        QString title = settings->value(QString::number(i)+"/Title").toString();
        QString url = settings->value(QString::number(i)+"/Url").toUrl().toString();
        QIcon icon = settings->value(QString::number(i)+"/Icon").value<QPixmap>();
        QString titleSmall = title;
        QString urlSmall = url;

        if(icon.isNull())
            icon = QIcon(iconColor+"empty.png");

        if(title.count()>20)
            titleSmall = title.left(20)+"...";
        if(url.count()>25)
            urlSmall = url.left(25)+"...";

        Item *btnItem = new Item;
        btnItem->btnIcon->setIcon(icon);
        btnItem->btnTitle->setText(titleSmall);
        btnItem->btnTitle->setToolTip(title);
        btnItem->btnUrl->setText(urlSmall);
        btnItem->btnUrl->setToolTip(url);
        btnItem->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
        layItems->addWidget(btnItem);

        signalExec->setMapping(btnItem,url);
        connect(btnItem,SIGNAL(exec()),signalExec,SLOT(map()));

        signalRemove->setMapping(btnItem,i);
        connect(btnItem,SIGNAL(remove()),signalRemove,SLOT(map()));

        signalEdit->setMapping(btnItem,i);
        connect(btnItem,SIGNAL(edited()),signalEdit,SLOT(map()));
    }

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Expanding);
    layItems->addWidget(spacer);

    settings->endGroup();
}

void ReadingList::removeItem(int id)
{
    settings->beginGroup("ReadingList");

    for(int i(0);i<=settings->childGroups().count();i++)
    {
        if(i>=id && i!=(settings->childGroups().count()-1))
        {
            //recupere value des i+1
            //remplace value des i

            QVariant title = settings->value(QString::number(i+1)+"/Title");
            QVariant url = settings->value(QString::number(i+1)+"/Url");
            QVariant icon = settings->value(QString::number(i+1)+"/Icon");

           // settings->remove(QString::number(i));

            settings->setValue(QString::number(i)+"/Title",title);
            settings->setValue(QString::number(i)+"/Url",url);
            settings->setValue(QString::number(i)+"/Icon",icon);
        }

        if(i==settings->childGroups().count()-1)
        {
            settings->remove(QString::number(i));
        }
    }

    settings->endGroup();

    updateItems();
}

void ReadingList::editItem(int id)
{
    QString title = qobject_cast<Item *>(layItems->itemAt(id)->widget())->btnTitle->text();
    QString url = qobject_cast<Item *>(layItems->itemAt(id)->widget())->btnUrl->text();
    QIcon icon = qobject_cast<Item *>(layItems->itemAt(id)->widget())->btnIcon->icon();

    settings->beginGroup("ReadingList");
    settings->setValue(QString::number(id)+"/Title",title);
    settings->setValue(QString::number(id)+"/Url",url);
    settings->setValue(QString::number(id)+"/Icon",icon.pixmap(55,55));
    settings->endGroup();
}

void ReadingList::search()
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

void ReadingList::searchItem(QString text)
{
    int foundItems = 0;
    for(int i(0);i<layItems->count()-1;i++)
    {
        QString title = qobject_cast<Item *>(layItems->itemAt(i)->widget())->btnTitle->toolTip();
        QString url = qobject_cast<Item *>(layItems->itemAt(i)->widget())->btnUrl->toolTip();

        if(title.contains(text,Qt::CaseInsensitive) || url.contains(text,Qt::CaseInsensitive))
        {
            qobject_cast<Item *>(layItems->itemAt(i)->widget())->show();
            foundItems = foundItems + 1;
        }
        else
            qobject_cast<Item *>(layItems->itemAt(i)->widget())->hide();
    }

    if(text=="")
        lblCount->setText(tr("%1 item(s)").arg(QString::number(layItems->count()-1)));
    else
        lblCount->setText(tr("%1 item(s) found").arg(QString::number(foundItems)));

}

// ITEM

Item::Item(QWidget *parent) : QFrame(parent)
{
    btnIcon = new QToolButton;
    btnTitle = new QToolButton;
    btnUrl = new QToolButton;
    QPushButton *btnMenu = new QPushButton;
    setObjectName("Item");

    settings = new QSettings;
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();

    editTitle = new QLineEdit;
    editUrl = new QLineEdit;

    editTitle->hide();
    editUrl->hide();

    btnIcon->setObjectName("Icon");
    btnTitle->setObjectName("Title");
    btnUrl->setObjectName("Url");
    editTitle->setObjectName("EditTitle");
    editUrl->setObjectName("EditUrl");
    btnMenu->setObjectName("Menu");

    btnIcon->setIconSize(QSize(iconSize,iconSize));
    btnMenu->setIconSize(QSize(iconSize,iconSize));
    btnTitle->setIconSize(QSize(0,0));
    btnUrl->setIconSize(QSize(0,0));

    btnTitle->setIcon(QIcon("none.png"));
    btnUrl->setIcon(QIcon("none.png"));
    btnMenu->setIcon(QIcon(iconColor+"menu2.png"));

    btnTitle->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnUrl->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    btnIcon->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    btnTitle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    btnUrl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    editTitle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    editUrl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    btnMenu->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);

    connect(btnIcon,SIGNAL(clicked()),this,SIGNAL(exec()));
    connect(btnTitle,SIGNAL(clicked()),this,SIGNAL(exec()));
    connect(btnUrl,SIGNAL(clicked()),this,SIGNAL(exec()));
    connect(editTitle,SIGNAL(returnPressed()),editUrl,SLOT(setFocus()));
    connect(editTitle,SIGNAL(returnPressed()),editUrl,SLOT(selectAll()));
    connect(editUrl,SIGNAL(returnPressed()),this,SLOT(showEditor()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(btnIcon,0,0,2,1);
    mainLayout->addWidget(btnTitle,0,1);
    mainLayout->addWidget(btnUrl,1,1);
    mainLayout->addWidget(editTitle,0,2);
    mainLayout->addWidget(editUrl,1,2);
    mainLayout->addWidget(btnMenu,0,3,2,1);
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
    setFixedHeight(60);
}

Item::~Item()
{

}

void Item::genActs()
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

void Item::showEditor()
{
    if(editTitle->isHidden())
    {
        editTitle->show();
        editUrl->show();
        btnTitle->hide();
        btnUrl->hide();

        actGoTo->setVisible(false);
        actRemove->setVisible(false);
        actStartEdit->setVisible(false);
        actFinishEdit->setVisible(true);
        actChooseIcon->setVisible(true);
        actResetIcon->setVisible(true);

        editTitle->setText(btnTitle->toolTip());
        editUrl->setText(btnUrl->toolTip());
        btnIcon->setEnabled(false);

       /* editTitle->setFocus();
        editTitle->selectAll();*/

        //--------------

        /*editTitle->setEnabled(false);
        editUrl->setEnabled(false);*/

        QLabel *lblTitle = new QLabel(tr("Edit an item"));
        lblTitle->setObjectName("Title");

        QPushButton *editIcon = new QPushButton;
        editIcon->setIcon(btnIcon->icon());
        editIcon->setIconSize(QSize(24,24));

        QLineEdit *formTitle = new QLineEdit;
        formTitle->setText(btnTitle->toolTip());
        connect(formTitle,SIGNAL(textEdited(QString)),editTitle,SLOT(setText(QString)));
        connect(formTitle,SIGNAL(textChanged(QString)),editTitle,SLOT(setText(QString)));

        QLineEdit *formUrl = new QLineEdit;
        formUrl->setText(btnUrl->toolTip());
        connect(formUrl,SIGNAL(textEdited(QString)),editUrl,SLOT(setText(QString)));
        connect(formUrl,SIGNAL(textChanged(QString)),editUrl,SLOT(setText(QString)));

        QToolButton *btnSave = new QToolButton;
        btnSave->setText(tr("Save"));
        connect(btnSave,SIGNAL(clicked()),this,SLOT(showEditor()));

        QToolButton *btnCancel = new QToolButton;
        btnCancel->setText(tr("Cancel"));
       // connect(btnCancel,SIGNAL(clicked()),this,SLOT(showEditor()));

        QGridLayout *mainLayout = new QGridLayout;
        mainLayout->setContentsMargins(20,20,30,30);
        mainLayout->addWidget(lblTitle,0,1);
        mainLayout->addWidget(editIcon,1,0);
        mainLayout->addWidget(formTitle,1,1);
        mainLayout->addWidget(formUrl,2,1);
        mainLayout->addWidget(btnSave,3,1);
        mainLayout->setAlignment(btnSave,Qt::AlignRight);

        QDialog *dialog = new QDialog(this);
        dialog->setModal(true);
        dialog->show();
        dialog->setWindowTitle(tr("Edit an item"));
        dialog->setLayout(mainLayout);
        dialog->setMinimumWidth(500);
        connect(btnSave,SIGNAL(clicked()),dialog,SLOT(close()));
    }
    else
    {
        editTitle->hide();
        editUrl->hide();
        btnTitle->show();
        btnUrl->show();

        actGoTo->setVisible(true);
        actRemove->setVisible(true);
        actStartEdit->setVisible(true);
        actFinishEdit->setVisible(false);
        actChooseIcon->setVisible(false);
        actResetIcon->setVisible(false);

        /*btnTitle->setText(editTitle->text());
        btnUrl->setText(editUrl->text());
        btnIcon->setEnabled(true);*/

        QString title = editTitle->text();
        QString url = editUrl->text();
        btnIcon->setEnabled(true);
        QString titleSmall = title;
        QString urlSmall = url;

        if(title.count()>20)
            titleSmall = title.left(20)+"...";
        if(url.count()>25)
            urlSmall = url.left(25)+"...";

        btnTitle->setText(titleSmall);
        btnTitle->setToolTip(title);
        btnUrl->setText(urlSmall);
        btnUrl->setToolTip(url);

        emit edited();
    }
}

void Item::resetIcon()
{
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    btnIcon->setIcon(QIcon(iconColor+"empty.png"));
}

void Item::chooseIcon()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), tr("Picture (*.bmp *.gif *.jpeg *.jpg *.png);;All files (*)"));
    if(path.isEmpty())
        return;

    btnIcon->setIcon(QIcon(path));
}
