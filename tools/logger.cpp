#include "logger.h"

/**
 * Constructor
 * @brief Logger::Logger
 * @param settings
 */
Logger::Logger(Settings* settings, QWidget *parent)
{
    this->loggerFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    this->settings = settings;
    this->parent = parent;
    QDir logDirectory = QDir(this->loggerFilePath);
    if (!logDirectory.exists() && !logDirectory.mkpath(this->loggerFilePath)) {
        QMessageBox::critical(this->parent, QObject::tr("Unable to write log"),
                              QObject::tr("Unable to create log file"));
        // Critical scenario.  Exit requested
        exit(EXIT_FAILURE);
    }
}

/**
 * Destructor
 */
Logger::~Logger()
{

}

/**
 * Main logging method
 * @brief Logger::log
 * @param level
 * @param message
 * @param context
 */
void Logger::log(LogLevel level, QString message, QMap<QString, QString> context)
{
    QDateTime now = QDateTime();
    QString output = "[";
    output += logLevelNames.find(level)->second;
    output += "] ";
    output += now.toString(Qt::ISODateWithMs);
    output +=" : ";
    output += message;

    // Adding existing context
    context = context.unite(this->context);
    this->context.clear();

    // Adding context
    if (context.size() > 0) {
        output += " [";
        QMap<QString, QString>::iterator i = context.begin();
        // Writing context elements
        while (i != context.end()) {
            output += "[";
            output += i.key();
            output += i.value();
            output += "],";
            ++i;
        }
        output.remove(output.length()-2, 1);
        output += "]";
    }

    this->write(output);

    // Show an alert message
    if (this->settings->getSetting(Verbosity) >= level) {
        if (level <= LogLevel::INFO) {
            QMessageBox::information(this->parent, QObject::tr("Information"), message);
        } else if (level <= LogLevel::WARNING) {
            QMessageBox::warning(this->parent, QObject::tr("Alerte"), message);
        } else {
            QMessageBox::critical(this->parent, QObject::tr("Erreur"), message);
        }
    }
}

/**
 * Write log to logfile
 * @brief Logger::write
 * @param log
 */
void Logger::write(QString log)
{
    QFile *logFile = new QFile(this->loggerFilePath+'/'+LogFilename);
    logFile->open(QIODevice::Append);
    QString test = logFile->errorString();
    logFile->write(log.toLocal8Bit());
    logFile->close();
    delete logFile;
}

/**
 * Add context for next log.
 * @brief Logger::addContext
 * @param key
 * @param value
 */
void Logger::addContext(QString key, QString value)
{
    this->context.insert(key, value);
}

/**
 * Special handling for exceptinos
 * @brief Logger::logException
 * @param exception
 */
void Logger::logException(ExceptionInterface* exception)
{
    return this->log(exception->getSeverity(), exception->getMessage(), QMap<QString, QString>());
}

/**
 * Debug log
 * @brief Logger::debug
 * @param message
 * @param context
 */
void Logger::debug(QString message, QMap<QString, QString> context = QMap<QString, QString>())
{
    return this->log(LogLevel::DEBUG, message, context);
}

/**
 * Info log
 * @brief Logger::info
 * @param message
 * @param context
 */
void Logger::info(QString message, QMap<QString, QString> context = QMap<QString, QString>())
{
    return this->log(LogLevel::INFO, message, context);
}

/**
 * Notice log
 * @brief Logger::notice
 * @param message
 * @param context
 */
void Logger::notice(QString message, QMap<QString, QString> context = QMap<QString, QString>())
{
    return this->log(LogLevel::NOTICE, message, context);
}

/**
 * Warning log
 * @brief Logger::warning
 * @param message
 * @param context
 */
void Logger::warning(QString message, QMap<QString, QString> context = QMap<QString, QString>())
{
    return this->log(LogLevel::WARNING, message, context);
}

/**
 * Error log
 * @brief Logger::error
 * @param message
 * @param context
 */
void Logger::error(QString message, QMap<QString, QString> context)
{
    return this->log(LogLevel::ERROR, message, context);
}

/**
 * Critical log
 * @brief Logger::critical
 * @param message
 * @param context
 */
void Logger::critical(QString message, QMap<QString, QString> context = QMap<QString, QString>())
{
    return this->log(LogLevel::CRITICAL, message, context);
}

/**
 * Alert log
 * @brief Logger::alert
 * @param message
 * @param context
 */
void Logger::alert(QString message, QMap<QString, QString> context)
{
    return this->log(LogLevel::ALERT, message, context);
}

/**
 * Emergency log
 * @brief Logger::emergency
 * @param message
 * @param context
 */
void Logger::emergency(QString message, QMap<QString, QString> context = QMap<QString, QString>())
{
    return this->log(LogLevel::EMERGENCY, message, context);
}
