#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H
/// Source: https://wiki.qt.io/Download_Data_from_URL
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>

class WordsStorage : public QObject
{
    Q_OBJECT
    QStringList words;
    const QString xmlFileName = "w.html";
    const QString wordsFileName = "w.txt";
    bool loadWordsFromFile();
    void requestWordsFromTheInternet(QUrl imageUrl);
public:
    explicit WordsStorage(const QUrl imageUrl, QObject *parent = 0);
    virtual ~WordsStorage();
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

#endif // FILEDOWNLOADER_H
