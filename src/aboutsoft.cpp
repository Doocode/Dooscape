#include "aboutsoft.h"

AboutSoft::AboutSoft(QWidget *parent) : Panel(parent)
{
    setObjectName("About");
    settings = new QSettings;

    lblTitle->setText(tr("About this browser"));
    lblCount->hide();

    QLabel *textView = new QLabel;
    textView->setObjectName("Text");
    textView->setWordWrap(true);
    textView->setCursor(Qt::IBeamCursor);
    textView->setTextInteractionFlags(Qt::TextBrowserInteraction);
   // textView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    textView->setText(tr("<b>%1 %2 by %3 :</b><br/>%1 is a web browser writed with the Qt framework and using QtWebEngine as web engine. It is different compared to its interface, customizable and animated.<br /><br /><b>Version : </b> %2 (%4)<br /><br /><b>Website : </b> %5<br /><br /><b>Licence : </b> %6<br /><br /><b>Developpers : </b> %7")
                      .arg(
                          QCoreApplication::applicationName(),    // Name of this software
                          QCoreApplication::applicationVersion(), // Version of this software
                          QCoreApplication::organizationName(),   // Organisation name
                          tr("Beta version"),                     // Type of version
                          QCoreApplication::organizationDomain(), // Organisation domain
                          tr("Licence GPL v.3"),                 // Licence
                          tr("aero15 (Creator)")                  // Developpers
                          ));

    QToolButton *btnQt = new QToolButton;
    btnQt->setText(tr("About Qt"));
    btnQt->setIcon(QIcon(":/img/information.png"));
    btnQt->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(btnQt,SIGNAL(clicked()),qApp,SLOT(aboutQt()));

    QToolButton *btnWebsite = new QToolButton;
    btnWebsite->setText(tr("Go to our website"));
    btnWebsite->setIcon(QIcon(":/img/information.png"));
    btnWebsite->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(btnWebsite,SIGNAL(clicked()),this,SIGNAL(openOrganisationDomain()));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QPushButton *lblIcon = new QPushButton;
    lblIcon->setIcon(QIcon(":/img/logo.png"));
    lblIcon->setObjectName("Logo");
    lblIcon->setIconSize(QSize(100,100));
    lblIcon->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,10);
    layout->addWidget(lblIcon);
    layout->setAlignment(lblIcon,Qt::AlignCenter);
    layout->addWidget(textView);
    layout->addWidget(btnWebsite);
    layout->setAlignment(btnWebsite,Qt::AlignLeft);
    layout->addWidget(btnQt);
    layout->setAlignment(btnQt,Qt::AlignLeft);
    layout->addWidget(spacer);

    QFrame *centralWidget = new QFrame;
    centralWidget->setLayout(layout);

    scrollArea->setWidget(centralWidget);

    loadSettings();
}

void AboutSoft::loadSettings()
{
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    actBack->setIcon(QIcon(iconColor+"back.png"));

    QSize iconSize = QSize(settings->value("Interface/ToolBar/IconSize").toInt(),settings->value("Interface/ToolBar/IconSize").toInt());
    toolBar->setIconSize(iconSize);
}

