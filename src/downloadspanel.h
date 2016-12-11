#ifndef DOWNLOADSPANEL_H
#define DOWNLOADSPANEL_H

#include <QtWebEngineWidgets>
#include "uix.h"

class DownloadItem : public QProgressBar
{
    Q_OBJECT
    public:
        explicit DownloadItem(QWidget *parent = 0);
        ~DownloadItem();

        QWebEngineDownloadItem *fileToDownload;

    signals:
        void itemRemoved();

    public slots:
        void setItem(QWebEngineDownloadItem *item);

    private slots:
        void genActs();

        void setProgression(qint64 received, qint64 total);
        void setFinished();
        void stateChanged();
        void openFile();
        void cancel();

    private:
        QSettings *settings;

        QAction *actCancel;
        QAction *actRemove;
        QAction *actOpenFile;
       // QAction *actRetry;

        QToolButton *btnIcon;
        QToolButton *btnTitle;
        QToolButton *btnDescription;

        QString fileOnPC;
};

class DownloadsPanel : public Panel
{
    Q_OBJECT
    public:
        explicit DownloadsPanel(QWidget *parent = 0);

    signals:
        void showDownloads();

    public slots:
        void downloadRequested(QWebEngineDownloadItem *item);
        void loadSettings();
        bool isDownloading();

    private slots:
        void search();
        void searchItem(QString text);

        void genActs();

        void update();

    private:
        QSettings *settings;
        QVBoxLayout *centralLayout;

        QAction *actSearch;
};

#endif // DOWNLOADSPANEL_H
