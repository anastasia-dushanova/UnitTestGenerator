#ifndef METHODINFO_H
#define METHODINFO_H

#include <QString>
#include <QStringList>
#include <chromosome.h>

/*!
 * \brief Класс, содержаций основную информацию. В дальнейшем она используется для генерации хромосом
 */
class GeneralInfo
{

public:
    /*!
     * \brief Конструкто класса
     */
    GeneralInfo();

    /*!
     * \brief Конструктор класса
     * \param className Информация о классе
     * \param method Информация о методе
     */
    GeneralInfo(ClassInfo className, Method method);

    /*!
     * \brief Деструктор класса
     */
    ~GeneralInfo() {}

    /*!
     * \brief Получить информацию о классе
     * \return Информация о классе
     */
    ClassInfo getClassInfo() { return classInfo; }

    /*!
     * \brief Установить информацию о классе
     * \param className Информация о классе
     */
    void setClassInfo(ClassInfo className) { this->classInfo = className; }

    /*!
     * \brief Установить информацию о методе
     * \param method Информация о методе
     */
    void setMethod(Method method) { this->method = method; }

    /*!
     * \brief Получить информацию о методе
     * \return Информация о методе
     */
    Method getMethod() { return method; }

    /*!
     * \brief Установить поля класса
     * \param globalFields Поля класса
     */
    void setGlobalFields(QStringList globalFields) { this->globalFields = globalFields; }

    /*!
     * \brief Получить поля класса
     * \return Поля класса
     */
    QStringList getGbobalFields() { return globalFields; }

    /*!
     * \brief Установить поля класса
     * \param otherClasses Поля класса
     */
    void setOtherClasses(QStringList otherClasses) { this->otherClasses = otherClasses; }

    /*!
     * \brief Получить поля класса
     * \return Поля класса
     */
    QStringList getOtherClasses() { return otherClasses; }

private:
    /*!
     * \brief Информация о класса
     */
    ClassInfo classInfo;

    /*!
     * \brief Информация о методе
     */
    Method method;

    /*!
     * \brief Список полей класса
     */
    QStringList globalFields;

    /*!
     * \brief Список других классов
     */
    QStringList otherClasses;

};

#endif // METHODINFO_H
