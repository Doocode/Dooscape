#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QtWidgets>
#include <QtWebEngineWidgets>

#include "uix.h"

class TabList : public QFrame
{
    Q_OBJECT
    public:
        explicit TabList(QWidget *parent = 0);

    signals:
        void showTab(int index);

    public slots:
        void addTab(QString title,int index);
        void setCurrent(int index);
        QString nameAt(int index);

    private slots:
        void setCurrent();

    private:
        QVBoxLayout *layout;
        QSignalMapper *map;
};

class SettingsPanel : public QFrame
{
    Q_OBJECT
    public:
        explicit SettingsPanel(QWidget *parent = 0);

    signals:

    public slots:
        QString title();
        void setTitle(QString title);
        void setMainLayout(QLayout *layout);

    private slots:

    private:
        QGridLayout *mainLayout;
        QLabel *lblTitle;
        QScrollArea *scrollWidget;

};

class ConfigWindow : public QMainWindow
{
    Q_OBJECT
    public:
        explicit ConfigWindow(QWidget *parent = 0);
        ~ConfigWindow();

    public slots:
        void loadSettings();
        void goBack();
        static QString hashPassword(QString password);

    signals:
        void showBrowser();

    private slots:
        void genUI();
        void genActs();
        void genMenu();
        void genToolBar();

        void genCustomize();
        void genNavigation();
        void genInterface();
        void genSkins();
        void genDownloads();
        void genSecurity();
        void genOptions();
        void genReset();

        void editing();
        void chooseColor();
        void chooseBgImage();
        void chooseDownloadPath();
        void choosePassword();
        void reset();
        void iconSizeEditing();

        void resizeSplitter();
        void saveSettings();

    private:
        bool isEdited;
        QMenu *mainMenu;
        QString iconColor;
        QSettings *settings;
        QAction *actBack;
        QAction *actRefresh;
        QStackedWidget *central;
        QToolBar *mainToolBar;
        QPushButton *btnMenu;
        QVBoxLayout *centralLayout;
        QHBoxLayout *layToolBar;
        QSplitter *center;
        QSize iconSize;

        TabList *listTab;
        SettingsPanel *tabCustomize;
        SettingsPanel *tabNavigation;
        SettingsPanel *tabInterface;
        SettingsPanel *tabSkins;
        SettingsPanel *tabDownloads;
        SettingsPanel *tabSecurity;
        SettingsPanel *tabOptions;
        SettingsPanel *tabReset;

        // Customize
        PreviewBox *editColor;
        PreviewBox *editBgImg;
        PreviewBox *editProfile;

        // Navigation
        QRadioButton *radioEmptyStartup;
        QRadioButton *radioHomeStartup;
        QRadioButton *radioSpecificStartup;
        QRadioButton *radioEmptyNewTab;
        QRadioButton *radioHomeNewTab;
        QRadioButton *radioSpecificNewTab;
        QLineEdit *editSpecificStartup;
        QLineEdit *editSpecificNewTab;
        QLineEdit *editHomePage;
        QLineEdit *editPrefixSearchUrl;
        QLineEdit *editSuffixSearchUrl;
        QCheckBox *checkCustomUserAgent;
        QLineEdit *editUserAgent;

        // Interface
        QToolButton *checkToolBarAddTab;
        QToolButton *checkToolBarBack;
        QToolButton *checkToolBarForward;
        QToolButton *checkToolBarReload;
        QToolButton *checkToolBarHome;
        QToolButton *checkToolBarLock;
        QToolButton *checkToolBarOpenUrlInOtherBrowser;
        QToolButton *checkToolBarSkin;
        QToolButton *checkToolBarFull;
        QToolButton *checkToolBarConfig;
        QToolButton *checkToolBarReadList;
        QToolButton *checkToolBarBookmarks;
        QToolButton *checkToolBarHistory;
        QToolButton *checkToolBarDownloads;
        QToolButton *checkToolBarScreen;
        QRadioButton *radioTopToolBar;
        QRadioButton *radioBottomToolBar;
        QRadioButton *radioBlackIcons;
        QRadioButton *radioWhiteIcons;
        QRadioButton *radioRightCompleter;
        QRadioButton *radioLeftCompleter;
        QSlider *editToolBarIconSize;
        QLabel *lblIconSize;

        // Skins
        QCheckBox *checkAutoPreview;
        QListView *listView;
        QDirModel *model;

        // Downloads
        QRadioButton *radioDownloadAskSave;
        QRadioButton *radioDownloadSaveAs;
        QCheckBox *checkDownloadOpenFile;
        QCheckBox *checkDownloadNotification;
        QCheckBox *checkDownloadRemoveList;
        QLineEdit *editDownloadPath;

        // Security
        QDialog *dialogChoosePass;
        QLineEdit *editUserName;
        QCheckBox *checkLockAtStartup;
        QCheckBox *checkUnlockToClose;
        QCheckBox *checkLockConfig;
        QCheckBox *checkLockLocker;
        QToolButton *btnInfoPass;
        QString hashedPassword;
        QString passIndice;

        // Options
        QCheckBox *checkAnimations;
        QCheckBox *checkJavaScript;
        QCheckBox *checkJavascriptCanOpenWindow;
        QCheckBox *checkJavascriptCanAccessClipboard;
        QCheckBox *checkCurrentPageOnTitleBar;
        QCheckBox *checkTabBarEnabled;
        QCheckBox *checkTabBarVisibleWithLessTwoTabs;
        QCheckBox *checkTabBarClosable;
        QCheckBox *checkTabBarMovable;
};

#endif // CONFIGWINDOW_H
