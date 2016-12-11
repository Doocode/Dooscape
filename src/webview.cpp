#include "webview.h"
#include "browserwindow.h"

WebWidget::WebWidget(QWidget *parent) : QMainWindow(parent)
{
    webViewWidget = new WebView;

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(webViewWidget);
    QFrame *frame = new QFrame;
    frame->setLayout(mainLayout);
    setCentralWidget(frame);

    setMouseTracking(true);

    loading = false;
    progression = 0;
    loadIsOk = true;

    connect(webViewWidget,SIGNAL(loadStarted()),this,SLOT(loadStarted()));
    connect(webViewWidget,SIGNAL(loadProgress(int)),this,SLOT(loadProgress(int)));
    connect(webViewWidget,SIGNAL(loadFinished(bool)),this,SLOT(loadFinished(bool)));
    connect(webViewWidget->page(),SIGNAL(linkHovered(QString)),this,SLOT(linkHovered(QString))); // Essayer de l'afficher dans un QLabel
}

WebWidget::~WebWidget()
{

}

WebView *WebWidget::view()
{
    return webViewWidget;
}

void WebWidget::loadStarted()
{
    loading = true;
    loadIsOk = false;
    progression = 0;
}

void WebWidget::loadProgress(int value)
{
    loading = true;
    loadIsOk = false;
    progression = value;
}

void WebWidget::loadFinished(bool finished)
{
    loading = false;
    loadIsOk = finished;
    progression = 0;
}

bool WebWidget::isLoading()
{
    return loading;
}

void WebWidget::linkHovered(QString url)
{
    QToolTip::showText(QCursor::pos(),url);
}

void WebWidget::mouseMoveEvent(QMouseEvent *event)
{
    setMouseTracking(true);
    qDebug()<<event->pos();
}

QPixmap WebWidget::screenShot()
{
    QPixmap pixmap(view()->width(), view()->height());
    QPainter painter(&pixmap); // msg dans debug ?
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    view()->render(&painter);
    return pixmap;
}

WebView::WebView(QWidget *parent) :
    QWebEngineView(parent)
  , m_iconReply(0)
  , networkAccessManager(0)
{
    QSettings *config = new QSettings;

    settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled,true);
    settings()->setAttribute(QWebEngineSettings::PluginsEnabled,true);
    settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,true);
    settings()->setAttribute(QWebEngineSettings::JavascriptEnabled,config->value("Navigation/Options/EnableJavaScript").toBool());
    settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows,config->value("Navigation/Options/EnableJavascriptCanOpenWindow").toBool());
    settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, config->value("Navigation/Options/EnableJavascriptCanAccessClipboard").toBool());

    networkAccessManager = new QNetworkAccessManager(this);

    setPage(new WebPage(this));

    connect(page(),SIGNAL(iconUrlChanged(QUrl)),this,SLOT(onIconUrlChanged(QUrl)));
}

WebView::~WebView()
{

}

QIcon WebView::icon() const
{
    return m_icon;
}

void WebView::onIconUrlChanged(const QUrl &url)
{
    QNetworkRequest iconRequest(url);
    m_iconReply = networkAccessManager->get(iconRequest);
    m_iconReply->setParent(this);
    connect(m_iconReply, SIGNAL(finished()), this, SLOT(iconLoaded()));
}

void WebView::iconLoaded()
{
    m_icon = QIcon();
    if (m_iconReply)
    {
        QByteArray data = m_iconReply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(data);
        m_icon.addPixmap(pixmap);
        m_iconReply->deleteLater();
        m_iconReply = 0;
    }
    emit iconChanged();
}

// WebPage

WebPage::WebPage(QWidget *parent) :
    QWebEnginePage(parent)
{
}

WebPage::~WebPage()
{
}

QWebEnginePage *WebPage::createWindow(QWebEnginePage::WebWindowType type)
{
    if (type == QWebEnginePage::WebBrowserTab)
    {
        QObject *w = this->parent()->parent()->parent();
        BrowserWindow *mw = qobject_cast<BrowserWindow*>(w->parent()->parent()->parent()->parent());
        return mw->openInNewTab()->page();
    }
    else if(type == QWebEnginePage::WebBrowserWindow)
    {
        QWebEngineView *v = new QWebEngineView;
        v->show();
        return v->page();
    }
    else
    {
        QWebEngineView *v = new QWebEngineView;
        v->show();
        return v->page();
    }
}
