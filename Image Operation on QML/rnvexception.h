#ifndef RNVEXCEPTION_H
#define RNVEXCEPTION_H
#include<stdexcept>
#include<QString>

class RnVException:std::exception
{
public:
    RnVException(const char *message);
    RnVException(const QString s);
    const char* what();
private:
    const char *msg;

};

#endif // RNVEXCEPTION_H
