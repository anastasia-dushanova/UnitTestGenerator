QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chromosome.cpp \
    jsonparser.cpp \
    main.cpp \
    methodinfo.cpp \
    population.cpp \
    testcasedecor.cpp \
    unittestgenerator.cpp

HEADERS += \
    chromosome.h \
    jsonparser.h \
    methodinfo.h \
    population.h \
    testcasedecor.h \
    unittestgenerator.h

FORMS += \
    unittestgenerator.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../build-UnitTestGenerator-Desktop_Qt_5_12_12_MinGW_64_bit-Debug/debug/text.json \
    ../build-UnitTestGenerator-Desktop_Qt_5_12_12_MinGW_64_bit-Debug/text.json
