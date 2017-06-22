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
        void startBrowser();
        void startConfig();

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
        QStackedWidget *mStacker;
        QAction *mActBack;
        QAction *mActForward;

        QLineEdit *mEditPseudo;

        PreviewBox *mEditColor;
        PreviewBox *mEditBgImg;
        PreviewBox *mEditProfile;
};

#endif // INTRODUCER_H
