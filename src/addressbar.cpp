#include "addressbar.h"

AddressBar::AddressBar(QWidget *parent) : QProgressBar(parent)
{
    setTextVisible(false);
    setMinimumHeight(30);
    setValue(0);
    setRange(0,100);
    setObjectName("AddressBar");

    lineEdit = new QToolButton;
    btnIcon = new QPushButton;
    lblProgress = new QLabel("0%");
    lblProgress->hide();

    QSettings *settings = new QSettings;
    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();

    btnIcon->setIconSize(QSize(iconSize,iconSize));
    btnIcon->setFlat(true);
    btnIcon->setObjectName("Icon");

    lineEdit->setIcon(QIcon("null.png"));
    lineEdit->setText(tr("Search"));
    lineEdit->setIconSize(QSize(0,0));
    lineEdit->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    connect(btnIcon,SIGNAL(clicked()),this,SIGNAL(showCompleter()));
    connect(lineEdit,SIGNAL(clicked()),this,SIGNAL(showCompleter()));

    lineEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(btnIcon);
    mainLayout->addWidget(lineEdit);
    mainLayout->addWidget(lblProgress);
    setLayout(mainLayout);

   // setCursor(Qt::IBeamCursor);
}

AddressBar::~AddressBar()
{

}

void AddressBar::setProgression(int value)
{
    if(value==0||value==100)
    {
        lblProgress->hide();
        lblProgress->setText("0%");
        setRange(0,100);
    }
    else
    {
        lblProgress->show();
        lblProgress->setText(QString::number(value)+"%");
        setRange(0,0);
    }
}
