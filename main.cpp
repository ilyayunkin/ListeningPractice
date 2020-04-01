#include "mainwindow.h"

#include <QApplication>
#include <QSslSocket>
#include <QDebug>
#include <QSharedPointer>

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

    QUrl wordsUrl("https://www.ef.com/wwen/english-resources/english-vocabulary/top-3000-words");
    WordsStorage wStorage(wordsUrl);
    QUrl phrasesUrl("https://www.phrases.org.uk/meanings/phrases-and-sayings-list.html");
    PhrasesStorage pStorage(phrasesUrl);

    ProviderFactory factory(wStorage, pStorage);

    MainWindow w(factory);
    w.show();

    return a.exec();
}
