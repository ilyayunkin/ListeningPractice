#include "ProviderFactory.h"

#include <QDateTime>

#include <QDebug>
#include <QSharedPointer>

#include <time.h>
#include <limits>
#include <random>

class NumberProvider : public AbstractListeningProvider
{
    std::default_random_engine randomDevice;
    int num;
    int range;
public:
    explicit NumberProvider(int range) : range(range)
    {
        randomDevice.seed(time(0));
        num = randomDevice() % (range + 1);
    }
    QString get() override
    {
        return QString::number(num);
    }
    QString check(const QString &input, bool &ok) override
    {
        ok = input.toInt() == num;
        return QString::number(num);
    }
    virtual QString formatHint() override
    {
        return QString{"12345678910987654321"}.left(QString::number(range).length());
    }
};
class PhoneNumberProvider : public AbstractListeningProvider
{
    std::default_random_engine randomDevice;
    QString num;
public:
    explicit PhoneNumberProvider()
    {
        randomDevice.seed(time(0));
        num = QString{"%1-%2-%3-%4"}.arg(rangedRandom(0, 999), 3, 10, QLatin1Char('0'))
                .arg(rangedRandom(0, 999), 3, 10, QLatin1Char('0'))
                .arg(rangedRandom(0, 99), 2, 10, QLatin1Char('0'))
                .arg(rangedRandom(0, 99), 2, 10, QLatin1Char('0'));
    }
    QString get() override
    {
        qDebug() << __LINE__ << num;
        return num;
    }
    QString check(const QString &input, bool &ok) override
    {
        ok = input == num;
        return num;
    }
    int rangedRandom(int minimum, int maximum)
    {
        int rNum = randomDevice() % (maximum - minimum) + minimum;
        return rNum;
    }
    virtual QString formatHint() override
    {
        return "123-456-78-90";
    }
};
class DateProvider : public AbstractListeningProvider
{
    std::default_random_engine randomDevice;
    QLocale locale = QLocale{QLocale::English, QLocale::UnitedStates};
    QDate date;
    QString format = "MMMM d yyyy";
public:
    DateProvider()
    {
        randomDevice.seed(time(0));
        date = QDate(rangedRandom(1900, 2050),
                     rangedRandom(1, 12),
                     rangedRandom(1, 31));
    }
    QString get() override
    {
        return toStr(date);
    }
    QString check(const QString &input, bool &ok) override
    {
        QDate inputDate = fromStr(input);
        ok = inputDate == date;
        qDebug() << input << inputDate << toStr(inputDate) << date  << toStr(date);
        return toStr(date);
    }
    int rangedRandom(int minimum, int maximum)
    {
        int rNum = randomDevice() % (maximum - minimum) + minimum;
        return rNum;
    }
    QString toStr(const QDate &date)
    {
        QString dateString = locale.toString(date, format);
        qDebug() << __LINE__ << dateString;
        return dateString;
    }
    QDate fromStr(const QString &input)
    {
        return locale.toDate(input, format);
    }
    virtual QString formatHint() override
    {
        return "January 56 1976";
    }
};
class TimeProvider : public AbstractListeningProvider
{
    std::default_random_engine randomDevice;
    QLocale locale = QLocale{QLocale::English, QLocale::UnitedStates};
    QTime t;
    QString format = "H:mm a";
public:
    TimeProvider()
    {
        randomDevice.seed(time(0));
        t = QTime(rangedRandom(0, 23),
                     rangedRandom(0, 59));
    }
    QString get() override
    {
        return toStr(t);
    }
    QString check(const QString &input, bool &ok) override
    {
        QTime inputTime = fromStr(input);
        ok = inputTime == t;
        qDebug() << input << inputTime << toStr(inputTime) << t  << toStr(t);
        return toStr(t);
    }
    int rangedRandom(int minimum, int maximum)
    {
        int rNum = randomDevice() % (maximum - minimum) + minimum;
        return rNum;
    }
    QString toStr(const QTime &time)
    {
        QString timeString = locale.toString(time, format);
        qDebug() << __LINE__ << timeString;
        return timeString;
    }
    QTime fromStr(const QString &input)
    {
        return locale.toTime(input, format);
    }
    virtual QString formatHint() override
    {
        return "01:45 pm";
    }
};

class WordProvider : public AbstractListeningProvider
{
    std::default_random_engine randomDevice;
    QString w;
    QLocale locale = QLocale{QLocale::English, QLocale::UnitedStates};
public:
    WordProvider(WordsStorage &s)
    {
        randomDevice.seed(time(0));
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
    QString get() override
    {
        return w;
    }
    QString check(const QString &input, bool &ok) override
    {
        ok = input.toLower() == w.toLower();
        return w;
    }
    virtual QString formatHint() override
    {
        return "cat";
    }
};

class PhraseProvider : public AbstractListeningProvider
{
    std::default_random_engine randomDevice;
    QString w;
    QLocale locale = QLocale{QLocale::English, QLocale::UnitedStates};
public:
    PhraseProvider(PhrasesStorage &s)
    {
        randomDevice.seed(time(0));
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
    QString get() override
    {
        return w;
    }
    QString check(const QString &input, bool &ok) override
    {
        ok = input.toLower() == w.toLower();
        if(!ok){
            qDebug() << input.toLatin1() << input.toLatin1().toLower() << input.toLatin1().toHex();
            qDebug() << w.toLatin1() << w.toLatin1().toLower() << w.toLatin1().toHex();
        }
        return w;
    }
    virtual QString formatHint() override
    {
        return "cat";
    }
};

QSharedPointer<AbstractListeningProvider> ProviderFactory::getProvider(ProviderType providerType, int range)
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

ProviderFactory::ProviderFactory(WordsStorage &wordsStorage, PhrasesStorage &phrasesStorage, QObject *parent) :
    QObject(parent), wordsStorage(wordsStorage), phrasesStorage(phrasesStorage)
{
}
