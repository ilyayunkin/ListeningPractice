#include "WordsStorage.h"
#include <QDebug>

#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <algorithm>
#include <assert.h>

WordsStorage::WordsStorage(QObject *parent) :
    QObject(parent)
{
    if(!loadWordsFromFile())
    {
        requestWordsFromTheInternet(QUrl("https://www.ef.com/wwen/english-resources/english-vocabulary/top-3000-words"));
    }
}

WordsStorage::~WordsStorage()
{

}

void WordsStorage::fileDownloaded(QNetworkReply *r)
{
    qDebug() << __PRETTY_FUNCTION__;
    m_DownloadedData = r->readAll();
    //emit a signal
    r->deleteLater();

    QDateTime begin = QDateTime::currentDateTime();
    {
        QFile f(xmlFileName);
        f.open(QIODevice::WriteOnly);
        if(f.isOpen())
        {
            QTextStream s(&f);
            s << m_DownloadedData;
        }
    }
    QDateTime t1 = QDateTime::currentDateTime();

    {
        constexpr char divBegin[] = "<div class=\"field-item even\"";
        constexpr char divEnd[] = "</div>";
        constexpr char pBegin[] = "<p>";
        constexpr char pEnd[] = "</p>";
        constexpr char br[] = "<br />";

        const int fieldBegin = m_DownloadedData.indexOf(divBegin);
        if(fieldBegin == -1)
            return;
        const int fieldEnd = m_DownloadedData.indexOf(divEnd, fieldBegin);
        if(fieldEnd == -1)
            return;

        constexpr int paragraphNumber = 2;
        int paragraphBegin = fieldBegin;
        for(int i = 0; i <paragraphNumber; ++i)
        {
            paragraphBegin = m_DownloadedData.indexOf(pBegin, paragraphBegin + 1);
            if(paragraphBegin == -1)
                return;
        }

        const int paragraphEnd = m_DownloadedData.indexOf(pEnd, paragraphBegin);
        QString paragraph = m_DownloadedData.mid(paragraphBegin, paragraphEnd - paragraphBegin);
        paragraph.replace(br, "");
        paragraph.replace("\t", "");

        QStringList rowsList = paragraph.split("\n");
        for(const auto &row : rowsList)
        {
            if(!row.isEmpty() &&
                    !row.contains(' ') &&
                    std::all_of(row.begin(), row.end(), [](QChar c){return isalpha(c.toLatin1());}))
            {
                words+= row.toLower();
            }
        }
    }
    QDateTime t2 = QDateTime::currentDateTime();
    words.removeDuplicates();
    QDateTime t3 = QDateTime::currentDateTime();
    {
        QFile f(wordsFileName);
        f.open(QIODevice::WriteOnly);
        if(f.isOpen())
        {
            QTextStream s(&f);
            for(auto word : words)
            {
                s << word + "\r\n" ;
            }
        }
    }
    QDateTime t4 = QDateTime::currentDateTime();
    qDebug() << __PRETTY_FUNCTION__
             << "Timings: " << begin.secsTo(t1)
             << t1.secsTo(t2)
             << t2.secsTo(t3)
             << t3.secsTo(t4);
    emit downloaded();
}

QByteArray WordsStorage::downloadedData() const
{
    return m_DownloadedData;
}

bool WordsStorage::loadWordsFromFile()
{
    bool ok = false;

    QFile f(wordsFileName);
    f.open(QIODevice::ReadOnly);
    if(f.isOpen())
    {
        QTextStream s(&f);
        while(!s.atEnd())
        {
            QString line = s.readLine();
            if(!line.isEmpty())
                words.push_back(line);
        }
    }

    ok = !words.isEmpty();
    qDebug() << __PRETTY_FUNCTION__ << ok;
    return ok;
}

void WordsStorage::requestWordsFromTheInternet(QUrl imageUrl)
{
    QNetworkRequest request(imageUrl);
    m_WebCtrl.get(request);
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
            this, SLOT (fileDownloaded(QNetworkReply*)));
}

QString WordsStorage::getWord(long index) const
{
    assert(!empty());
    assert(index < size());
    return words.at(index);
}
