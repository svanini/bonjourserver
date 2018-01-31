#-------------------------------------------------
#
# Project created by QtCreator 2018-01-17T12:39:32
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bonjourserver
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES       = server.cpp \
                main.cpp \
                bonjourserviceregister.cpp

HEADERS       = server.h \
                bonjourserviceregister.h

!mac:x11:LIBS+=-ldns_sd

win32 {
    LIBS+=-L"C:/Program Files/Bonjour SDK/Lib/Win32" -ldnssd
    # Add your path to bonjour here.
    INCLUDEPATH += "C:/Program Files/Bonjour SDK/Include"
}

unix:LIBS+=/usr/lib/x86_64-linux-gnu/libdns_sd.so

HEADERS += dns_sd.h

# Add utility class for Diffie-Helmann
INCLUDEPATH += ../bonjourclient

HEADERS += utility.h

SOURCES += ../bonjourclient/utility.cpp
