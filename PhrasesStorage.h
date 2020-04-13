#ifndef PHRASESSTORAGE_H
#define PHRASESSTORAGE_H

/// Source: https://wiki.qt.io/Download_Data_from_URL
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>

class PhrasesStorage : public QObject
{
    Q_OBJECT
    QStringList words;
    const QString xmlFileName = "p.html";
    const QString phrasesFileName = "p.txt";
    bool loadWordsFromFile();
    void requestWordsFromTheInternet(QUrl imageUrl);
public:
    explicit PhrasesStorage(QObject *parent = 0);
    virtual ~PhrasesStorage();
    QByteArray downloadedData() const;
    bool empty() const { return words.isEmpty();}
    long size() const { return words.size();}
    QString getWord(long index) const;
signals:
    void downloaded();

private slots:
    void fileDownloaded(QNetworkReply*r);

private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;
};

#endif // PHRASESSTORAGE_H
