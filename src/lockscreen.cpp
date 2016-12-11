#include "lockscreen.h"

LockScreen::LockScreen(QWidget *parent) : QLabel(parent)
{
    settings = new QSettings;
    setMinimumSize(1,1);
    setAlignment(Qt::AlignCenter);

    genActs();
    genCentral();
    genForm();

    QStackedLayout *centralLayout = new QStackedLayout;
    centralLayout->setContentsMargins(0,0,0,0);
    centralLayout->setSpacing(0);
    centralLayout->addWidget(central);

    setLayout(centralLayout);

    updateTime();
}

LockScreen::~LockScreen()
{

}

void LockScreen::genActs()
{
    actUnlock = new QAction(tr("Unlock"),this);
    connect(actUnlock,SIGNAL(triggered()),this,SLOT(showForm()));

    actIndication = new QAction(tr("Indication"),this);
    connect(actIndication,SIGNAL(triggered()),this,SLOT(showIndication()));

    actLeaveMsg = new QAction(tr("Leave a message"),this);
    actLeaveMsg->setVisible(false);
   // connect(actLeaveMsg,SIGNAL(triggered()),this,SLOT(showIndication()));
}

void LockScreen::genCentral()
{
    lblTime = new QLabel(tr("00:00"));
    lblTime->setObjectName("Time");
   // lblTime->setAlignment(Qt::AlignCenter);

    lblDate = new QLabel(tr("Monday 1st January 0000"));
    lblDate->setObjectName("Date");
   // lblDate->setAlignment(Qt::AlignCenter);

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    toolBar = new QToolBar;
    toolBar->addAction(actUnlock);

    QVBoxLayout *screenLayout = new QVBoxLayout;
    screenLayout->setMargin(0);
    screenLayout->setSpacing(0);
    screenLayout->addWidget(lblTime);
    screenLayout->addWidget(lblDate);
    screenLayout->addWidget(spacer);
    screenLayout->addWidget(toolBar);

    central = new QFrame;
    central->setObjectName("Screen");
    central->setLayout(screenLayout);

    btnLock = new QPushButton(central);
    btnLock->setObjectName("TranslucidArea");
    btnLock->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    btnLock->setGeometry(QRect(0,0,this->geometry().width(),this->geometry().height()));
    connect(btnLock,SIGNAL(clicked()),this,SLOT(showForm()));

    lblScreenBrowser = new QLabel(central);
    lblScreenBrowser->setGeometry(QRect(this->geometry().width()/2,0,this->geometry().width(),this->geometry().height()));
}

void LockScreen::genForm()
{
    iconPhoto = new RoundedLabel;
    iconPhoto->setFixedSize(150,150);

    lblUserName = new QLabel(tr("User"));
    lblUserName->setObjectName("UserName");

    QVBoxLayout *userLayout = new QVBoxLayout;
    userLayout->setMargin(30);
    userLayout->addWidget(iconPhoto);
    userLayout->addWidget(lblUserName);

    editPass = new QLineEdit;
    editPass->setPlaceholderText(tr("Password"));
    editPass->setEchoMode(QLineEdit::Password);
    editPass->setMaximumWidth(300);
    connect(editPass,SIGNAL(returnPressed()),this,SLOT(unlockWindow()));
    connect(editPass,SIGNAL(textChanged(QString)),this,SLOT(showForm()));

    btnUnlock = new QToolButton;
    btnUnlock->setDefaultAction(actUnlock);
    connect(btnUnlock,SIGNAL(clicked(bool)),this,SLOT(unlockWindow()));

    btnIndication = new QToolButton;
    btnIndication->setDefaultAction(actIndication);

    btnLeaveMsg = new QToolButton;
    btnLeaveMsg->setDefaultAction(actLeaveMsg);

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *formLayout = new QGridLayout;
    formLayout->setMargin(30);
    formLayout->setMargin(0);
    formLayout->setSpacing(0);
    formLayout->addLayout(userLayout,0,0);
    formLayout->addWidget(editPass);
    formLayout->addWidget(btnUnlock);
    formLayout->addWidget(btnIndication);
   // formLayout->addWidget(btnLeaveMsg);
    formLayout->addWidget(spacer);

    for(int i(2);i<formLayout->count()-1;i++)
    {
        qobject_cast<QToolButton *>(formLayout->itemAt(i)->widget())->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        qobject_cast<QToolButton *>(formLayout->itemAt(i)->widget())->setSizePolicy(QSizePolicy::Expanding,formLayout->itemAt(i)->widget()->sizePolicy().horizontalPolicy());
    }

    form = new QFrame(central);
    form->setObjectName("Form");
    form->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    form->setLayout(formLayout);
    form->setFixedWidth(300);
    form->move(-300,0);
}

void LockScreen::loadSettings()
{
    bgImage = settings->value("Interface/BgImgUrl").toString();
    setPixmap(bgImage);
    iconPhoto->setImage(settings->value("Account/Photo").toString());
    lblUserName->setText(settings->value("Account/Pseudo").toString());
    editPass->clear();
    btnLock->hide();
    if(settings->value("Interface/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(form,"geometry");
        animation->setDuration(350);
        animation->setEasingCurve(QEasingCurve::InExpo);
        animation->setStartValue(QRect(0,0,form->geometry().width(),this->geometry().height()));
        animation->setEndValue(QRect(-300,0,form->geometry().width(),this->geometry().height()));
        animation->start();
    }
    else
        form->setGeometry(QRect(-300,0,form->geometry().width(),this->geometry().height()));

    int iconSize = settings->value("Interface/ToolBar/IconSize").toInt();
    btnUnlock->setIconSize(QSize(iconSize,iconSize));
    btnIndication->setIconSize(QSize(iconSize,iconSize));
    btnLeaveMsg->setIconSize(QSize(iconSize,iconSize));
    toolBar->setIconSize(QSize(iconSize,iconSize));

    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";
    actUnlock->setIcon(QIcon(iconColor+"lock.png"));
    actIndication->setIcon(QIcon(iconColor+"info.png"));
    actLeaveMsg->setIcon(QIcon(iconColor+"empty.png"));

    form->move(-300,0);
    lblScreenBrowser->move(-lblScreenBrowser->geometry().width(),0);

    if(settings->value("Interface/Animations").toBool() && lblScreenBrowser->pixmap()->size()==this->size())
    {
        QPropertyAnimation *animBrowser = new QPropertyAnimation(lblScreenBrowser,"geometry");
        animBrowser->setDuration(700);
        animBrowser->setEasingCurve(QEasingCurve::InQuint);
        animBrowser->setStartValue(this->geometry());
        animBrowser->setEndValue(QRect(-lblScreenBrowser->geometry().width(),0,lblScreenBrowser->geometry().width(),lblScreenBrowser->geometry().height()));
        animBrowser->start();
    }
}

void LockScreen::showForm()
{
    if(sender()==btnLock)
    {
        if(settings->value("Interface/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(form,"geometry");
            animation->setDuration(350);
            animation->setEasingCurve(QEasingCurve::InExpo);
            animation->setStartValue(QRect(0,0,form->geometry().width(),this->geometry().height()));
            animation->setEndValue(QRect(-300,0,form->geometry().width(),this->geometry().height()));
            animation->start();
        }
        else
            form->setGeometry(QRect(-300,0,form->geometry().width(),this->geometry().height()));

        btnLock->hide();
    }
    else
    {
        if(!settings->value("Account/Password").toString().isEmpty()&&settings->value("Security/Lock/Locker").toBool())
        {
            btnLock->show();
            editPass->setFocus();

            if(settings->value("Interface/Animations").toBool())
            {
                QPropertyAnimation *animation = new QPropertyAnimation(form,"geometry");
                animation->setDuration(550);
                animation->setEasingCurve(QEasingCurve::OutExpo);
                animation->setStartValue(form->geometry());
                animation->setEndValue(QRect(0,0,form->geometry().width(),this->geometry().height()));
                animation->start();
            }
            else
                form->move(0,0);
        }
        else
            unlockWindow();
    }
}

void LockScreen::showIndication()
{
    QMessageBox::information(this,tr("Indication"),tr("<h2 style=\"margin: 0px; font-weight: normal; font-family: arial; margin-bottom: 10px;\">Indication</h2>")+settings->value("Account/Indice").toString(),QMessageBox::Ok);
    editPass->setFocus();
}

void LockScreen::unlockWindow()
{
     if(settings->value("Account/Password").toString().isEmpty() || (settings->value("Account/Password").toString().isEmpty()&&!settings->value("Security/Lock/Locker").toBool()) || (editPass->text()==settings->value("Account/Password").toString()))
     {
         if(settings->value("Interface/Animations").toBool() && lblScreenBrowser->pixmap()->size()==this->size())
         {
             QPropertyAnimation *animForm = new QPropertyAnimation(form,"geometry");
             animForm->setDuration(350);
             animForm->setEasingCurve(QEasingCurve::OutExpo);
             animForm->setStartValue(form->geometry());
             animForm->setEndValue(QRect(-300,0,form->geometry().width(),this->geometry().height()));
             animForm->start();

             QPropertyAnimation *animBrowser = new QPropertyAnimation(lblScreenBrowser,"geometry");
             animBrowser->setDuration(550);
             animBrowser->setEasingCurve(QEasingCurve::OutExpo);
             animBrowser->setStartValue(lblScreenBrowser->geometry());
             animBrowser->setEndValue(QRect(0,0,lblScreenBrowser->geometry().width(),lblScreenBrowser->geometry().height()));
             animBrowser->start();
             connect(animBrowser,SIGNAL(finished()),this,SIGNAL(showBrowser()));
         }
         else
             emit showBrowser();
     }
     else if(editPass->text()!=settings->value("Account/Password").toString())
     {
         QMessageBox::critical(this,tr("Incorrect password"),tr("Incorrect password"),QMessageBox::Ok);
         editPass->setFocus();
         editPass->selectAll();
     }
}

void LockScreen::updateTime()
{
    lblTime->setText(QTime::currentTime().toString(tr("HH:mm ap")));
    lblDate->setText(QDate::currentDate().toString(tr("dddd MMMM d yyyy")));

    QTimer::singleShot(1000, this, SLOT(updateTime()));
}

void LockScreen::setScreenBrowser(QPixmap pixmap)
{
    lblScreenBrowser->setPixmap(pixmap);
}

void LockScreen::setPixmap(const QPixmap & p)
{
    pix = p;
    QLabel::setPixmap(p);
}

int LockScreen::heightForWidth(int width) const
{
    return ((qreal)pix.height()*width)/pix.width();
}

QSize LockScreen::sizeHint() const
{
    int w = this->width();
    return QSize(w,heightForWidth(w));
}

void LockScreen::resizeEvent(QResizeEvent *e)
{
    QLabel::setPixmap(pix.scaled(this->size(),Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    if(form->geometry().x()==0)
        form->setGeometry(0,0,300,this->geometry().height());
    else
        form->setGeometry(-300,0,300,this->geometry().height());

    btnLock->setGeometry(QRect(0,0,this->geometry().width(),this->geometry().height()));

    lblScreenBrowser->setGeometry(QRect(-this->geometry().width(),0,this->geometry().width(),this->geometry().height()));

    e->accept();
}
