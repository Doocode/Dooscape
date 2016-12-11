#ifndef SKINMANAGER_H
#define SKINMANAGER_H

#include <QtWidgets>

class SkinManager : public QMainWindow
{
    Q_OBJECT
    public:
        explicit SkinManager(QWidget *parent = 0);
        ~SkinManager();
        bool isSaved;

    signals:
        void showBrowser();

    public slots:
        void start();
        void loadConfig(); // Pour charger le theme de l'application
        void loadSettings(); // Pour cette fenêtre uniquement
        bool canClose();

    private slots:
        void genUI();
        void genActs();
        void genMenu();
        void genToolBars();
        void genConnexion();

        void openFile();
        void saveFile();
        void preview();
        void editFile();
        void updateColor();
        void showMenu();

        void showFind();

    private:
        QPlainTextEdit *textEdit;
        QString iconColor;
        QSettings *settings;
        QToolBar *mainToolBar;
        QVBoxLayout *centralLayout;

        QListView *listView;
        QDirModel *model;

        QPushButton *btnMenu;
        QPushButton *btnEdit;
        QMenu *mainMenu;
        QMenu *menuEdit;

        QAction *actBack;
       // QAction *actNewFile;
        QAction *actOpenFile;
        QAction *actSaveFile;
        QAction *actPreview;
        QAction *actPreviewBack;

        QAction *actUndo;
        QAction *actRedo;
        QAction *actCut;
        QAction *actCopy;
        QAction *actPaste;
        QAction *actSelectAll;

        QAction *actFind;
};

#endif // SKINMANAGER_H
