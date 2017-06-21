#include "networkmanager.h"

/**
 * Constructor
 * @brief NetworkManager::NetworkManager
 * @param logger
 * @param parent
 */
NetworkManager::NetworkManager(Logger *logger, QObject *parent) : QObject(parent)
{
    this->logger = logger;
    this->manager = new QNetworkAccessManager();
    this->isLoaded = false;
    this->error = QString();
}

/**
 * Destructor
 * @brief NetworkManager::~NetworkManager
 */
NetworkManager::~NetworkManager()
{
    delete this->manager;
    if (this->isLoaded) {
        delete this->request;
        delete this->reply;
    }
}

/**
 * /!\/!\/!\ WARNING /!\/!\/!\
 * This method is time-consuming as it's blocking main thread!
 * /!\/!\/!\ WARNING /!\/!\/!\
 *
 * @brief NetworkManager::downloadFile
 * @param url
 * @return
 */
QByteArray NetworkManager::downloadFile(QString url)
{
    if (!this->isURLValid(url)) {
        return QByteArray();
    }

    QNetworkAccessManager networkManager;
    QNetworkRequest request(QUrl(url, QUrl::StrictMode));
    QNetworkReply *reponse = networkManager.get(request);
    QEventLoop eventLoop;
    connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    if (reponse->error() == QNetworkReply::NoError) {
        return reponse->readAll();
    } else {
        QMap<QString, QString> context;
        context.insert("file", url);
        context.error("error", reponse->errorString());
        this->logger->warning("Download failure", context);
    }
    return QByteArray();
}

/**
 * Connectors for the class
 * @brief NetworkManager::setConnectors
 */
void NetworkManager::setConnectors()
{
    connect(this->manager, SIGNAL(finished()), this, SLOT(handleResponseStatus()));
}

/**
 * Start downloading a file
 * @brief NetworkManager::download
 * @param url
 */
void NetworkManager::download(QString url)
{
    if (!this->isURLValid(url)) {
        emit(this->downloadFailed());
    }
    if (this->isLoaded) {
        // Cut previous download and start new one
        emit(this->downloadAborted());
        delete this->manager;
        this->manager = new QNetworkAccessManager();
    }
    this->request = new QNetworkRequest(QUrl(url, QUrl::StrictMode));
    this->reply = new QNetworkReply(); // For safety
    this->isLoaded = true;
    this->reply = this->manager->get(this->request);
}

/**
 * Relay download signal to end user and check reply status
 * @brief NetworkManager::relayFinish
 */
void NetworkManager::handleResponseStatus()
{
    if (!this->isLoaded) {
        this->logger->alert("Critical failure!  Finished download was not started");
        emit(this->downloadAborted());
        return;
    }

    if (this->reply->error() != QNetworkReply::NoError) {
        this->error = this->reply->errorString();
        this->logger->addContext("error", this->error);
        this->logger->addContext("file", this->request->url().toString());
        this->logger->warning("Unable to download file");
        emit(this->downloadFailed());
    } else {
        emit(this->downloadFinished());
    }
}

/**
 * Return content of the downloaded file
 * @brief NetworkManager::getFile
 * @return
 */
QByteArray NetworkManager::getFile()
{
    if (!this->isLoaded) {
        return QByteArray();
    }
    return this->reply->readAll();
}

/**
 * Return last error message
 * @brief NetworkManager::getError
 * @return
 */
QString NetworkManager::getError()
{
    return this->error;
}

/**
 * Check if URL is valid with strict requirements
 * @brief NetworkManager::isURLValid
 * @param url
 * @return
 */
bool NetworkManager::isURLValid(QString url)
{
    QUrl urlObject = QUrl(url, QUrl::StrictMode);
    return urlObject.isValid();
}
