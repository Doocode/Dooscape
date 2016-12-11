#ifndef ADDRESSBAR_H
#define ADDRESSBAR_H

#include <QtWidgets>

class AddressBar : public QProgressBar
{
    Q_OBJECT
    public:
        explicit AddressBar(QWidget *parent = 0);
        ~AddressBar();

        QLabel *lblProgress;
        QPushButton *btnIcon;
        QToolButton *lineEdit;

    signals:
        void showCompleter();

    public slots:
        void setProgression(int value);

    private:
};

#endif // ADDRESSBAR_H
