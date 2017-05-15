#include "introducer.h"

Introducer::Introducer(QWidget *parent) : QFrame(parent)
{
    stack = new QStackedWidget;
    stack->addWidget(genIntro());
    stack->addWidget(genAccount());
    stack->addWidget(genCustomize());
    stack->addWidget(genFinish());
   /* stack->setMaximumWidth(1000);
    stack->setMinimumWidth(500);*/

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(stack);
    mainLayout->setAlignment(stack,Qt::AlignCenter);
    mainLayout->addWidget(genToolbar());
    setLayout(mainLayout);

    setColor("rgb(0,100,200)");
    //setMinimumSize(QSize(700,500));
}

QToolBar *Introducer::genToolbar()
{
    actBack = new QAction(QIcon(":/img/white/back.png"),tr("Back"),this);
    actBack->setVisible(false);
    connect(actBack,SIGNAL(triggered()),this,SLOT(execNavig()));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);

    actForward = new QAction(QIcon(":/img/white/forward.png"),tr("Continue"),this);
    connect(actForward,SIGNAL(triggered()),this,SLOT(execNavig()));

    QToolBar *toolBar = new QToolBar;
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->setIconSize(QSize(16,16));
    toolBar->addWidget(spacer);
    toolBar->addAction(actBack);
    toolBar->addAction(actForward);
    return toolBar;
}

IntroducerFrame *Introducer::genIntroducerFrame(QString title, QString text, QLayout *layout)
{
    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(layout,0,0);
    mainLayout->addWidget(spacer,1,1);

    IntroducerFrame *frame = new IntroducerFrame(title,text);
    frame->setBoxLayout(mainLayout);
    return frame;
}

IntroducerFrame *Introducer::genIntro()
{
    return genIntroducerFrame(
                tr("Welcome"),
                tr("Welcome to the Setup Wizard of %1. We will guide you through setting up and customizing your browser. To begin, click Continue.").arg(QCoreApplication::applicationName())
                );
}

IntroducerFrame *Introducer::genAccount()
{
    QLabel *lblPseudo = new QLabel(tr("<b>Pseudo</b>"));
    editPseudo = new QLineEdit;
    editPseudo->setText(QDir::home().dirName());

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(lblPseudo,0,0);
    layout->addWidget(editPseudo,0,1);

    return genIntroducerFrame(
                tr("Account"),
                tr("As it is a personal navigator, you must meet certain information that will allow others to recognize who owns this browser."),
                layout
                );
}

IntroducerFrame *Introducer::genCustomize()
{
    QLabel *lblColor = new QLabel(tr("<b>Main color</b>"));
    editColor = new PreviewBox;
    editColor->setColor("rgb(0,120,200)");
    connect(editColor,SIGNAL(editRequested()),this,SLOT(chooseColor()));

    QLabel *lblBgImg = new QLabel(tr("<b>Background image</b>"));
    editBgImg = new PreviewBox;
    editBgImg->setBgImage(":/img/background.png");
    connect(editBgImg,SIGNAL(editRequested()),this,SLOT(chooseBgImage()));

    QLabel *lblProfile = new QLabel(tr("<b>Profile photo</b>"));
    editProfile = new PreviewBox;
    editProfile->setBgImage(":/img/user.png");
    connect(editProfile,SIGNAL(editRequested()),this,SLOT(chooseBgImage()));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(lblColor,0,0);
    layout->addWidget(editColor,1,0);
    layout->addWidget(lblBgImg,0,1);
    layout->addWidget(editBgImg,1,1);
    layout->addWidget(lblProfile,0,2);
    layout->addWidget(editProfile,1,2);

    return genIntroducerFrame(
                tr("Customize"),
                tr("Choose a color, your background image and a profile photo."),
                layout
                );
}

IntroducerFrame *Introducer::genFinish()
{
    QPushButton *btnEdit = new QPushButton(tr("Edit settings"));
    connect(btnEdit,SIGNAL(clicked()),this,SLOT(saveSettings()));
    connect(btnEdit,SIGNAL(clicked()),this,SIGNAL(showConfig()));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(btnEdit,0,0);
    layout->setAlignment(btnEdit,Qt::AlignLeft);

    return genIntroducerFrame(
                tr("Finish"),
                tr("Now all is OK ! You can start browsing or continue edit settings."),
                layout
                );
}

void Introducer::execNavig()
{
    int index = stack->currentIndex();
    if(sender()==actBack)
    {
        if(index==1)
        {
            actBack->setVisible(false);
            stack->setCurrentIndex(0);
        }
        else if(index>1)
        {
            actBack->setVisible(true);
            stack->setCurrentIndex(index-1);
        }
    }
    else if(sender()==actForward)
    {
        if(index==stack->count()-1)
        {
            emit showLoader();
            saveSettings();
            emit startBrowsing();
        }
        else if(index<stack->count())
        {
            actBack->setVisible(true);
            stack->setCurrentIndex(index+1);
        }
    }
}

void Introducer::chooseColor()
{
    QColor choosedColor = QColorDialog::getColor(editColor->color);
    if(choosedColor.name()=="#000000")
        return;

    editColor->setColor(choosedColor.name());
}

void Introducer::chooseBgImage()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), tr("Picture (*.bmp *.gif *.jpeg *.jpg *.png);;All files (*)"));
    if(path.isEmpty())
        return;

    if(sender()==editBgImg)
        editBgImg->setBgImage(path);
    else if(sender()==editProfile)
        editProfile->setBgImage(path);
}

void Introducer::setValue(QString key, QVariant value)
{
    QSettings *settings = new QSettings;
  //  if(!settings->contains(key))
        settings->setValue(key,value);
}

void Introducer::saveSettings()
{
    QSettings *settings = new QSettings;

    settings->setValue("Version",QCoreApplication::applicationVersion());
    setValue("LastSession/ProperlyClosed",true);

    setValue("Interface/Color",editColor->color);
    setValue("Interface/BgImgUrl",editBgImg->bgImage);

    setValue("Navigation/Startup/Empty",false);
    setValue("Navigation/Startup/Home",true);
    setValue("Navigation/Startup/Specific",false);
    setValue("Navigation/Startup/SpecificPage","");
    setValue("Navigation/NewTab/Empty",false);
    setValue("Navigation/NewTab/Home",true);
    setValue("Navigation/NewTab/Specific",false);
    setValue("Navigation/NewTab/SpecificPage","");
    setValue("Navigation/Home","http://doosearch.esy.es/");
    setValue("Navigation/PrefixSearch","https://www.google.fr/search?q=");
    setValue("Navigation/SuffixSearch","");
    setValue("Navigation/UserAgent/Enable",false);
    setValue("Navigation/UserAgent/Value","");

    setValue("Interface/Position/Toolbar","top");
    setValue("Interface/ToolBar/IconSize",16);
    setValue("Interface/IconColor","white");

    setValue("Interface/ToolBar/AddTab",false);
    setValue("Interface/ToolBar/Back",true);
    setValue("Interface/ToolBar/Forward",true);
    setValue("Interface/ToolBar/Reload",true);
    setValue("Interface/ToolBar/Home",true);
    setValue("Interface/ToolBar/OpenUrlInOtherBrowser",false);
    setValue("Interface/ToolBar/Skin",false);
    setValue("Interface/ToolBar/Lock",false);
    setValue("Interface/ToolBar/Config",false);
    setValue("Interface/ToolBar/Full",false);
    setValue("Interface/ToolBar/Tabs",true);
    setValue("Interface/ToolBar/ReadList",false);
    setValue("Interface/ToolBar/Bookmarks",false);
    setValue("Interface/ToolBar/History",false);
    setValue("Interface/ToolBar/Downloads",false);
    setValue("Interface/ToolBar/Screen",false);
    setValue("Interface/ToolBar/Completer","left");

    setValue("Interface/Skins/AutoPreview",true);

    setValue("Downloads/AskSave",true);
    setValue("Downloads/DownloadPath","");
    setValue("Downloads/OpenAfterDownload",false);
    setValue("Downloads/ShowNotification",true);
    setValue("Downloads/RemoveList",false);

    setValue("Interface/Tabbar/Enable",false);
    setValue("Interface/Tabbar/VisibleWithLessTwoTabs",false);
    setValue("Interface/Tabbar/Closable",true);
    setValue("Interface/Tabbar/Movable",false);

    setValue("Account/Photo",editProfile->bgImage);
    setValue("Account/Pseudo",editPseudo->text());
    setValue("Account/Password","");
    setValue("Account/Indice","");

    setValue("Security/Lock/AtStart",true);
    setValue("Security/Lock/AtClose",false);
    setValue("Security/Lock/ToConfig",true);
    setValue("Security/Lock/Locker",true);

    setValue("Navigation/Options/EnableJavaScript",true);
    setValue("Navigation/Options/EnableJavascriptCanOpenWindow",true);
    setValue("Navigation/Options/EnableJavascriptCanAccessClipboard",false);
    setValue("Navigation/Options/CurrentPageOnTitleBar",false);
    setValue("Interface/Animations",true);
}

// Screen

IntroducerFrame::IntroducerFrame(QString title, QString text, QWidget *parent):
    QFrame(parent)
{
    QLabel *lblTitle = new QLabel(title);
    lblTitle->setObjectName("Title");

    QLabel *lblText = new QLabel;
    lblText->setWordWrap(true);
    lblText->setText(text);

    mContainer = new QFrame;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(lblTitle);
    mainLayout->addWidget(lblText);
    mainLayout->addWidget(mContainer);
    setLayout(mainLayout);
}

void IntroducerFrame::setBoxLayout(QLayout *layout)
{
    layout->setMargin(0);
    mContainer->setLayout(layout);
}

void Introducer::setColor(QString color)
{
    setStyleSheet("\
                  QFrame {\
                      background: transparent;\
                      border: none;\
                  }\
                  Introducer {\
                      background: rgb(60,60,60);\
                      padding: 30px;\
                  }\
                  QLabel {\
                      color: white;\
                      font-size: 14px;\
                      font-family: arial;\
                      margin: 5px;\
                  }\
                  QLineEdit {\
                      font-family: arial;\
                      background: rgba(255,255,255,.1);\
                      border: 1px solid rgba(255,255,255,.3);\
                      border-radius: 4px;\
                      padding: 10px 20px;\
                      font-size: 16px;\
                      color: white;\
                      margin-left: 10px;\
                  }\
                  #Title {\
                      color: white;\
                      font-size: 30px;\
                      font-family: arial;\
                      padding: 0px;\
                      margin: 0px;\
                      margin-top: 10px;\
                      margin-bottom: 10px;\
                  }\
                  QToolBar {\
                      background: transparent;\
                      border: none;\
                      padding: 0px;\
                      spacing: 15px;\
                  }\
                  QToolButton,QPushButton {\
                      font-family: arial;\
                      margin-left: 5px;\
                      background: rgb(60,60,60);\
                      border: 2px solid white;\
                      border-radius: 4px;\
                      padding: 10px 20px;\
                      font-size: 14px;\
                      color: white;\
                  }\
                  QToolButton:hover,QPushButton:hover {\
                      background: rgb(70,70,70);\
                  }\
                  QToolButton:pressed,QPushButton:pressed {\
                      background: "+color+";\
                      border-color: "+color+";\
                  }\
                  #PreviewButton {\
                      background-color: rgba(0,0,0,.1);\
                      border: none;\
                      margin: 5px;\
                  }\
                  #PreviewButton QPushButton {\
                      border-radius: 0px;\
                      border: none;\
                      background: none;\
                      margin: 0px;\
                  }\
                  #PreviewButton QPushButton:hover {\
                      background: rgba(0,0,0,.3);\
                      border: none;\
                  }\
                  #PreviewButton QPushButton:pressed {\
                      background: rgba(0,0,0,.5);\
                      border: none;\
                  }");
}
