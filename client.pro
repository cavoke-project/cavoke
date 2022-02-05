QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quick

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cavokeclientcontroller.cpp \
    cavokeclientmodel.cpp \
    cavokeclientview.cpp \
    cavokeqmlgamemodel.cpp \
    tictactoelogic.cpp \ # FIXME: prototype only
    main.cpp

HEADERS += \
    cavokeclientcontroller.h \
    cavokeclientmodel.h \
    cavokeclientview.h \
    cavokeqmlgamemodel.h \
    tictactoelogic.h # FIXME: prototype only

FORMS += \
    cavokeclientview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
