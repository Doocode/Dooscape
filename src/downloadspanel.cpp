#include "downloadspanel.h"

DownloadsPanel::DownloadsPanel(QWidget *parent) : Panel(parent)
{
    setObjectName("Downloads");
    settings = new QSettings;

    lblTitle->setText(tr("Downloads"));

    genActs();
    //toolBar->addAction(actSearch);

    connect(searchBar,SIGNAL(textEdited(QString)),this,SLOT(searchItem(QString)));

    QFrame *centralWidget = new QFrame;
    scrollArea->setWidget(centralWidget);

    centralLayout = new QVBoxLayout;
    centralLayout->setContentsMargins(0,0,0,0);
    centralLayout->setSpacing(0);
    centralLayout->setAlignment(Qt::AlignTop);
    centralWidget->setLayout(centralLayout);

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    centralLayout->addWidget(spacer);
}

void DownloadsPanel::genActs()
{
    actSearch = new QAction(tr("Search"),this);
    actSearch->setCheckable(true);
    connect(actSearch,SIGNAL(triggered()),this,SLOT(search()));
}

void DownloadsPanel::search()
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

void DownloadsPanel::searchItem(QString text)
{
    /*for(int i(0);i<tabsLayout->count()-1;i++)
    {
        QString title = qobject_cast<TabButton *>(tabsLayout->itemAt(i)->widget())->title();

        if(title.contains(text,Qt::CaseInsensitive))
            qobject_cast<TabButton *>(tabsLayout->itemAt(i)->widget())->show();
        else
            qobject_cast<TabButton *>(tabsLayout->itemAt(i)->widget())->hide();
    }*/

    int foundItems = 0;
    lblCount->setText(tr("%1 item(s) found").arg(QString::number(foundItems)));
}

bool DownloadsPanel::isDownloading()
{
    bool busy = false;

    for(int i(0);i<centralLayout->count()-1;i++)
    {
        int state = qobject_cast<DownloadItem *>(centralLayout->itemAt(i)->widget())->fileToDownload->state();
        if(state == QWebEngineDownloadItem::DownloadInProgress)
            busy = true;
    }

    return busy;
}

void DownloadsPanel::downloadRequested(QWebEngineDownloadItem *item)
{
    if(!settings->value("Downloads/AskSave").toBool())
        item->setPath(settings->value("Downloads/DownloadPath").toString() + "/" + item->url().fileName());

    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), QString(item->path()), tr("All Files (*);;HTML Files (*.html)"));
    if (!path.isEmpty())
    {
        item->setPath(path);

        DownloadItem *btnItem = new DownloadItem;
        btnItem->setItem(item);
        btnItem->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);
        centralLayout->insertWidget(centralLayout->count()-1,btnItem);
        connect(btnItem,SIGNAL(itemRemoved()),this,SLOT(update()));

        lblCount->setText(tr("%1 download(s)").arg(QString::number(centralLayout->count()-1)));
        emit showDownloads();
    }
    else
        item->cancel();

    loadSettings();
}

void DownloadsPanel::update()
{
    lblCount->setText(tr("%1 download(s)").arg(QString::number(centralLayout->count()-1)));
    qDebug()<<centralLayout->count()-1;
}

void DownloadsPanel::loadSettings()
{
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    QSize iconSize = QSize(settings->value("Interface/ToolBar/IconSize").toInt(),settings->value("Interface/ToolBar/IconSize").toInt());
    toolBar->setIconSize(iconSize);

    actSearch->setIcon(QIcon(iconColor + "search.png"));

   /* for(int i(0);i<centralLayout->count();i++)
    {
        qobject_cast<QToolButton *>(centralLayout->itemAt(i)->widget())->setIcon(QIcon(iconColor + "downloads.png"));
        qobject_cast<QToolButton *>(centralLayout->itemAt(i)->widget())->setIconSize(QSize(iconSize,iconSize));
    }*/
}

// ITEM

DownloadItem::DownloadItem(QWidget *parent) : QProgressBar(parent)
{
    btnIcon = new QToolButton;
    btnTitle = new QToolButton;
    btnDescription = new QToolButton;
    QPushButton *btnMenu = new QPushButton;
    setObjectName("Item");
    setTextVisible(false);

    settings = new QSettings;
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();

    btnIcon->setObjectName("Icon");
    btnTitle->setObjectName("Title");
    btnDescription->setObjectName("Description");
    btnMenu->setObjectName("Menu");

    btnIcon->setIconSize(QSize(iconSize,iconSize));
    btnMenu->setIconSize(QSize(iconSize,iconSize));
    btnTitle->setIconSize(QSize(0,0));
    btnDescription->setIconSize(QSize(0,0));

    btnTitle->setIcon(QIcon("none.png"));
    btnDescription->setIcon(QIcon("none.png"));
    btnMenu->setIcon(QIcon(iconColor+"menu2.png"));

    btnTitle->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnDescription->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    btnIcon->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    btnTitle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    btnDescription->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    btnMenu->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);

   /* connect(btnIcon,SIGNAL(clicked()),this,SIGNAL(exec()));
    connect(btnTitle,SIGNAL(clicked()),this,SIGNAL(exec()));*/

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(btnIcon,0,0,2,1);
    mainLayout->addWidget(btnTitle,0,1);
    mainLayout->addWidget(btnDescription,1,1);
    mainLayout->addWidget(btnMenu,0,2,2,1);
    setLayout(mainLayout);

    genActs();

    QMenu *mainMenu = new QMenu;
    mainMenu->addAction(actOpenFile);
    mainMenu->addAction(actCancel);
   // mainMenu->addAction(actRetry);
    mainMenu->addAction(actRemove);
    btnMenu->setMenu(mainMenu);
    setFixedHeight(60);
}

DownloadItem::~DownloadItem()
{

}

void DownloadItem::genActs()
{
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    actCancel = new QAction(QIcon(iconColor+"cross.png"),tr("Cancel"),this);
    connect(actCancel,SIGNAL(triggered()),this,SLOT(cancel()));

   /* actRetry = new QAction(QIcon(iconColor+"undo.png"),tr("Retry"),this);
    connect(actRetry,SIGNAL(triggered()),fileToDownload,SLOT(accept()));*/

    actRemove = new QAction(QIcon(iconColor+"cross.png"),tr("Delete"),this);
    actRemove->setVisible(false);
    connect(actRemove,SIGNAL(triggered(bool)),this,SLOT(deleteLater()));

    actOpenFile = new QAction(QIcon(iconColor+"open.png"),tr("Open"),this);
    actOpenFile->setVisible(false);
    connect(actOpenFile,SIGNAL(triggered(bool)),this,SLOT(openFile()));
}

void DownloadItem::setItem(QWebEngineDownloadItem *item)
{
    item->accept();
    fileToDownload = item;

    btnIcon->setIcon(QIcon(":/img/white/downloads.png"));
    btnTitle->setText(item->url().fileName());
    setValue(0);
    setRange(0,0);

    fileOnPC = item->path();

    connect(item,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(setProgression(qint64,qint64)));
    connect(item,SIGNAL(finished()),this,SLOT(setFinished()));
    connect(item,SIGNAL(finished()),this,SIGNAL(itemRemoved()));
    connect(item,SIGNAL(stateChanged(DownloadState)),this,SLOT(stateChanged()));
}

void DownloadItem::setProgression(qint64 received, qint64 total)
{
    setRange(0,100);
    setMaximum(100);
    setValue(((received * 100) / total));
    btnDescription->setText(tr("Downloading - %1%").arg(QString::number(value())));
}

void DownloadItem::cancel()
{
    setValue(0);
    fileToDownload->cancel();

    btnDescription->setText(tr("Canceled"));

    actRemove->setVisible(true);
    actCancel->setVisible(false);
    //actRetry->setVisible(true);
}

void DownloadItem::openFile()
{
    QUrl url(QUrl(fileOnPC, QUrl::TolerantMode));
    url.setScheme("file");
    QDesktopServices::openUrl(url);
}

void DownloadItem::setFinished()
{
    setValue(0);

    actRemove->setVisible(true);
    actCancel->setVisible(false);

    if(fileToDownload->state() == QWebEngineDownloadItem::DownloadCompleted)
    {
        btnDescription->setText(tr("Completed"));

        actOpenFile->setVisible(true);
        connect(btnIcon,SIGNAL(clicked(bool)),this,SLOT(openFile()));
        connect(btnTitle,SIGNAL(clicked(bool)),this,SLOT(openFile()));
        connect(btnDescription,SIGNAL(clicked(bool)),this,SLOT(openFile()));

        if(settings->value("Downloads/OpenAfterDownload").toBool())
            openFile();
        if(settings->value("Downloads/RemoveList").toBool())
            hide();
    }
}

void DownloadItem::stateChanged()
{
    if(fileToDownload->state() == QWebEngineDownloadItem::DownloadInterrupted)
    {
        btnDescription->setText(tr("Failed"));

        actRemove->setVisible(true);
        actCancel->setVisible(false);
        //actRetry->setVisible(true);
    }
}
