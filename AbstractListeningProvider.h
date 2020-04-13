#ifndef ABSTRACTNUMBERPROVIDER_H
#define ABSTRACTNUMBERPROVIDER_H

#include <QString>

enum ProviderType
{
    NUMBER,
    DATE,
    TIME,
    PHONE_NUMBER,
    WORD,
    PHRASE,

    PROVIDERS_COUNT
};

class AbstractListeningProvider
{
    ProviderType type;
public:
    virtual QString get() const = 0;
    virtual QString check(const QString &input, bool &ok) const = 0;
    virtual QString formatHint() const = 0;
    virtual ~AbstractListeningProvider(){};

    AbstractListeningProvider(ProviderType type) : type(type)
    {};

    AbstractListeningProvider & operator=(const AbstractListeningProvider &) = delete;
    AbstractListeningProvider & operator=(const AbstractListeningProvider &&) = delete;
    AbstractListeningProvider(AbstractListeningProvider &) = delete;
    AbstractListeningProvider(AbstractListeningProvider &&) = delete;

    ProviderType getType() const {return type;}
};

#endif // ABSTRACTNUMBERPROVIDER_H
