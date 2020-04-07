#include "ProviderFactory.h"

#include <QDateTime>

#include <QDebug>
#include <QSharedPointer>

#include <time.h>
#include <limits>
#include <random>

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
        range(range),
        num(randomDevice() % (range + 1))
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
        num(QString{"%1-%2-%3-%4"}.arg(rangedRandom(0, 999), 3, 10, QLatin1Char('0'))
            .arg(rangedRandom(0, 999), 3, 10, QLatin1Char('0'))
            .arg(rangedRandom(0, 99), 2, 10, QLatin1Char('0'))
            .arg(rangedRandom(0, 99), 2, 10, QLatin1Char('0')))
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
        date(QDate(rangedRandom(1900, 2050),
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
        t(QTime(rangedRandom(0, 23),
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
    QString w;
public:
    WordProvider(const WordsStorage &s)
    {
        if(s.empty())
        {
            w = "cat";
        }else
        {
            long count = s.size();
            long i = randomDevice() % count;
            w = s.getWord(i);
        }
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
    QString w;
public:
    PhraseProvider(const PhrasesStorage &s)
    {
        if(s.empty())
        {
            w = "paper tiger";
        }else
        {
            long count = s.size();
            long i = randomDevice() % count;
            w = s.getWord(i);
        }
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

ProviderFactory::ProviderFactory(const WordsStorage &wordsStorage,
                                 const PhrasesStorage &phrasesStorage,
                                 QObject *parent) :
    QObject(parent), wordsStorage(wordsStorage), phrasesStorage(phrasesStorage)
{
    randomDevice.seed(time(0));
}
