#ifndef TABBAR_H
#define TABBAR_H

#include "qprogressindicator.h"
#include "webview.h"
#include "uix.h"

class TabButton : public QLabel
{
    Q_OBJECT
    public:
        explicit TabButton(QString objectName, QWidget *parent = 0);

    signals:
        void clicked();
        void closeRequested();

    public slots:
        void setTitle(QString title);
        void setIcon(QIcon icon);
        void setLoading(bool loading);
        void setClosable(bool closable);
        void setSpacerVisible(bool visible);

        QString title();

    private:
        QToolButton *btnIcon;
        QToolButton *btnTitle;
        QToolButton *btnClose;
        QString iconColor;
        QProgressIndicator *loadLogo;
        QWidget *progressWidget;
        QToolButton *spacer;
};

class TabBar : public Panel
{
    Q_OBJECT
    public:
        explicit TabBar(QWidget *parent = 0);
        ~TabBar();

    signals:
        void addTab();
        void duplicateTab();
        void openTabs(int index);
        void closeTabs(int index);
        void restoreTab(int index);

    public slots:
        void setTabWidget(QStackedWidget *tabs);
        void loadSettings();

    private slots:
        void search();
        void searchTab(QString text);
        void updateClosedTabs();
        void showClosedTabs();

        void genActs();
        QFrame *genTabsOpenWidget();
        QFrame *genTabsClosedWidget();

    private:
        QVBoxLayout *tabsOpenLayout;
        QVBoxLayout *tabsClosedLayout;
        QSignalMapper *openTabsMaps;
        QSignalMapper *closeTabsMaps;
        QSignalMapper *restoreTabsMaps;
        QStackedWidget *centralWidget;
        QString iconColor;
        QSettings *settings;

        QAction *actAddTab;
        QAction *actDuplicateTab;
        QAction *actSearch;
        QAction *actClosedTabs;
};

#endif // TABBAR_H
