#ifndef ABSTRACTNUMBERPROVIDER_H
#define ABSTRACTNUMBERPROVIDER_H

#include <QString>
#include "ProviderType.h"

class AbstractListeningProvider
{
    ProviderType type;
public:
    virtual QString get() const = 0;
    virtual QString check(const QString &input, bool &ok) const = 0;
    virtual QString formatHint() const = 0;
    virtual ~AbstractListeningProvider() = default;

    AbstractListeningProvider(ProviderType type) : type(type)
    {};

    AbstractListeningProvider & operator=(const AbstractListeningProvider &) = delete;
    AbstractListeningProvider & operator=(const AbstractListeningProvider &&) = delete;
    AbstractListeningProvider(AbstractListeningProvider &) = delete;
    AbstractListeningProvider(AbstractListeningProvider &&) = delete;

    ProviderType getType() const {return type;}
};

#endif // ABSTRACTNUMBERPROVIDER_H
