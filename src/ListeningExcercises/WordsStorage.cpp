#include "WordsStorage.h"
#include <QDebug>

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>

#include <algorithm>
#include <assert.h>

#include "ExceptionClasses.h"

WordsStorage::WordsStorage(QObject *parent) :
    QObject(parent)
{
    if(!loadWordsFromFile())
    {
        requestWordsFromTheInternet(QUrl("https://www.ef.com/wwen/english-resources/english-vocabulary/top-3000-words/"));
    }
}

WordsStorage::~WordsStorage()
{

}

void WordsStorage::fileDownloaded(QNetworkReply *r)
{
    qDebug() << __PRETTY_FUNCTION__;
    const int statusCode = r->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QByteArray reason = r->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
    const QUrl redirection = r->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    const QByteArray loadDetails = QByteArray("code: ") + QString::number(statusCode).toLatin1()
            + "\nReason: " + reason
            + "\nRedirect: " + redirection.toString().toLatin1();
    qDebug() << __PRETTY_FUNCTION__ << loadDetails;

    m_DownloadedData = r->readAll();
    //emit a signal
    r->deleteLater();

    try {
        if(r->error() != QNetworkReply::NoError)
        {
            throw HttpLoadException(r->errorString().toLatin1());
        }

        if(statusCode != 200)
        {
            throw ResourceUnavailibleException(loadDetails);
        }

        if(m_DownloadedData.isEmpty())
        {
            throw EmptyFileException();
        }

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

            const int fieldBegin = m_DownloadedData.indexOf(divBegin);
            if(fieldBegin == -1)
            {
                throw ParsingFailedException(divBegin);
            }

            const int fieldEnd = m_DownloadedData.indexOf(divEnd, fieldBegin);
            if(fieldEnd == -1)
            {
                throw ParsingFailedException(divEnd);
            }

            constexpr int paragraphNumber = 2;
            int paragraphBegin = fieldBegin;
            for(int i = 0; i <paragraphNumber; ++i)
            {
                paragraphBegin = m_DownloadedData.indexOf(pBegin, paragraphBegin + 1);
                if(paragraphBegin == -1)
                {
                    throw ParsingFailedException(pBegin);
                }
            }

            const int paragraphEnd = m_DownloadedData.indexOf(pEnd, paragraphBegin);
            QString paragraph = m_DownloadedData.mid(paragraphBegin, paragraphEnd - paragraphBegin);

            {
                constexpr char br1[] = "<br />";
                constexpr char br2[] = "<br/>";
                constexpr char br3[] = "<br>";
                paragraph.replace(br1, "");
                paragraph.replace(br2, "");
                paragraph.replace(br3, "");
                paragraph.replace("\t", "");
            }

            QStringList rowsList = paragraph.split("\n");
            for(const auto &row : rowsList)
            {
                qDebug() << __PRETTY_FUNCTION__ << row;
                if(!row.isEmpty() &&
                        !row.contains(' ') &&
                        std::all_of(row.begin(), row.end(), [](QChar c){return isalpha(c.toLatin1());}))
                {
                    qDebug() << __PRETTY_FUNCTION__ << row;
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
    } catch (std::runtime_error &e) {
        QMessageBox::critical(0, "Words loading error", e.what());
    }
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
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
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
