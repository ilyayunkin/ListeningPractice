#include "PhrasesStorage.h"
#include <QDebug>

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>

#include <algorithm>
#include <assert.h>

#include "ExceptionClasses.h"

PhrasesStorage::PhrasesStorage(QObject *parent) :
    QObject(parent)
{
    if(!loadWordsFromFile())
    {
        requestWordsFromTheInternet(QUrl("https://www.phrases.org.uk/meanings/phrases-and-sayings-list.html"));
    }
}

PhrasesStorage::~PhrasesStorage()
{

}

void PhrasesStorage::fileDownloaded(QNetworkReply *r)
{
    qDebug() << __PRETTY_FUNCTION__;
    m_DownloadedData = r->readAll();
    //emit a signal
    r->deleteLater();

    try {
        if(r->error() != QNetworkReply::NoError)
            throw HttpLoadException(r->errorString().toLatin1());

        if(m_DownloadedData.isEmpty())
            throw EmptyFileException();

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
            constexpr char divBegin[] = "<div class=\"content\">";
            constexpr char divEnd[] = "</div>";
            constexpr char br[] = "<br />";

            const int fieldBegin = m_DownloadedData.indexOf(divBegin);
            if(fieldBegin == -1)
                throw ParsingFailedException(divBegin);

            const int fieldEnd = m_DownloadedData.indexOf(divEnd, fieldBegin);
            if(fieldEnd == -1)
                throw ParsingFailedException(divEnd);

            QString paragraph = m_DownloadedData.mid(fieldBegin, fieldEnd - fieldBegin);
            paragraph.replace(br, "");
            paragraph.replace("\t", "");

            QStringList rowsList = paragraph.split("\n");
            for(const auto &row : rowsList)
            {
                constexpr char aBegin[] = "<a";
                constexpr char aEnd[] = "</a";
                const int aaBegin = row.indexOf(aBegin);
                const int textBegin = row.indexOf(">", aaBegin) + 1;
                const int aaEnd = row.indexOf(aEnd);
                if((aaEnd != -1) && (aaBegin != -1) && (textBegin != -1))
                {
                    QString phrase = row.mid(textBegin, aaEnd - textBegin);

                    if(!phrase.isEmpty() &&
                            std::all_of(phrase.begin(), phrase.end(),
                                        [](QChar c){return isalpha(c.toLatin1()) || c.toLatin1() == ' ';}))
                    {
                        QStringList wordsList = phrase.split(' ');
                        wordsList.erase(std::remove_if(wordsList.begin(),
                                                       wordsList.end(),
                                                       [](const QString &s){return s.isEmpty();}),
                                        wordsList.end());
                        words+= wordsList.join(' ').toLower();
                    }
                }
            }
        }
        QDateTime t2 = QDateTime::currentDateTime();
        words.removeDuplicates();
        QDateTime t3 = QDateTime::currentDateTime();
        {
            QFile f(phrasesFileName);
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
        QMessageBox::critical(0, "Phrases loading error", e.what());
    }
}

QByteArray PhrasesStorage::downloadedData() const
{
    return m_DownloadedData;
}

bool PhrasesStorage::loadWordsFromFile()
{
    bool ok = false;

    QFile f(phrasesFileName);
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

void PhrasesStorage::requestWordsFromTheInternet(QUrl imageUrl)
{
    QNetworkRequest request(imageUrl);
    m_WebCtrl.get(request);
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
            this, SLOT (fileDownloaded(QNetworkReply*)));
}

QString PhrasesStorage::getWord(long index) const
{
    assert(!empty());
    assert(index < size());
    return words.at(index);
}
