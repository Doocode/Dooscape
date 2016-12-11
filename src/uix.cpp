#include "uix.h"

HideBlock::HideBlock(QString title, QWidget *parent) :
    QFrame(parent)
{
    setObjectName("HideBlock");

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    btnTitle = new QToolButton;
    btnTitle->setObjectName("BtnTitle");
    btnTitle->setText(title);
    btnTitle->setIcon(QIcon("null.png"));
    btnTitle->setIconSize(QSize(0,0));
    btnTitle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    btnTitle->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mainLayout->addWidget(btnTitle);
    connect(btnTitle,SIGNAL(clicked()),this,SLOT(showBlock()));

    block = new QFrame;
    block->setObjectName("Block");
    block->hide();
    mainLayout->addWidget(block);
}

void HideBlock::setBlockLayout(QLayout *layout)
{
    block->setLayout(layout);
    blockHeight = block->geometry().height();
}

void HideBlock::showBlock()
{
    if(block->isVisible())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(block,"maximumHeight");
        animation->setDuration(350);
        animation->setEasingCurve(QEasingCurve::OutCirc);
        animation->setStartValue(blockHeight);
        animation->setEndValue(0);
        animation->start();

        connect(animation,SIGNAL(finished()),block,SLOT(hide()));
    }
    else
    {
        block->show();

        QPropertyAnimation *animation = new QPropertyAnimation(block,"maximumHeight");
        animation->setDuration(500);
        animation->setEasingCurve(QEasingCurve::InCirc);
        animation->setStartValue(0);
        animation->setEndValue(blockHeight+10000);
        animation->start();
    }
}

void HideBlock::showBlock(bool show)
{
    if(!show)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(block,"maximumHeight");
        animation->setDuration(350);
        animation->setEasingCurve(QEasingCurve::OutCirc);
        animation->setStartValue(blockHeight);
        animation->setEndValue(0);
        animation->start();

        connect(animation,SIGNAL(finished()),block,SLOT(hide()));
    }
    else
    {
        block->show();

        QPropertyAnimation *animation = new QPropertyAnimation(block,"maximumHeight");
        animation->setDuration(500);
        animation->setEasingCurve(QEasingCurve::InCirc);
        animation->setStartValue(0);
        animation->setEndValue(blockHeight+10000);
        animation->start();
    }
}

QLayout *HideBlock::blockLayout()
{
    return block->layout();
}


PreviewBox::PreviewBox(QWidget *parent) :
    QLabel(parent)
{
    setFixedSize(200,200);
    setObjectName("PreviewButton");
    setAlignment(Qt::AlignCenter);

    QPushButton *btnEdit = new QPushButton;
    btnEdit->setIconSize(QSize(60,60));
    btnEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(btnEdit,SIGNAL(clicked()),this,SIGNAL(editRequested()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(btnEdit);
    setLayout(mainLayout);

    setColor("rgb(200,200,200)");
}

void PreviewBox::setColor(QString colorName)
{
    color = colorName;
    setStyleSheet("QLabel {background: " + colorName + ";}");
}

void PreviewBox::setBgImage(QString bgImageUrl)
{
    bgImage = bgImageUrl;

    QPixmap pixmap(bgImageUrl);
    setPixmap(pixmap.scaled(200,200,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
}

// Panel

Panel::Panel(QWidget *parent) : QFrame(parent)
{
    setObjectName("PanelView");

    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    actBack = new QAction(QIcon(":/img/white/back.png"),tr("Back"),this);

    lblTitle = new QLabel(tr("Title"));
    lblTitle->setObjectName("Title");
    lblTitle->setWordWrap(true);
    lblTitle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    searchBar = new QLineEdit;
    searchBar->setPlaceholderText(tr("Search"));
    searchBar->hide();

    lblCount = new QLabel;
    lblCount->setObjectName("Counter");

    toolBar = new QToolBar;
    toolBar->setObjectName("ActionsBar");
    toolBar->addAction(actBack);
    toolBar->addWidget(lblTitle);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->setContentsMargins(0,0,0,0);
    centralLayout->setSpacing(0);
    centralLayout->addWidget(toolBar);
    centralLayout->addWidget(searchBar);
    centralLayout->addWidget(lblCount);
    centralLayout->addWidget(scrollArea);
    setLayout(centralLayout);
}

Panel::~Panel()
{

}



RoundedLabel::RoundedLabel(QWidget *parent) :
    QLabel(parent)
{
    // Some settings
    setRounded(true);
    setFixedSize(200,200);
    setAlignment(Qt::AlignCenter);
}

void RoundedLabel::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(!bgImage.isEmpty())
    {
        QImage image(bgImage);
        QBrush brush(image.scaled(width(),height(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
        painter.setBrush(brush);
    }

    painter.setPen(Qt::transparent);
    if(isRounded)
        painter.drawRoundedRect(0, 0, width(), height(), width()/2, height()/2);
    else
        painter.drawRoundedRect(0, 0, width(), height(), 0, 0);
}

void RoundedLabel::setRounded(bool rounded)
{
    isRounded = rounded;
}

void RoundedLabel::setImage(QString imageUrl)
{
    bgImage = imageUrl;

    QPixmap pixmap(imageUrl);
    setPixmap(pixmap.scaled(width(),height(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
}
