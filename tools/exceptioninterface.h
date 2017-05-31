#ifndef EXCEPTIONINTERFACE_H
#define EXCEPTIONINTERFACE_H

#include "config/config.h"

class ExceptionInterface
{
public:
    ExceptionInterface();
    virtual QString getMessage();
    virtual LogLevel getSeverity();
};

#endif // EXCEPTIONINTERFACE_H
