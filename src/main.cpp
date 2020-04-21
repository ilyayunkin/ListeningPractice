#include "mainwindow.h"

#include <QApplication>
#include <QSslSocket>
#include <QDebug>

#include "WordsStorage.h"
#include "PhrasesStorage.h"
#include "ProviderFactory.h"

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

    MainWindow w(factory);
    w.show();

    return a.exec();
}
