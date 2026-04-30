#include "rnvexception.h"

RnVException::RnVException(const char *message):msg(message){}

RnVException::RnVException(const QString s)
{
    msg=s.toStdString().c_str();
}

const char *RnVException::what()
{
    return msg;
}

