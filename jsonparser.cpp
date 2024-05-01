#include "jsonparser.h"

JSONparser::JSONparser() : fileName(""){ }

JSONparser::JSONparser(const QString fileName) : fileName(fileName) {}

QList<GeneralInfo *> JSONparser::parseJSON()
{
    QList<GeneralInfo*> listInfo;
    QList<Method> listMethod;

    ClassInfo info;
    Method method;
    QFile file(QApplication::applicationDirPath() +"/"+fileName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Не удается открыть файл";

    }

    QString val = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonArray mainArr = doc.array();
    foreach(const QJsonValue& arr, mainArr){
        if(arr.isObject()){

            QJsonObject json = arr.toObject();
            int id = json["id"].toInt();

            QJsonValue classInfo = json.value("class");

            if(classInfo.isObject()){

                QJsonObject obj = classInfo.toObject();
                QString className = obj["name"].toString();

                QJsonArray methods = obj["methods"].toArray();
    //            qDebug() << "methods.size = "<<methods.size();
                foreach(const QJsonValue& value, methods){

                    if(value.isObject()){

                        QJsonObject obj = value.toObject();
                        QString name = obj["name"].toString();
                        QJsonArray args = obj["args"].toArray();
                        int linesCount = obj["linesCount"].toInt();
                        bool retVal = obj["retVal"].toBool();

                        QStringList listArgs;
                        for(auto ar : args)
                            listArgs.append(ar.toString());

                        method.setName(name);
                        method.setArgs(listArgs);
                        method.setLinesCount(linesCount);
                        method.setReturnValue(retVal);
                        listMethod.append(method);
                    }
                }

                QJsonArray fields = obj["fields"].toArray();

                QStringList listFields;
                for(auto f : fields)
                    listFields.append(f.toString());

                info.setName(className);
                info.setFields(listFields);

            }

            QJsonArray array1 = json["global_fields"].toArray();
            QStringList gl_fields;
            for(auto f : array1)
                gl_fields.append(f.toString());

            QJsonArray array2 = json["other_classes"].toArray();
            QStringList otherClasses;
            for(auto c : array2)
                otherClasses.append(c.toString());

            for(auto m : listMethod){
                GeneralInfo* generalInfo = new GeneralInfo();
                generalInfo->setMethod(m);
                generalInfo->setClassInfo(info);
                generalInfo->setGlobalFields(gl_fields);
                generalInfo->setOtherClasses(otherClasses);

                listInfo.append(generalInfo);
            }
        }
        listMethod.clear();
    }

    return listInfo;
}
