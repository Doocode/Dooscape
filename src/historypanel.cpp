#include "historypanel.h"

HistoryPanel::HistoryPanel(QWidget *parent) : Panel(parent)
{
    setObjectName("History");
    settings = new QSettings;

    lblTitle->setText(tr("History"));

    connect(searchBar,SIGNAL(textEdited(QString)),this,SLOT(searchItem(QString)));

    lblCount->setText(tr("History of this tab"));

    genActs();

    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();
    toolBar->setIconSize(QSize(iconSize,iconSize));
    toolBar->addAction(actSearch);

    layItems = new QVBoxLayout;
    layItems->setMargin(0);
    layItems->setSpacing(0);
    layItems->setAlignment(Qt::AlignTop);

    QFrame *centralWidget = new QFrame;
    centralWidget->setLayout(layItems);
    centralWidget->setObjectName("List");
    scrollArea->setWidget(centralWidget);
}

void HistoryPanel::genActs()
{
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    actSearch = new QAction(QIcon(iconColor+"search.png"),tr("Search"),this);
    actSearch->setCheckable(true);
    actSearch->setVisible(false);
    connect(actSearch,SIGNAL(triggered()),this,SLOT(search()));
}

void HistoryPanel::search()
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

void HistoryPanel::searchItem(QString text)
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

void HistoryPanel::loadSettings()
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
