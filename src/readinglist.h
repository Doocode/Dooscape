#ifndef READINGLIST_H
#define READINGLIST_H

#include "uix.h"

class ReadingList : public Panel
{
    Q_OBJECT
    public:
        explicit ReadingList(QWidget *parent = 0);
        ~ReadingList();

    signals:
        void exec(QString url);
        void addToList();

    public slots:
        void updateItems();
        void removeItem(int id);
        void editItem(int id);

    private slots:
        void search();
        void searchItem(QString text);

    private:
        QVBoxLayout *layItems;
        QSettings *settings;
        QSignalMapper *signalExec;
        QSignalMapper *signalRemove;
        QSignalMapper *signalEdit;
        QToolButton *btnAddItem;
        QToolButton *btnSearch;
};

class Item : public QFrame
{
    Q_OBJECT
    public:
        explicit Item(QWidget *parent = 0);
        ~Item();

        QToolButton *btnIcon;
        QToolButton *btnTitle;
        QToolButton *btnUrl;

        QLineEdit *editTitle;
        QLineEdit *editUrl;

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

#endif // READINGLIST_H
