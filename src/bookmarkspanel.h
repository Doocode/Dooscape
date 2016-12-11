#ifndef BOOKMARKSPANEL_H
#define BOOKMARKSPANEL_H

#include "uix.h"

class BookmarksItem : public QFrame
{
    Q_OBJECT
    public:
        explicit BookmarksItem(QWidget *parent = 0);
        ~BookmarksItem();

        QToolButton *btnIcon;
        QToolButton *btnTitle;

    signals:
        void exec();
        void remove();
        void edited();

    private slots:
        void genActs();

        void showEditor();
        void chooseIcon();
        void resetIcon();

    private:
        QSettings *settings;

        QAction *actGoTo;
        QAction *actRemove;
        QAction *actStartEdit;
        QAction *actFinishEdit;
        QAction *actChooseIcon;
        QAction *actResetIcon;
};

class BookmarksPanel : public Panel
{
    Q_OBJECT
    public:
        explicit BookmarksPanel(QWidget *parent = 0);

    signals:
        void addToBookmarks();
        void exec(QString url);

    public slots:
        void updateItems();

    private slots:
        void search();
        void searchItem(QString text);
        void removeItem(int id);

        void genActs();

    private:
        QVBoxLayout *layItems;
        QSettings *settings;

        QSignalMapper *signalExec;
        QSignalMapper *signalRemove;
       // QSignalMapper *signalEdit;

        QAction *actAddItem;
        QAction *actSearch;
};

#endif // BOOKMARKSPANEL_H
