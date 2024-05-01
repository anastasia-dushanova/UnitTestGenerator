#ifndef METHODINFO_H
#define METHODINFO_H

#include <QString>
#include <QStringList>
#include <chromosome.h>

class GeneralInfo
{

public:
    GeneralInfo();
    GeneralInfo(ClassInfo className, Method method);
    ~GeneralInfo() {}

    ClassInfo getClassInfo() { return classInfo; }
    void setClassInfo(ClassInfo className) { this->classInfo = className; }

    void setMethod(Method method) { this->method = method; }
    Method getMethod() { return method; }

    void setGlobalFields(QStringList globalFields) { this->globalFields = globalFields; }
    QStringList getGbobalFields() { return globalFields; }

    void setOtherClasses(QStringList otherClasses) { this->otherClasses = otherClasses; }
    QStringList getOtherClasses() { return otherClasses; }

private:
    ClassInfo classInfo;
    Method method;
    QStringList globalFields;
    QStringList otherClasses;

};

#endif // METHODINFO_H
