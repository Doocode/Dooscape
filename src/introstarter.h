#ifndef INTROSTARTER_H
#define INTROSTARTER_H

#include <QtWidgets>
#include <QDesktopServices>
#include "uix.h"

class IntroScreen : public QFrame
{
    Q_OBJECT
    public:
        explicit IntroScreen(QString title = 0, QWidget *parent = 0);

    signals:
        void goBackRequest();
        void goForwardRequest();

    public slots:
        void setBoxLayout(QLayout *layout);

    private:
        QVBoxLayout *mainLayout;
        QLabel *lblTitle;
        QFrame *central;

};

class IntroStarter : public QFrame
{
    Q_OBJECT
    public:
        explicit IntroStarter(QWidget *parent = 0);
        ~IntroStarter();

    signals:
        void showBrowser();
        void showConfig();
        void showLoader();

    private slots:
        void genIntro();
        void genAccount();
        void genCustomize();
        void genFinish();

        void genToolbar();
        void setColor(QString color);
        void setValue(QString key, QVariant value);

        void chooseColor();
        void chooseBgImage();

        void execNavig();
        void saveSettings();

    private:
        QStackedWidget *stack;
        QToolBar *mainToolBar;
        QAction *actBack;
        QAction *actForward;

        IntroScreen *scrIntro;
        IntroScreen *scrAccount;
        IntroScreen *scrCustomize;
        IntroScreen *scrFinish;

        QLineEdit *editPseudo;
        QLineEdit *editPass1;
        QLineEdit *editPass2;
        QLineEdit *editIndice;

        PreviewBox *editColor;
        PreviewBox *editBgImg;
        PreviewBox *editProfile;
};

#endif // INTROSTARTER_H
