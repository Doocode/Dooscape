#include "skinmanager.h"

SkinManager::SkinManager(QWidget *parent) : QMainWindow(parent)
{
    genUI();
}

SkinManager::~SkinManager()
{

}

void SkinManager::genUI()
{
    settings = new QSettings;
    iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    genActs();
    genMenu();
    genToolBars();

    // Remplacer par des radios
    model = new QDirModel;
    listView = new QListView;
    listView->setModel(model);
    listView->setRootIndex(model->index(QCoreApplication::applicationDirPath()+QDir::separator()+"skins"));
    listView->setIconSize(QSize(32,32));
   // listView->setVisible(false);
    connect(listView,SIGNAL(clicked(QModelIndex)),this,SLOT(editFile()));

    textEdit = new QPlainTextEdit;
    textEdit->setWordWrapMode(QTextOption::NoWrap);
    textEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(textEdit,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showMenu()));
    connect(textEdit,SIGNAL(textChanged()),this,SLOT(updateColor()));

    QList<int> list;
    list << 1000 << 300;

    QSplitter *central = new QSplitter(Qt::Vertical);
    central->addWidget(textEdit);
    central->addWidget(listView);
    central->setSizes(list);

    centralLayout = new QVBoxLayout;
    centralLayout->setContentsMargins(0,0,0,0);
    centralLayout->setSpacing(0);
    centralLayout->addWidget(mainToolBar);
    centralLayout->addWidget(central);

    QWidget *center = new QWidget;
    center->setLayout(centralLayout);

    setCentralWidget(center);

    genConnexion();

    isSaved = true;
}

void SkinManager::genActs()
{
    actBack = new QAction(tr("Back"),this);
    connect(actBack,SIGNAL(triggered()),this,SIGNAL(showBrowser()));

    actOpenFile = new QAction(tr("Open file"),this);
    actOpenFile->setShortcut(QKeySequence::Open);
    connect(actOpenFile,SIGNAL(triggered()),this,SLOT(openFile()));

    actSaveFile = new QAction(tr("Save file"),this);
    actSaveFile->setShortcut(QKeySequence::Save);
    connect(actSaveFile,SIGNAL(triggered()),this,SLOT(saveFile()));

    actPreview = new QAction(tr("Preview"),this);
    actPreview->setShortcut(QKeySequence("Ctrl+R"));
    connect(actPreview,SIGNAL(triggered()),this,SLOT(preview()));

    actPreviewBack = new QAction(tr("Preview and back"),this);
    actPreviewBack->setShortcut(QKeySequence("Ctrl+T"));
    actPreviewBack->setVisible(false);
    connect(actPreviewBack,SIGNAL(triggered()),this,SLOT(preview()));
    connect(actPreviewBack,SIGNAL(triggered()),this,SIGNAL(showBrowser()));


    actUndo = new QAction(tr("Undo"),this);
    actUndo->setShortcut(QKeySequence::Undo);

    actRedo = new QAction(tr("Redo"),this);
    actRedo->setShortcut(QKeySequence::Redo);

    actCut = new QAction(tr("Cut"),this);
    actCut->setShortcut(QKeySequence::Cut);

    actCopy = new QAction(tr("Copy"),this);
    actCopy->setShortcut(QKeySequence::Copy);

    actPaste = new QAction(tr("Paste"),this);
    actPaste->setShortcut(QKeySequence::Paste);

    actSelectAll = new QAction(tr("Select all"),this);
    actSelectAll->setShortcut(QKeySequence::SelectAll);


    actFind = new QAction(tr("Find text"),this);
    actFind->setShortcut(QKeySequence::Find);
    actFind->setVisible(false);
    connect(actFind,SIGNAL(triggered()),this,SLOT(showFind()));

}

void SkinManager::genMenu()
{
    mainMenu = new QMenu;
    mainMenu->addAction(actBack);
    mainMenu->addSeparator();
    mainMenu->addAction(actOpenFile);
    mainMenu->addAction(actSaveFile);
    mainMenu->addSeparator();
    mainMenu->addAction(actPreview);
    mainMenu->addAction(actPreviewBack);

    menuEdit = mainMenu->addMenu(tr("Edit"));
    menuEdit->addAction(actUndo);
    menuEdit->addAction(actRedo);
    menuEdit->addSeparator();
    menuEdit->addAction(actCut);
    menuEdit->addAction(actCopy);
    menuEdit->addAction(actPaste);
    menuEdit->addSeparator();
    menuEdit->addAction(actSelectAll);
    menuEdit->addSeparator();
    menuEdit->addAction(actFind);
}

void SkinManager::genToolBars()
{
    mainToolBar = new QToolBar;
   // mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    mainToolBar->addAction(actBack);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actOpenFile);
    mainToolBar->addAction(actSaveFile);

    btnEdit = new QPushButton;
    btnEdit->setIcon(menuEdit->icon());
    btnEdit->setIconSize(QSize(24,24));
    btnEdit->setMenu(menuEdit);
    mainToolBar->addWidget(btnEdit);

    mainToolBar->addSeparator();
    mainToolBar->addAction(actPreview);
    mainToolBar->addAction(actPreviewBack);

  //  mainToolBar->addSeparator();
    mainToolBar->addAction(actFind);

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);
    mainToolBar->addWidget(spacer);

    btnMenu = new QPushButton;
    btnMenu->setIconSize(QSize(24,24));
    btnMenu->setMenu(mainMenu);
    mainToolBar->addWidget(btnMenu);
}

void SkinManager::genConnexion()
{
    connect(actUndo,SIGNAL(triggered()),textEdit,SLOT(undo()));
    connect(actRedo,SIGNAL(triggered()),textEdit,SLOT(redo()));
    connect(actCut,SIGNAL(triggered()),textEdit,SLOT(cut()));
    connect(actCopy,SIGNAL(triggered()),textEdit,SLOT(copy()));
    connect(actPaste,SIGNAL(triggered()),textEdit,SLOT(paste()));
    connect(actSelectAll,SIGNAL(triggered()),textEdit,SLOT(selectAll()));
}

void SkinManager::start()
{
    loadSettings();
    listView->setRootIndex(model->index(QCoreApplication::applicationDirPath()+QDir::separator()+"skins"));
}

void SkinManager::loadConfig()
{
    QString fileName = "bi-default.nsf";
    if(settings->value("Interface/IconColor").toString()=="white")
        fileName = "wi-default.nsf";

    QString path(QCoreApplication::applicationDirPath() + QDir::separator() + "skins" + QDir::separator() + fileName);
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString text = file.readAll();
            file.close();

            text.replace("@color",settings->value("Interface/Color").toString());
            text.replace("@bgImgUrl",settings->value("Interface/BgImgUrl").toString());
            text.replace("@iconSize",settings->value("Interface/ToolBar/IconSize").toString());
            text.replace("@skinsPath",QCoreApplication::applicationDirPath()+"/skins");

            qApp->setStyleSheet(text);
        }
        else
            QMessageBox::warning(this, tr("Error"), tr("Unable to open skin file !"));
    }
    else
        return;
}

void SkinManager::loadSettings()
{
    // Toolbar position
    if(settings->value("Interface/Position/Toolbar").toString()=="top")
        centralLayout->setDirection(QBoxLayout::TopToBottom);
    else
        centralLayout->setDirection(QBoxLayout::BottomToTop);

    // Icon Size
    int size = settings->value("Interface/ToolBar/IconSize").toInt();
    QSize iconSize(size,size);

    mainToolBar->setIconSize(iconSize);
    btnMenu->setIconSize(iconSize);
    btnEdit->setIconSize(iconSize);

    // Icon Color
    iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    actBack->setIcon(QIcon(iconColor+"back.png"));
    actOpenFile->setIcon(QIcon(iconColor+"open.png"));
    actSaveFile->setIcon(QIcon(iconColor+"save.png"));
    actPreview->setIcon(QIcon(iconColor+"preview.png"));
    actPreviewBack->setIcon(QIcon(iconColor+"preview.png"));
    actUndo->setIcon(QIcon(iconColor+"undo.png"));
    actRedo->setIcon(QIcon(iconColor+"redo.png"));
    actCut->setIcon(QIcon(iconColor+"cut.png"));
    actCopy->setIcon(QIcon(iconColor+"copy.png"));
    actPaste->setIcon(QIcon(iconColor+"paste.png"));
    actSelectAll->setIcon(QIcon(iconColor+"select-all.png"));
    actFind->setIcon(QIcon(iconColor+"search.png"));

    menuEdit->setIcon(QIcon(iconColor+"edition.png"));

    btnMenu->setIcon(QIcon(iconColor+"menu.png"));
    btnEdit->setIcon(menuEdit->icon());
}

void SkinManager::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QString(QCoreApplication::applicationDirPath() + QDir::separator() + "skins"), tr("%1 skin files (*.nsf);;All files (*)").arg(QCoreApplication::applicationName()));
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString text = file.readAll();

            file.close();

            textEdit->clear();
            textEdit->setPlainText(text);

            isSaved = true;

            //urlSavedFile = path+QDir::separator()+file.fileName();

            text.replace("@color",settings->value("Interface/Color").toString());
            text.replace("@bgImgUrl",settings->value("Interface/BgImgUrl").toString());
            text.replace("@iconSize",settings->value("Interface/ToolBar/IconSize").toString());
            text.replace("@skinsPath",QCoreApplication::applicationDirPath()+"/skins");
            qApp->setStyleSheet(text);
        }
        else
            QMessageBox::warning(this, tr("Error"), tr("Unable to open file !"));
    }
    else
        return;
}

void SkinManager::saveFile()
{
    QString path;
    /*if(!urlSavedFile.isEmpty())
        path = QFileDialog::getSaveFileName(this, tr("Save a file"), path, tr("All files (*);;Skins files (*.css)"));
    else*/
        path = QFileDialog::getSaveFileName(this, tr("Save a file"), QString(QCoreApplication::applicationDirPath() + "/skins/default"), tr("%1 skin files (*.nsf);;All files (*)").arg(QCoreApplication::applicationName()));

    if (!path.isEmpty())
    {
        QFile file(path);

        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream flux(&file);
            flux << textEdit->toPlainText();
            file.close();

            isSaved = true;
            /*actSave->setEnabled(false);*/

            listView->setRootIndex(model->index(QCoreApplication::applicationDirPath()+QDir::separator()+"skins"));
        }
    }
}

void SkinManager::preview()
{
    QString style = textEdit->toPlainText();
    style.replace("@color",settings->value("Interface/Color").toString());
    style.replace("@bgImgUrl",settings->value("Interface/BgImgUrl").toString());
    style.replace("@iconSize",settings->value("Interface/ToolBar/IconSize").toString());
    style.replace("@skinsPath",QCoreApplication::applicationDirPath()+"/skins");

    qApp->setStyleSheet(style);
}

void SkinManager::showMenu()
{
    menuEdit->popup(QCursor::pos());
}

void SkinManager::showFind()
{

}

void SkinManager::editFile()
{
    QItemSelectionModel *selection = listView->selectionModel();
    QModelIndex indexSelectedElement = selection->currentIndex();
    QVariant selectedElement = model->data(indexSelectedElement, Qt::DisplayRole);
    QString path = QCoreApplication::applicationDirPath() + "/skins/" + selectedElement.toString();

    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString text = file.readAll();

        file.close();

        textEdit->clear();
        textEdit->setPlainText(text);

        isSaved = true;
        //urlSavedFile = path+QDir::separator()+file.fileName();

        text.replace("@color",settings->value("Interface/Color").toString());
        text.replace("@bgImgUrl",settings->value("Interface/BgImgUrl").toString());
        text.replace("@iconSize",settings->value("Interface/ToolBar/IconSize").toString());
        text.replace("@skinsPath",QCoreApplication::applicationDirPath()+"/skins");
        qApp->setStyleSheet(text);
    }
    else
        QMessageBox::warning(this, tr("Error"), tr("Unable to open file !"));
}

bool SkinManager::canClose()
{
    if(!isSaved)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Unsaved file - Skin manager"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("<h3>Skin manager</h3>Do you want to save your changes ?"));
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
        msgBox.setButtonText(QMessageBox::No,tr("No"));
        msgBox.setButtonText(QMessageBox::Cancel,tr("Cancel"));
        int value = msgBox.exec();

        if(value == QMessageBox::Yes)
        {
            saveFile();
            return true;
        }
        else if(value == QMessageBox::No)
            return true;
        else
            return false;
    }
    else
        return true;
}

void SkinManager::updateColor()
{
    //qDebug()<<"syntaxcolor";
    isSaved = false;
}
