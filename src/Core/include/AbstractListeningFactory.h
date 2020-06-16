#ifndef ABSTRACTLISTENINGFACTORY_H
#define ABSTRACTLISTENINGFACTORY_H

#include <QString>

#include <ProviderType.h>

class AbstractListeningFactory
{
public:
    virtual ~AbstractListeningFactory() = default;
    virtual QString get(const ProviderType providerType, int range, bool repeatFails, int probability) = 0;
    virtual QString repeat() = 0;
    virtual QString check(const QString &input, bool &ok) = 0;
    virtual QString formatHint() = 0;
    virtual bool inWork() = 0;
};

#endif // ABSTRACTLISTENINGFACTORY_H
