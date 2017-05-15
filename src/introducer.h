#ifndef INTRODUCER_H
#define INTRODUCER_H

#include <QtWidgets>
#include <QDesktopServices>
#include "uix.h"

class IntroducerFrame : public QFrame
{
    Q_OBJECT
    public:
        explicit IntroducerFrame(QString title = tr("Title"), QString text = tr("Text"), QWidget *parent = 0);

    signals:
        void goBackRequest();
        void goForwardRequest();

    public slots:
        void setBoxLayout(QLayout *layout);

    private:
        QFrame *mContainer;

};

class Introducer : public QFrame
{
    Q_OBJECT
    public:
        explicit Introducer(QWidget *parent = 0);

    signals:
        void startBrowsing();
        void showConfig();
        void showLoader();

    private slots:
        IntroducerFrame *genIntroducerFrame(QString title, QString text, QLayout *layout = 0);
        IntroducerFrame *genIntro();
        IntroducerFrame *genAccount();
        IntroducerFrame *genCustomize();
        IntroducerFrame *genFinish();
        QToolBar *genToolbar();

        void setColor(QString color);
        void setValue(QString key, QVariant value);

        void chooseColor();
        void chooseBgImage();

        void execNavig();
        void saveSettings();

    private:
        QStackedWidget *stack;
        QAction *actBack;
        QAction *actForward;

        QLineEdit *editPseudo;

        PreviewBox *editColor;
        PreviewBox *editBgImg;
        PreviewBox *editProfile;
};

#endif // INTRODUCER_H
