#include "logger.h"

/**
 * Constructor
 * @brief Logger::Logger
 * @param settings
 */
Logger::Logger(Settings settings)
{
    this->loggerFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    this->settings = settings;
}

/**
 * Destructor
 */
Logger::~Logger()
{
    delete settings;
}
