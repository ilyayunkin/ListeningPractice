#ifndef ABSTRACTNUMBERPROVIDER_H
#define ABSTRACTNUMBERPROVIDER_H

#include <QString>
#include <QSharedPointer>

enum ProviderType
{
    NUMBER,
    DATE,
    TIME,
    PHONE_NUMBER,

    PROVIDERS_COUNT
};

class AbstractNumberProvider
{
public:
    virtual QString getNumber() = 0;
    virtual QString checkNumber(const QString &input, bool &ok) = 0;
    virtual QString formatHint() = 0;
    virtual ~AbstractNumberProvider(){};

    AbstractNumberProvider() = default;

    AbstractNumberProvider & operator=(const AbstractNumberProvider &) = delete;
    AbstractNumberProvider & operator=(const AbstractNumberProvider &&) = delete;
    AbstractNumberProvider(AbstractNumberProvider &) = delete;
    AbstractNumberProvider(AbstractNumberProvider &&) = delete;

    static QSharedPointer<AbstractNumberProvider> getProvider(ProviderType providerType, int range);
};

#endif // ABSTRACTNUMBERPROVIDER_H
