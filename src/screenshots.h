#ifndef SCREENSHOTS_H
#define SCREENSHOTS_H

#include "uix.h"

class ScreenshotsViewer : public QMainWindow
{
    Q_OBJECT
    public:
        explicit ScreenshotsViewer(QWidget *parent = 0);

    signals:
        void showBrowser();

    public slots:
        void loadSettings();
        void setScreen(QPixmap screen,QString name);
        bool canClose();

    private slots:
        void genActs();
        void genMenu();
        void genToolBar();

        void updateState();
        void editZoom();
        void saveScreen();

    private:
        QSettings *settings;
        QVBoxLayout *centralLayout;
        QToolBar *mainToolBar;

        QPushButton *btnMenu;
        QMenu *mainMenu;
        QSlider *sliderZoom;
        QLabel *lblZoom;
        QGraphicsView *view;
        QPixmap image;
        QString fileName;
        bool isSaved;

        QAction *actBack;
        QAction *actSave;
        QAction *actZoomIn;
        QAction *actZoomOut;

        QGraphicsPixmapItem *item;
};

#endif // SCREENSHOTS_H
