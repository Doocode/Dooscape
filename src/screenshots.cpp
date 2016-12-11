#include "screenshots.h"

ScreenshotsViewer::ScreenshotsViewer(QWidget *parent) : QMainWindow(parent)
{
    QGraphicsScene *scene = new QGraphicsScene;

    item = new QGraphicsPixmapItem;
    item->setPixmap(QPixmap(":/img/background.png"));
    scene->addItem(item);

    view = new QGraphicsView;
    view->setScene(scene);
    view->scale(.75,.75);

    genActs();
    genMenu();
    genToolBar();

    centralLayout = new QVBoxLayout;
    centralLayout->setContentsMargins(0,0,0,0);
    centralLayout->setSpacing(0);
    centralLayout->addWidget(mainToolBar);
    centralLayout->addWidget(view);

    QWidget *center = new QWidget;
    center->setLayout(centralLayout);

    setCentralWidget(center);

    settings = new QSettings;
}

void ScreenshotsViewer::genActs()
{
    actBack = new QAction(tr("Back"),this);
    connect(actBack,SIGNAL(triggered()),this,SLOT(canClose()));

    actSave = new QAction(tr("Save screenshot"),this);
    actSave->setShortcut(QKeySequence::Save);
    connect(actSave,SIGNAL(triggered()),this,SLOT(saveScreen()));

    actZoomIn = new QAction(tr("Zoom +"),this);
    actZoomIn->setShortcut(QKeySequence("Ctrl++"));
    connect(actZoomIn,SIGNAL(triggered()),this,SLOT(editZoom()));

    actZoomOut = new QAction(tr("Zoom -"),this);
    actZoomOut->setShortcut(QKeySequence("Ctrl+-"));
    connect(actZoomOut,SIGNAL(triggered()),this,SLOT(editZoom()));
}

void ScreenshotsViewer::genMenu()
{
    mainMenu = new QMenu;
    mainMenu->addAction(actBack);
    mainMenu->addSeparator();
    mainMenu->addAction(actSave);
    mainMenu->addSeparator();
    mainMenu->addAction(actZoomIn);
    mainMenu->addAction(actZoomOut);
}

void ScreenshotsViewer::genToolBar()
{
    mainToolBar = new QToolBar;
    //mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    mainToolBar->addAction(actBack);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actSave);

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);
    mainToolBar->addWidget(spacer);

    sliderZoom = new QSlider(Qt::Horizontal);
    sliderZoom->setMaximumWidth(200);
    sliderZoom->setMinimum(0);
    sliderZoom->setMaximum(200);
    sliderZoom->setValue(75);
    sliderZoom->setTickInterval(50);
    sliderZoom->setPageStep(50);
    sliderZoom->setSingleStep(50);
    sliderZoom->setTickPosition(QSlider::TicksBothSides);
    connect(sliderZoom,SIGNAL(valueChanged(int)),this,SLOT(updateState()));

    lblZoom = new QLabel("75%");

    mainToolBar->addAction(actZoomOut);
    mainToolBar->addWidget(sliderZoom);
    mainToolBar->addAction(actZoomIn);
    mainToolBar->addWidget(lblZoom);
    mainToolBar->addSeparator();

    btnMenu = new QPushButton;
    btnMenu->setIconSize(QSize(24,24));
    btnMenu->setMenu(mainMenu);
    mainToolBar->addWidget(btnMenu);
}

void ScreenshotsViewer::loadSettings()
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

    // Icon Color
    QString iconColor = ":/img/"+settings->value("Interface/IconColor").toString()+"/";

    actBack->setIcon(QIcon(iconColor+"back.png"));
    actSave->setIcon(QIcon(iconColor+"save.png"));
    btnMenu->setIcon(QIcon(iconColor+"menu.png"));
    actZoomIn->setIcon(QIcon(iconColor+"zoom-in.png"));
    actZoomOut->setIcon(QIcon(iconColor+"zoom-out.png"));
}

void ScreenshotsViewer::updateState()
{
    double zoom(sliderZoom->value()*.01);
    lblZoom->setText(QString::number(sliderZoom->value())+"%");

    view->resetMatrix();
    view->scale(zoom,zoom);
}

void ScreenshotsViewer::editZoom()
{
    int range(25);

    if(sender()==actZoomIn)
        sliderZoom->setValue(sliderZoom->value()+range);
    else if(sender()==actZoomOut)
        sliderZoom->setValue(sliderZoom->value()-range);
}

void ScreenshotsViewer::setScreen(QPixmap screen, QString name)
{
    image = screen;
    fileName = name;
    isSaved = false;

    item->setPixmap(screen);
    view->setSceneRect(item->boundingRect());

    if(settings->value("Interface/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(sliderZoom,"value");
        animation->setDuration(1000);
        animation->setEasingCurve(QEasingCurve::OutElastic);
        animation->setStartValue(100);
        animation->setEndValue(75);
        animation->start();
    }
    else
        sliderZoom->setValue(75);
}

bool ScreenshotsViewer::canClose()
{
    if(!isSaved)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Unsaved file - Screenshot viewer"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("Do you want to save the screenshot ?"));
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
        msgBox.setButtonText(QMessageBox::No,tr("No"));
        msgBox.setButtonText(QMessageBox::Cancel,tr("Cancel"));
        int value = msgBox.exec();

        if(value == QMessageBox::Yes)
        {
            saveScreen();
            emit showBrowser();
            return true;
        }
        else if(value == QMessageBox::No)
        {
            emit showBrowser();
            return true;
        }
        else
            return false;
    }
    else
    {
        emit showBrowser();
        return true;
    }
}

void ScreenshotsViewer::saveScreen()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save Screenshot"), QString(QDir::homePath()+QDir::separator()+fileName), tr("PNG Files (*.png);;BMP Files (*.bmp);;GIF Files (*.gif);;All Files (*)"));
    if (!path.isEmpty())
    {
        QFile file(path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            image.save(path);
            isSaved = true;
        }
    }
}
