#ifndef FIELDEDITOR_H
#define FIELDEDITOR_H

#include <QtWidgets>
#include <QtWebEngineWidgets>
#include "uix.h"

class FieldEditor : public QFrame
{
    Q_OBJECT
    public:
        explicit FieldEditor(QWidget *parent = 0);
        ~FieldEditor();

        QPushButton *btnIcon;
        QLineEdit *titleBar;
        QPushButton *btnMenu;
    signals:
        void execute();
        void showBrowser();

        void goToUrl(QString url);

    public slots:
        void loadSettings();
        void updateQuery();
        void setHistoryItems(QList<QWebEngineHistoryItem> list);

    private slots:
        void itemClicked();
        void searchQuery();

        void genActions();
        void genEditBar();
        void genMenu();

        void showMenu();

        QFrame *genReadingList();
        QFrame *genBookmarks();
        QFrame *genHistory();

    private:
        QScrollArea *scrollArea;

        QToolBar *mainToolBar;
        QToolBar *editBar;

        QToolButton *btnGoTo;
        QToolButton *btnSearch;

        QSettings *settings;

        QMenu *mainMenu;

        QVBoxLayout *mainLayout;
        QVBoxLayout *layItems;

        QAction *actBack;
        QAction *actPasteGo;
        QAction *actPasteSearch;
        QAction *actUndo;
        QAction *actRedo;
        QAction *actCut;
        QAction *actCopy;
        QAction *actPaste;
        QAction *actSelectAll;
        QAction *actClear;

        QList<QWebEngineHistoryItem> historyItems;
};

#endif // FIELDEDITOR_H
