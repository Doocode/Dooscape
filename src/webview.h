#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QtWidgets>
#include <QtNetwork>
#include <QtWebEngineWidgets>

class WebPage : public QWebEnginePage
{
    Q_OBJECT
    public:
        explicit WebPage(QWidget *parent = 0);
        ~WebPage();

    signals:

    protected:
        QWebEnginePage *createWindow(QWebEnginePage::WebWindowType type);

    public slots:

    private slots:

    private:
};

class WebView : public QWebEngineView
{
    Q_OBJECT
    public:
        explicit WebView(QWidget *parent = 0);
        ~WebView();
        QIcon icon() const;

    signals:
        void iconChanged();

    public slots:

    private slots:
        void onIconUrlChanged(const QUrl &url);
        void iconLoaded();

    private:
        QIcon m_icon;
        QNetworkReply *m_iconReply;
        QNetworkAccessManager *networkAccessManager;
};

class WebWidget : public QMainWindow
{
    Q_OBJECT
    public:
        explicit WebWidget(QWidget *parent = 0);
        ~WebWidget();

        int progression;
        bool loadIsOk;

    signals:

    public slots:
        WebView *view();
        QPixmap screenShot();
        bool isLoading();

    private slots:
        void loadStarted();
        void loadProgress(int value);
        void loadFinished(bool finished);
        void linkHovered(QString url);

    protected:
        void mouseMoveEvent(QMouseEvent *event);

    private:
        WebView *webViewWidget;
        bool loading;
};

#endif // WEBVIEW_H
