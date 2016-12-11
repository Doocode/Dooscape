#include "menupanel.h"

MenuPanel::MenuPanel(QWidget *parent) : Panel(parent)
{
    setObjectName("MainMenu");
    settings = new QSettings;

    lblTitle->setText(tr("All actions"));

    connect(searchBar,SIGNAL(textEdited(QString)),this,SLOT(searchItem(QString)));

    lblCount->setText(tr("All items"));

    genActs();

    QSettings *settings = new QSettings;
    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();

    toolBar->setIconSize(QSize(iconSize,iconSize));
    toolBar->addAction(actCollapseExpandAll);
    toolBar->addAction(actSearch);

    layBoxes = new QVBoxLayout;
    layBoxes->setMargin(0);
    layBoxes->setSpacing(0);

    QFrame *centralWidget = new QFrame;
    centralWidget->setLayout(layBoxes);
    scrollArea->setWidget(centralWidget);

    isExtended = false;
}

void MenuPanel::genActs()
{
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    actSearch = new QAction(QIcon(iconColor+"search.png"),tr("Search"),this);
    actSearch->setCheckable(true);
    actSearch->setVisible(false);
    connect(actSearch,SIGNAL(triggered()),this,SLOT(search()));

    actCollapseExpandAll = new QAction(QIcon(iconColor+"menu.png"),tr("Expand/collapse all"),this);
    connect(actCollapseExpandAll,SIGNAL(triggered()),this,SLOT(expandOrCollapse()));
}

void MenuPanel::search()
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

void MenuPanel::searchItem(QString text)
{
    // Set all visible
    for(int i(0);i<layBoxes->count()-1;i++)
    {
        qobject_cast<HideBlock *>(layBoxes->itemAt(i)->widget())->setVisible(true);

        QVBoxLayout *layBlock = qobject_cast<QVBoxLayout *>(qobject_cast<HideBlock *>(layBoxes->itemAt(i)->widget())->blockLayout());
        QToolBar *frame = qobject_cast<QToolBar *>(layBlock->itemAt(0)->widget());

        int total = frame->actions().count();

        for(int act(0);act!=total;act++)
            qobject_cast<QWidget *>(frame->widgetForAction(qobject_cast<QAction *>(frame->actions().at(act))))->setVisible(true);
            //qobject_cast<QAction *>(frame->actions().at(i))->setVisible(true);
    }

    // If search nothing
    if(text=="")
        lblCount->setText(tr("All items")); // Display all
    else // Else if it search something
    {
        int foundItems = 0;

        for(int i(0);i<layBoxes->count()-1;i++)
        {
            QVBoxLayout *layBlock = qobject_cast<QVBoxLayout *>(qobject_cast<HideBlock *>(layBoxes->itemAt(i)->widget())->blockLayout());
            QToolBar *frame = qobject_cast<QToolBar *>(layBlock->itemAt(0)->widget());

            int total = frame->actions().count();
            int visibleItems = 0;

            for(int act(0);act!=total;act++)
            {
                QString title = qobject_cast<QAction *>(frame->actions().at(act))->text();

                if(title.contains(text,Qt::CaseInsensitive))
                {
                    //qobject_cast<QAction *>(frame->actions().at(act))->setVisible(true);
                    qobject_cast<QWidget *>(frame->widgetForAction(qobject_cast<QAction *>(frame->actions().at(act))))->setVisible(true);
                    visibleItems = visibleItems+1;
                }
                else
                    qobject_cast<QWidget *>(frame->widgetForAction(qobject_cast<QAction *>(frame->actions().at(act))))->setVisible(false);
                    //qobject_cast<QAction *>(frame->actions().at(act))->setVisible(false);
            }

            if(visibleItems==0)
                qobject_cast<HideBlock *>(layBoxes->itemAt(i)->widget())->hide();
        }

        lblCount->setText(tr("%1 item(s) found").arg(QString::number(foundItems)));
    }
}

QToolBar *MenuPanel::addGroup(QString name)
{
    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();

    QToolBar *frame = new QToolBar;
    frame->setOrientation(Qt::Vertical);
    frame->setIconSize(QSize(iconSize,iconSize));
    frame->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    frame->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    QVBoxLayout *layBox = new QVBoxLayout;
    layBox->setMargin(0);
    layBox->addWidget(frame);

    HideBlock *block = new HideBlock(name);
    block->setObjectName("Group");
    block->setBlockLayout(layBox);
   // block->showBlock();

    layBoxes->addWidget(block);
    layBoxes->setAlignment(block,Qt::AlignTop);

    return frame;
}

void MenuPanel::addSpacer()
{
    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Expanding);
    layBoxes->addWidget(spacer);
}

void MenuPanel::expandOrCollapse()
{
    isExtended = !isExtended;

    for(int i(0);i<layBoxes->count()-1;i++)
        qobject_cast<HideBlock *>(layBoxes->itemAt(i)->widget())->showBlock(isExtended);
}

void MenuPanel::loadSettings()
{
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    QSize iconSize = QSize(settings->value("Interface/ToolBar/IconSize").toInt(),settings->value("Interface/ToolBar/IconSize").toInt());
    toolBar->setIconSize(iconSize);

    for(int i(0);i<layBoxes->count()-1;i++)
        qobject_cast<QToolBar *>(qobject_cast<HideBlock *>(layBoxes->itemAt(i)->widget())->blockLayout()->itemAt(0)->widget())->setIconSize(iconSize);

    actSearch->setIcon(QIcon(iconColor+"search.png"));
    actCollapseExpandAll->setIcon(QIcon(iconColor+"menu.png"));
}
