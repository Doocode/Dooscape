#ifndef HISTORYPANEL_H
#define HISTORYPANEL_H

#include "uix.h"

class HistoryPanel : public Panel
{
    Q_OBJECT
    public:
        explicit HistoryPanel(QWidget *parent = 0);

        QVBoxLayout *layItems;

    signals:

    public slots:
        void loadSettings();

    private slots:
        void search();
        void searchItem(QString text);

        void genActs();

    private:
        QSettings *settings;

        QAction *actSearch;
};

#endif // HISTORYPANEL_H
