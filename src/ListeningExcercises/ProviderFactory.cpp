#include "ProviderFactory.h"

#include <QDateTime>

#include <QDebug>
#include <QSharedPointer>
#include <QFile>
#include <QTextStream>

#include <time.h>
#include <limits>
#include <random>
#include <assert.h>

namespace
{
static const QLocale locale = QLocale{QLocale::English, QLocale::UnitedStates};
static std::default_random_engine randomDevice;

static int rangedRandom(const int minimum, const int maximum)
{
    int rNum = randomDevice() % (maximum - minimum) + minimum;
    return rNum;
}

class NumberProvider : public AbstractListeningProvider
{
    const int range;
    const int num;
public:
    explicit NumberProvider(int range) :
        AbstractListeningProvider(NUMBER),
        range(range),
        num(randomDevice() % (range + 1))
    {
    }
    explicit NumberProvider(const QString &str) :
        AbstractListeningProvider(NUMBER),
        range(0),
        num(str.toInt())
    {
    }
    QString get() const override
    {
        return QString::number(num);
    }
    QString check(const QString &input, bool &ok) const override
    {
        ok = input.toInt() == num;
        return QString::number(num);
    }
    virtual QString formatHint() const override
    {
        return QString{"12345678910987654321"}.left(QString::number(range).length());
    }
};
class PhoneNumberProvider : public AbstractListeningProvider
{
    const QString num;
public:
    explicit PhoneNumberProvider() :
        AbstractListeningProvider(PHONE_NUMBER),
        num(QString{"%1-%2-%3-%4"}.arg(rangedRandom(0, 999), 3, 10, QLatin1Char('0'))
            .arg(rangedRandom(0, 999), 3, 10, QLatin1Char('0'))
            .arg(rangedRandom(0, 99), 2, 10, QLatin1Char('0'))
            .arg(rangedRandom(0, 99), 2, 10, QLatin1Char('0')))
    {
    }
    explicit PhoneNumberProvider(const QString &str) :
        AbstractListeningProvider(PHONE_NUMBER),
        num(str)
    {
    }
    QString get() const override
    {
        qDebug() << __LINE__ << num;
        return num;
    }
    QString check(const QString &input, bool &ok) const override
    {
        ok = input == num;
        return num;
    }
    virtual QString formatHint() const override
    {
        return "123-456-78-90";
    }
};
class DateProvider : public AbstractListeningProvider
{
    const QDate date;
    static const QString format;
public:
    DateProvider() :
        AbstractListeningProvider(DATE),
        date(QDate(rangedRandom(1900, 2050),
                   rangedRandom(1, 12),
                   rangedRandom(1, 31)))
    {
    }
    DateProvider(const QString &str) :
        AbstractListeningProvider(DATE),
        date(fromStr(str).isValid() ? fromStr(str) : QDate(rangedRandom(1900, 2050),
                                                           rangedRandom(1, 12),
                                                           rangedRandom(1, 31)))
    {
    }
    QString get() const override
    {
        return toStr(date);
    }
    QString check(const QString &input, bool &ok) const override
    {
        QDate inputDate = fromStr(input);
        ok = inputDate == date;
        qDebug() << input << inputDate << toStr(inputDate) << date  << toStr(date);
        return toStr(date);
    }
    static QString toStr(const QDate &date)
    {
        QString dateString = locale.toString(date, format);
        qDebug() << __LINE__ << dateString;
        return dateString;
    }
    static  QDate fromStr(const QString &input)
    {
        return locale.toDate(input, format);
    }
    virtual QString formatHint() const override
    {
        return "January 56 1976";
    }
};
const QString DateProvider::format = "MMMM d yyyy";

class TimeProvider : public AbstractListeningProvider
{
    const QTime t;
    static const QString format;
public:
    TimeProvider() :
        AbstractListeningProvider(TIME),
        t(QTime(rangedRandom(0, 23),
                rangedRandom(0, 59)))
    {
    }
    TimeProvider(const QString &str) :
        AbstractListeningProvider(TIME),
        t(fromStr(str).isValid() ? fromStr(str) : QTime(rangedRandom(0, 23),
                                                        rangedRandom(0, 59)))
    {
    }
    QString get() const override
    {
        return toStr(t);
    }
    QString check(const QString &input, bool &ok) const override
    {
        QTime inputTime = fromStr(input);
        ok = inputTime == t;
        qDebug() << input << inputTime << toStr(inputTime) << t  << toStr(t);
        return toStr(t);
    }
    static QString toStr(const QTime &time)
    {
        QString timeString = locale.toString(time, format);
        qDebug() << __LINE__ << timeString;
        return timeString;
    }
    static QTime fromStr(const QString &input)
    {
        return locale.toTime(input, format);
    }
    virtual QString formatHint() const override
    {
        return "01:45 pm";
    }
};
const QString TimeProvider::format = "h:mm a";

class WordProvider : public AbstractListeningProvider
{
    const QString w;
public:
    WordProvider(const WordsStorage &s) :
        AbstractListeningProvider(WORD),
        w(s.empty() ? QString("cat") : s.getWord(randomDevice() % s.size()))
    {
    }
    WordProvider(const QString &str) :
        AbstractListeningProvider(WORD),
        w(str)
    {
    }
    QString get() const override
    {
        return w;
    }
    QString check(const QString &input, bool &ok) const override
    {
        ok = input.toLower() == w.toLower();
        return w;
    }
    virtual QString formatHint() const override
    {
        return "cat";
    }
};

class PhraseProvider : public AbstractListeningProvider
{
    const QString w;
public:
    PhraseProvider(const PhrasesStorage &s) :
        AbstractListeningProvider(PHRASE),
        w(s.empty() ? QString("paper tiger") : s.getWord(randomDevice() % s.size()))
    {
    }
    PhraseProvider(const QString &str) :
        AbstractListeningProvider(PHRASE),
        w(str)
    {
    }
    QString get() const override
    {
        return w;
    }
    QString check(const QString &input, bool &ok) const override
    {
        ok = input.toLower() == w.toLower();
        if(!ok){
            qDebug() << input.toLatin1() << input.toLatin1().toLower() << input.toLatin1().toHex();
            qDebug() << w.toLatin1() << w.toLatin1().toLower() << w.toLatin1().toHex();
        }
        return w;
    }
    virtual QString formatHint() const override
    {
        return "cat";
    }
};
}

QSharedPointer<AbstractListeningProvider> ProviderFactory::getProvider(
        const ProviderType providerType, const int range) const
{
    switch(providerType)
    {
    default:
    case NUMBER: return QSharedPointer<AbstractListeningProvider>(new NumberProvider{range});
        break;
    case DATE: return QSharedPointer<AbstractListeningProvider>(new DateProvider);
        break;
    case TIME: return QSharedPointer<AbstractListeningProvider>(new TimeProvider);
        break;
    case PHONE_NUMBER: return QSharedPointer<AbstractListeningProvider>(new PhoneNumberProvider);
        break;
    case WORD: return QSharedPointer<AbstractListeningProvider>(new WordProvider(wordsStorage));
        break;
    case PHRASE: return QSharedPointer<AbstractListeningProvider>(new PhraseProvider(phrasesStorage));
        break;
    }
}

QSharedPointer<AbstractListeningProvider> ProviderFactory::getProvider(
        const ProviderType providerType, const QString &str) const
{
    switch(providerType)
    {
    default:
    case NUMBER: return QSharedPointer<AbstractListeningProvider>(new NumberProvider{str});
        break;
    case DATE: return QSharedPointer<AbstractListeningProvider>(new DateProvider{str});
        break;
    case TIME: return QSharedPointer<AbstractListeningProvider>(new TimeProvider{str});
        break;
    case PHONE_NUMBER: return QSharedPointer<AbstractListeningProvider>(new PhoneNumberProvider{str});
        break;
    case WORD: return QSharedPointer<AbstractListeningProvider>(new WordProvider{str});
        break;
    case PHRASE: return QSharedPointer<AbstractListeningProvider>(new PhraseProvider{str});
        break;
    }
}

ProviderFactory::ProviderFactory(const WordsStorage &wordsStorage,
                                 const PhrasesStorage &phrasesStorage,
                                 QObject *parent) :
    QObject(parent), wordsStorage(wordsStorage), phrasesStorage(phrasesStorage)
{
    randomDevice.seed(time(0));
    for(int i = 0; i < PROVIDERS_COUNT; ++i)
    {
        QFile f(QString::number(i));
        if(f.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&f);
            QString s;
            while(!(s = stream.readLine()).isNull())
            {
                failed[i].push_back(s);
            }
        }
    }

}

ProviderFactory::~ProviderFactory()
{
    for(int i = 0; i < PROVIDERS_COUNT; ++i)
    {
        QFile f(QString::number(i));
        if(f.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&f);
            for(const auto &s : failed[i])
            {
                stream << s << "\r\n";
            }
        }else
        {
            assert(f.isOpen());
        }
    }
}

QString ProviderFactory::get(const ProviderType providerType, int range, bool repeatFails, int probability)
{
    this->repeatFails = repeatFails;
    int randomN = (rangedRandom(0, 100));
    qDebug() << __PRETTY_FUNCTION__
             << providerType
             << "repeat fails:" << repeatFails
             << randomN << '/' << probability
             << "phrases to repeat:" << failed[providerType].size();
    if(repeatFails && (randomN < probability) && !failed[providerType].isEmpty())
    {
        QString word = failed[providerType].first();
        failed[providerType].pop_front();
        qDebug() << __PRETTY_FUNCTION__ << "repeat:" << word;
        numberProvider = getProvider(providerType, word);
    }else
    {
        numberProvider = getProvider(providerType, range);
    }
    QString ret = numberProvider->get();
    return ret;
}
QString ProviderFactory::repeat()
{
    assert(!numberProvider.isNull());
    QString ret = numberProvider->get();
    return ret;
}
QString ProviderFactory::check(const QString &input, bool &ok)
{
    assert(!numberProvider.isNull());
    QString rightAnswer = numberProvider->check(input, ok);
    if(repeatFails && !ok)
    {
        ProviderType type = numberProvider->getType();
        failed[type].push_back(rightAnswer);
        qDebug() << __PRETTY_FUNCTION__ << "to repeats:" << rightAnswer;
    }
    return rightAnswer;
}
QString ProviderFactory::formatHint()
{
    assert(!numberProvider.isNull());
    QString ret = numberProvider->formatHint();
    return ret;
}

bool ProviderFactory::inWork()
{
    return !numberProvider.isNull();
}
