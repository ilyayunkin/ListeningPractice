#include "mainwindow.h"

#include <QApplication>
#include <QSslSocket>
#include <QDebug>

#include "WordsStorage.h"
#include "PhrasesStorage.h"
#include "ProviderFactory.h"
#include "SpeechConfigDialog.h"
#include "SayDialog.h"
#include "Speaker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Ilya");
    a.setApplicationName("ListeningPractice");

    qDebug() << "C++ version: " << __cplusplus;
    qDebug() << "Qt version: " << QT_VERSION_STR;

#if __GNUC__
    qDebug() << "GCC version: " << __MINGW_GCC_VERSION;
#endif

    bool ssl = QSslSocket::supportsSsl();
    qDebug() << "Supports SSL: " << ssl
             << QSslSocket::sslLibraryBuildVersionString()
             << QSslSocket::sslLibraryVersionString();

    WordsStorage wStorage;
    PhrasesStorage pStorage;

    ProviderFactory factory(wStorage, pStorage);

    Speaker speaker;

    MainWindow w(factory);
    SayDialog sayDialog;
    SpeechConfigDialog speechConfigDialog(speaker);

    QObject::connect(&sayDialog, &SayDialog::say, &speaker, &Speaker::say);
    QObject::connect(&w, &MainWindow::say, &speaker, &Speaker::say);

    QObject::connect(&w, &MainWindow::showSayDialog, &sayDialog, &QWidget::show);
    QObject::connect(&w, &MainWindow::showSpeechConfigDialog, &speechConfigDialog, &QWidget::show);
    w.show();

    return a.exec();
}
