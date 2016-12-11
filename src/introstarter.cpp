#include "introstarter.h"

IntroStarter::IntroStarter(QWidget *parent) : QFrame(parent)
{
    genIntro();
    genAccount();
    genToolbar();
    genCustomize();
    genFinish();

    stack = new QStackedWidget;
    stack->addWidget(scrIntro);
    stack->addWidget(scrAccount);
    stack->addWidget(scrCustomize);
    stack->addWidget(scrFinish);
    stack->setMaximumWidth(1000);
    stack->setMinimumWidth(500);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(stack);
    mainLayout->setAlignment(stack,Qt::AlignCenter);
    mainLayout->addWidget(mainToolBar);
    setLayout(mainLayout);

    setColor("rgb(0,100,200)");
    setMinimumSize(QSize(700,500));
}

IntroStarter::~IntroStarter()
{

}

void IntroStarter::genToolbar()
{
    actBack = new QAction(QIcon(":/img/white/back.png"),tr("Back"),this);
    actBack->setVisible(false);
    connect(actBack,SIGNAL(triggered()),this,SLOT(execNavig()));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);

    actForward = new QAction(QIcon(":/img/white/forward.png"),tr("Continue"),this);
    connect(actForward,SIGNAL(triggered()),this,SLOT(execNavig()));

    mainToolBar = new QToolBar;
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mainToolBar->setIconSize(QSize(16,16));
    mainToolBar->addWidget(spacer);
    mainToolBar->addAction(actBack);
    mainToolBar->addAction(actForward);
}

void IntroStarter::genIntro()
{
    scrIntro = new IntroScreen(tr("Welcome"));

    QLabel *text = new QLabel;
    text->setWordWrap(true);
   // text->setText(tr("Bienvenue dans l'assistant de configuration rapide de %1. Nous allons vous guider dans la configuration et la personnalisation de votre navigateur. Pour commencer, cliquez sur Continuer.").arg(QCoreApplication::applicationName());
    text->setText(tr("Welcome to the Setup Wizard of %1. We will guide you through setting up and customizing your browser. To begin, click Continue.").arg(QCoreApplication::applicationName()));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(text);
    mainLayout->addWidget(spacer);

    scrIntro->setBoxLayout(mainLayout);
}

void IntroStarter::genAccount()
{
    scrAccount = new IntroScreen(tr("Account"));

    QLabel *text = new QLabel;
    text->setWordWrap(true);
   // text->setText(tr("Comme c'est un navigateur personnel, vous devez remplir certaines informations qui vont permettre aux autres de reconnaître à qui appartient ce navigateur."));
    text->setText(tr("As it is a personal navigator, you must meet certain information that will allow others to recognize who owns this browser."));

    QLabel *lblPseudo = new QLabel(tr("<b>Pseudo</b>"));
    editPseudo = new QLineEdit;
    editPseudo->setText(QDir::home().dirName());

   // QLabel *lblPass1 = new QLabel(tr("Password"));
    editPass1 = new QLineEdit;
    editPass1->setEchoMode(QLineEdit::Password);

    //QLabel *lblPass2 = new QLabel(tr("Retype the password"));
    editPass2 = new QLineEdit;
    editPass2->setEchoMode(QLineEdit::Password);

    //QLabel *lblIndice = new QLabel(tr("Indication"));
    editIndice = new QLineEdit;

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(text,0,0,1,2);

    mainLayout->addWidget(lblPseudo,1,0);
    mainLayout->addWidget(editPseudo,1,1);/*

    mainLayout->addWidget(lblPass1,2,0);
    mainLayout->addWidget(editPass1,2,1);

    mainLayout->addWidget(lblPass2,3,0);
    mainLayout->addWidget(editPass2,3,1);

    mainLayout->addWidget(lblIndice,4,0);
    mainLayout->addWidget(editIndice,4,1);*/

    mainLayout->addWidget(spacer,5,1);

    scrAccount->setBoxLayout(mainLayout);
}

void IntroStarter::genCustomize()
{
    scrCustomize = new IntroScreen(tr("Customize"));

    QLabel *text = new QLabel;
    text->setText(tr("Choose a color, your background image and a profile photo."));

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

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(text,0,0,1,3);
    mainLayout->addWidget(lblColor,1,0);
    mainLayout->addWidget(editColor,2,0);
    mainLayout->addWidget(lblBgImg,1,1);
    mainLayout->addWidget(editBgImg,2,1);
    mainLayout->addWidget(lblProfile,1,2);
    mainLayout->addWidget(editProfile,2,2);
    mainLayout->addWidget(spacer,3,3);

    scrCustomize->setBoxLayout(mainLayout);
}

void IntroStarter::genFinish()
{
    scrFinish = new IntroScreen(tr("Finish"));

    QLabel *text = new QLabel;
    text->setWordWrap(true);
    text->setText(tr("Now all is OK ! You can start browsing or continue edit settings."));

    QPushButton *btnEdit = new QPushButton(tr("Edit settings"));
    connect(btnEdit,SIGNAL(clicked()),this,SLOT(saveSettings()));
    connect(btnEdit,SIGNAL(clicked()),this,SIGNAL(showConfig()));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(text);
    mainLayout->addWidget(btnEdit);
    mainLayout->setAlignment(btnEdit,Qt::AlignLeft);
    mainLayout->addWidget(spacer);

    scrFinish->setBoxLayout(mainLayout);
}

void IntroStarter::execNavig()
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
            emit showBrowser();
        }
        else if(index<stack->count())
        {
            actBack->setVisible(true);
            stack->setCurrentIndex(index+1);
        }
    }
}

void IntroStarter::chooseColor()
{
    QColor choosedColor = QColorDialog::getColor(editColor->color);
    if(choosedColor.name()=="#000000")
        return;

    editColor->setColor(choosedColor.name());
}

void IntroStarter::chooseBgImage()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), tr("Picture (*.bmp *.gif *.jpeg *.jpg *.png);;All files (*)"));
    if(path.isEmpty())
        return;

    if(sender()==editBgImg)
        editBgImg->setBgImage(path);
    else if(sender()==editProfile)
        editProfile->setBgImage(path);
}

void IntroStarter::setValue(QString key, QVariant value)
{
    QSettings *settings = new QSettings;
  //  if(!settings->contains(key))
        settings->setValue(key,value);
}

void IntroStarter::saveSettings()
{
    QSettings *settings = new QSettings;

    settings->setValue("Version",QCoreApplication::applicationVersion());
    this->setValue("LastSession/ProperlyClosed",true);

    this->setValue("Interface/Color",editColor->color);
    this->setValue("Interface/BgImgUrl",editBgImg->bgImage);

    this->setValue("Navigation/Startup/Empty",false);
    this->setValue("Navigation/Startup/Home",true);
    this->setValue("Navigation/Startup/Specific",false);
    this->setValue("Navigation/Startup/SpecificPage","");
    this->setValue("Navigation/NewTab/Empty",false);
    this->setValue("Navigation/NewTab/Home",true);
    this->setValue("Navigation/NewTab/Specific",false);
    this->setValue("Navigation/NewTab/SpecificPage","");
    this->setValue("Navigation/Home","http://doosearch.esy.es/");
    this->setValue("Navigation/PrefixSearch","https://www.google.fr/search?q=");
    this->setValue("Navigation/SuffixSearch","");
    this->setValue("Navigation/UserAgent/Enable",false);
    this->setValue("Navigation/UserAgent/Value","");

    this->setValue("Interface/Position/Toolbar","top");
    this->setValue("Interface/ToolBar/IconSize",16);
    this->setValue("Interface/IconColor","white");

    this->setValue("Interface/ToolBar/AddTab",false);
    this->setValue("Interface/ToolBar/Back",true);
    this->setValue("Interface/ToolBar/Forward",true);
    this->setValue("Interface/ToolBar/Reload",true);
    this->setValue("Interface/ToolBar/Home",true);
    this->setValue("Interface/ToolBar/OpenUrlInOtherBrowser",false);
    this->setValue("Interface/ToolBar/Skin",false);
    this->setValue("Interface/ToolBar/Lock",false);
    this->setValue("Interface/ToolBar/Config",false);
    this->setValue("Interface/ToolBar/Full",false);
    this->setValue("Interface/ToolBar/Tabs",true);
    this->setValue("Interface/ToolBar/ReadList",false);
    this->setValue("Interface/ToolBar/Bookmarks",false);
    this->setValue("Interface/ToolBar/History",false);
    this->setValue("Interface/ToolBar/Downloads",false);
    this->setValue("Interface/ToolBar/Screen",false);
    this->setValue("Interface/ToolBar/Completer","left");

    this->setValue("Interface/Skins/AutoPreview",true);

    this->setValue("Downloads/AskSave",true);
    this->setValue("Downloads/DownloadPath","");
    this->setValue("Downloads/OpenAfterDownload",false);
    this->setValue("Downloads/ShowNotification",true);
    this->setValue("Downloads/RemoveList",false);

    this->setValue("Interface/Tabbar/Enable",false);
    this->setValue("Interface/Tabbar/VisibleWithLessTwoTabs",false);
    this->setValue("Interface/Tabbar/Closable",true);
    this->setValue("Interface/Tabbar/Movable",false);

    this->setValue("Account/Photo",editProfile->bgImage);
    this->setValue("Account/Pseudo",editPseudo->text());
    this->setValue("Account/Password",editPass1->text());
    this->setValue("Account/Indice",editIndice->text());

    this->setValue("Security/Lock/AtStart",true);
    this->setValue("Security/Lock/AtClose",false);
    this->setValue("Security/Lock/ToConfig",true);
    this->setValue("Security/Lock/Locker",true);

    this->setValue("Navigation/Options/EnableJavaScript",true);
    this->setValue("Navigation/Options/EnableJavascriptCanOpenWindow",true);
    this->setValue("Navigation/Options/EnableJavascriptCanAccessClipboard",false);
    this->setValue("Navigation/Options/CurrentPageOnTitleBar",false);
    this->setValue("Interface/Animations",true);
}

// Screen

IntroScreen::IntroScreen(QString title, QWidget *parent):
    QFrame(parent)
{
    if(title.isEmpty())
        title = tr("Title");

    lblTitle = new QLabel(title);
    lblTitle->setObjectName("Title");

    central = new QFrame;

    mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(lblTitle);
    mainLayout->addWidget(central);
    setLayout(mainLayout);
}

void IntroScreen::setBoxLayout(QLayout *layout)
{
    layout->setMargin(0);
    central->setLayout(layout);
}

void IntroStarter::setColor(QString color)
{
    setStyleSheet("\
                  QFrame {\
                      background: transparent;\
                      border: none;\
                  }\
                  IntroStarter {\
                      background: rgb(60,60,60);\
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
                      padding: 30px;\
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
