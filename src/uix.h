#ifndef UIX_H
#define UIX_H

#include <QtWidgets>

class Panel : public QFrame
{
    Q_OBJECT
    public:
        explicit Panel(QWidget *parent = 0);
        ~Panel();

        QAction *actBack;
        QToolBar *toolBar;
        QLabel *lblTitle;
        QLineEdit *searchBar;
        QLabel *lblCount;
        QScrollArea *scrollArea;
    signals:

    public slots:

    private slots:

    private:
};

class HideBlock : public QFrame
{
    Q_OBJECT
    public:
        explicit HideBlock(QString title = tr("Block"), QWidget *parent = 0);

    signals:

    public slots:
        void setBlockLayout(QLayout *layout);
        void showBlock();
        void showBlock(bool show);

        QLayout *blockLayout();

    private slots:

    private:
        QFrame *block;
        QToolButton *btnTitle;
        int blockHeight;

};

class RoundedLabel : public QLabel
{
    Q_OBJECT
    public:
        explicit RoundedLabel(QWidget *parent = 0);

    public slots:
        void setImage(QString imageUrl);
        void setRounded(bool rounded = true);

    private slots:
        void paintEvent(QPaintEvent *e);

    private:
        QString bgImage;
        bool isRounded;

};

class PreviewBox : public QLabel
{
    Q_OBJECT
    public:
        explicit PreviewBox(QWidget *parent = 0);
        QString color;
        QString bgImage;

    signals:
        void editRequested();

    public slots:
        void setColor(QString colorName);
        void setBgImage(QString bgImageUrl);

};

#endif // UIX_H
