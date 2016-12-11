#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#include <QtWidgets>
#include "uix.h"

class LockScreen : public QLabel
{
    Q_OBJECT
    public:
        explicit LockScreen(QWidget *parent = 0);
        ~LockScreen();
        virtual int heightForWidth(int width) const;
        virtual QSize sizeHint() const;

    signals:
        void showBrowser();

    protected:

    public slots:
        void loadSettings();
        void setPixmap(const QPixmap &);
        void resizeEvent(QResizeEvent *e);
        void setScreenBrowser(QPixmap pixmap); // For animation

    private slots:
        void genActs();
        void genCentral();
        void genForm();

        void unlockWindow();

        void showForm();
        void showIndication();

        void updateTime();

    private:
        QSettings *settings;
        QPixmap bgImage;
        QPixmap pix;
        QVBoxLayout *mainLayout;

        QToolBar *toolBar;

        QFrame *central;
        QFrame *form;

        QLabel *lblTime;
        QLabel *lblDate;

        RoundedLabel *iconPhoto;
        QLabel *lblUserName;
        QLineEdit *editPass;

        QAction *actUnlock;
        QAction *actIndication;
        QAction *actLeaveMsg;

        QPushButton *btnLock;
        QLabel *lblScreenBrowser;

        QToolButton *btnUnlock;
        QToolButton *btnIndication;
        QToolButton *btnLeaveMsg;
};

#endif // LOCKSCREEN_H
