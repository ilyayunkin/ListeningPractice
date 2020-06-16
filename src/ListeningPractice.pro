QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets texttospeech

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Core/src/Controller.cpp \
    Settings/Settings.cpp \
    main.cpp \
    ListeningExcercises/WordsStorage.cpp \
    ListeningExcercises/PhrasesStorage.cpp \
    ListeningExcercises/ProviderFactory.cpp \
    Speech/Speaker.cpp \
    Gui/SayDialog.cpp \
    Gui/SpeechConfigDialog.cpp \
    Gui/mainwindow.cpp

HEADERS += \
    Core/Include/AbstractListeningFactory.h \
    Core/Include/AbstractSettings.h \
    Core/Include/AbstractSpeaker.h \
    Core/Include/AbstractView.h \
    Core/Include/ExceptionClasses.h \
    Core/Include/GameCounters.h \
    Core/Include/ProviderType.h \
    Core/Include/ProvidersFlags.h \
    Core/Include/AbstractControllerManipulator.h \
    Core/src/Controller.h \
    ListeningExcercises/Include/AbstractListeningProvider.h \
    ListeningExcercises/WordsStorage.h \
    ListeningExcercises/PhrasesStorage.h \
    ListeningExcercises/ProviderFactory.h \
    Settings/Settings.h \
    Speech/Speaker.h \
    Gui/SayDialog.h \
    Gui/SpeechConfigDialog.h \
    Gui/mainwindow.h

TRANSLATIONS += \
    NumbersListeningPractice_ru_RU.ts

INCLUDEPATH+= Core\Include\
INCLUDEPATH+= Gui

QMAKE_CXXFLAGS+= -Wall -Werror
DESTDIR= ../bin

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    Gui/SayDialog.ui \
    Gui/SpeechConfigDialog.ui
