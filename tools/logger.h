#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <QDir>
#include <QException>
#include <QFile>
#include <QMap>
#include <QMessageBox>
#include <QStandardPaths>

#include "config/config.h"
#include "tools/exceptioninterface.h"
#include "tools/settings.h"

class Logger : public QMessageLogger
{
public:
    Logger(Settings *settings, QWidget *parent);
    ~Logger();
    void log(LogLevel level, QString message, QMap<QString, QString> context);
    void logException(ExceptionInterface *exception);
    // Following methods are just an alias of log()
    void debug(QString message, QMap<QString, QString> context);
    void info(QString message, QMap<QString, QString> context);
    void notice(QString message, QMap<QString, QString> context);
    void warning(QString message, QMap<QString, QString> context);
    void error(QString message, QMap<QString, QString> context = QMap<QString, QString>());
    void critical(QString message, QMap<QString, QString> context);
    void alert(QString message, QMap<QString, QString> context);
    void emergency(QString message, QMap<QString, QString> context);

private:
    void write(QString log);
    QString loggerFilePath;
    Settings *settings;
    QWidget *parent;
};

#endif // LOGGER_H
