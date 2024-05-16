#include "jsonparser.h"

JSONparser::JSONparser() : fileName(""){ }

JSONparser::JSONparser(const QString fileName) : fileName(fileName) {}

QList<GeneralInfo *> JSONparser::parseJSONPopulation()
{
    QList<GeneralInfo*> listInfo;
    QList<Method> listMethod;

    ClassInfo info;
    Method method;
    QFile file(QApplication::applicationDirPath() +"/"+fileName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "JSONparser::parseJSONPopulation:\tНе удается открыть файл "<<fileName;
        return QList<GeneralInfo*>();
    }

    QString val = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonArray mainArr = doc.array();

    foreach(const QJsonValue& arr, mainArr){
        if(arr.isObject()){

            QJsonObject json = arr.toObject();

            QJsonValue classInfo = json.value("class");

            if(classInfo.isObject()){

                QJsonObject obj = classInfo.toObject();

                QJsonArray fields = obj["fields"].toArray();

                QJsonObject m = obj["method"].toObject();
//                qDebug() << "methods.size = "<<methods.size();

                QJsonArray args = m["args"].toArray();
                int linesCount = m["linesCount"].toInt();
                QString name = m["name"].toString();
                bool retVal = m["retVal"].toBool();

                QStringList listArgs;
                for(auto ar : args)
                    listArgs.append(ar.toString());

                method.setName(name);
                method.setArgs(listArgs);
                method.setLinesCount(linesCount);
                method.setReturnValue(retVal);
//                listMethod.append(method);


                QString className = obj["name"].toString();

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

//            for(auto m : listMethod){
                GeneralInfo* generalInfo = new GeneralInfo();
                generalInfo->setMethod(method);
                generalInfo->setClassInfo(info);
                generalInfo->setGlobalFields(gl_fields);
                generalInfo->setOtherClasses(otherClasses);

                listInfo.append(generalInfo);
//            }
        }
        listMethod.clear();
    }

    return listInfo;
}

void JSONparser::parseJSONCluster()
{
    qDebug() << QApplication::applicationDirPath()+"/"+fileName;
    QFile file(QApplication::applicationDirPath()+"/"+fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "JSONparser::parseJSONCluster\tНе удается открыть файл "<<fileName;
        return;
    }
    QString val = file.readAll();
    file.close();
    QList<QJsonObject> listClass_Returning;
    QList<QJsonObject> listClass_NonReturning;
    QList<QJsonObject> listGlobal_Returning;
    QList<QJsonObject> listGlobal_NonReturning;
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonArray mainArr = doc.array();
    foreach(const QJsonValue& arr, mainArr){

        if(arr.isObject()){

            QJsonObject json = arr.toObject();
            QJsonObject recordObject;

            QJsonValue classInfo = json.value("class");
            QString className{""};
            QJsonObject classObject;
            QJsonArray methodsReturning;
            QJsonArray methodsNonReturning;
            if(classInfo.isObject()){

                QJsonObject obj = classInfo.toObject();

                QJsonArray fields = obj["fields"].toArray();
                classObject.insert("fields", fields);

                QJsonArray methods = obj["methods"].toArray();

                foreach(const QJsonValue& value, methods){

                    if(value.isObject()){

                        QJsonObject obj = value.toObject();
//                        QString name = obj["name"].toString();
//                        QJsonArray args = obj["args"].toArray();
//                        int linesCount = obj["linesCount"].toInt();
                        bool retVal = obj["retVal"].toBool();

                        if(retVal == true)
                            methodsReturning.push_back(obj);
                        else
                            methodsNonReturning.push_back(obj);
                    }
                }
                className = obj["name"].toString();
                classObject.insert("name", className);
            }
            QJsonArray array1 = json["global_fields"].toArray();
            QJsonArray array2 = json["other_classes"].toArray();
            for(int i{0}; i < methodsReturning.size(); ++i){
                classObject.insert("method", methodsReturning.at(i));
                recordObject.insert("class", classObject);
                recordObject.insert("global_fields", array1);
                recordObject.insert("other_classes", array2);
                if(!className.isEmpty())
                    listClass_Returning.append(recordObject);
                else
                    listGlobal_Returning.append(recordObject);
            }
            for(int i{0}; i < methodsNonReturning.size(); ++i){
                classObject.insert("method", methodsNonReturning.at(i));
                recordObject.insert("class", classObject);
                recordObject.insert("global_fields", array1);
                recordObject.insert("other_classes", array2);
                if(!className.isEmpty())
                    listClass_NonReturning.append(recordObject);
                else
                    listGlobal_NonReturning.append(recordObject);
            }
        }
    }

    QJsonArray arr1;
    for(auto r : listClass_Returning)
        arr1.push_back(r);

    QJsonArray arr2;
    for(auto r : listClass_NonReturning)
        arr2.push_back(r);

    QJsonArray arr3;
    for(auto r : listGlobal_Returning)
        arr3.push_back(r);

    QJsonArray arr4;
    for(auto r : listGlobal_NonReturning)
        arr4.push_back(r);

    /*
     * 1_text.json - listClass_Returning
     * 2_text.json - listClass_NonReturning
     * 3_text.json - listGlobal_Returning
     * 4_text.json - listGlobal_NonReturning
     */
    QFile file1(QApplication::applicationDirPath()+"/1_text.json");
    QFile file2(QApplication::applicationDirPath()+"/2_text.json");
    QFile file3(QApplication::applicationDirPath()+"/3_text.json");
    QFile file4(QApplication::applicationDirPath()+"/4_text.json");

    //очищаем все файлы
    file1.open(QIODevice::WriteOnly|QIODevice::Truncate);
    file1.close();

    file2.open(QIODevice::WriteOnly|QIODevice::Truncate);
    file2.close();

    file3.open(QIODevice::WriteOnly|QIODevice::Truncate);
    file3.close();

    file4.open(QIODevice::WriteOnly|QIODevice::Truncate);
    file4.close();

    if(arr1.size() != 0){
        file1.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream( &file1 );
        stream << "[\n";
        for(int i{0}; i < arr1.size(); ++i){
            QJsonDocument doc(arr1.at(i).toObject());
            QString jsonString = doc.toJson(QJsonDocument::Indented);
            stream << jsonString;
            if(i < arr1.size()-1)
                stream << ",\n";
        }
        stream << "\n]";
        file1.close();
    }

    if(arr2.size() != 0){
        file2.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream( &file2 );
        stream << "[\n";
        for(int i{0}; i < arr2.size(); ++i){
            QJsonDocument doc(arr2.at(i).toObject());
            QString jsonString = doc.toJson(QJsonDocument::Indented);
            stream << jsonString;
            if(i < arr2.size()-1)
                stream << ",\n";
        }
        stream << "\n]";
        file2.close();
    }

    if(arr3.size() != 0){
        file3.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream( &file3 );
        stream << "[\n";
        for(int i{0}; i < arr3.size(); ++i){
            QJsonDocument doc(arr3.at(i).toObject());
            QString jsonString = doc.toJson(QJsonDocument::Indented);
            stream << jsonString;
            if(i < arr3.size()-1)
                stream << ",\n";
        }
        stream << "\n]";
        file3.close();
    }

    if(arr4.size() != 0){
        file4.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream( &file4 );
        stream << "[\n";
        for(int i{0}; i < arr4.size(); ++i){
            QJsonDocument doc(arr4.at(i).toObject());
            QString jsonString = doc.toJson(QJsonDocument::Indented);
            stream << jsonString;
            if(i < arr4.size()-1)
                stream << ",\n";
        }
        stream << "\n]";
        file4.close();
    }

}
