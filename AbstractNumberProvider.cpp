#include "AbstractNumberProvider.h"

#include <QDateTime>

#include <QDebug>

#include <time.h>
#include <limits>
#include <random>

class NumberProvider : public AbstractNumberProvider
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
    QString getNumber() override
    {
        return QString::number(num);
    }
    QString checkNumber(const QString &input, bool &ok) override
    {
        ok = input.toInt() == num;
        return QString::number(num);
    }
    virtual QString formatHint() override
    {
        return QString{"12345678910987654321"}.left(QString::number(range).length());
    }
};
class PhoneNumberProvider : public AbstractNumberProvider
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
    QString getNumber() override
    {
        qDebug() << __LINE__ << num;
        return num;
    }
    QString checkNumber(const QString &input, bool &ok) override
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
class DateProvider : public AbstractNumberProvider
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
    QString getNumber() override
    {
        return toStr(date);
    }
    QString checkNumber(const QString &input, bool &ok) override
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
class TimeProvider : public AbstractNumberProvider
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
    QString getNumber() override
    {
        return toStr(t);
    }
    QString checkNumber(const QString &input, bool &ok) override
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

QSharedPointer<AbstractNumberProvider> AbstractNumberProvider::getProvider(ProviderType providerType, int range)
{
    switch(providerType)
    {
    default:
    case NUMBER: return QSharedPointer<AbstractNumberProvider>(new NumberProvider{range});
        break;
    case DATE: return QSharedPointer<AbstractNumberProvider>(new DateProvider);
        break;
    case TIME: return QSharedPointer<AbstractNumberProvider>(new TimeProvider);
        break;
    case PHONE_NUMBER: return QSharedPointer<AbstractNumberProvider>(new PhoneNumberProvider);
        break;
    }
}
