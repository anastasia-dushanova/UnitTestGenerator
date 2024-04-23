#ifndef METHODINFO_H
#define METHODINFO_H

#include <QString>

class MethodInfo
{

public:
    MethodInfo();
    MethodInfo(QString method, size_t args, int type, bool returnValue);
    ~MethodInfo() {}

    void setMethodInfo(QString method, size_t args, int type, bool returnValue);

    void setMethod(QString method) { this->method = method; }
    QString getMethod() { return method; }

    void setArgs(size_t args) { this->args = args; }
    size_t getArgs() { return args; }

    void setType(int type) { this->type = type; }
    int getType() { return type; }

    void setReturnValue(bool returnValue) { this->returnValue = returnValue; }
    bool getReturnValue() { return returnValue; }

private:
    QString method;
    size_t args;
    int type;
    bool returnValue;
};

#endif // METHODINFO_H
