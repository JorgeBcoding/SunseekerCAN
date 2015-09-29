#-------------------------------------------------
#
# Project created by QtCreator 2013-02-12T12:55:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sunseeker_CAN_GUI
TEMPLATE = app


SOURCES += main.cpp\
    sunseekercan.cpp \
    CANusb/canusbwriter.cpp \
    CANusb/canusbreader.cpp \
    CANusb/canusb.cpp \
    DecodedCANmsg/decodedcanmsg.cpp \
    CANmsgDecode/canmsgdecode.cpp \
    CANtoFile/cantofile.cpp \
    addtabdialog.cpp

HEADERS  += sunseekercan.h \
    CANusb/WinTypes.h \
    CANusb/ftd2xx.h \
    CANusb/canusb.h \
    CANusb/canthread.h \
    DecodedCANmsg/decodedcanmsg.h \
    CANmsgDecode/canmsgdecode.h \
    CANtoFile/cantofile.h \
    CANusb/threadglobals.h \
    addtabdialog.h

FORMS += \
    sunseekercan.ui

OTHER_FILES += \
    sunseeker_msg_info.csv

unix:!macx:!symbian: LIBS += -L$$PWD/usr/local/lib/ -lftd2xx

INCLUDEPATH += $$PWD/../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../usr/local/include

#unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../../../../usr/local/lib/libftd2xx.a

LIBS += -lpthread
#LIBS += -L$$quote($$PWD/libs) -lftd2xx
LIBS += -lftd2xx

RESOURCES +=


