#ifndef PROVIDERFACTORY_H
#define PROVIDERFACTORY_H

#include <QObject>

#include "AbstractListeningProvider.h"
#include "WordsStorage.h"

class ProviderFactory : public QObject
{
    Q_OBJECT
    WordsStorage &s;
public:
    explicit ProviderFactory (WordsStorage &s, QObject *parent = nullptr);
    QSharedPointer<AbstractListeningProvider> getProvider(ProviderType providerType, int range);

signals:

};

#endif // PROVIDERFACTORY_H
