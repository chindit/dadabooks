#ifndef LOGGER_H
#define LOGGER_H

#include <QStandardPaths>

#include "tools/settings.h"

class Logger : public QMessageLogger
{
public:
    Logger(Settings settings);

private:
    QString loggerFilePath;
    Settings *settings;
};

#endif // LOGGER_H
